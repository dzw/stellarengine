//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Quaternion - 4D orientation                                               //
// ### # #      ###                                                                            //
// # ### #      ###  Stores an orientation in 3D space as a 4D directional vector              //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                                       (C) Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_MATH_QUATERNION_H
#define NUCLEX_MATH_QUATERNION_H

#include "Nuclex/Nuclex.h"

namespace Nuclex { namespace Math {

//  //
//  Nuclex::Math::Quaternion                                                                   //
//  //
/// Quaternion
/**
*/
template<typename VarType>
class Quaternion {
  public:
    Quaternion() :
      X(0), Y(0), Z(0), W(0) {}
    
  //
  // Quaternion implementation
  //
  public:
    

  public:
    VarType X, Y, Z, W;
};

}} // namespace Nuclex::Math

#endif // NUCLEX_MATH_QUATERNION_H
