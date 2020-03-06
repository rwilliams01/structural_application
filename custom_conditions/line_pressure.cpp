/*
==============================================================================
KratosStructuralApplication
A library based on:
Kratos
A General Purpose Software for Multi-Physics Finite Element Analysis
Version 1.0 (Released on march 05, 2007).

Copyright 2007
Pooyan Dadvand, Riccardo Rossi, Janosch Stascheit, Felix Nagel
pooyan@cimne.upc.edu
rrossi@cimne.upc.edu
janosch.stascheit@rub.de
nagel@sd.rub.de
- CIMNE (International Center for Numerical Methods in Engineering),
Gran Capita' s/n, 08034 Barcelona, Spain
- Ruhr-University Bochum, Institute for Structural Mechanics, Germany


Permission is hereby granted, free  of charge, to any person obtaining
a  copy  of this  software  and  associated  documentation files  (the
"Software"), to  deal in  the Software without  restriction, including
without limitation  the rights to  use, copy, modify,  merge, publish,
distribute,  sublicense and/or  sell copies  of the  Software,  and to
permit persons to whom the Software  is furnished to do so, subject to
the following condition:

Distribution of this code for  any  commercial purpose  is permissible
ONLY BY DIRECT ARRANGEMENT WITH THE COPYRIGHT OWNERS.

The  above  copyright  notice  and  this permission  notice  shall  be
included in all copies or substantial portions of the Software.

THE  SOFTWARE IS  PROVIDED  "AS  IS", WITHOUT  WARRANTY  OF ANY  KIND,
EXPRESS OR  IMPLIED, INCLUDING  BUT NOT LIMITED  TO THE  WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT  SHALL THE AUTHORS OR COPYRIGHT HOLDERS  BE LIABLE FOR ANY
CLAIM, DAMAGES OR  OTHER LIABILITY, WHETHER IN AN  ACTION OF CONTRACT,
TORT  OR OTHERWISE, ARISING  FROM, OUT  OF OR  IN CONNECTION  WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

==============================================================================
*/
/* **************************************************************************************
*
*   Last Modified by:    $Author: hbui $
*   Date:                $Date: 16 Dec 2017 $
*   Revision:            $Revision: 1.0 $
*
* ***************************************************************************************/


// System includes


// External includes


// Project includes
#include "includes/define.h"
#include "custom_conditions/line_pressure.h"
#include "structural_application.h"
#include "utilities/math_utils.h"
#include "custom_utilities/sd_math_utils.h"

namespace Kratos
{
//***********************************************************************************
//***********************************************************************************
// -------- //
//  PUBLIC  //
// -------- //

// Constructor
LinePressure::LinePressure()
{
}

// Constructor
LinePressure::LinePressure( IndexType NewId, GeometryType::Pointer pGeometry )
    : Condition( NewId, pGeometry )
{
}

// Constructor
LinePressure::LinePressure( IndexType NewId, GeometryType::Pointer pGeometry,
                          PropertiesType::Pointer pProperties )
    : Condition( NewId, pGeometry, pProperties )
{
}

//***********************************************************************************
//***********************************************************************************
Condition::Pointer LinePressure::Create( IndexType NewId,
                                        NodesArrayType const& ThisNodes,
                                        PropertiesType::Pointer pProperties ) const
{
    return Condition::Pointer( new LinePressure( NewId, GetGeometry().Create( ThisNodes ), pProperties ) );
}

//***********************************************************************************
//***********************************************************************************
Condition::Pointer LinePressure::Create( IndexType NewId,
                                        GeometryType::Pointer pGeom,
                                        PropertiesType::Pointer pProperties ) const
{
    return Condition::Pointer( new LinePressure( NewId, pGeom, pProperties ) );
}

//***********************************************************************************
//***********************************************************************************
// Destructor
LinePressure::~LinePressure()
{
}

//***********************************************************************************
//***********************************************************************************
void LinePressure::EquationIdVector( EquationIdVectorType& rResult,
                                    ProcessInfo& rCurrentProcessInfo )
{
    KRATOS_TRY

    DofsVectorType ConditionalDofList;
    GetDofList(ConditionalDofList, rCurrentProcessInfo);

    if (rResult.size() != ConditionalDofList.size())
        rResult.resize(ConditionalDofList.size(), false);

    for(unsigned int i = 0; i < ConditionalDofList.size(); ++i)
    {
        rResult[i] = ConditionalDofList[i]->EquationId();
    }

    KRATOS_CATCH( "" )
}

//***********************************************************************************
//***********************************************************************************
void LinePressure::GetDofList( DofsVectorType& ElementalDofList,
                              ProcessInfo& rCurrentProcessInfo )
{
    const unsigned int dim = GetGeometry().WorkingSpaceDimension();

    ElementalDofList.resize( 0 );

    for ( unsigned int i = 0; i < GetGeometry().size(); ++i )
    {
        ElementalDofList.push_back( GetGeometry()[i].pGetDof( DISPLACEMENT_X ) );
        ElementalDofList.push_back( GetGeometry()[i].pGetDof( DISPLACEMENT_Y ) );
    }
}

//***********************************************************************************
//***********************************************************************************
void LinePressure::CalculateRightHandSide( VectorType& rRightHandSideVector,
        ProcessInfo& rCurrentProcessInfo )
{
    KRATOS_TRY

    const unsigned int number_of_nodes = GetGeometry().size();
    const unsigned int dim = GetGeometry().WorkingSpaceDimension();
    unsigned int MatSize = number_of_nodes * dim;

    //resizing as needed the RHS
    if ( rRightHandSideVector.size() != MatSize )
        rRightHandSideVector.resize( MatSize, false );
    rRightHandSideVector = ZeroVector( MatSize ); //resetting RHS

    //reading integration points and local gradients
    const GeometryType::IntegrationPointsArrayType& integration_points =
        GetGeometry().IntegrationPoints();

    // DN_DeContainer is the array of shape function gradients at each integration points
    const GeometryType::ShapeFunctionsGradientsType& DN_De =
        GetGeometry().ShapeFunctionsLocalGradients();

    // Ncontainer is the array of shape function values at each integration points
    const Matrix& Ncontainer = GetGeometry().ShapeFunctionsValues();

    const double& P = this->GetValue(PRESSURE);

    // loop over integration points
    Vector Load( dim );
    Vector t( dim );
    for ( unsigned int PointNumber = 0; PointNumber < integration_points.size(); ++PointNumber )
    {
        // compute integration weight
        double IntegrationWeight = integration_points[PointNumber].Weight();

        IntegrationWeight *= GetProperties()[THICKNESS];

        // compute length
        noalias( t ) = ZeroVector( dim );//tangential vector
        for ( unsigned int n = 0; n < GetGeometry().size(); ++n )
        {
            t[0] += GetGeometry().GetPoint( n ).X0() * DN_De[PointNumber]( n, 0 );
            t[1] += GetGeometry().GetPoint( n ).Y0() * DN_De[PointNumber]( n, 0 );
        }
//        KRATOS_WATCH(t)

        //calculating load
        Load[0] = -P*t[1];
        Load[1] = P*t[0];
//        KRATOS_WATCH(Load)

        // contribute to RIGHT HAND SIDE VECTOR
        for ( unsigned int prim = 0; prim < GetGeometry().size(); ++prim )
            for ( unsigned int i = 0; i < dim; ++i )
                rRightHandSideVector( prim * dim + i ) +=
                    Ncontainer( PointNumber, prim ) * Load( i ) * IntegrationWeight;
    }

    KRATOS_CATCH( "" )
}

//***********************************************************************************
//***********************************************************************************
void LinePressure::CalculateLocalSystem( MatrixType& rLeftHandSideMatrix,
                                        VectorType& rRightHandSideVector,
                                        ProcessInfo& rCurrentProcessInfo )
{
    const unsigned int number_of_nodes = GetGeometry().size();
    const unsigned int dim = GetGeometry().WorkingSpaceDimension();
    rLeftHandSideMatrix = ZeroMatrix( dim * number_of_nodes, dim * number_of_nodes );
    CalculateRightHandSide( rRightHandSideVector, rCurrentProcessInfo);
}

//***********************************************************************************
//***********************************************************************************
void LinePressure::CalculateMassMatrix( MatrixType& rMassMatrix,
                              ProcessInfo& rCurrentProcessInfo )
{
    KRATOS_TRY
    rMassMatrix.resize( 0, 0, false );
    KRATOS_CATCH( "" )
}

//***********************************************************************************
//***********************************************************************************
void LinePressure::CalculateDampingMatrix( MatrixType& rDampingMatrix,
                              ProcessInfo& rCurrentProcessInfo )
{
    KRATOS_TRY
    rDampingMatrix.resize( 0, 0, false );
    KRATOS_CATCH( "" )
}

/**
 * This function provides the place to perform checks on the completeness of the input.
 * It is designed to be called only once (or anyway, not often) typically at the beginning
 * of the calculations, so to verify that nothing is missing from the input
 * or that no common error is found.
 * @param rCurrentProcessInfo
 */
int LinePressure::Check( const Kratos::ProcessInfo& rCurrentProcessInfo )
{
    return 0;
}

//***********************************************************************************
//***********************************************************************************
} // Namespace Kratos.
