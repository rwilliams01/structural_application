/*
==============================================================================
KratosR1StructuralApplication 
A library based on:
Kratos
A General Purpose Software for Multi-Physics Finite Element Analysis
Version 1.0 (Released on march 05, 2007).

Copyright 2007
Pooyan Dadvand, Riccardo Rossi, Janip_utilityosch Stascheit, Felix Nagel 
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
//   
//   Project Name:        Kratos       
//   Last Modified by:    $Author: nagel $
//   Date:                $Date: 2007-10-16 12:48:37 $
//   Revision:            $Revision: 1.1 $
//
//
#if !defined(KRATOS_FOUNDATION_CONDITION_H_INCLUDED )
#define  KRATOS_FOUNDATION_CONDITION_H_INCLUDED


// External includes 
#include "boost/smart_ptr.hpp"

// Project includes
#include "includes/define.h"
#include "includes/element.h"
#include "includes/condition.h"
#include "includes/serializer.h"
#include "includes/ublas_interface.h"
#include "includes/variables.h"
#include "utilities/math_utils.h"

namespace Kratos
{
    /**
     * Contact link element for 3D contact problems.
     * This condition links two contact surfaces (one master and
     * one slave element) to a condition which may be assembled
     * at once by the builder.
     * Within this condition all system contribution with regard to
     * the gap function, the contact stress, the normal vectors
     * and their linearizations are calculated.
     */
    class FoundationCondition : public Condition
    {
        public:
            // Counted pointer of FoundationCondition
            KRATOS_CLASS_POINTER_DEFINITION(FoundationCondition);
            
            /** 
             * Default constructor.
             */
            FoundationCondition();
            FoundationCondition( IndexType NewId, GeometryType::Pointer pGeometry);
            
            FoundationCondition( IndexType NewId, GeometryType::Pointer pGeometry,  
                           PropertiesType::Pointer pProperties
                         );
            
            FoundationCondition( IndexType NewId, GeometryType::Pointer pGeometry,  
                                  	PropertiesType::Pointer pProperties,
					Element::Pointer& soilElement,
					Element::Pointer& FoundationElement,
					Point<3>& SoilLocalPoint, Point<3>& rFoundationLocalPoint);

            /**
             * Destructor.
             */
            virtual ~FoundationCondition();
      
            /**
             * Operations.
             */
            
            
            
            Condition::Pointer Create( IndexType NewId, 
                                       NodesArrayType const& ThisNodes,  
                                       PropertiesType::Pointer pProperties) const;
            
            /**
             * Calculates the local system contributions for this contact element
             */
            void CalculateLocalSystem( MatrixType& rLeftHandSideMatrix, 
                                       VectorType& rRightHandSideVector, 
                                       ProcessInfo& rCurrentProcessInfo);
            
            void CalculateRightHandSide( VectorType& rRightHandSideVector, 
                                         ProcessInfo& rCurrentProcessInfo);
//    	    void DampMatrix(MatrixType& rDampMatrix, ProcessInfo& rCurrentProcessInfo);

            void EquationIdVector( EquationIdVectorType& rResult, 
                                   ProcessInfo& rCurrentProcessInfo);
            
            void GetDofList( DofsVectorType& ConditionalDofList,
                             ProcessInfo& CurrentProcessInfo);
            

            void Initialize();
            /**
             * Turn back information as a string.
             * (DEACTIVATED)
             */
            //std::string Info();
      
            /**
             * Print information about this object.
             * (DEACTIVATED)
             */
            //virtual void PrintInfo(std::ostream& rOStream) const;

            /**
             * Print object's data.
             * (DEACTIVATED)
             */
            //virtual void PrintData(std::ostream& rOStream) const;
      
        protected:
        
        
        private:

      friend class Serializer;

      virtual void save ( Serializer& rSerializer ) const
      {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS ( rSerializer, Condition )
      }

      virtual void load ( Serializer& rSerializer )
      {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS ( rSerializer, Condition )
      }
            void CalculateAll( MatrixType& rLeftHandSideMatrix, 
                               VectorType& rRightHandSideVector,
                               ProcessInfo& rCurrentProcessInfo,
                               bool CalculateStiffnessMatrixFlag,
                               bool CalculateResidualVectorFlag);


            Point<3> mFoundationLocalPoint;
            Point<3> mSoilLocalPoint;
            Element::Pointer mpFoundationElement;
            Element::Pointer mpSoilElement;
	        Point<3> mFoundationGlobalPoint;
            Point<3> mSoilGlobalPoint;
//            Vector mTPileGlobalVector;
///	    int iSoilIntegrationPointIndex;

/*    void CalculateAndAdd_RHS( Vector& rRightHandSideVector,
            const Vector& NSoil,
            const Vector& NPile,
            const Vector& vPileNorm,
            const Matrix& T,
            const Vector& tangentialStresses,
            double Gap,
            double normalStress,
////	    double SoilIntegrationWeight,
            double dASoil,
		double friction_coeff);

	   Vector GetRelativTangentialVelocity(Matrix& T);

	   Vector GetRelativVelocity();
            
            void UpdateTipLocalPoint( );
     	    Point<3>& GlobalCoordinatesSoil(Element::Pointer tip_soilElements, Point<3>& rResult, Point<3> const& LocalCoordinates);
     	    Point<3>& GlobalCoordinatesPile(Element::Pointer FoundationElements, Point<3>& rResult, Point<3> const& LocalCoordinates);*/
     	 //   Vector& GlobalCoordinatesTPileVector(Element::Pointer FoundationElements, Vector& rResult, Point<3> const& LocalCoordinates);
    }; // Class FoundationCondition 
}  // namespace Kratos.
  

#endif // KRATOS_PILE_CONDITION_H_INCLUDED defined 

