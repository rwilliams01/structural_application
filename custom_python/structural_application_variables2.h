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
//
//   Project Name:        Kratos
//   Last Modified by:    $Author: nagel $
//   Date:                $Date: 2009-03-20 08:55:34 $
//   Revision:            $Revision: 1.20 $
//
//


#if !defined(KRATOS_STRUCTURAL_APPLICATION_VARIABLES_2_H_INCLUDED )
#define  KRATOS_STRUCTURAL_APPLICATION_VARIABLES_2_H_INCLUDED



// System includes
#include <string>
#include <iostream>


// External includes


// Project includes
#include "includes/define.h"
#include "includes/node.h"
#include "includes/condition.h"
#include "includes/element.h"
#include "includes/ublas_interface.h"
#include "includes/constitutive_law.h"

#include "includes/legacy_structural_app_vars.h"
#include "includes/deprecated_variables.h"

namespace Kratos
{

// Variables definition
//	KRATOS_DEFINE_VARIABLE(int, WRINKLING_APPROACH )
//	KRATOS_DEFINE_VARIABLE(Matrix, GREEN_LAGRANGE_STRAIN_TENSOR )
//	KRATOS_DEFINE_VARIABLE(Matrix, PK2_STRESS_TENSOR )
//	KRATOS_DEFINE_VARIABLE(Matrix, AUXILIARY_MATRIX_1 )
//	KRATOS_DEFINE_VARIABLE(double, YOUNG_MODULUS )
//	KRATOS_DEFINE_VARIABLE(double, POISSON_RATIO )
//	KRATOS_DEFINE_VARIABLE(double, MU )
KRATOS_DEFINE_VARIABLE( double, ALPHA )
KRATOS_DEFINE_VARIABLE( double, RETRACTION_TIME )
//	KRATOS_DEFINE_VARIABLE(double, THICKNESS )
//	KRATOS_DEFINE_VARIABLE(double, NEGATIVE_FACE_PRESSURE )
//	KRATOS_DEFINE_VARIABLE(double, POSITIVE_FACE_PRESSURE )

typedef Matrix fix_matrix_33;
//typedef boost::numeric::ublas::bounded_matrix<double,3,3> fix_matrix_33;
typedef Vector array3;
//typedef array_1d<double,3> array3;

KRATOS_DEFINE_VARIABLE( ConstitutiveLaw::Pointer, CONSTITUTIVE_LAW_NO_INITIALIZE );

KRATOS_DEFINE_VARIABLE( fix_matrix_33 , MATRIX_A )
KRATOS_DEFINE_VARIABLE( fix_matrix_33 , MATRIX_B )
KRATOS_DEFINE_VARIABLE( fix_matrix_33 , MATRIX_D )
KRATOS_DEFINE_VARIABLE( array3, COMPOSITE_DIRECTION )
KRATOS_DEFINE_VARIABLE( array3, ORTHOTROPIC_YOUNG_MODULUS )
KRATOS_DEFINE_VARIABLE( array3, ORTHOTROPIC_SHEAR_MODULUS )
KRATOS_DEFINE_VARIABLE( Matrix, ORTHOTROPIC_POISSON_RATIO )
KRATOS_DEFINE_VARIABLE( Matrix , GEOMETRIC_STIFFNESS )
KRATOS_DEFINE_VARIABLE( Matrix , MATERIAL_DIRECTION )
KRATOS_DEFINE_VARIABLE( Matrix , JOINT_STIFFNESS )
KRATOS_DEFINE_VARIABLE( double , LINING_JOINT_STIFFNESS )
//CONTACT_LINK_MASTER is defined in condition.h
KRATOS_DEFINE_VARIABLE( Condition::Pointer, CONTACT_LINK_MASTER )
//CONTACT_LINK_SLAVE is defined in condition.h
KRATOS_DEFINE_VARIABLE( Condition::Pointer, CONTACT_LINK_SLAVE )
KRATOS_DEFINE_VARIABLE( Node<3>::Pointer,  NEAR_NODE)
KRATOS_DEFINE_VARIABLE( Point<3>, MASTER_CONTACT_LOCAL_POINT )
KRATOS_DEFINE_VARIABLE( Point<3>, MASTER_CONTACT_CURRENT_LOCAL_POINT )
KRATOS_DEFINE_VARIABLE( Point<3>, MASTER_CONTACT_LAST_CURRENT_LOCAL_POINT )
KRATOS_DEFINE_VARIABLE( Point<3>, SLAVE_CONTACT_LOCAL_POINT )
KRATOS_DEFINE_VARIABLE( Point<3>, MASTER_CONTACT_GLOBAL_POINT )
KRATOS_DEFINE_VARIABLE( Point<3>, MASTER_CONTACT_CURRENT_GLOBAL_POINT )
KRATOS_DEFINE_VARIABLE( Point<3>, SLAVE_CONTACT_GLOBAL_POINT )
KRATOS_DEFINE_VARIABLE( double , INSITU_STRESS_SCALE )
KRATOS_DEFINE_VARIABLE( double , LAGRANGE_SCALE )
KRATOS_DEFINE_VARIABLE( double, REFERENCE_PRESSURE )
KRATOS_DEFINE_VARIABLE( double, REFERENCE_WATER_PRESSURE )
KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_SCALE )
KRATOS_DEFINE_VARIABLE( double , OVERCONSOLIDATION_RATIO )
KRATOS_DEFINE_VARIABLE(double, EXCESS_PORE_WATER_PRESSURE)
KRATOS_DEFINE_VARIABLE(double, PRESSURE_P)
KRATOS_DEFINE_VARIABLE(double, PRESSURE_Q)
KRATOS_DEFINE_VARIABLE(Vector, COORDINATES)
KRATOS_DEFINE_VARIABLE(Vector, FLUID_FLOWS)

KRATOS_DEFINE_VARIABLE( double , CONTACT_PENETRATION )
KRATOS_DEFINE_VARIABLE( double, DAMAGE_E0 )
KRATOS_DEFINE_VARIABLE( double, DAMAGE_EF )

KRATOS_DEFINE_VARIABLE(double, BASE )
KRATOS_DEFINE_VARIABLE(double, HEIGHT )
KRATOS_DEFINE_VARIABLE(double, CROSS_AREA)
KRATOS_DEFINE_VARIABLE(double, AREA )
KRATOS_DEFINE_VARIABLE(double, AREA_X )
KRATOS_DEFINE_VARIABLE(double, AREA_Y )
KRATOS_DEFINE_VARIABLE(double, AREA_Z )
KRATOS_DEFINE_VARIABLE(Matrix, LOCAL_INERTIA)
KRATOS_DEFINE_VARIABLE(double, INERTIA_X)
KRATOS_DEFINE_VARIABLE(double, INERTIA_Y)
KRATOS_DEFINE_VARIABLE(double, INERTIA_Z)
KRATOS_DEFINE_VARIABLE(double, FC)
KRATOS_DEFINE_VARIABLE(double, FT)
KRATOS_DEFINE_VARIABLE(double, CONCRETE_YOUNG_MODULUS_C)
KRATOS_DEFINE_VARIABLE(double, CONCRETE_YOUNG_MODULUS_T)
KRATOS_DEFINE_VARIABLE(double, FRACTURE_ENERGY)
KRATOS_DEFINE_VARIABLE(double, CRUSHING_ENERGY)
KRATOS_DEFINE_VARIABLE(double, ELASTIC_ENERGY)
KRATOS_DEFINE_VARIABLE(double, PLASTIC_ENERGY)

//       KRATOS_DEFINE_VARIABLE(double, YIELD_STRESS)
KRATOS_DEFINE_VARIABLE(double, PLASTIC_MODULUS)
KRATOS_DEFINE_VARIABLE(double, PLASTICITY_INDICATOR)
KRATOS_DEFINE_VARIABLE(double, ISOTROPIC_HARDENING_MODULUS)
KRATOS_DEFINE_VARIABLE(double, KINEMATIC_HARDENING_MODULUS)
KRATOS_DEFINE_VARIABLE(int, ISOTROPIC_HARDENING_TYPE)
KRATOS_DEFINE_VARIABLE(int, KINEMATIC_HARDENING_TYPE)
KRATOS_DEFINE_VARIABLE(int, DRUCKER_PRAGER_MATCHING_TYPE)
KRATOS_DEFINE_VARIABLE(Matrix, HARDENING_POINTS_ON_CURVE)
KRATOS_DEFINE_VARIABLE(double, LAMNDA) // Load factor
KRATOS_DEFINE_VARIABLE(double, DAMAGE )
KRATOS_DEFINE_VARIABLE(double, ORTHOTROPIC_ANGLE)
KRATOS_DEFINE_VARIABLE(double, VOLUMEN_FRACTION)
KRATOS_DEFINE_VARIABLE(double, MAX_INTERNAL_FRICTION_ANGLE)
KRATOS_DEFINE_VARIABLE(double, DILATANCY_ANGLE)
KRATOS_DEFINE_VARIABLE(double, MAX_DILATANCY_ANGLE)
KRATOS_DEFINE_VARIABLE(double, COHESION)
KRATOS_DEFINE_VARIABLE(double, ISOTROPIC_ELASTIC_LIMIT)
KRATOS_DEFINE_VARIABLE(Vector, ORTHOTROPIC_ELASTIC_LIMIT)
KRATOS_DEFINE_VARIABLE(Vector, VECTOR_DAMAGE)
KRATOS_DEFINE_VARIABLE(Vector, ORTHOTROPIC_YOUNG_MODULUS_2D) // [E1 E2 G12]
KRATOS_DEFINE_VARIABLE(Vector, ORTHOTROPIC_POISSON_RATIO_2D) // [v12 v21]
KRATOS_DEFINE_VARIABLE(Matrix, GREEN_LAGRANGE_PLASTIC_STRAIN_TENSOR)
KRATOS_DEFINE_VARIABLE(Vector, ELASTIC_STRAIN_VECTOR)
KRATOS_DEFINE_VARIABLE(Vector, PLASTIC_STRAIN_VECTOR)
KRATOS_DEFINE_VARIABLE(Vector, CURRENT_STRAIN_VECTOR)
KRATOS_DEFINE_VARIABLE(Vector, INTEGRATION_POINT_STRAIN_VECTOR)
KRATOS_DEFINE_VARIABLE(double, EQUIVALENT_VOLUMETRIC_ELASTIC_STRAIN)
KRATOS_DEFINE_VARIABLE(double, EQUIVALENT_VOLUMETRIC_PLASTIC_STRAIN)
KRATOS_DEFINE_VARIABLE(double, EQUIVALENT_DEVIATORIC_ELASTIC_STRAIN)
KRATOS_DEFINE_VARIABLE(double, EQUIVALENT_DEVIATORIC_PLASTIC_STRAIN)
KRATOS_DEFINE_VARIABLE(double, EQUIVALENT_VOLUMETRIC_STRAIN)
KRATOS_DEFINE_VARIABLE(double, EQUIVALENT_DEVIATORIC_STRAIN)
KRATOS_DEFINE_VARIABLE(double, EQUIVALENT_STRAIN)
KRATOS_DEFINE_VARIABLE(Vector, ALMANSI_PLASTIC_STRAIN)
KRATOS_DEFINE_VARIABLE(Vector, ALMANSI_ELASTIC_STRAIN)
KRATOS_DEFINE_VARIABLE(Matrix, NODAL_STRESS)
KRATOS_DEFINE_VARIABLE(Vector, NODAL_STRESS_VECTOR)
KRATOS_DEFINE_VARIABLE(Matrix, NODAL_STRAIN)
KRATOS_DEFINE_VARIABLE(Vector, NODAL_STRAIN_VECTOR)
KRATOS_DEFINE_VARIABLE(Matrix, CONSTRAINT_MATRIX)
KRATOS_DEFINE_VARIABLE(Vector, PRESTRESS)
KRATOS_DEFINE_VARIABLE(double, PRESTRESS_ZZ)
KRATOS_DEFINE_VARIABLE(double, PRESTRESS_FACTOR )
KRATOS_DEFINE_VARIABLE(Vector, INITIAL_STRESS)
KRATOS_DEFINE_VARIABLE(Vector, CONSTRAINT_VECTOR)
KRATOS_DEFINE_VARIABLE(double, DISIPATION)
KRATOS_DEFINE_VARIABLE(int,     NODAL_VALUES)
KRATOS_DEFINE_VARIABLE(double,  NODAL_DAMAGE)
KRATOS_DEFINE_VARIABLE(bool, IS_TARGET)
KRATOS_DEFINE_VARIABLE(bool, IS_CONTACTOR)
KRATOS_DEFINE_VARIABLE(double, DAMPING_RATIO)
//KRATOS_DEFINE_VARIABLE(double, KINETIC_ENERGY)
KRATOS_DEFINE_VARIABLE(double, POTENCIAL_ENERGY)
KRATOS_DEFINE_VARIABLE(double, DEFORMATION_ENERGY)
KRATOS_DEFINE_VARIABLE(double, VON_MISES_STRESS)
KRATOS_DEFINE_VARIABLE(int, YIELD_STATE)
KRATOS_DEFINE_VARIABLE(double, YIELD_SURFACE)
KRATOS_DEFINE_VARIABLE(double, RHS_PRESSURE)
KRATOS_DEFINE_VARIABLE(bool, COMPUTE_TANGENT_MATRIX)
KRATOS_DEFINE_VARIABLE(double, IS_DISCRETE)
KRATOS_DEFINE_VARIABLE(double, TENSILE_STRENGTH)
KRATOS_DEFINE_VARIABLE(double, SHEAR_STRENGTH)
KRATOS_DEFINE_VARIABLE(double, VISCOUS_DAMPING)
KRATOS_DEFINE_VARIABLE(double, MAX_FRECUENCY1)
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(JOINT_FORCE_REACTION)
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(JOINT_MOMENT_REACTION)
//KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(INTERNAL_FORCE) //already put on variables.h (warning was appearing on Windows)
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(ELASTIC_BEDDING_STIFFNESS)
KRATOS_DEFINE_VARIABLE(bool, IS_BBAR)
KRATOS_DEFINE_VARIABLE(int, NEIGHBOUR_EXPANSION_LEVEL)
KRATOS_DEFINE_VARIABLE(int, STRESS_RECOVERY_TYPE)
KRATOS_DEFINE_VARIABLE(double, RAYLEIGH_DAMPING_ALPHA)
KRATOS_DEFINE_VARIABLE(double, RAYLEIGH_DAMPING_BETA)
KRATOS_DEFINE_VARIABLE(double, STABILISATION_FACTOR)
KRATOS_DEFINE_VARIABLE(double, SHEAR_MODULUS)
KRATOS_DEFINE_VARIABLE(double, SHEAR_MODULUS_EVOLUTION)
KRATOS_DEFINE_VARIABLE(Vector, RECOVERY_STRESSES)
KRATOS_DEFINE_VARIABLE(Vector, THREED_STRESSES)
KRATOS_DEFINE_VARIABLE(Vector, THREED_PRESTRESS)
KRATOS_DEFINE_VARIABLE(Vector, STRESSES_OLD)
KRATOS_DEFINE_VARIABLE(Vector, THREED_STRAIN)
KRATOS_DEFINE_VARIABLE(Vector, PRE_STRAIN_VECTOR)
KRATOS_DEFINE_VARIABLE(Vector, POST_STRAIN_VECTOR)
KRATOS_DEFINE_VARIABLE(Matrix, STRESS_TENSOR)
KRATOS_DEFINE_VARIABLE(Matrix, STRAIN_TENSOR)
KRATOS_DEFINE_VARIABLE(Matrix, ELASTIC_STRAIN_TENSOR)
KRATOS_DEFINE_VARIABLE(Matrix, PLASTIC_STRAIN_TENSOR)
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PRESCRIBED_DELTA_DISPLACEMENT)
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(LINE_LOAD)
KRATOS_DEFINE_VARIABLE(bool,   IS_CONTACT_NODE)
KRATOS_DEFINE_VARIABLE(double, LAGRANGE_MULTIPLIER)
KRATOS_DEFINE_VARIABLE(bool, HAS_STRAIN_AT_NODE)
KRATOS_DEFINE_VARIABLE(bool, HAS_STRESSES_AT_NODE)
KRATOS_DEFINE_VARIABLE( bool, HAS_NODAL_ERROR )
KRATOS_DEFINE_VARIABLE( bool, FORCE_EQUAL_ORDER_INTERPOLATION )
KRATOS_DEFINE_VARIABLE( double, NODAL_ERROR_1 )
KRATOS_DEFINE_VARIABLE( double, DUMMY_DOF )
KRATOS_DEFINE_VARIABLE( double, PRECONSOLIDATION_PRESSURE )
KRATOS_DEFINE_VARIABLE( double, PRECONSOLIDATION_PRESSURE_MIN )
KRATOS_DEFINE_VARIABLE( double, PRECONSOLIDATION_PRESSURE_DEF )
KRATOS_DEFINE_VARIABLE( double, CSL_SLOPE )
KRATOS_DEFINE_VARIABLE( double, VIRGIN_COMPRESSION_INDEX )
KRATOS_DEFINE_VARIABLE( double, RECOMPRESSION_INDEX )
KRATOS_DEFINE_VARIABLE( double, SWELL_INDEX )
KRATOS_DEFINE_VARIABLE( double, VOID_RATIO )
KRATOS_DEFINE_VARIABLE( double, SPACING_RATIO )
KRATOS_DEFINE_VARIABLE( double, ASSOCIATIVITY )
KRATOS_DEFINE_VARIABLE( double, SHAPE_PARAMETER )
KRATOS_DEFINE_VARIABLE( Vector, NEIGHBOUR_WEIGHTS )
KRATOS_DEFINE_VARIABLE( double, MATERIAL_DENSITY )
KRATOS_DEFINE_VARIABLE( double, MATERIAL_DENSITY_NEW )
KRATOS_DEFINE_VARIABLE( double, MATERIAL_DENSITY_FILTERED )
KRATOS_DEFINE_VARIABLE( double, ELEMENT_DC )
KRATOS_DEFINE_VARIABLE( double, ELEMENT_DC_FILTERED )
KRATOS_DEFINE_VARIABLE( double, ELEMENT_DV )
KRATOS_DEFINE_VARIABLE( double, ELEMENT_DV_FILTERED )
KRATOS_DEFINE_VARIABLE( double, YOUNG_MODULUS_0 )
KRATOS_DEFINE_VARIABLE( double, YOUNG_MODULUS_MIN )
KRATOS_DEFINE_VARIABLE( double, PENALIZATION_FACTOR )
KRATOS_DEFINE_VARIABLE( double, GEOMETRICAL_DOMAIN_SIZE )
KRATOS_DEFINE_VARIABLE( double, JACOBIAN_0 )
KRATOS_DEFINE_VARIABLE( Node<3>::Pointer, ASSOCIATED_NODE )
KRATOS_DEFINE_VARIABLE( Element::Pointer, ASSOCIATED_ELEMENT )
KRATOS_DEFINE_VARIABLE( Condition::Pointer, ASSOCIATED_CONDITION )
KRATOS_DEFINE_VARIABLE( Vector, PRINCIPAL_STRESS )
KRATOS_DEFINE_VARIABLE( Vector, PRINCIPAL_STRAIN )
KRATOS_DEFINE_VARIABLE( Matrix, ALGORITHMIC_TANGENT )
KRATOS_DEFINE_VARIABLE( Matrix, ELASTIC_TANGENT )
KRATOS_DEFINE_VARIABLE( bool, IS_MARKED_FOR_REACTION )
KRATOS_DEFINE_VARIABLE( double, EXTRAPOLATION_FACTOR )
KRATOS_DEFINE_VARIABLE( bool, FIX_POROSITY )
KRATOS_DEFINE_VARIABLE( int, QUAD_POINT_STATUS )
KRATOS_DEFINE_VARIABLE( Matrix, LOCAL_FRAME )
KRATOS_DEFINE_VARIABLE( Vector, STRESS_LIKE_INTERNAL_VARIABLES )
KRATOS_DEFINE_VARIABLE( Vector, STRAIN_LIKE_INTERNAL_VARIABLES )
KRATOS_DEFINE_VARIABLE ( double, PRIMARY_HYDRATION_TIME )
KRATOS_DEFINE_VARIABLE ( double, PRIMARY_HYDRATION_TIME_GRADIENT )
KRATOS_DEFINE_VARIABLE ( double, STIFFNESS_RATIO )
KRATOS_DEFINE_VARIABLE ( double, L2_ERROR )
KRATOS_DEFINE_VARIABLE ( double, H1_ERROR )
KRATOS_DEFINE_VARIABLE ( double, ROTATIONAL_STIFFNESS )

// 	KRATOS_DEFINE_VARIABLE(double, DP_EPSILON )
// 	KRATOS_DEFINE_VARIABLE(Vector, INSITU_STRESS )
// 	KRATOS_DEFINE_VARIABLE(double, DP_ALPHA1 )
// 	KRATOS_DEFINE_VARIABLE(double, DP_K )
//KRATOS_DEFINE_VARIABLE(double,TO_ERASE )
// 	KRATOS_DEFINE_VARIABLE(int, CALCULATE_INSITU_STRESS )
// 	KRATOS_DEFINE_VARIABLE(int, CONTACT_RAMP )
// 	KRATOS_DEFINE_VARIABLE(Vector, PENALTY )
// 	KRATOS_DEFINE_VARIABLE(double, INITIAL_PENALTY )
// 	KRATOS_DEFINE_VARIABLE(double, MAXIMUM_PENALTY )
// 	KRATOS_DEFINE_VARIABLE(double, RAMP_CRITERION )
// 	KRATOS_DEFINE_VARIABLE(double, RAMP_FACTOR )
// 	KRATOS_DEFINE_VARIABLE(Vector, PENALTY_T )
//  KRATOS_DEFINE_VARIABLE(double, INITIAL_PENALTY_T )
// 	KRATOS_DEFINE_VARIABLE(double, MAXIMUM_PENALTY_T )
// 	KRATOS_DEFINE_VARIABLE(double, RAMP_CRITERION_T )
// 	KRATOS_DEFINE_VARIABLE(double, RAMP_FACTOR_T )
// 	KRATOS_DEFINE_VARIABLE(double, FRICTION_COEFFICIENT )
// 	KRATOS_DEFINE_VARIABLE(Vector, LAMBDAS )
// 	KRATOS_DEFINE_VARIABLE(Matrix, LAMBDAS_T )
// 	KRATOS_DEFINE_VARIABLE(Vector, GAPS )
// 	KRATOS_DEFINE_VARIABLE(Vector, DELTA_LAMBDAS )
// 	KRATOS_DEFINE_VARIABLE(Matrix, DELTA_LAMBDAS_T )
// 	KRATOS_DEFINE_VARIABLE(int, MAX_UZAWA_ITERATIONS)
// 	KRATOS_DEFINE_VARIABLE(int, CONTACT_SLAVE_INTEGRATION_POINT_INDEX )
// 	KRATOS_DEFINE_VARIABLE( Matrix, CONTACT_LINK_M )
// 	KRATOS_DEFINE_VARIABLE( int, CONTACT_DOUBLE_CHECK )
// 	KRATOS_DEFINE_VARIABLE( int, IS_CONTACT_MASTER )
// 	KRATOS_DEFINE_VARIABLE( int, IS_CONTACT_SLAVE )
// 	KRATOS_DEFINE_VARIABLE( double, K_CONTACT )
// 	KRATOS_DEFINE_VARIABLE( double, K_CONTACT_T )
// 	KRATOS_DEFINE_VARIABLE( Vector, STICK )
// 	KRATOS_DEFINE_VARIABLE( int, FIRST_TIME_STEP )
// 	KRATOS_DEFINE_VARIABLE( int, QUASI_STATIC_ANALYSIS )
// 	KRATOS_DEFINE_VARIABLE( Vector, NORMAL_STRESS )
// 	KRATOS_DEFINE_VARIABLE( Vector, TANGENTIAL_STRESS )
// 	KRATOS_DEFINE_VARIABLE( double, NORMAL_CONTACT_STRESS )
// 	KRATOS_DEFINE_VARIABLE( double, TANGENTIAL_CONTACT_STRESS )
// 	KRATOS_DEFINE_VARIABLE( double, CONTACT_STICK )

//
// 	KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE )
// 	KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_DT )
// 	KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_ACCELERATION )
// 	KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_NULL )
// 	KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_NULL_DT )
// 	KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_NULL_ACCELERATION )
// 	KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_EINS )
// 	KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_EINS_DT )
// 	KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_EINS_ACCELERATION )
//
// 	KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE )
// 	KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_DT )
// 	KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_ACCELERATION )
// 	KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_NULL )
// 	KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_NULL_DT )
// 	KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_NULL_ACCELERATION )
// 	KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_EINS )
// 	KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_EINS_DT )
// 	KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_EINS_ACCELERATION )
//
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_OLD)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_DT)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_NULL)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_NULL_DT)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION_NULL)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_EINS)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_EINS_DT)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION_EINS)
// 	KRATOS_DEFINE_VARIABLE( Matrix, ELASTIC_LEFT_CAUCHY_GREEN_OLD )
//
// 	KRATOS_DEFINE_VARIABLE(int, ACTIVATION_LEVEL)
//KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(VAUX);

}  // namespace Kratos.

#endif // KRATOS_STRUCTURAL_APPLICATION_VARIABLES_2_H_INCLUDED  defined 
