//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Box3.h - Box with 3 dimensions                                            //
// ### # #      ###                                                                            //
// # ### #      ###  Representation of a three dimensional box                                 //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                             (C)2002-2005 Markus Ewald -> License.txt   //
//  //
#ifndef NUCLEX_MATH_BOX3_H
#define NUCLEX_MATH_BOX3_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Math.h"
#include "Nuclex/Math/Point3.h"
#include "Nuclex/Math/Sphere3.h"
#include "Nuclex/Math/Volume3.h"

/*

template<typename VarType> typedef Point<VarType, 2> Point2;
template<typename VarType> typedef Point<VarType, 3> Point3;

template<typename VarType, size_t DimensionCount>
class Box {
  public:
    Point<VarType, DimensionCount> Start;
    Point<VarType, DimensionCount> End;
};

*/

namespace Nuclex { namespace Math {

//  //
//  Nuclex::Math::Box3                                                                         //
//  //
/// 3D Box
/** Represents a box in 3D space by its top left front and bottom right
    back corners. This class operates under the assumption, that the box
    is never flipped, meaning that all its dimensions are of zero size
    or above.
    
    Intersection queries assume points lying on the top left front corner
    as contained within the box, while points lying on the bottom right
    back corner are considered as being outside of the box.
    
    This convention avoids cumbersome +1/-1 adjustments to calculate the
    size of a box (which wouldn't work too well for floating point boxes
    anyway).
*/
template <typename VarType>
class Box3 :
  public Volume3<VarType> {
  public:
    /// Constructor
    inline Box3() {}
    /// Values constructor
    inline Box3(
      VarType X1, VarType Y1, VarType Z1,         
      VarType X2, VarType Y2, VarType Z2
    );
    /// Points constructor
    inline Box3(const Point3<VarType> &TLF, const Point3<VarType> &BRB);
    /// Conversion constructor
    template<typename Y> inline Box3(const Box3<Y> &Other) :
      TLF(Other.TLF), BRB(Other.BRB) {}
    /// Conversion constructor
    template<typename Y> inline Box3(const Box3<Y> &Other, StaticCastTag) :
      TLF(Point3<VarType>(Other.TLF, StaticCastTag())),
      BRB(Point3<VarType>(Other.BRB, StaticCastTag())) {}

    /// Get intersection box of two boxes
    inline static Box3<VarType> getIntersection(const Box3 &FirstBox, const Box3 &SecondBox);

  //
  // Box3 implementation
  //
  public:
    /// Get box center
    inline Point3<VarType> getCenter() const { return (BR + TL) / 2; }
    /// Get Box size
    inline Point3<VarType> getSize() const { return BRB - TLF; }
    /// Get Box width
    inline VarType getWidth() const { return BRB.X - TLF.X; }
    /// Get Box height
    inline VarType getHeight() const { return BRB.Y - TLF.Y; }
    /// Get Box depth
    inline VarType getDepth() const { return BRB.Z - TLF.Z; }

    inline void set(VarType X1, VarType Y1, VarType Z1, VarType X2, VarType Y2, VarType Z2) {
      TLF.X = X1;
      TLF.Y = Y1;
      TLF.Z = Z1;
      BRB.X = X2;
      BRB.Y = Y2;
      BRB.Z = Z2;
    }

    /// Assign a vector of a different type
    template<typename Y> inline Box3 &operator =(const Box3<Y> &Other) {
      TLF = Other.TLF;
      BRB = Other.BRB;
    }

    /// Check if the box does not enclose anything
    inline operator bool() const { return (TL != BR); }

    /// Check if two boxes are equal
    inline bool operator ==(const Box3 &Other) const {
      return (TLF == Other.TLF) && (BRB == Other.BRB);
    }
    /// Check if two boxes are not equal
    inline bool operator !=(const Box3 &Other) const {
      return (TLF != Other.TLF) || (BRB != Other.BRB);
    }
    
  //
  // Volume3 implementation
  //  
  public:
    /// Check if the volume intersects with another volume
    inline bool intersects(const Volume3<VarType> &Other) const {
      return Other.intersects(*this);
    }
    /// Check if the volume intersects with a point
    inline bool intersects(const Point3<VarType> &Point) const {
      return (Point.X >= TLF.X) && (Point.X < BRB.X) &&
             (Point.Y >= TLF.Y) && (Point.Y < BRB.Y) &&
             (Point.Z >= TLF.Z) && (Point.Z < BRB.Z) &&;
    }
    /// Check if the volume intersects with a box
    inline bool intersects(const Box3 &Box) const {
      return (Box.BRB.X > TLF.X) && (Box.TLF.X < BRB.X) &&
             (Box.BRB.Y > TLF.X) && (Box.TLF.Y < BRB.Y) &&
             (Box.BRB.Z > TLF.X) && (Box.TLF.Z < BRB.Z);
    }
    /// Check if the volume intersects with a sphere
    inline bool intersects(const Sphere3<VarType> &Sphere) const {
      VarType Distance = 0;

      if(Sphere.Center.X < TLF.X)
        Distance += Math::pow2(Sphere.Center.X - TLF.X);
      else if(Sphere.Center.X > BRB.X)
        Distance += Math::pow2(BRB.X - Sphere.Center.X);

      if(Sphere.Center.Y < TLF.Y)
        Distance += Math::pow2(Sphere.Center.Y - TLF.Y);
      else if(Sphere.Center.Y > BRB.Y)
        Distance += Math::pow2(BRB.Y - Sphere.Center.Y);

      if(Sphere.Center.Z < TLF.Z)
        Distance += Math::pow2(Sphere.Center.Z - TLF.Z);
      else if(Sphere.Center.Z > BRB.Z)
        Distance += Math::pow2(BRB.Z - Sphere.Center.Z);

      return Distance < Math::pow2(Sphere.R);
    }

  private:
    /// The box' top left front corner
    Point3<VarType> TLF;
    /// The box' bottom right back corner
    Point3<VarType> BRB;
};

// ############################################################################################# //
// # Nuclex::Box3::getIntersection()                                                           # //
// ############################################################################################# //
/** Returns the intersection box of two boxes. If the boxes do not
    overlap, at least one dimension of the resulting intersection
    box is of negative size.

    @param  FirstBox   First intersection Box
    @param  SecondBox  Second intersection Box
    @return The intersection Box of the two Boxes
*/
template <typename VarType>
inline Box3<VarType> Box3<VarType>::getIntersection(const Box3 &FirstBox, const Box3 &SecondBox) {
  return Box3(
    (FirstBox.TLF.X > SecondBox.TLF.X) ? FirstBox.TLF.X : SecondBox.TLF.X,
    (FirstBox.TLF.Y > SecondBox.TLF.Y) ? FirstBox.TLF.Y : SecondBox.TLF.Y,
    (FirstBox.TLF.Z > SecondBox.TLF.Z) ? FirstBox.TLF.Z : SecondBox.TLF.Z,
    (FirstBox.BRB.X < SecondBox.BRB.X) ? FirstBox.BRB.X : SecondBox.BRB.X,
    (FirstBox.BRB.Y < SecondBox.BRB.Y) ? FirstBox.BRB.Y : SecondBox.BRB.Y,
    (FirstBox.BRB.Z < SecondBox.BRB.Z) ? FirstBox.BRB.Z : SecondBox.BRB.Z
  );
}

// ############################################################################################# //
// # Nuclex::Box3::Box3()                                                          Constructor # //
// ############################################################################################# //
/** Initializes an instance of Box3 with the specified coordinates

    @param  X1, Y1, Z1  Coordinates of the first Box corner
    @param  X2, Y2, Z2  Coordinates of the second Box corner
*/
template <typename VarType>
inline Box3<VarType>::Box3(
  VarType X1, VarType Y1, VarType Z1, VarType X2, VarType Y2, VarType Z2
) :
  TLF.X(X1), TLF.Y(Y1), TLF.Z(Z1),
  BRB.X(X2), BRB.Y(Y2), BRB.Z(Z2) {}

// ############################################################################################# //
// # Nuclex::Box3::Box3()                                                          Constructor # //
// ############################################################################################# //
/** Initializes an instance of Box3 from two corner point Points

    @param  Point1  First Box corner
    @param  Point2  Second Box corner
*/
template <typename VarType>
inline Box3<VarType>::Box3(const Point3<VarType> &Point1, const Point3<VarType> &Point2) :
  TLF(Point1), BRB(Point2) {}

}} // namespace Nuclex::Math

#endif // NUCLEX_MATH_BOX3_H
