//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Sphere3.h - Nuclex 3D sphere                        //
// ### # #      ###                                                      //
// # ### #      ###  Nuclex sphere                                       //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_MATH_SPHERE3_H
#define NUCLEX_MATH_SPHERE3_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Math.h"
#include "Nuclex/Math/Point3.h"
#include "Nuclex/Math/Volume3.h"

namespace Nuclex { namespace Math {

//  //
//  Nuclex::Math::Sphere3                                                //
//  //
/// 3D sphere
/** A 3D sphere defined by a center and a radius
*/
template <typename VarType>
class Sphere3 :
  public Volume3<VarType> {
  public:
    /// Constructor
    inline Sphere3();
    /// Values constructor
    inline Sphere3(VarType X, VarType Y, VarType Z, VarType R);
    /// Points constructor
    inline Sphere3(const Point3<VarType> &Center, VarType R);

  //
  // Sphere3 implementation
  //
  public:
    /// Check if all sphere coordinates are set to 0
    inline bool isNull() const;
  
  //
  // Volume3 implementation
  //
  public:
    /// Check for intersection with other volume
    inline bool intersects(const Volume3<VarType> &Other) const {
       return Other.intersects(*this);
    }
    /// Check for intersection with point
    inline bool intersects(const Point3<VarType> &Point) const {
      return (Math::pow2(Point.X - Center.X) +
              Math::pow2(Point.Y - Center.Y) +
              Math::pow2(Point.Z - Center.Z)) < Math::pow2(R);
    }
    /// Check for intersection with box
    inline bool intersects(const Box3<VarType> &Box) const {
      VarType Distance = 0;

      if(Center.X < Box.X1)
        Distance += Math::pow2(Center.X - Box.X1);
      else if(Center.X > Box.X2)
        Distance += Math::pow2(Box.X2 - Center.X);

      if(Center.Y < Box.Y1)
        Distance += Math::pow2(Center.Y - Box.Y1);
      else if(Center.Y > Box.Y2)
        Distance += Math::pow2(Box.Y2 - Center.Y);

      if(Center.Z < Box.Z1)
        Distance += Math::pow2(Center.Z - Box.Z1);
      else if(Center.Z > Box.Z2)
        Distance += Math::pow2(Box.Z2 - Center.Z);

      return Distance < Math::pow2(R);
    }
    /// Check for intersection with sphere
    inline bool intersects(const Sphere3 &Sphere) const {
      Point3<VarType> Distance = Center - Sphere.Center;
      return (Math::pow2(Distance.X) + Math::pow2(Distance.Y) + Math::pow2(Distance.Z)) < Math::pow2(R + Sphere.R);
    }

  public:
    Point3<VarType> Center;
    VarType         R;
};

// ####################################################################### //
// # Nuclex::Sphere3::Sphere3()                              Constructor # // 
// ####################################################################### //
/** Initializes an instance of Sphere3
*/
template <typename VarType>
inline Sphere3<VarType>::Sphere3() :
  X(0), Y(0), Z(0),
  R(0) {}

// ####################################################################### //
// # Nuclex::Sphere3::Sphere3()                              Constructor # // 
// ####################################################################### //
/** Initializes an instance of Sphere3 with the specified coordinates

    @param  X, Y, Z  Coordinates of the sphere's center
    @param  R        Radius
*/
template <typename VarType>
inline Sphere3<VarType>::Sphere3(VarType X, VarType Y, VarType Z, VarType R) :
  Center(X, Y, Z),
  R(R) {}

// ####################################################################### //
// # Nuclex::Sphere3::Sphere3()                              Constructor # // 
// ####################################################################### //
/** Initializes an instance of Sphere3 from two corner point Points

    @param  Center  The sphere's center
    @param  R       Radius
*/
template <typename VarType>
inline Sphere3<VarType>::Sphere3(const Point3<VarType> &Center, VarType R) :
  Center(Center),
  R(R) {}

// ####################################################################### //
// # Nuclex::Sphere3::isNull()                                           # // 
// ####################################################################### //
/** Returns true if all coordinates of the rectangle are se to 0

    @return True if all coordinates of the rectangle are se to 0
*/
template <typename VarType>
inline bool Sphere3<VarType>::isNull() const {
  return (Center.X == 0) && (Center.Y == 0) && (Center.Z == 0) && (R == 0);
}

}} // namespace Nuclex::Math

#endif // NUCLEX_MATH_SPHERE3_H
