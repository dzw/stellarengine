//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Volume2.h - 2D volume                               //
// ### # #      ###                                                      //
// # ### #      ###  Base class for a 2d volumes                         //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_MATH_VOLUME2_H
#define NUCLEX_MATH_VOLUME2_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Math.h"

namespace Nuclex { namespace Math {

template<typename VarType> class Point2;
template<typename VarType> class Box2;
template<typename VarType> class Sphere2;

//  //
//  Nuclex::Math::Volume2                                                                      //
//  //
/// 2D volume
/** An abstract 2d volume primitive. Uses the double dispatch pattern
    to check for intersections between two unknown volumes.
*/
template<typename VarType>
class Volume2 {
  public:
    /// Destructor
    inline virtual ~Volume2() {};

  //
  // Volume2 implementation
  //
  public:
    /// Check if the volume intersects with another volume
    virtual bool intersects(const Volume2 &Other) const = 0;
    /// Check if the volume intersects with a point
    virtual bool intersects(const Point2<VarType> &Point) const = 0;
    /// Check if the volume intersects with a box
    virtual bool intersects(const Box2<VarType> &Box) const = 0;
    /// Check if the volume intersects with a sphere
    virtual bool intersects(const Sphere2<VarType> &Sphere) const = 0;

    /// Trace line segment for point of impact
    /** Returns the first intersection of the specified line segment with the volume,
        tracing it from Start to End

        @param  Start  Starting location of the line segment to be traced
        @param  End    Ending location of the line segment to be traced
        @return The starting point or a point on the specified line segment if an
                intersection was found, otherwise the ending point of the line segment
    */
/*    
    virtual Point2<VarType> trace(
      const Point2<VarType> &Start, const Point2<VarType> &End
    ) const = 0;
*/
};

}} // namespace Nuclex::Math

#endif // NUCLEX_MATH_VOLUME2_H
