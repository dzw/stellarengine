//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Sphere2.h - Nuclex 2D sphere                        //
// ### # #      ###                                                      //
// # ### #      ###  Nuclex sphere                                       //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_MATH_SPHERE2_H
#define NUCLEX_MATH_SPHERE2_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Math.h"
#include "Nuclex/Math/Point2.h"

namespace Nuclex { namespace Math {

template<typename VarType> class Box2;

//  //
//  Nuclex::Math::Sphere2                                                                      //
//  //
/// 2D sphere
/** A 2D sphere defined by a center and a radius
*/
template<typename VarType>
class Sphere2 {
  public:
    /// Constructor
    inline Sphere2();
    /// Values constructor
    inline Sphere2(VarType X, VarType Y, VarType R);
    /// Points constructor
    inline Sphere2(const Point2<VarType> &Center, VarType R);

  //
  // Sphere2 implementation
  //
  public:
    /// Check if all sphere coordinates are set to 0
    inline operator bool() const { return (R == 0); }
  
  //
  // Volume2 implementation
  //
  public:
    /// Check if the volume intersects with a point
    inline bool intersects(const Point2<VarType> &Point) const;
    /// Check if the volume intersects with a box
    inline bool intersects(const Box2<VarType> &Box) const;
    /// Check if the volume intersects with a sphere
    inline bool intersects(const Sphere2 &Sphere) const;

  public:
    /// Sphere center
    Point2<VarType> Center;
    /// Radius
    VarType R;
};

#include "Nuclex/Math/Box2.h"

// ############################################################################################# //
// # Nuclex::Math::Sphere2::Sphere2()                                              Constructor # //
// ############################################################################################# //
/** Initializes an new two dimensional sphere
*/
template <typename VarType>
inline Sphere2<VarType>::Sphere2() :
  R(0) {}

// ############################################################################################# //
// # Nuclex::Sphere2::Sphere2()                                                    Constructor # //
// ############################################################################################# //
/** Initializes an instance of Sphere2 with the specified coordinates

    @param  X, Y  Coordinates of the sphere's center
    @param  R     Radius of the sphere
*/
template <typename VarType>
inline Sphere2<VarType>::Sphere2(VarType X, VarType Y, VarType R) :
  Center(X, Y),
  R(R) {}

// ############################################################################################# //
// # Nuclex::Sphere2::Sphere2()                                                    Constructor # //
// ############################################################################################# //
/** Initializes an instance of Sphere2 using a center point and a radius

    @param  Center  The sphere's center
    @param  R       Radius of the sphere
*/
template <typename VarType>
inline Sphere2<VarType>::Sphere2(const Point2<VarType> &Center, VarType R) :
  Center(Center),
  R(R) {}

// ############################################################################################# //
// # Nuclex::Sphere2::intersects()                                                             # //
// ############################################################################################# //
template <typename VarType>
inline bool Sphere2<VarType>::intersects(const Point2<VarType> &Point) const {
  return (Math::pow2(Point.X - Center.X) + Math::pow2(Point.Y - Center.Y)) < Math::pow2(R);
}

// ############################################################################################# //
// # Nuclex::Sphere2::intersects()                                                             # //
// ############################################################################################# //
template <typename VarType>
inline bool Sphere2<VarType>::intersects(const Box2<VarType> &Box) const {
  return Box.intersects(*this);
}

// ############################################################################################# //
// # Nuclex::Sphere2::intersects()                                                             # //
// ############################################################################################# //
template <typename VarType>
inline bool Sphere2<VarType>::intersects(const Sphere2 &Sphere) const {
  return (Math::pow2(Point.X - Center.X) + Math::pow2(Point.Y - Center.Y)) <
           Math::pow2(R + Sphere.R);
}

}} // namespace Nuclex::Math

#endif // NUCLEX_MATH_SPHERE2_H
