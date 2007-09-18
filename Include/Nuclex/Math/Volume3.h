//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Volume3.h - Nuclex 3D Volume                        //
// ### # #      ###                                                      //
// # ### #      ###  Nuclex Volume class                                 //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_MATH_VOLUME3_H
#define NUCLEX_MATH_VOLUME3_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Math.h"

namespace Nuclex { namespace Math {

template<typename VarType> class Point3;
template<typename VarType> class Box3;
template<typename VarType> class Sphere3;

//  //
//  Nuclex::Math::Volume3                                                                      //
//  //
/// 3D Volume class
/** Manages a Volume in 3D space. Each axis of the Volume can either
    be normal of flipped. Flipped means that the axis' second coordinate
    is lower than the first coordinate (eg. X3 < X1). If this is the case,
    the insides and outsides for that axis are also flipped
*/
template<typename VarType>
class Volume3 {
  public:
    /// Destructor
    inline virtual ~Volume3() {};

  //
  // Volume3 implementation
  //
  public:
    /// Check if the volume intersects with another volume
    virtual bool intersects(const Volume3 &Other) const = 0;
    /// Check if the volume intersects with a point
    virtual bool intersects(const Point3<VarType> &Point) const = 0;
    /// Check if the volume intersects with a box
    virtual bool intersects(const Box3<VarType> &Box) const = 0;
    /// Check if the volume intersects with a sphere
    virtual bool intersects(const Sphere3<VarType> &Sphere) const = 0;

    /// Trace line segment for point of impact
    /** Returns the first intersection of the specified line segment with the volume,
        tracing it from Start to End

        @param  Start  Starting location of the line segment to be traced
        @param  End    Ending location of the line segment to be traced
        @return The starting point or a point on the specified line segment if an
                intersection was found, otherwise the ending point of the line segment
    */
/*    
    virtual Point3<VarType> trace(
      const Point3<VarType> &Start, const Point3<VarType> &End
    ) const = 0;
*/    
};

}} // namespace Nuclex::Math

#endif // NUCLEX_MATH_VOLUME3_H
