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
/* *********************************************************
*
*   Last Modified by:    $Author: nagel $
*   Date:                $Date: 2009-03-20 08:56:48 $
*   Revision:            $Revision: 1.7 $
*
* ***********************************************************/


#if !defined(KRATOS_RESIDUALBASED_NEWMARK_SCHEME )
#define  KRATOS_RESIDUALBASED_NEWMARK_SCHEME

/* System includes */

/* External includes */

/* Project includes */
#include "includes/define.h"
#include "includes/kratos_flags.h"
#include "includes/model_part.h"
#include "solving_strategies/schemes/scheme.h"
#include "includes/variables.h"
#include "containers/array_1d.h"
#include "includes/element.h"
#include "structural_application_variables.h"

// #define ENABLE_ROTATION
#define ENABLE_LAMBDA

namespace Kratos
{
/**@name Kratos Globals */
/*@{ */
/*@} */
/**@name Type Definitions */
/*@{ */
/*@} */
/**@name  Enum's */
/*@{ */
/*@} */
/**@name  Functions */
/*@{ */
/*@} */
/**@name Kratos Classes */
/*@{ */
/** Short class definition.

This class provides the implementation of the basic tasks that are needed by the solution strategy.
It is intended to be the place for tailoring the solution strategies to problem specific tasks.

Detail class definition.

\URL[Example of use html]{ extended_documentation/no_ex_of_use.html}

\URL[Example of use pdf]{ extended_documentation/no_ex_of_use.pdf}

\URL[Example of use doc]{ extended_documentation/no_ex_of_use.doc}

\URL[Example of use ps]{ extended_documentation/no_ex_of_use.ps}


\URL[Extended documentation html]{ extended_documentation/no_ext_doc.html}

\URL[Extended documentation pdf]{ extended_documentation/no_ext_doc.pdf}

\URL[Extended documentation doc]{ extended_documentation/no_ext_doc.doc}

\URL[Extended documentation ps]{ extended_documentation/no_ext_doc.ps}*/

template<class TSparseSpace,  class TDenseSpace >
class ResidualBasedNewmarkScheme: public Scheme<TSparseSpace,TDenseSpace>
{
public:
    /**@name Type Definitions */
    KRATOS_CLASS_POINTER_DEFINITION( ResidualBasedNewmarkScheme );

    typedef Scheme<TSparseSpace,TDenseSpace> BaseType;

    typedef typename BaseType::TDataType TDataType;

    typedef typename BaseType::DofsArrayType DofsArrayType;

    typedef typename BaseType::ElementsArrayType ElementsArrayType;

    typedef typename BaseType::ConditionsArrayType ConditionsArrayType;

    typedef typename Element::DofsVectorType DofsVectorType;

    typedef typename BaseType::TSystemMatrixType TSystemMatrixType;

    typedef typename BaseType::TSystemVectorType TSystemVectorType;

    typedef typename BaseType::LocalSystemVectorType LocalSystemVectorType;

    typedef typename BaseType::LocalSystemMatrixType LocalSystemMatrixType;

    /**
     * Constructor for PURE Newmark scheme
     * @ref Chung&Hulbert: "A time integration algorithm for structural dynamics with improved
     *  numerical dissipation: The generalized alpha method" Journal of applied mechanics, 60, 371-375
     */
    ResidualBasedNewmarkScheme() : BaseType()
    {
        //For pure Newmark Scheme
        mAlpha_f = 0.0;
        mAlpha_m = 0.0;
        mBeta = 1.0 / 4.0;
        mGamma = 0.5;

        std::cout << "PURE Newmark Time !!!!!!!!!!!!!!!!!!!!!" << " alpha_f= " << mAlpha_f << " alpha_m= " << mAlpha_m << " beta= " << mBeta << " gamma= " << mGamma << std::endl;
        //(...)_NULL DOF at the begin of time step, (...)_EINS DOF at the end of time step, (...)
        // DOF at the midpoint of time step, Please recognize that the name of the DOF is (...)
        // while the iteration is done towards the (...)_EINS DOF value at end of time step
    }

    /**
     * Constructor.
     * @param mDissipationRadius if the scheme is numerically energy conserving or not
     *                              == 1.0 energy conserving
     *                              < 1.0 numerically discipating
     * @ref Chung&Hulbert: "A time integration algorithm for structural dynamics with improved
     *  numerical dissipation: The generalized alpha method" Journal of applied mechanics, 60, 371-375
     */
    ResidualBasedNewmarkScheme(double mDissipationRadius ) : BaseType()
    {
        mAlpha_f = mDissipationRadius / (1.0 + mDissipationRadius);
        mAlpha_m = (2.0 * mDissipationRadius - 1.0) / (mDissipationRadius + 1.0);
        mBeta = (1.0 + mAlpha_f - mAlpha_m) * (1.0 + mAlpha_f - mAlpha_m)/4.0;
        mGamma = 0.5 + mAlpha_f - mAlpha_m;

        std::cout << "using the Generalized alpha Time Integration Scheme with radius= "<< mDissipationRadius << " alpha_f= " << mAlpha_f << " alpha_m= " << mAlpha_m << " beta= " << mBeta << " gamma= " << mGamma << std::endl;
    }

    /**
     * Constructor with full options.
     * @param mDissipationRadius if the scheme is numerically energy conserving or not
     *                              == 1.0 energy conserving
     *                              < 1.0 numerically discipating
     * @ref Chung&Hulbert: "A time integration algorithm for structural dynamics with improved
     *  numerical dissipation: The generalized alpha method" Journal of applied mechanics, 60, 371-375
     */
    ResidualBasedNewmarkScheme(int option, double mDissipationRadius ) : BaseType()
    {
        if (option == 0)
        {
            mAlpha_f = 0.0;
            mAlpha_m = 0.0;
            mBeta = 1.0 / 4.0;
            mGamma = 0.5;

            std::cout << "PURE Newmark Time Integration Scheme!!!!!!!!!!!!!!!!!!!!!" << " alpha_f= " << mAlpha_f << " alpha_m= " << mAlpha_m << " beta= " << mBeta << " gamma= " << mGamma << std::endl;
        }
        else if (option == 1)
        {
            mAlpha_f = mDissipationRadius / (1.0 + mDissipationRadius);
            mAlpha_m = (2.0 * mDissipationRadius - 1.0) / (mDissipationRadius + 1.0);
            mBeta = (1.0 + mAlpha_f - mAlpha_m) * (1.0 + mAlpha_f - mAlpha_m)/4.0;
            mGamma = 0.5 + mAlpha_f - mAlpha_m;

            std::cout << "using the Generalized-alpha Time Integration Scheme with radius= "<< mDissipationRadius << " alpha_f= " << mAlpha_f << " alpha_m= " << mAlpha_m << " beta= " << mBeta << " gamma= " << mGamma << std::endl;
        }
        else if (option == 2)
        {
            mAlpha_f = 0.0;
            mAlpha_m = (2.0 * mDissipationRadius - 1.0) / (mDissipationRadius + 1.0);
            mBeta = (1.0 - mAlpha_m) * (1.0 - mAlpha_m)/4.0;
            mGamma = 0.5 - mAlpha_m;

            std::cout << "using the Bossak-alpha Time Integration Scheme with radius= "<< mDissipationRadius << " alpha_m= " << mAlpha_m << " beta= " << mBeta << " gamma= " << mGamma << std::endl;
        }
        else if (option == 3)
        {
            mAlpha_f = mDissipationRadius / (1.0 + mDissipationRadius);
            mAlpha_m = 0.0;
            mBeta = (1.0 + mAlpha_f) * (1.0 + mAlpha_f)/4.0;
            mGamma = 0.5 + mAlpha_f;

            std::cout << "using the Hilber-alpha Time Integration Scheme with radius= "<< mDissipationRadius << " alpha_f= " << mAlpha_f << " beta= " << mBeta << " gamma= " << mGamma << std::endl;
        }
        else
        {
            KRATOS_THROW_ERROR(std::logic_error, "Invalid Time integration option", option)
        }
    }

    /** Destructor.*/
    virtual ~ResidualBasedNewmarkScheme()
    {}

    /**@name Operators */

    /** Performing the update of the solution.*/
    /**
     * incremental update within newton iteration. It updates the state variables at the end of the time step: u_{n+1}^{k+1}= u_{n+1}^{k}+ \Delta u
     * @param r_model_part
     * @param rDofSet set of all primary variables
     * @param A LHS matrix
     * @param Dx incremental update of primary variables
     * @param b RHS Vector
     */
    void UpdateAll(
        ModelPart& r_model_part,
        DofsArrayType& rDofSet,
        TSystemMatrixType& A,
        TSystemVectorType& Dx,
        TSystemVectorType& b )
    {
        KRATOS_TRY

        for(ModelPart::NodeIterator i = r_model_part.NodesBegin() ;
                i != r_model_part.NodesEnd() ; i++)
        {
            if( i->HasDofFor(DISPLACEMENT_X) )
            {
                if(i->GetDof(DISPLACEMENT_X).IsFree())
                {
                    i->GetSolutionStepValue(DISPLACEMENT_EINS_X)
                    +=Dx[i->GetDof(DISPLACEMENT_X).EquationId()];
                }
            }
            if( i->HasDofFor(DISPLACEMENT_Y) )
            {
                if(i->GetDof(DISPLACEMENT_Y).IsFree())
                {
                    i->GetSolutionStepValue(DISPLACEMENT_EINS_Y)
                    +=Dx[i->GetDof(DISPLACEMENT_Y).EquationId()];
                }
            }
            if( i->HasDofFor(DISPLACEMENT_Z) )
            {
                if(i->GetDof(DISPLACEMENT_Z).IsFree())
                {
                    i->GetSolutionStepValue(DISPLACEMENT_EINS_Z)
                    +=Dx[i->GetDof(DISPLACEMENT_Z).EquationId()];
                }
            }
            if( i->HasDofFor(WATER_PRESSURE) )
            {
                if(i->GetDof(WATER_PRESSURE).IsFree())
                {
                    i->GetSolutionStepValue(WATER_PRESSURE_EINS)
                    +=Dx[i->GetDof(WATER_PRESSURE).EquationId()];
                }
            }
            if( i->HasDofFor(AIR_PRESSURE) )
            {
                if(i->GetDof(AIR_PRESSURE).IsFree())
                {
                    i->GetSolutionStepValue(AIR_PRESSURE_EINS)
                    +=Dx[i->GetDof(AIR_PRESSURE).EquationId()];
                }
            }

            // update this variables to account for Lagrange multiplier
            if( i->HasDofFor(LAGRANGE_DISPLACEMENT_X) )
            {
                if(i->GetDof(LAGRANGE_DISPLACEMENT_X).IsFree())
                {
                    i->GetSolutionStepValue(LAGRANGE_DISPLACEMENT_X)
                    +=Dx[i->GetDof(LAGRANGE_DISPLACEMENT_X).EquationId()];
                }
            }
            if( i->HasDofFor(LAGRANGE_DISPLACEMENT_Y) )
            {
                if(i->GetDof(LAGRANGE_DISPLACEMENT_Y).IsFree())
                {
                    i->GetSolutionStepValue(LAGRANGE_DISPLACEMENT_Y)
                    +=Dx[i->GetDof(LAGRANGE_DISPLACEMENT_Y).EquationId()];
                }
            }
            if( i->HasDofFor(LAGRANGE_DISPLACEMENT_Z) )
            {
                if(i->GetDof(LAGRANGE_DISPLACEMENT_Z).IsFree())
                {
                    i->GetSolutionStepValue(LAGRANGE_DISPLACEMENT_Z)
                    +=Dx[i->GetDof(LAGRANGE_DISPLACEMENT_Z).EquationId()];
                }
            }
            if( i->HasDofFor(LAGRANGE_WATER_PRESSURE) )
            {
                if(i->GetDof(LAGRANGE_WATER_PRESSURE).IsFree())
                {
                    i->GetSolutionStepValue(LAGRANGE_WATER_PRESSURE)
                    +=Dx[i->GetDof(LAGRANGE_WATER_PRESSURE).EquationId()];
                }
            }
            if( i->HasDofFor(LAGRANGE_AIR_PRESSURE) )
            {
                if(i->GetDof(LAGRANGE_AIR_PRESSURE).IsFree())
                {
                    i->GetSolutionStepValue(LAGRANGE_AIR_PRESSURE)
                    +=Dx[i->GetDof(LAGRANGE_AIR_PRESSURE).EquationId()];
                }
            }
        }

        KRATOS_CATCH("")
    }

    /**@name Operators */

    /** Performing the update of the solution.*/
    /**
     * incremental update within newton iteration. It updates the state variables at the end of the time step: u_{n+1}^{k+1}= u_{n+1}^{k}+ \Delta u
     * @param r_model_part
     * @param rDofSet set of all primary variables
     * @param A LHS matrix
     * @param Dx incremental update of primary variables
     * @param b RHS Vector
     */
    void Update(
        ModelPart& r_model_part,
        DofsArrayType& rDofSet,
        TSystemMatrixType& A,
        TSystemVectorType& Dx,
        TSystemVectorType& b )
    {
        KRATOS_TRY

        for (typename DofsArrayType::iterator dof_iterator = rDofSet.begin(); dof_iterator != rDofSet.end(); ++dof_iterator)
        {
            ModelPart::NodeType& rNode = r_model_part.GetNode(dof_iterator->Id());

            if (dof_iterator->GetVariable() == DISPLACEMENT_X)
            {
                if (dof_iterator->IsFree())
                {
                    rNode.GetSolutionStepValue(DISPLACEMENT_EINS_X)
                    += Dx[dof_iterator->EquationId()];
                }
            }
            else if (dof_iterator->GetVariable() == DISPLACEMENT_Y)
            {
                if (dof_iterator->IsFree())
                {
                    rNode.GetSolutionStepValue(DISPLACEMENT_EINS_Y)
                    += Dx[dof_iterator->EquationId()];
                }
            }
            else if (dof_iterator->GetVariable() == DISPLACEMENT_Z)
            {
                if (dof_iterator->IsFree())
                {
                    rNode.GetSolutionStepValue(DISPLACEMENT_EINS_Z)
                    += Dx[dof_iterator->EquationId()];
                }
            }
            else if (dof_iterator->GetVariable() == WATER_PRESSURE)
            {
                if (dof_iterator->IsFree())
                {
                    rNode.GetSolutionStepValue(WATER_PRESSURE_EINS)
                    += Dx[dof_iterator->EquationId()];
                }
            }
            else if (dof_iterator->GetVariable() == AIR_PRESSURE)
            {
                if (dof_iterator->IsFree())
                {
                    rNode.GetSolutionStepValue(AIR_PRESSURE_EINS)
                    += Dx[dof_iterator->EquationId()];
                }
            }
            #ifdef ENABLE_ROTATION
            else if (dof_iterator->GetVariable() == ROTATION_X)
            {
                if (dof_iterator->IsFree())
                {
                    rNode.GetSolutionStepValue(ROTATION_EINS_X)
                    += Dx[dof_iterator->EquationId()];
                }
            }
            else if (dof_iterator->GetVariable() == ROTATION_Y)
            {
                if (dof_iterator->IsFree())
                {
                    rNode.GetSolutionStepValue(ROTATION_EINS_Y)
                    += Dx[dof_iterator->EquationId()];
                }
            }
            else if (dof_iterator->GetVariable() == ROTATION_Z)
            {
                if (dof_iterator->IsFree())
                {
                    rNode.GetSolutionStepValue(ROTATION_EINS_Z)
                    += Dx[dof_iterator->EquationId()];
                }
            }
            #endif
            #ifdef ENABLE_LAMBDA
            else if (dof_iterator->GetVariable() == LAMBDA)
            {
                if (dof_iterator->IsFree())
                {
                    rNode.GetSolutionStepValue(LAMBDA_EINS)
                    += Dx[dof_iterator->EquationId()];
                }
            }
            #endif
            else
            {
                // update for non-dynamics variable, i.e. Lagrange multiplier
                if (dof_iterator->IsFree())
                {
                    dof_iterator->GetSolutionStepValue()
                    += Dx[dof_iterator->EquationId()];
                }
            }
        }

        KRATOS_CATCH("")
    }

    /**
     * initializes next newton step by calculating
     * u_{n+1-alpha}= u_n*alpha+U_n+1^k+1*(1-alpha)
     * @param r_model_part
     * @param A LHS matrix
     * @param Dx incremental update of primary variables
     * @param b RHS Vector
     */
    void InitializeNonLinIteration(ModelPart& r_model_part,
                                   TSystemMatrixType& A,
                                   TSystemVectorType& Dx,
                                   TSystemVectorType& b )
    {
        KRATOS_TRY

        ProcessInfo& CurrentProcessInfo = r_model_part.GetProcessInfo();

        ElementsArrayType& pElements = r_model_part.Elements();
        bool element_is_active;
        for (typename ElementsArrayType::iterator it = pElements.begin(); it != pElements.end(); ++it)
        {
            element_is_active = true;
            if(it->IsDefined(ACTIVE))
                element_is_active = it->Is(ACTIVE);
            if (it->Has(IS_INACTIVE))
                element_is_active |= (!it->GetValue(IS_INACTIVE));

            if (element_is_active)
                it->InitializeNonLinearIteration(CurrentProcessInfo);
        }

        bool condition_is_active;
        ConditionsArrayType& pConditions = r_model_part.Conditions();
        for (typename ConditionsArrayType::iterator it = pConditions.begin(); it != pConditions.end(); ++it)
        {
            condition_is_active = true;
            if( it->IsDefined( ACTIVE ) )
                condition_is_active = it->Is(ACTIVE);
            if (it->Has(IS_INACTIVE))
                condition_is_active |= (!it->GetValue(IS_INACTIVE));

            if (condition_is_active)
                it->InitializeNonLinearIteration(CurrentProcessInfo);
        }

        //Update nodal values and nodal velocities at mAlpha_f
        for(ModelPart::NodeIterator i = r_model_part.NodesBegin() ; i != r_model_part.NodesEnd() ; i++)
        {
            if( i->HasDofFor(DISPLACEMENT_X) )
            {
                i->GetSolutionStepValue(ACCELERATION_EINS_X)
                = 1.0/(mBeta*CurrentProcessInfo[DELTA_TIME]
                       *CurrentProcessInfo[DELTA_TIME])
                  * (i->GetSolutionStepValue(DISPLACEMENT_EINS_X)
                     -i->GetSolutionStepValue(DISPLACEMENT_NULL_X))
                  -1.0/(mBeta*CurrentProcessInfo[DELTA_TIME])
                  *i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_X)
                  -(1.0-2.0*mBeta)/(2.0*mBeta)*i->GetSolutionStepValue(ACCELERATION_NULL_X);

                i->GetSolutionStepValue(DISPLACEMENT_EINS_DT_X)
                = (i->GetSolutionStepValue(DISPLACEMENT_EINS_X)
                   -i->GetSolutionStepValue(DISPLACEMENT_NULL_X))
                  *mGamma/(mBeta*CurrentProcessInfo[DELTA_TIME])
                  -(mGamma-mBeta)/mBeta*(i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_X))
                  -(mGamma-2.0*mBeta)/(2.0*mBeta)*CurrentProcessInfo[DELTA_TIME]
                  *(i->GetSolutionStepValue(ACCELERATION_NULL_X));

                i->GetSolutionStepValue(ACCELERATION_X)
                = mAlpha_m*i->GetSolutionStepValue(ACCELERATION_NULL_X)
                  +(1.0-mAlpha_m)*i->GetSolutionStepValue(ACCELERATION_EINS_X);

                i->GetSolutionStepValue(DISPLACEMENT_DT_X)
                = mAlpha_f*i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_X)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(DISPLACEMENT_EINS_DT_X);

                i->GetSolutionStepValue(DISPLACEMENT_X)
                = mAlpha_f*i->GetSolutionStepValue(DISPLACEMENT_NULL_X)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(DISPLACEMENT_EINS_X);

            }
            if( i->HasDofFor(DISPLACEMENT_Y) )
            {
                i->GetSolutionStepValue(ACCELERATION_EINS_Y)
                = 1.0/(mBeta*CurrentProcessInfo[DELTA_TIME]
                       *CurrentProcessInfo[DELTA_TIME])
                  * (i->GetSolutionStepValue(DISPLACEMENT_EINS_Y)
                    -i->GetSolutionStepValue(DISPLACEMENT_NULL_Y))
                 -1.0/(mBeta*CurrentProcessInfo[DELTA_TIME])
                 *i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Y)
                 -(1.0-2.0*mBeta)/(2.0*mBeta)*
                 i->GetSolutionStepValue(ACCELERATION_NULL_Y);

                i->GetSolutionStepValue(DISPLACEMENT_EINS_DT_Y)
                =(i->GetSolutionStepValue(DISPLACEMENT_EINS_Y)
                  -i->GetSolutionStepValue(DISPLACEMENT_NULL_Y))
                 *mGamma/(mBeta*CurrentProcessInfo[DELTA_TIME])
                 -(mGamma-mBeta)/mBeta*(i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Y))
                 -(mGamma-2.0*mBeta)/(2.0*mBeta)*CurrentProcessInfo[DELTA_TIME]
                 *(i->GetSolutionStepValue(ACCELERATION_NULL_Y));

                i->GetSolutionStepValue(ACCELERATION_Y)
                =mAlpha_m*i->GetSolutionStepValue(ACCELERATION_NULL_Y)
                 +(1.0-mAlpha_m)*i->GetSolutionStepValue(ACCELERATION_EINS_Y);

                i->GetSolutionStepValue(DISPLACEMENT_DT_Y)
                = mAlpha_f*i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Y)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(DISPLACEMENT_EINS_DT_Y);

                i->GetSolutionStepValue(DISPLACEMENT_Y)
                = mAlpha_f*i->GetSolutionStepValue(DISPLACEMENT_NULL_Y)+(1.0-mAlpha_f)*
                  i->GetSolutionStepValue(DISPLACEMENT_EINS_Y);
            }
            if( i->HasDofFor(DISPLACEMENT_Z) )
            {
                i->GetSolutionStepValue(ACCELERATION_EINS_Z)
                = 1.0/(mBeta*CurrentProcessInfo[DELTA_TIME]
                       *CurrentProcessInfo[DELTA_TIME])
                  * (i->GetSolutionStepValue(DISPLACEMENT_EINS_Z)
                     -i->GetSolutionStepValue(DISPLACEMENT_NULL_Z))
                  -1.0/(mBeta*CurrentProcessInfo[DELTA_TIME])
                  *i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Z)
                  -(1.0-2.0*mBeta)/(2.0*mBeta)*i->GetSolutionStepValue(ACCELERATION_NULL_Z);

                i->GetSolutionStepValue(DISPLACEMENT_EINS_DT_Z)
                = (i->GetSolutionStepValue(DISPLACEMENT_EINS_Z)
                   -i->GetSolutionStepValue(DISPLACEMENT_NULL_Z))
                  *mGamma/(mBeta*CurrentProcessInfo[DELTA_TIME])-(mGamma-mBeta)/mBeta*
                  (i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Z))
                  -(mGamma-2.0*mBeta)/(2.0*mBeta)*CurrentProcessInfo[DELTA_TIME]
                  *(i->GetSolutionStepValue(ACCELERATION_NULL_Z));

                i->GetSolutionStepValue(ACCELERATION_Z)
                = mAlpha_m*i->GetSolutionStepValue(ACCELERATION_NULL_Z)
                  +(1.0-mAlpha_m)*i->GetSolutionStepValue(ACCELERATION_EINS_Z);

                i->GetSolutionStepValue(DISPLACEMENT_DT_Z)
                = mAlpha_f*i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Z)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(DISPLACEMENT_EINS_DT_Z);

                i->GetSolutionStepValue(DISPLACEMENT_Z)
                = mAlpha_f*i->GetSolutionStepValue(DISPLACEMENT_NULL_Z)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(DISPLACEMENT_EINS_Z);
            }
            if( i->HasDofFor(WATER_PRESSURE) )
            {
                i->GetSolutionStepValue(WATER_PRESSURE_EINS_ACCELERATION)
                = 1.0/(mBeta*CurrentProcessInfo[DELTA_TIME]*CurrentProcessInfo[DELTA_TIME])
                  * (i->GetSolutionStepValue(WATER_PRESSURE_EINS)
                     -i->GetSolutionStepValue(WATER_PRESSURE_NULL))
                  -1.0/(mBeta*CurrentProcessInfo[DELTA_TIME])
                  *i->GetSolutionStepValue(WATER_PRESSURE_NULL_DT)
                  -(1.0-2.0*mBeta)/(2.0*mBeta)*i->GetSolutionStepValue(WATER_PRESSURE_NULL_ACCELERATION);

                i->GetSolutionStepValue(WATER_PRESSURE_EINS_DT)
                = (i->GetSolutionStepValue(WATER_PRESSURE_EINS)
                   -i->GetSolutionStepValue(WATER_PRESSURE_NULL))
                  *mGamma/(mBeta*CurrentProcessInfo[DELTA_TIME])
                  -(mGamma-mBeta)/mBeta*(i->GetSolutionStepValue(WATER_PRESSURE_NULL_DT))
                  -(mGamma-2.0*mBeta)/(2.0*mBeta)*CurrentProcessInfo[DELTA_TIME]
                  *(i->GetSolutionStepValue(WATER_PRESSURE_NULL_ACCELERATION));

                i->GetSolutionStepValue(WATER_PRESSURE_ACCELERATION)
                = mAlpha_m*i->GetSolutionStepValue(WATER_PRESSURE_NULL_ACCELERATION)
                  +(1.0-mAlpha_m)*i->GetSolutionStepValue(WATER_PRESSURE_EINS_ACCELERATION);

                i->GetSolutionStepValue(WATER_PRESSURE_DT)
                = mAlpha_f*i->GetSolutionStepValue(WATER_PRESSURE_NULL_DT)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(WATER_PRESSURE_EINS_DT);

                i->GetSolutionStepValue(WATER_PRESSURE)
                = mAlpha_f*i->GetSolutionStepValue(WATER_PRESSURE_NULL)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(WATER_PRESSURE_EINS);
            }
            if( i->HasDofFor(AIR_PRESSURE) )
            {
                i->GetSolutionStepValue(AIR_PRESSURE_EINS_ACCELERATION)
                = 1.0/(mBeta*CurrentProcessInfo[DELTA_TIME]*CurrentProcessInfo[DELTA_TIME])
                  * (i->GetSolutionStepValue(AIR_PRESSURE_EINS)
                     -i->GetSolutionStepValue(AIR_PRESSURE_NULL))
                  -1.0/(mBeta*CurrentProcessInfo[DELTA_TIME])
                  *i->GetSolutionStepValue(AIR_PRESSURE_NULL_DT)
                  -(1.0-2.0*mBeta)/(2.0*mBeta)*i->GetSolutionStepValue(AIR_PRESSURE_NULL_ACCELERATION);

                i->GetSolutionStepValue(AIR_PRESSURE_EINS_DT)
                = (i->GetSolutionStepValue(AIR_PRESSURE_EINS)
                   -i->GetSolutionStepValue(AIR_PRESSURE_NULL))
                  *mGamma/(mBeta*CurrentProcessInfo[DELTA_TIME])
                  -(mGamma-mBeta)/mBeta*
                  (i->GetSolutionStepValue(AIR_PRESSURE_NULL_DT))
                  -(mGamma-2.0*mBeta)/(2.0*mBeta)*CurrentProcessInfo[DELTA_TIME]
                  *(i->GetSolutionStepValue(AIR_PRESSURE_NULL_ACCELERATION));

                i->GetSolutionStepValue(AIR_PRESSURE_ACCELERATION)
                = mAlpha_m*i->GetSolutionStepValue(AIR_PRESSURE_NULL_ACCELERATION)
                  +(1.0-mAlpha_m)*i->GetSolutionStepValue(AIR_PRESSURE_EINS_ACCELERATION);

                i->GetSolutionStepValue(AIR_PRESSURE_DT)
                = mAlpha_f*i->GetSolutionStepValue(AIR_PRESSURE_NULL_DT)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(AIR_PRESSURE_EINS_DT);

                i->GetSolutionStepValue(AIR_PRESSURE)
                = mAlpha_f*i->GetSolutionStepValue(AIR_PRESSURE_NULL)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(AIR_PRESSURE_EINS);
            }
            #ifdef ENABLE_ROTATION
            if( i->HasDofFor(ROTATION_X) )
            {
                i->GetSolutionStepValue(ANGULAR_ACCELERATION_EINS_X)
                = 1.0/(mBeta*CurrentProcessInfo[DELTA_TIME]
                       *CurrentProcessInfo[DELTA_TIME])
                  * (i->GetSolutionStepValue(ROTATION_EINS_X)
                     -i->GetSolutionStepValue(ROTATION_NULL_X))
                  -1.0/(mBeta*CurrentProcessInfo[DELTA_TIME])
                  *i->GetSolutionStepValue(ROTATION_NULL_DT_X)
                  -(1.0-2.0*mBeta)/(2.0*mBeta)*i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_X);

                i->GetSolutionStepValue(ROTATION_EINS_DT_X)
                = (i->GetSolutionStepValue(ROTATION_EINS_X)
                   -i->GetSolutionStepValue(ROTATION_NULL_X))
                  *mGamma/(mBeta*CurrentProcessInfo[DELTA_TIME])
                  -(mGamma-mBeta)/mBeta*(i->GetSolutionStepValue(ROTATION_NULL_DT_X))
                  -(mGamma-2.0*mBeta)/(2.0*mBeta)*CurrentProcessInfo[DELTA_TIME]
                  *(i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_X));

                i->GetSolutionStepValue(ANGULAR_ACCELERATION_X)
                = mAlpha_m*i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_X)
                  +(1.0-mAlpha_m)*i->GetSolutionStepValue(ANGULAR_ACCELERATION_EINS_X);

                i->GetSolutionStepValue(ROTATION_DT_X)
                = mAlpha_f*i->GetSolutionStepValue(ROTATION_NULL_DT_X)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(ROTATION_EINS_DT_X);

                i->GetSolutionStepValue(ROTATION_X)
                = mAlpha_f*i->GetSolutionStepValue(ROTATION_NULL_X)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(ROTATION_EINS_X);

            }
            if( i->HasDofFor(ROTATION_Y) )
            {
                i->GetSolutionStepValue(ANGULAR_ACCELERATION_EINS_Y)
                =1.0/(mBeta*CurrentProcessInfo[DELTA_TIME]*CurrentProcessInfo[DELTA_TIME])
                 * (i->GetSolutionStepValue(ROTATION_EINS_Y)
                    -i->GetSolutionStepValue(ROTATION_NULL_Y))
                 -1.0/(mBeta*CurrentProcessInfo[DELTA_TIME])
                 *i->GetSolutionStepValue(ROTATION_NULL_DT_Y)
                 -(1.0-2.0*mBeta)/(2.0*mBeta)*
                 i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_Y);

                i->GetSolutionStepValue(ROTATION_EINS_DT_Y)
                =(i->GetSolutionStepValue(ROTATION_EINS_Y)
                  -i->GetSolutionStepValue(ROTATION_NULL_Y))
                 *mGamma/(mBeta*CurrentProcessInfo[DELTA_TIME])
                 -(mGamma-mBeta)/mBeta*(i->GetSolutionStepValue(ROTATION_NULL_DT_Y))
                 -(mGamma-2.0*mBeta)/(2.0*mBeta)*CurrentProcessInfo[DELTA_TIME]
                 *(i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_Y));

                i->GetSolutionStepValue(ANGULAR_ACCELERATION_Y)
                =mAlpha_m*i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_Y)
                 +(1.0-mAlpha_m)*i->GetSolutionStepValue(ANGULAR_ACCELERATION_EINS_Y);

                i->GetSolutionStepValue(ROTATION_DT_Y)
                = mAlpha_f*i->GetSolutionStepValue(ROTATION_NULL_DT_Y)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(ROTATION_EINS_DT_Y);

                i->GetSolutionStepValue(ROTATION_Y)
                = mAlpha_f*i->GetSolutionStepValue(ROTATION_NULL_Y)+(1.0-mAlpha_f)*
                  i->GetSolutionStepValue(ROTATION_EINS_Y);
            }
            if( i->HasDofFor(ROTATION_Z) )
            {
                i->GetSolutionStepValue(ANGULAR_ACCELERATION_EINS_Z)
                = 1.0/(mBeta*CurrentProcessInfo[DELTA_TIME]*CurrentProcessInfo[DELTA_TIME])
                  * (i->GetSolutionStepValue(ROTATION_EINS_Z)
                     -i->GetSolutionStepValue(ROTATION_NULL_Z))
                  -1.0/(mBeta*CurrentProcessInfo[DELTA_TIME])
                  *i->GetSolutionStepValue(ROTATION_NULL_DT_Z)
                  -(1.0-2.0*mBeta)/(2.0*mBeta)*i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_Z);

                i->GetSolutionStepValue(ROTATION_EINS_DT_Z)
                = (i->GetSolutionStepValue(ROTATION_EINS_Z)
                   -i->GetSolutionStepValue(ROTATION_NULL_Z))
                  *mGamma/(mBeta*CurrentProcessInfo[DELTA_TIME])-(mGamma-mBeta)/mBeta*
                  (i->GetSolutionStepValue(ROTATION_NULL_DT_Z))
                  -(mGamma-2.0*mBeta)/(2.0*mBeta)*CurrentProcessInfo[DELTA_TIME]
                  *(i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_Z));

                i->GetSolutionStepValue(ANGULAR_ACCELERATION_Z)
                = mAlpha_m*i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_Z)
                  +(1.0-mAlpha_m)*i->GetSolutionStepValue(ANGULAR_ACCELERATION_EINS_Z);

                i->GetSolutionStepValue(ROTATION_DT_Z)
                = mAlpha_f*i->GetSolutionStepValue(ROTATION_NULL_DT_Z)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(ROTATION_EINS_DT_Z);

                i->GetSolutionStepValue(ROTATION_Z)
                = mAlpha_f*i->GetSolutionStepValue(ROTATION_NULL_Z)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(ROTATION_EINS_Z);
            }
            #endif
            #ifdef ENABLE_LAMBDA
            if( i->HasDofFor(LAMBDA) )
            {
                i->GetSolutionStepValue(LAMBDA_EINS_DT2)
                = 1.0/(mBeta*CurrentProcessInfo[DELTA_TIME]
                       *CurrentProcessInfo[DELTA_TIME])
                  * (i->GetSolutionStepValue(LAMBDA_EINS)
                     -i->GetSolutionStepValue(LAMBDA_NULL))
                  -1.0/(mBeta*CurrentProcessInfo[DELTA_TIME])
                  *i->GetSolutionStepValue(LAMBDA_NULL_DT)
                  -(1.0-2.0*mBeta)/(2.0*mBeta)*i->GetSolutionStepValue(LAMBDA_NULL_DT2);

                i->GetSolutionStepValue(LAMBDA_EINS_DT)
                = (i->GetSolutionStepValue(LAMBDA_EINS)
                   -i->GetSolutionStepValue(LAMBDA_NULL))
                  *mGamma/(mBeta*CurrentProcessInfo[DELTA_TIME])
                  -(mGamma-mBeta)/mBeta*(i->GetSolutionStepValue(LAMBDA_NULL_DT))
                  -(mGamma-2.0*mBeta)/(2.0*mBeta)*CurrentProcessInfo[DELTA_TIME]
                  *(i->GetSolutionStepValue(LAMBDA_NULL_DT2));

                i->GetSolutionStepValue(LAMBDA_DT2)
                = mAlpha_m*i->GetSolutionStepValue(LAMBDA_NULL_DT2)
                  +(1.0-mAlpha_m)*i->GetSolutionStepValue(LAMBDA_EINS_DT2);

                i->GetSolutionStepValue(LAMBDA_DT)
                = mAlpha_f*i->GetSolutionStepValue(LAMBDA_NULL_DT)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(LAMBDA_EINS_DT);

                i->GetSolutionStepValue(LAMBDA)
                = mAlpha_f*i->GetSolutionStepValue(LAMBDA_NULL)
                  +(1.0-mAlpha_f)*i->GetSolutionStepValue(LAMBDA_EINS);
            }
            #endif
        }

        //For total Lagrangian
        for(ModelPart::NodeIterator i = r_model_part.NodesBegin() ;
                i != r_model_part.NodesEnd() ; ++i)
        {
            (i)->X() = (i)->X0() + i->GetSolutionStepValue(DISPLACEMENT_X);
            (i)->Y() = (i)->Y0() + i->GetSolutionStepValue(DISPLACEMENT_Y);
            (i)->Z() = (i)->Z0() + i->GetSolutionStepValue(DISPLACEMENT_Z);
        }

        KRATOS_CATCH("")
    }

    /**
     */
    void FinalizeNonLinIteration(
        ModelPart& r_model_part,
        TSystemMatrixType& A,
        TSystemVectorType& Dx,
        TSystemVectorType& b)
    {
        KRATOS_TRY

        ProcessInfo& CurrentProcessInfo = r_model_part.GetProcessInfo();

        ElementsArrayType& pElements = r_model_part.Elements();
        bool element_is_active;
        for (typename ElementsArrayType::iterator it = pElements.begin(); it != pElements.end(); ++it)
        {
            element_is_active = true;
            if(it->IsDefined(ACTIVE))
                element_is_active = it->Is(ACTIVE);
            if (it->Has(IS_INACTIVE))
                element_is_active |= (!it->GetValue(IS_INACTIVE));

            if (element_is_active)
                it->FinalizeNonLinearIteration(CurrentProcessInfo);
        }

        bool condition_is_active;
        ConditionsArrayType& pConditions = r_model_part.Conditions();
        for (typename ConditionsArrayType::iterator it = pConditions.begin(); it != pConditions.end(); ++it)
        {
            condition_is_active = true;
            if( it->IsDefined( ACTIVE ) )
                condition_is_active = it->Is(ACTIVE);
            if (it->Has(IS_INACTIVE))
                condition_is_active |= (!it->GetValue(IS_INACTIVE));

            if (condition_is_active)
                it->FinalizeNonLinearIteration(CurrentProcessInfo);
        }

        KRATOS_CATCH("")
    }

    /**
     * initializes time step solution
     * only for reasons if the time step solution is restarted
     * @param r_model_part
     * @param A LHS matrix
     * @param Dx incremental update of primary variables
     * @param b RHS Vector
     */
    void InitializeSolutionStep(
        ModelPart& r_model_part,
        TSystemMatrixType& A,
        TSystemVectorType& Dx,
        TSystemVectorType& b)
    {
        KRATOS_TRY

        ProcessInfo CurrentProcessInfo= r_model_part.GetProcessInfo();

        BaseType::InitializeSolutionStep(r_model_part,A,Dx,b);
        //Update nodal values and nodal velocities at mAlpha_f
        for(ModelPart::NodeIterator i = r_model_part.NodesBegin() ;
                i != r_model_part.NodesEnd() ; i++)
        {
            if( i->HasDofFor(DISPLACEMENT_X) &&  i->GetDof(DISPLACEMENT_X).IsFree())
            {
                i->GetSolutionStepValue(ACCELERATION_EINS_X)=
                    i->GetSolutionStepValue(ACCELERATION_NULL_X);
                i->GetSolutionStepValue(DISPLACEMENT_EINS_DT_X)=
                    i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_X);
                i->GetSolutionStepValue(DISPLACEMENT_EINS_X )=
                    i->GetSolutionStepValue(DISPLACEMENT_NULL_X);
            }
            if( i->HasDofFor(DISPLACEMENT_Y) &&  i->GetDof(DISPLACEMENT_Y).IsFree())
            {
                i->GetSolutionStepValue(ACCELERATION_EINS_Y)=
                    i->GetSolutionStepValue(ACCELERATION_NULL_Y);
                i->GetSolutionStepValue(DISPLACEMENT_EINS_DT_Y)=
                    i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Y);
                i->GetSolutionStepValue(DISPLACEMENT_EINS_Y)=
                    i->GetSolutionStepValue(DISPLACEMENT_NULL_Y);
            }
            if( i->HasDofFor(DISPLACEMENT_Z) && i->GetDof(DISPLACEMENT_Z).IsFree() )
            {
                i->GetSolutionStepValue(ACCELERATION_EINS_Y)=
                    i->GetSolutionStepValue(ACCELERATION_NULL_Z);
                i->GetSolutionStepValue(DISPLACEMENT_EINS_DT_Z)=
                    i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Z);
                i->GetSolutionStepValue(DISPLACEMENT_EINS_Z)=
                    i->GetSolutionStepValue(DISPLACEMENT_NULL_Z);
            }
            if( i->HasDofFor(WATER_PRESSURE) && i->GetDof(WATER_PRESSURE).IsFree())
            {
                i->GetSolutionStepValue(WATER_PRESSURE_EINS_DT)=
                    i->GetSolutionStepValue(WATER_PRESSURE_NULL_DT);
                i->GetSolutionStepValue(WATER_PRESSURE_EINS_ACCELERATION)=
                    i->GetSolutionStepValue(WATER_PRESSURE_NULL_ACCELERATION);
                i->GetSolutionStepValue(WATER_PRESSURE_EINS)=
                    i->GetSolutionStepValue(WATER_PRESSURE_NULL);
            }
            if( i->HasDofFor(AIR_PRESSURE) && i->GetDof(AIR_PRESSURE).IsFree())
            {
                i->GetSolutionStepValue(AIR_PRESSURE_EINS_DT)=
                    i->GetSolutionStepValue(AIR_PRESSURE_NULL_DT);
                i->GetSolutionStepValue(AIR_PRESSURE_EINS_ACCELERATION)=
                    i->GetSolutionStepValue(AIR_PRESSURE_NULL_ACCELERATION);
                i->GetSolutionStepValue(AIR_PRESSURE_EINS)=
                    i->GetSolutionStepValue(AIR_PRESSURE_NULL);
            }
            #ifdef ENABLE_LAMBDA
            if( i->HasDofFor(LAMBDA) &&  i->GetDof(LAMBDA).IsFree())
            {
                i->GetSolutionStepValue(LAMBDA_EINS_DT2)=
                    i->GetSolutionStepValue(LAMBDA_NULL_DT2);
                i->GetSolutionStepValue(LAMBDA_EINS_DT)=
                    i->GetSolutionStepValue(LAMBDA_NULL_DT);
                i->GetSolutionStepValue(LAMBDA_EINS )=
                    i->GetSolutionStepValue(LAMBDA_NULL);
            }
            #endif
        }

        KRATOS_CATCH("")
    }

    /**
     * finalizes time step solution
     * by setting u_n= u_n+1^k etc.
     * u_{n+1-alpha}= u_n*alpha+U_n+1^k+1*(1-alpha)
     * @param r_model_part
     * @param A LHS matrix
     * @param Dx incremental update of primary variables
     * @param b RHS Vector
     */
    void FinalizeSolutionStep(
        ModelPart& r_model_part,
        TSystemMatrixType& A,
        TSystemVectorType& Dx,
        TSystemVectorType& b)
    {
        ProcessInfo& CurrentProcessInfo = r_model_part.GetProcessInfo();

        KRATOS_WATCH(CurrentProcessInfo[FIRST_TIME_STEP])

        // we manually FinalizeSolutionStep for each entities because the parent function is multithreaded
        ElementsArrayType& pElements = r_model_part.Elements();
        for (typename ElementsArrayType::ptr_iterator it = pElements.ptr_begin(); it != pElements.ptr_end(); ++it)
        {
            if ( (*it)->Is(ACTIVE) )
                (*it)->FinalizeSolutionStep(CurrentProcessInfo);
        }

        ConditionsArrayType& pConditions = r_model_part.Conditions();
        for (typename ConditionsArrayType::ptr_iterator it = pConditions.ptr_begin(); it != pConditions.ptr_end(); ++it)
        {
            if ( (*it)->Is(ACTIVE) )
                (*it)->FinalizeSolutionStep(CurrentProcessInfo);
        }

        if(CurrentProcessInfo[CALCULATE_INSITU_STRESS])
        {
            for(ModelPart::NodeIterator i = r_model_part.NodesBegin() ; i != r_model_part.NodesEnd() ; ++i)
            {
                if( i->HasDofFor(DISPLACEMENT_X))
                {
                    i->GetSolutionStepValue(DISPLACEMENT_OLD_X) = 0.0;
                    i->GetSolutionStepValue(DISPLACEMENT_NULL_X) = 0.0;
                    i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_X) = 0.0;
                    i->GetSolutionStepValue(ACCELERATION_NULL_X) = 0.0;
                }
                if( i->HasDofFor(DISPLACEMENT_Y) )
                {
                    i->GetSolutionStepValue(DISPLACEMENT_OLD_Y) = 0.0;
                    i->GetSolutionStepValue(DISPLACEMENT_NULL_Y) = 0.0;
                    i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Y) = 0.0;
                    i->GetSolutionStepValue(ACCELERATION_NULL_Y) = 0.0;
                }
                if( i->HasDofFor(DISPLACEMENT_Z) )
                {
                    i->GetSolutionStepValue(DISPLACEMENT_OLD_Z) = 0.0;
                    i->GetSolutionStepValue(DISPLACEMENT_NULL_Z) = 0.0;
                    i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Z) = 0.0;
                    i->GetSolutionStepValue(ACCELERATION_NULL_Z) = 0.0;
                }
            }
        }
        else
        {
            for(ModelPart::NodeIterator i = r_model_part.NodesBegin() ; i != r_model_part.NodesEnd() ; ++i)
            {
                if( i->HasDofFor(DISPLACEMENT_X))
                {
                    i->GetSolutionStepValue(DISPLACEMENT_OLD_X) = i->GetSolutionStepValue(DISPLACEMENT_X);

                    if(CurrentProcessInfo[FIRST_TIME_STEP])
                    {
                        i->GetSolutionStepValue(ACCELERATION_NULL_X) =i->GetSolutionStepValue(ACCELERATION_X);
                        i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_X) = i->GetSolutionStepValue(DISPLACEMENT_DT_X);
                        i->GetSolutionStepValue(DISPLACEMENT_NULL_X) = i->GetSolutionStepValue(DISPLACEMENT_X);
                    }
                    else
                    {
                        i->GetSolutionStepValue(DISPLACEMENT_NULL_X) = i->GetSolutionStepValue(DISPLACEMENT_EINS_X);
                        i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_X) = i->GetSolutionStepValue(DISPLACEMENT_EINS_DT_X);
                        i->GetSolutionStepValue(ACCELERATION_NULL_X) = i->GetSolutionStepValue(ACCELERATION_EINS_X);
                    }
                }
                if( i->HasDofFor(DISPLACEMENT_Y) )
                {
                    i->GetSolutionStepValue(DISPLACEMENT_OLD_Y) = i->GetSolutionStepValue(DISPLACEMENT_Y);

                    if(CurrentProcessInfo[FIRST_TIME_STEP])
                    {
                        i->GetSolutionStepValue(ACCELERATION_NULL_Y) = i->GetSolutionStepValue(ACCELERATION_Y);
                        i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Y) = i->GetSolutionStepValue(DISPLACEMENT_DT_Y);
                        i->GetSolutionStepValue(DISPLACEMENT_NULL_Y) = i->GetSolutionStepValue(DISPLACEMENT_Y);
                    }
                    else
                    {
                        i->GetSolutionStepValue(DISPLACEMENT_NULL_Y) = i->GetSolutionStepValue(DISPLACEMENT_EINS_Y);
                        i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Y) = i->GetSolutionStepValue(DISPLACEMENT_EINS_DT_Y);
                        i->GetSolutionStepValue(ACCELERATION_NULL_Y) = i->GetSolutionStepValue(ACCELERATION_EINS_Y);
                    }
                }
                if( i->HasDofFor(DISPLACEMENT_Z) )
                {
                    i->GetSolutionStepValue(DISPLACEMENT_OLD_Z) = i->GetSolutionStepValue(DISPLACEMENT_Z);

                    if(CurrentProcessInfo[FIRST_TIME_STEP])
                    {
                        i->GetSolutionStepValue(ACCELERATION_NULL_Z) = i->GetSolutionStepValue(ACCELERATION_Z);
                        i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Z) = i->GetSolutionStepValue(DISPLACEMENT_DT_Z);
                        i->GetSolutionStepValue(DISPLACEMENT_NULL_Z) = i->GetSolutionStepValue(DISPLACEMENT_Z);
                    }
                    else
                    {
                        i->GetSolutionStepValue(DISPLACEMENT_NULL_Z) = i->GetSolutionStepValue(DISPLACEMENT_EINS_Z);
                        i->GetSolutionStepValue(DISPLACEMENT_NULL_DT_Z) = i->GetSolutionStepValue(DISPLACEMENT_EINS_DT_Z);
                        i->GetSolutionStepValue(ACCELERATION_NULL_Z) = i->GetSolutionStepValue(ACCELERATION_EINS_Z);
                    }
                }
                if( i->HasDofFor(WATER_PRESSURE))
                {
                    if(CurrentProcessInfo[FIRST_TIME_STEP])
                    {
                        i->GetSolutionStepValue(WATER_PRESSURE_NULL_ACCELERATION) = i->GetSolutionStepValue(WATER_PRESSURE_ACCELERATION);
                        i->GetSolutionStepValue(WATER_PRESSURE_NULL_DT) = i->GetSolutionStepValue(WATER_PRESSURE_DT);
                        i->GetSolutionStepValue(WATER_PRESSURE_NULL) = i->GetSolutionStepValue(WATER_PRESSURE);
                    }
                    else
                    {
                        i->GetSolutionStepValue(WATER_PRESSURE_NULL_DT) = i->GetSolutionStepValue(WATER_PRESSURE_EINS_DT);
                        i->GetSolutionStepValue(WATER_PRESSURE_NULL) = i->GetSolutionStepValue(WATER_PRESSURE_EINS);
                        i->GetSolutionStepValue(WATER_PRESSURE_NULL_ACCELERATION) = i->GetSolutionStepValue(WATER_PRESSURE_EINS_ACCELERATION);
                    }
                }
                if( i->HasDofFor(AIR_PRESSURE) )
                {
                    if(CurrentProcessInfo[FIRST_TIME_STEP])
                    {
                        i->GetSolutionStepValue(AIR_PRESSURE_NULL_ACCELERATION) = i->GetSolutionStepValue(AIR_PRESSURE_ACCELERATION);
                        i->GetSolutionStepValue(AIR_PRESSURE_NULL_DT) = i->GetSolutionStepValue(AIR_PRESSURE_DT);
                        i->GetSolutionStepValue(AIR_PRESSURE_NULL) = i->GetSolutionStepValue(AIR_PRESSURE);
                    }
                    else
                    {
                        i->GetSolutionStepValue(AIR_PRESSURE_NULL_DT) = i->GetSolutionStepValue(AIR_PRESSURE_EINS_DT);
                        i->GetSolutionStepValue(AIR_PRESSURE_NULL) = i->GetSolutionStepValue(AIR_PRESSURE_EINS);
                        i->GetSolutionStepValue(AIR_PRESSURE_NULL_ACCELERATION) = i->GetSolutionStepValue(AIR_PRESSURE_EINS_ACCELERATION);
                    }
                }
                #ifdef ENABLE_ROTATION
                if( i->HasDofFor(ROTATION_X))
                {
                    if(CurrentProcessInfo[FIRST_TIME_STEP])
                    {
                        i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_X) = i->GetSolutionStepValue(ANGULAR_ACCELERATION_X);
                        i->GetSolutionStepValue(ROTATION_NULL_DT_X) = i->GetSolutionStepValue(ROTATION_DT_X);
                        i->GetSolutionStepValue(ROTATION_NULL_X) = i->GetSolutionStepValue(ROTATION_X);
                    }
                    else
                    {
                        i->GetSolutionStepValue(ROTATION_NULL_X) = i->GetSolutionStepValue(ROTATION_EINS_X);
                        i->GetSolutionStepValue(ROTATION_NULL_DT_X) = i->GetSolutionStepValue(ROTATION_EINS_DT_X);
                        i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_X) = i->GetSolutionStepValue(ANGULAR_ACCELERATION_EINS_X);
                    }
                }
                if( i->HasDofFor(ROTATION_Y) )
                {
                    if(CurrentProcessInfo[FIRST_TIME_STEP])
                    {
                        i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_Y) = i->GetSolutionStepValue(ANGULAR_ACCELERATION_Y);
                        i->GetSolutionStepValue(ROTATION_NULL_DT_Y) = i->GetSolutionStepValue(ROTATION_DT_Y);
                        i->GetSolutionStepValue(ROTATION_NULL_Y) = i->GetSolutionStepValue(ROTATION_Y);
                    }
                    else
                    {
                        i->GetSolutionStepValue(ROTATION_NULL_Y) = i->GetSolutionStepValue(ROTATION_EINS_Y);
                        i->GetSolutionStepValue(ROTATION_NULL_DT_Y) = i->GetSolutionStepValue(ROTATION_EINS_DT_Y);
                        i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_Y) = i->GetSolutionStepValue(ANGULAR_ACCELERATION_EINS_Y);
                    }
                }
                if( i->HasDofFor(ROTATION_Z) )
                {
                    if(CurrentProcessInfo[FIRST_TIME_STEP])
                    {
                        i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_Z) = i->GetSolutionStepValue(ANGULAR_ACCELERATION_Z);
                        i->GetSolutionStepValue(ROTATION_NULL_DT_Z) = i->GetSolutionStepValue(ROTATION_DT_Z);
                        i->GetSolutionStepValue(ROTATION_NULL_Z) = i->GetSolutionStepValue(ROTATION_Z);
                    }
                    else
                    {
                        i->GetSolutionStepValue(ROTATION_NULL_Z) = i->GetSolutionStepValue(ROTATION_EINS_Z);
                        i->GetSolutionStepValue(ROTATION_NULL_DT_Z) = i->GetSolutionStepValue(ROTATION_EINS_DT_Z);
                        i->GetSolutionStepValue(ANGULAR_ACCELERATION_NULL_Z) = i->GetSolutionStepValue(ANGULAR_ACCELERATION_EINS_Z);
                    }
                }
                #endif
                #ifdef ENABLE_LAMBDA
                if( i->HasDofFor(LAMBDA))
                {
                    i->GetSolutionStepValue(LAMBDA_OLD) = i->GetSolutionStepValue(LAMBDA);

                    if(CurrentProcessInfo[FIRST_TIME_STEP])
                    {
                        i->GetSolutionStepValue(LAMBDA_NULL_DT2) = i->GetSolutionStepValue(LAMBDA_DT2);
                        i->GetSolutionStepValue(LAMBDA_NULL_DT) = i->GetSolutionStepValue(LAMBDA_DT);
                        i->GetSolutionStepValue(LAMBDA_NULL) = i->GetSolutionStepValue(LAMBDA);
                    }
                    else
                    {
                        i->GetSolutionStepValue(LAMBDA_NULL) = i->GetSolutionStepValue(LAMBDA_EINS);
                        i->GetSolutionStepValue(LAMBDA_NULL_DT) = i->GetSolutionStepValue(LAMBDA_EINS_DT);
                        i->GetSolutionStepValue(LAMBDA_NULL_DT2) =i->GetSolutionStepValue(LAMBDA_EINS_DT2);
                    }
                }
                #endif
            }
        }
    }
    //***************************************************************************
    //***************************************************************************

    /** this function is designed to be called in the builder and solver to introduce*/

    void CalculateSystemContributions(
        Element::Pointer rCurrentElement,
        LocalSystemMatrixType& LHS_Contribution,
        LocalSystemVectorType& RHS_Contribution,
        Element::EquationIdVectorType& EquationId,
        ProcessInfo& CurrentProcessInfo)
    {
        KRATOS_TRY

        (rCurrentElement)->CalculateLocalSystem(LHS_Contribution, RHS_Contribution, CurrentProcessInfo);

        (rCurrentElement)->EquationIdVector(EquationId,CurrentProcessInfo);

        if (CurrentProcessInfo[QUASI_STATIC_ANALYSIS])
        {
            Matrix DampingMatrix;

            (rCurrentElement)->CalculateDampingMatrix(DampingMatrix, CurrentProcessInfo);

            if (norm_frobenius(DampingMatrix) > 0.0) // filter out the element that did not calculate damping and then set it to a zero matrix
                AssembleTimeSpaceLHS_QuasiStatic(LHS_Contribution, DampingMatrix, CurrentProcessInfo);
            else
                AssembleTimeSpaceLHS_QuasiStatic(LHS_Contribution, CurrentProcessInfo);
        }
        else
        {
            Matrix DampingMatrix;

            Matrix MassMatrix;

            (rCurrentElement)->CalculateDampingMatrix(DampingMatrix, CurrentProcessInfo);

            (rCurrentElement)->CalculateMassMatrix(MassMatrix, CurrentProcessInfo);

            // KRATOS_WATCH(rCurrentElement->Id())
            // KRATOS_WATCH(MassMatrix)
            // KRATOS_WATCH(DampingMatrix)
            // KRATOS_WATCH(LHS_Contribution)

            if ((norm_frobenius(DampingMatrix) == 0.0) && (norm_frobenius(MassMatrix) > 0.0))
            {
                AddInertiaToRHS(rCurrentElement, RHS_Contribution, MassMatrix, CurrentProcessInfo);

                if ((DampingMatrix.size1() != MassMatrix.size1()) || (DampingMatrix.size2() != MassMatrix.size2()))
                    DampingMatrix.resize(MassMatrix.size1(), MassMatrix.size2(), false);
                noalias(DampingMatrix) = ZeroMatrix(MassMatrix.size1(), MassMatrix.size2());

                AssembleTimeSpaceLHS_Dynamics(LHS_Contribution, DampingMatrix, MassMatrix, CurrentProcessInfo);
            }
            else if ((norm_frobenius(DampingMatrix) > 0.0) && (norm_frobenius(MassMatrix) == 0.0))
            {
                AddDampingToRHS(rCurrentElement, RHS_Contribution, DampingMatrix, CurrentProcessInfo);

                AssembleTimeSpaceLHS_QuasiStatic(LHS_Contribution, DampingMatrix, CurrentProcessInfo);
            }
            else if ((norm_frobenius(DampingMatrix) > 0.0) && (norm_frobenius(MassMatrix) > 0.0))
            {
                AddInertiaToRHS(rCurrentElement, RHS_Contribution, MassMatrix, CurrentProcessInfo);

                AddDampingToRHS(rCurrentElement, RHS_Contribution, DampingMatrix, CurrentProcessInfo);

                AssembleTimeSpaceLHS_Dynamics(LHS_Contribution, DampingMatrix, MassMatrix, CurrentProcessInfo);
            }
        }

        KRATOS_CATCH("")
    }

    void Calculate_RHS_Contribution(
        Element::Pointer rCurrentElement,
        LocalSystemVectorType& RHS_Contribution,
        Element::EquationIdVectorType& EquationId,
        ProcessInfo& CurrentProcessInfo)
    {
        KRATOS_TRY

        (rCurrentElement)->CalculateRightHandSide(RHS_Contribution,CurrentProcessInfo);
        (rCurrentElement)->EquationIdVector(EquationId,CurrentProcessInfo);

        KRATOS_CATCH("")
    }


    /** functions totally analogous to the precedent but applied to
          the "condition" objects
    *       At the current status of implementation it does nothing
    */
    void Condition_CalculateSystemContributions(
        Condition::Pointer rCurrentCondition,
        LocalSystemMatrixType& LHS_Contribution,
        LocalSystemVectorType& RHS_Contribution,
        Element::EquationIdVectorType& EquationId,
        ProcessInfo& CurrentProcessInfo)
    {
        KRATOS_TRY

        Matrix DampingMatrix;

        (rCurrentCondition)->CalculateLocalSystem(LHS_Contribution,RHS_Contribution,CurrentProcessInfo);

        (rCurrentCondition)->EquationIdVector(EquationId, CurrentProcessInfo);

        (rCurrentCondition)->CalculateDampingMatrix(DampingMatrix, CurrentProcessInfo);

        if (norm_frobenius(DampingMatrix) > 0.0) // filter out the condition that did not calculate damping and then set it to a zero matrix
            AssembleTimeSpaceLHS_QuasiStatic(LHS_Contribution, DampingMatrix, CurrentProcessInfo);
        else
            AssembleTimeSpaceLHS_QuasiStatic(LHS_Contribution, CurrentProcessInfo);

        KRATOS_CATCH("")
    }


    /**       At the current status of implementation it does nothing
     */

    void Condition_Calculate_RHS_Contribution(
        Condition::Pointer rCurrentCondition,
        LocalSystemVectorType& RHS_Contribution,
        Element::EquationIdVectorType& EquationId,
        ProcessInfo& CurrentProcessInfo)
    {
        KRATOS_TRY

        (rCurrentCondition)->CalculateRightHandSide(RHS_Contribution,CurrentProcessInfo);
        (rCurrentCondition)->EquationIdVector(EquationId,CurrentProcessInfo);

        KRATOS_CATCH("")
    }

    /*@} */
    /**@name Operations */
    /*@{ */


    /*@} */
    /**@name Access */
    /*@{ */


    /*@} */
    /**@name Inquiry */
    /*@{ */


    /*@} */
    /**@name Friends */
    /*@{ */

protected:

    void AddInertiaToRHS(
        Element::Pointer rCurrentElement,
        LocalSystemVectorType& RHS_Contribution,
        LocalSystemMatrixType& M,
        ProcessInfo& CurrentProcessInfo)
    {
        // adding inertia contribution
        Vector acceleration;
        rCurrentElement->GetSecondDerivativesVector(acceleration, 0);
        noalias(RHS_Contribution) -= prod(M, acceleration );
    }

    void AddDampingToRHS(
        Element::Pointer rCurrentElement,
        LocalSystemVectorType& RHS_Contribution,
        LocalSystemMatrixType& D,
        ProcessInfo& CurrentProcessInfo)
    {
        // adding damping contribution
        Vector velocity;
        rCurrentElement->GetFirstDerivativesVector(velocity, 0);
        noalias(RHS_Contribution) -= prod(D, velocity );
    }

    void AssembleTimeSpaceLHS_QuasiStatic(
        LocalSystemMatrixType& LHS_Contribution,
        LocalSystemMatrixType& DampingMatrix,
        const ProcessInfo& CurrentProcessInfo)
    {
        const double Dt = CurrentProcessInfo[DELTA_TIME];
        double aux;

        // adding stiffness contribution to the dynamic stiffness
        aux = (1-mAlpha_f);
        LHS_Contribution *= aux;

        // adding damping contribution to the dynamic stiffness
        aux = (1-mAlpha_f)*mGamma/(mBeta*Dt);
        noalias(LHS_Contribution) += aux * DampingMatrix;
    }

    void AssembleTimeSpaceLHS_QuasiStatic(
        LocalSystemMatrixType& LHS_Contribution,
        const ProcessInfo& CurrentProcessInfo)
    {
        const double Dt = CurrentProcessInfo[DELTA_TIME];
        double aux;

        // adding stiffness contribution to the dynamic stiffness
        aux = (1-mAlpha_f);
        LHS_Contribution *= aux;
    }

    void AssembleTimeSpaceLHS_Dynamics(
        LocalSystemMatrixType& LHS_Contribution,
        LocalSystemMatrixType& DampingMatrix,
        LocalSystemMatrixType& MassMatrix,
        const ProcessInfo& CurrentProcessInfo)
    {
        const double Dt = CurrentProcessInfo[DELTA_TIME];
        double aux;

        // adding stiffness contribution to the dynamic stiffness
        aux = (1-mAlpha_f);
        LHS_Contribution *= aux;

        // adding damping contribution to the dynamic stiffness
        aux = (1-mAlpha_f)*mGamma/(mBeta*Dt);
        noalias(LHS_Contribution) += aux * DampingMatrix;

        // adding mass contribution to the dynamic stiffness
        aux = (1-mAlpha_m)*1/(mBeta*pow(Dt, 2));
        noalias(LHS_Contribution) += aux * MassMatrix;
    }

    /*@} */
    /**@name Protected member Variables */
    /*@{ */
    /*@} */
    /**@name Protected Operators*/
    /*@{ */
    /*@} */
    /**@name Protected Operations*/
    /*@{ */
    /*@} */
    /**@name Protected  Access */
    /*@{ */
    /*@} */
    /**@name Protected Inquiry */
    /*@{ */
    /*@} */
    /**@name Protected LifeCycle */
    /*@{ */
private:
    /**@name Static Member Variables */
    /*@{ */
    /*@} */
    /**@name Member Variables */
    /*@{ */

    double mAlpha_f;
    double mAlpha_m;
    double mBeta;
    double mGamma;

    /*@} */
    /**@name Private Operators*/
    /*@{ */
    /*@} */
    /**@name Private Operations*/
    /*@{ */
    /*@} */
    /**@name Private  Access */
    /*@{ */
    /*@} */
    /**@name Private Inquiry */
    /*@{ */
    /*@} */
    /**@name Un accessible methods */
    /*@{ */
}; /* Class Scheme */
}  /* namespace Kratos.*/

#undef ENABLE_ROTATION
#undef ENABLE_LAMBDA

#endif /* KRATOS_RESIDUALBASED_NEWMARK_SCHEME  defined */


