//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Controller.h - Controller base                      //
// ### # #      ###                                                      //
// # ### #      ###  Base class for controllers                          //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_MATH_CONTROLLER_H
#define NUCLEX_MATH_CONTROLLER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Math.h"

namespace Nuclex { namespace Math {

//  //
//  Nuclex::Math::Controller                                             //
//  //
/// Controller base
/** Base class for controllers. Controllers provide interpolated,
    calculated or otherwise dynamically created variables.
*/
template<typename VarType>
class Controller {
  public:
    /// Destructor
    inline virtual ~Controller() {};

  //
  // Controller implementation
  //
  public:
    virtual VarType operator()(float fTime) const = 0;
};

}} // namespace Nuclex::Math

#endif // NUCLEX_MATH_CONTROLLER_H
