//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Box2.h - Box with 2 dimensions                                            //
// ### # #      ###                                                                            //
// # ### #      ###  Representation of a two dimensional box                                   //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                             (C)2002-2005 Markus Ewald -> License.txt   //
//  //
#ifndef NUCLEX_MATH_BOX2_H
#define NUCLEX_MATH_BOX2_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Math.h"
#include "Nuclex/Math/Point2.h"
#include "Nuclex/Math/Sphere2.h"

namespace Nuclex { namespace Math {

//  //
//  Nuclex::Math::Box2                                                                         //
//  //
/// 2D Box
/** Represents a box in 2D space by its top left and bottom right corners.
    This class operates under the assumption, that the box is never flipped,
    meaning that all its dimensions are of zero size or above.
    
    Intersection queries assume points lying on the top left corner as
    contained within the box, while points lying on the bottom right corner
    are considered as being outside of the box.
    
    This convention avoids cumbersome +1/-1 adjustments to calculate the
    size of a box (which wouldn't work too well for floating point boxes
    anyway).
*/
template<typename VarType>
class Box2 {
  public:
    /// Get intersection box of two boxes
    inline static Box2<VarType> getIntersection(const Box2 &FirstBox, const Box2 &SecondBox);

    /// Constructor
    inline Box2() {}
    /// Values constructor
    inline Box2(VarType X1, VarType Y1, VarType X2, VarType Y2) :
      TL(X1, Y1), BR(X2, Y2) {}
    /// Points constructor
    inline Box2(const Point2<VarType> &TL, const Point2<VarType> &BR) :
      TL(TL), BR(BR) {}
    /// Conversion constructor
    template<typename Y> inline Box2(const Box2<Y> &Other) :
      TL(Other.TL), BR(Other.BR) {}
    /// Conversion constructor
    template<typename Y> inline Box2(const Box2<Y> &Other, StaticCastTag) :
      TL(Point2<VarType>(Other.TL, StaticCastTag())),
      BR(Point2<VarType>(Other.BR, StaticCastTag())) {}

  //
  // Box2 implementation
  //
  public:
    /// Get box center
    inline Point2<VarType> getCenter() const { return (BR + TL) / 2; }
    /// Get box size
    inline Point2<VarType> getSize() const { return BR - TL; }
    /// Get box width
    inline VarType getWidth() const { return BR.X - TL.X; }
    /// Get box height
    inline VarType getHeight() const { return BR.Y - TL.Y; }
    /// Set box from coordinates
    inline void set(VarType X1, VarType Y1, VarType X2, VarType Y2) {
      TL.X = X1;
      TL.Y = Y1;
      BR.X = X2;
      BR.Y = Y2;
    }

    /// Check if the box does not enclose anything
    inline operator bool() const { return (TL != BR); }

    /// Assign from a box of a different type
    template<typename Y> inline Box2 &operator =(const Box2<Y> &Other) {
      TL = Other.TL;
      BR = Other.BR;
    }
    
    inline Box2 &operator +=(const Point2<VarType> &Offset) {
      TL += Offset;
      BR += Offset;
      return *this;
    }
    inline Box2 &operator -=(const Point2<VarType> &Offset) {
      TL -= Offset;
      BR -= Offset;
      return *this;
    }
    
    inline Box2 operator +(const Point2<VarType> &Offset) const {
      return Box2(*this) += Offset;
    }
    inline Box2 operator -(const Point2<VarType> &Offset) const {
      return Box2(*this) -= Offset;
    }

    /// Check if two boxes are equal
    inline bool operator ==(const Box2 &Other) const {
      return (TL == Other.TL) && (BR == Other.BR);
    }
    /// Check if two boxes are not equal
    inline bool operator !=(const Box2 &Other) const {
      return (TL != Other.TL) || (BR != Other.BR);
    }

  //
  // Volume2 implementation
  //  
  public:
    /// Check if the volume intersects with a point
    inline bool intersects(const Point2<VarType> &Point) const {
      return (Point.X >= TL.X) && (Point.X < BR.X) &&
             (Point.Y >= TL.Y) && (Point.Y < BR.Y);
    }
    /// Check if the volume intersects with a box
    inline bool intersects(const Box2 &Box) const { 
      return ((Box.BR.X > TL.X) && (Box.TL.X < BR.X) &&
              (Box.BR.Y > TL.Y) && (Box.TL.Y < BR.Y));
    }
    /// Check if the volume intersects with a sphere
    inline bool intersects(const Sphere2<VarType> &Sphere) const {
      VarType Distance = 0;

      if(Sphere.Center.X < TL.X)
        Distance += Math::pow2(Sphere.Center.X - TL.X);
      else if(Sphere.Center.X > BR.X)
        Distance += Math::pow2(BR.X - Sphere.Center.X);

      if(Sphere.Center.Y < TL.Y)
        Distance += Math::pow2(Sphere.Center.Y - TL.Y);
      else if(Sphere.Center.Y > BR.Y)
        Distance += Math::pow2(BR.Y - Sphere.Center.Y);

      return Distance < Math::pow2(Sphere.R);
    }

    inline Point2<VarType> trace(
      const Point2<VarType> &Start, const Point2<VarType> &End
    );

  public:
    Point2<VarType> TL, BR;
};

// ############################################################################################# //
// # Nuclex::Box2::intersection()                                                              # //
// ############################################################################################# //
/** Returns the intersection area of two boxes. If the boxes do not
    overlap, at least one dimension of the resulting intersection
    box is of negative size.

    @param  FirstBox   First intersection Box
    @param  SecondBox  Second intersection Box
    @return The intersection Box of the two Boxes
*/
template<typename VarType>
inline Box2<VarType> Box2<VarType>::getIntersection(const Box2 &FirstBox, const Box2 &SecondBox) {
  return Box2(
    (FirstBox.TL.X > SecondBox.TL.X) ? FirstBox.TL.X : SecondBox.TL.X,
    (FirstBox.TL.Y > SecondBox.TL.Y) ? FirstBox.TL.Y : SecondBox.TL.Y,
    (FirstBox.BR.X < SecondBox.BR.X) ? FirstBox.BR.X : SecondBox.BR.X,
    (FirstBox.BR.Y < SecondBox.BR.Y) ? FirstBox.BR.Y : SecondBox.BR.Y
  );
}

template<typename VarType>
inline Point2<VarType> Box2<VarType>::trace(
  const Point2<VarType> &Start, const Point2<VarType> &End
) {
/*
  Dim st As Double, et As Double, fst As Double
  Dim fet As Double: fet = 1
  Dim di As Double
  
  Dim l As Long
  For l = 0 To 1
      Dim dLineStart As Double, dLineEnd As Double
      Dim dBoxStart As Double, dBoxEnd As Double

      If l = 0 Then
          dLineStart = oLineStart.X
          dLineEnd = oLineEnd.X
          dBoxStart = oBox.TL.X
          dBoxEnd = oBox.BR.X
      Else
          dLineStart = oLineStart.Y
          dLineEnd = oLineEnd.Y
          dBoxStart = oBox.TL.Y
          dBoxEnd = oBox.BR.Y
      End If
      
      If dLineStart < dLineEnd Then
          If (dLineStart > dBoxEnd) Or (dLineEnd < dBoxStart) Then _
              Exit Property
  
          di = dLineEnd - dLineStart
          If dLineStart < dBoxStart Then st = (dBoxStart - dLineStart) / di Else st = 0
          If dLineEnd > dBoxEnd Then et = (dBoxEnd - dLineStart) / di Else et = 1
      Else
          If (dLineEnd > dBoxEnd) Or (dLineStart < dBoxStart) Then _
              Exit Property
  
          di = dLineEnd - dLineStart
          If dLineStart > dBoxEnd Then st = (dBoxEnd - dLineStart) / di Else st = 0
          If dLineEnd < dBoxStart Then et = (dBoxStart - dLineStart) / di Else et = 1
      End If
  
      If st > fst Then _
          fst = st
      If et < fet Then _
          fet = et
      If fet < fst Then _
          Exit Property
  Next

  Set IntersectionQuery_Box_LineSegment = _
      oLineStart.Clone.Add(oLineEnd.Clone.Subtract(oLineStart).Multiply(fst))
*/      
/*    
  float st,et,fst = 0,fet = 1;
  float const *bmin = &box.min.x;
  float const *bmax = &box.max.x;
  float const *si = &start.x;
  float const *ei = &end.x;
  
  for (int i = 0; i < 3; i++) {
      if (*si < *ei) {
          if (*si > *bmax || *ei < *bmin)
              return false;
          
          float di = *ei - *si;
          st = (*si < *bmin) ? (*bmin - *si) / di : 0;
          et = (*ei > *bmax) ? (*bmax - *si) / di : 1;
      } else {
          if (*ei > *bmax || *si < *bmin)
              return false;

          float di = *ei - *si;
          st = (*si > *bmax) ? (*bmax - *si) / di : 0;
          et = (*ei < *bmin) ? (*bmin - *si) / di : 1;
      }
      
      if (st > fst)
          fst = st;
      if (et < fet)
          fet = et;
      if (fet < fst)
          return false;

      bmin++;
      bmax++;
      si++;
      ei++;
  }
  *time = fst;
  return true;
*/
}

}} // namespace Nuclex::Math

#endif // NUCLEX_MATH_BOX2_H
