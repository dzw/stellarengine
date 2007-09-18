//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Point2.h - A point in 2 dimensions                                        //
// ### # #      ###                                                                            //
// # ### #      ###  Two dimensional point and vector template                                 //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                             (C)2002-2005 Markus Ewald -> License.txt   //
//  //
#ifndef NUCLEX_MATH_POINT2_H
#define NUCLEX_MATH_POINT2_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Math.h"
#include <iostream>

namespace Nuclex { namespace Math {

//  //
//  Nuclex::Math::Point2                                                                       //
//  //
/// 2D point
/** Represents a location or dimension in 2D space.
    All standard methods for vectors like normalization, dot and cross
    products as well as arithmetical operations are supplied.
*/
template<typename VarType>
class Point2 {
  public:
    /// Simple representation
    /** POD stands for "plain old data" and indicates that the
        structure does not contain any virtual members or overloaded
        operators.
        The template provides transparent conversion from and to its
        POD representation, so this structure is ideal for usage in
        vertex buffers and the like.
    */
    struct POD { 
      VarType X, Y;
    };

    /// Right pointing vector
    static const Point2 Right;
    /// Up pointing vector
    static const Point2 Up;

    /// Constructor
    inline Point2();
    /// Point constructor
    inline Point2(VarType X, VarType Y);
    /// POD constructor
    inline Point2(const POD &Other);
    /// Conversion constructor
    template<typename Y> inline Point2(const Point2<Y> &Other) :
      X(Other.X), Y(Other.Y) {}
    /// Conversion constructor
    template<typename Y> inline Point2(const Point2<Y> &Other, StaticCastTag) :
      X(static_cast<VarType>(Other.X)), Y(static_cast<VarType>(Other.Y)) {}

  //
  // Point2 implementation
  //
  public:
    /// Set point from individual components
    inline void set(VarType X, VarType Y);

    /// Negate the vector
    inline void negate();

    /// Get vector length
    inline VarType getLength() const { return Math::sqrt(getLengthSquared()); }
    /// Get vector length squared
    inline VarType getLengthSquared() const { return (X * X) + (Y * Y); }

    /// Normalize vector
    inline void normalize() { operator /=(getLength()); }
    /// Calculate dot product towards other vector
    inline VarType dotProduct(const Point2 &Point) const;
    /// Calculate cross product with other vector
    inline Point2 crossProduct(const Point2 &Point) const;

    /// Rotate point around Z axis
    void rotate(VarType Angle);

    /// POD conversion operator
    inline operator POD() const { POD Converted = { X, Y }; return Converted; }

    /// Add other point to point
    inline Point2 &operator +=(const Point2 &Summand);
    /// Subtract other point from point
    inline Point2 &operator -=(const Point2 &Subtrahend);
    /// Multiply point with other point
    inline Point2 &operator *=(const Point2 &Multiplicand);
    /// Scale point by value
    inline Point2 &operator *=(VarType Factor);
    /// Divide point by other point
    inline Point2 &operator /=(const Point2 &Quotient);
    /// Descale point by value
    inline Point2 &operator /=(VarType Quotient);

    /// Assign components from POD representation
    inline Point2 &operator =(const POD &Other);
    /// Assign a vector of a different type
    template<typename Y> inline Point2 &operator =(const Point2<Y> &Other) {
      Point2::X = Other.X;
      Point2::Y = Other.Y;
      return *this;
    }
    /// Return negated point
    inline Point2 operator -() const;
    /// Return point added to other point
    inline Point2 operator +(const Point2 &Summand) const { return Point2(*this) += Summand; }
    /// Return point subtracted by other point
    inline Point2 operator -(const Point2 &Subtrahend) const { return Point2(*this) -= Subtrahend; }
    /// Return point multiplied by other point 
    inline Point2 operator *(const Point2 &Multiplicand) const { return Point2(*this) *= Multiplicand; }
    /// Return point scaled by a factor
    inline Point2 operator *(VarType Factor) const { return Point2(*this) *= Factor; }
    /// Return point divided by other point
    inline Point2 operator /(const Point2 &Quotient) const { return Point2(*this) /= Quotient; }
    /// Return point divided by a factor
    inline Point2 operator /(VarType Quotient) const { return Point2(*this) /= Quotient; }

    /// Check for equality with other point
    inline bool operator ==(const Point2 &Point) const { return (X == Point.X) && (Y == Point.Y); }
    /// Check for unequality with other point
    inline bool operator !=(const Point2 &Point) const { return (X != Point.X) || (Y != Point.Y); }

    friend inline std::ostream &operator <<(std::ostream &o, const Point2 &Point) {
      return o << "{" << Point.X << ";" << Point.Y << "}";
    }
      
  public:
    VarType X, Y;                                     ///< Point components
};

template<typename VarType> const Point2<VarType> Point2<VarType>::Right(1, 0);
template<typename VarType> const Point2<VarType> Point2<VarType>::Up(0, 1);

// ############################################################################################# //
// # Nuclex::Point2::Point2()                                                      Constructor # //
// ############################################################################################# //
/** Creates a new point set to {0,0}
*/
template<typename VarType>
inline Point2<VarType>::Point2() :
  X(0),
  Y(0) {}

// ############################################################################################# //
// # Nuclex::Point2::Point2()                                                      Constructor # //
// ############################################################################################# //
/** Creates a new point initialized from its POD representation

    @param  Source  POD representation with which to initialize the point
*/
template<typename VarType>
inline Point2<VarType>::Point2(const POD &Source) :
  X(Source.X),
  Y(Source.Y) {}

// ############################################################################################# //
// # Nuclex::Point2::Point2()                                                      Constructor # //
// ############################################################################################# //
/** Creates a new point set to the specified values

    @param  X, Y  Point components
*/
template<typename VarType>
inline Point2<VarType>::Point2(VarType X, VarType Y) :
  X(X),
  Y(Y) {}

// ############################################################################################# //
// # Nuclex::Point2::set()                                                                     # //
// ############################################################################################# //
/** Sets the point by its components

    @param  X, Y  Point components to assign to the point
*/
template<typename VarType>
inline void Point2<VarType>::set(VarType X, VarType Y) {
  Point2::X = X;
  Point2::Y = Y;
}

// ############################################################################################# //
// # Nuclex::Point2::operator =()                                                              # //
// ############################################################################################# //
template<typename VarType>
inline Point2<VarType> &Point2<VarType>::operator =(const POD &Other) {
  X = Other.X;
  Y = Other.Y;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point2::operator +=()                                                             # //
// ############################################################################################# //
/** Adds the point to another point

    @param  Summand  Point which to add to the point
*/
template<typename VarType>
inline Point2<VarType> &Point2<VarType>::operator +=(const Point2 &Summand) {
  X += Summand.X;
  Y += Summand.Y;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point2::operator -=()                                                             # //
// ############################################################################################# //
/** Subtracts another Point from this Point

    @param  Subtrahend  Point which to subtract
*/
template<typename VarType>
inline Point2<VarType> &Point2<VarType>::operator -=(const Point2 &Subtrahend) {
  X -= Subtrahend.X;
  Y -= Subtrahend.Y;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point2::operator *=()                                                             # //
// ############################################################################################# //
/** Multiplicates the vector by a scaling value

    @param  Factor  Scaling value to multiply factor with
*/
template<typename VarType>
inline Point2<VarType> &Point2<VarType>::operator *=(VarType Factor) {
  X *= Factor;
  Y *= Factor;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point2::operator *=()                                                             # //
// ############################################################################################# //
/** Multiplicates the vector by another vector

    @param  Multiplicand  Vector with which to multiplicate
*/
template<typename VarType>
inline Point2<VarType> &Point2<VarType>::operator *=(const Point2 &Multiplicand) {
  X *= Multiplicand.X;
  Y *= Multiplicand.Y;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point2::operator /=()                                                             # //
// ############################################################################################# //
/** Divides the vector by a scaling value

    @param  Quotient  Scaling value to divide vector by
*/
template<typename VarType>
inline Point2<VarType> &Point2<VarType>::operator /=(VarType Quotient) {
  X /= Quotient;
  Y /= Quotient;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point2::operator /=()                                                             # //
// ############################################################################################# //
/** Divides the vector by another vector

    @param  Quotient  Point by which to divide
*/
template<typename VarType>
inline Point2<VarType> &Point2<VarType>::operator /=(const Point2 &Quotient) {
  X /= Quotient.X;
  Y /= Quotient.Y;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point2::negate()                                                                  # //
// ############################################################################################# //
/** Negates the point
*/
template<typename VarType>
inline void Point2<VarType>::negate() {
  X = -X;
  Y = -Y;
}

// ############################################################################################# //
// # Nuclex::Point2::dotProduct()                                                              # //
// ############################################################################################# //
/** Calculates the dot product of the point with another point

    @param  Point  Point with which to calculate the dot product
    @return The dot product
*/
template<typename VarType>
inline VarType Point2<VarType>::dotProduct(const Point2 &Point) const {
  return (X * Point.X) + (Y * Point.Y);
}

// ############################################################################################# //
// # Nuclex::Point2::crossProduct()                                                            # //
// ############################################################################################# //
/** Calculates the cross product of the point to another point

    @param  Point  Point towards which to calculate the cross product
    @return The cross product Point
*/
template<typename VarType>
inline Point2<VarType> Point2<VarType>::crossProduct(const Point2 &Point) const {
  return Point2((Y * Point.X) - (X * Point.Y),
                (X * Point.Y) - (Y * Point.X));
}

// ############################################################################################# //
// # Nuclex::Point2::rotate()                                                                  # //
// ############################################################################################# //
/** Rotates the point around the global Z axis

    @param  Angle  Rotation angle in RAD
*/
template<typename VarType>
inline void Point2<VarType>::rotate(VarType Angle) {
  VarType CX = X, CY = Y;
  VarType Sin = Math::sin(Angle);
  VarType Cos = Math::cos(Angle);

  X = (CX * Cos) - (CY * Sin);
  Y = (CX * Sin) + (CY * Cos);
}

// ############################################################################################# //
// # Nuclex::Point2::operator -()                                                              # //
// ############################################################################################# //
/** Negates the Point

    @return The negated point
*/
template<typename VarType>
inline Point2<VarType> Point2<VarType>::operator -() const {
  return Point2(-X, -Y);
}

#define Vector2 Point2

}} // namespace Nuclex::Math

#endif // NUCLEX_MATH_POINT2_H
