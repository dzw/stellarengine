//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Point3.h - A point in 3 dimensions                                        //
// ### # #      ###                                                                            //
// # ### #      ###  Three dimensional point and vector template                               //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                             (C)2002-2005 Markus Ewald -> License.txt   //
//  //
#ifndef NUCLEX_MATH_POINT3_H
#define NUCLEX_MATH_POINT3_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Math.h"

namespace Nuclex { namespace Math {

//  //
//  Nuclex::Math::Point3                                                                       //
//  //
/// 3D point
/** Represents a location or dimension in 3D space.
    All standard methods for vectors like normalization, dot and cross
    products as well as arithmetical operations are supplied.
*/
template<typename VarType>
class Point3 {
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
      VarType X, Y, Z;
    };

    /// Right pointing vector
    static const Point3 Right;
    /// Up pointing vector
    static const Point3 Up;
    /// Up pointing vector
    static const Point3 In;

    /// Constructor
    inline Point3();
    /// Point constructor
    inline Point3(VarType X, VarType Y, VarType Z);
    /// POD constructor
    inline Point3(const POD &Source);
    /// Conversion constructor
    template<typename Y> inline Point3(const Point3<Y> &Other) :
      X(Other.X), Y(Other.Y), Z(Other.Z) {}
    /// Conversion constructor
    template<typename Y> inline Point3(const Point3<Y> &Other, StaticCastTag) :
      X(static_cast<VarType>(Other.X)),
      Y(static_cast<VarType>(Other.Y)),
      Z(static_cast<VarType>(Other.Z)) {}

  //
  // Point3 implementation
  //
  public:
    /// Set point from individual components
    inline void set(VarType X, VarType Y, VarType Z);

    /// Negate the vector
    inline void negate();

    /// Get vector length
    inline VarType getLength() const { return Math::sqrt(getLengthSquared()); }
    /// Get vector length squared
    inline VarType getLengthSquared() const { return (X * X) + (Y * Y) + (Z * Z); }
    
    /// Normalize vector
    inline void normalize() { operator /=(getLength()); }
    /// Calculate dot product to other vector
    inline VarType dotProduct(const Point3 &Point) const;
    /// Calculate cross product with other vector
    inline Point3 crossProduct(const Point3 &Point) const;

    /// Rotate point around global X axis
    void rotateX(VarType Angle);
    /// Rotate point around global Y axis
    void rotateY(VarType Angle);
    /// Rotate point around global Z axis
    void rotateZ(VarType Angle);
    /// Rotate point around arbitrary axis
    void rotateAroundAxis(const Point3 &Axis, VarType Angle);

    /// POD conversion operator
    inline operator POD() const { POD Converted = { X, Y, Z }; return Converted; }

    /// Add other point to point
    inline Point3 &operator +=(const Point3 &Point);
    /// Subtract other point from point
    inline Point3 &operator -=(const Point3 &Point);
    /// Multiply point with other point
    inline Point3 &operator *=(const Point3 &Point);
    /// Scale point by value
    inline Point3 &operator *=(VarType V);
    /// Divide point by other point
    inline Point3 &operator /=(const Point3 &Point);
    /// Descale point by value
    inline Point3 &operator /=(VarType V);

    /// Assign value to all components
    inline Point3 &operator =(VarType V);
    /// Assign components from POD representation
    inline Point3 &operator =(const POD &Other);
    /// Assign a vector of a different type
    template<typename Y> inline Point3 &operator =(const Point3<Y> &Other) {
      X = Other.X;
      Y = Other.Y;
      Z = Other.Z;
    }
    /// Return negated point
    inline Point3 operator -() const;
    /// Return point added to other point
    inline Point3 operator +(const Point3 &Summand) const;
    /// Return point subtracted by other point
    inline Point3 operator -(const Point3 &Subtrahend) const;
    /// Return point added to other point
    inline Point3 operator *(VarType Factor) const;
    /// Return point added to other point
    inline Point3 operator *(const Point3 &Multiplicand) const;
    /// Return point subtracted by other point
    inline Point3 operator /(VarType Quotient) const;
    /// Return point subtracted by other point
    inline Point3 operator /(const Point3 &Quotient) const;

    /// Check for equality with other point
    inline bool operator ==(const Point3 &Point) const { return (X == Point.X) && (Y == Point.Y) && (Z == Point.Z); }
    /// Check for unequality with other point
    inline bool operator !=(const Point3 &Point) const { return (X != Point.X) || (Y == Point.Y) || (Z == Point.Z); }

  public:
    VarType X, Y, Z;                            ///< Point components
};

template<typename VarType> const Point3<VarType> Point3<VarType>::Right(1, 0, 0);
template<typename VarType> const Point3<VarType> Point3<VarType>::Up(0, 1, 0);
template<typename VarType> const Point3<VarType> Point3<VarType>::In(0, 0, 1);

// ############################################################################################# //
// # Nuclex::Point3::Point3()                                                      Constructor # //
// ############################################################################################# //
/** Creates a new point set to {0,0,0}
*/
template<typename VarType>
inline Point3<VarType>::Point3() :
  X(0), Y(0), Z(0) {}

// ############################################################################################# //
// # Nuclex::Point3::Point3()                                                      Constructor # //
// ############################################################################################# //
/** Creates a new point initialized from its POD representation

    @param  Source  POD representation with which to initialize the point
*/
template<typename VarType>
inline Point3<VarType>::Point3(const POD &Source) :
  X(Source.X), Y(Source.Y), Z(Source.Z) {}

// ############################################################################################# //
// # Nuclex::Point3::Point3()                                                      Constructor # //
// ############################################################################################# //
/** Creates a new point set to the specified values

    @param  X, Y, Z  Point components
*/
template<typename VarType>
inline Point3<VarType>::Point3(VarType X, VarType Y, VarType Z) :
  X(X), Y(Y), Z(Z) {}

// ############################################################################################# //
// # Nuclex::Point3::set()                                                                     # //
// ############################################################################################# //
/** Sets the point by its components

    @param  X, Y, Z  Point components to assign to the Point
*/
template<typename VarType>
inline void Point3<VarType>::set(VarType X, VarType Y, VarType Z) {
  Point3::X = X;
  Point3::Y = Y;
  Point3::Z = Z;
}

// ############################################################################################# //
// # Nuclex::Point3::operator +=()                                                             # //
// ############################################################################################# //
/** Adds the point to another point

    @param  Summand  Point which to add to the point
*/
template<typename VarType>
inline Point3<VarType> &Point3<VarType>::operator +=(const Point3 &Summand) {
  X += Summand.X;
  Y += Summand.Y;
  Z += Summand.Z;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point3::operator =()                                                              # //
// ############################################################################################# //
template<typename VarType>
inline Point3<VarType> &Point3<VarType>::operator =(VarType V) {
  X = V;
  Y = V;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point3::operator =()                                                              # //
// ############################################################################################# //
template<typename VarType>
inline Point3<VarType> &Point3<VarType>::operator =(const POD &Other) {
  X = Other.X;
  Y = Other.Y;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point3::operator -=()                                                             # //
// ############################################################################################# //
/** Subtracts another Point from this Point

    @param  Subtrahend  Point which to subtract
*/
template<typename VarType>
inline Point3<VarType> &Point3<VarType>::operator -=(const Point3 &Subtrahend) {
  X -= Subtrahend.X;
  Y -= Subtrahend.Y;
  Z -= Subtrahend.Z;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point3::operator *=()                                                             # //
// ############################################################################################# //
/** Multiplicates the vector by a scaling value

    @param  Factor  Scaling value to multiply factor with
*/
template<typename VarType>
inline Point3<VarType> &Point3<VarType>::operator *=(VarType Factor) {
  X *= Factor;
  Y *= Factor;
  Z *= Factor;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point3::operator *=()                                                             # //
// ############################################################################################# //
/** Multiplicates the vector by another vector

    @param  Multiplicand  Vector with which to multiplicate
*/
template<typename VarType>
inline Point3<VarType> &Point3<VarType>::operator *=(const Point3 &Multiplicand) {
  X *= Multiplicand.X;
  Y *= Multiplicand.Y;
  Z *= Multiplicand.Z;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point3::operator /=()                                                             # //
// ############################################################################################# //
/** Divides the vector by a scaling value

    @param  Quotient  Scaling value to divide vector by
*/
template<typename VarType>
inline Point3<VarType> &Point3<VarType>::operator /=(VarType Quotient) {
  X /= Quotient;
  Y /= Quotient;
  Z /= Quotient;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point3::operator /=()                                                             # //
// ############################################################################################# //
/** Divides the vector by another vector

    @param  Quotient  Point by which to divide
*/
template<typename VarType>
inline Point3<VarType> &Point3<VarType>::operator /=(const Point3 &Quotient) {
  X /= Quotient.X;
  Y /= Quotient.Y;
  Z /= Quotient.Z;
  return *this;
}

// ############################################################################################# //
// # Nuclex::Point3::negate()                                                                  # //
// ############################################################################################# //
/** Negates the Point
*/
template<typename VarType>
inline void Point3<VarType>::negate() {
  X = -X;
  Y = -Y;
  Z = -Z;
}

// ############################################################################################# //
// # Nuclex::Point3::dotProduct()                                                              # //
// ############################################################################################# //
/** Calculates the dot product of the vector with another vector

    @param  Point  Vector with which to calculate the dot product
    @return The dot product
*/
template<typename VarType>
inline VarType Point3<VarType>::dotProduct(const Point3 &Point) const {
  return (X * Point.X) + (Y * Point.Y) + (Z * Point.Z);
}

// ############################################################################################# //
// # Nuclex::Point3::crossProduct()                                                            # //
// ############################################################################################# //
/** Calculates the cross product of the vector to another vector

    @param  Point  Vector towards which to calculate the cross product
    @return The cross product vector
*/
template<typename VarType>
inline Point3<VarType> Point3<VarType>::crossProduct(const Point3 &Point) const {
  return Point3((Y * Point.Z) - (Z * Point.Y),
                (Z * Point.X) - (X * Point.Z),
                (X * Point.Y) - (Y * Point.X));
}

// ############################################################################################# //
// # Nuclex::Point3::rotateX()                                                                 # //
// ############################################################################################# //
/** Rotates the point around the global X axis

    @param  Angle  Rotation angle in RAD
*/
template<typename VarType>
inline void Point3<VarType>::rotateX(VarType Angle) {
  VarType CY = Y, CZ = Z;
  VarType Sin = Math::sin(Angle);
  VarType Cos = Math::cos(Angle);

  Y = (CY * Cos) - (CZ * Sin);
  Z = (CY * Sin) + (CZ * Cos);
}

// ############################################################################################# //
// # Nuclex::Point3::rotateY()                                                                 # //
// ############################################################################################# //
/** Rotates the point around the global Y axis

    @param  Angle  Rotation angle in RAD
*/
template<typename VarType>
inline void Point3<VarType>::rotateY(VarType Angle) {
  VarType CX = X, CZ = Z;
  VarType Sin = Math::sin(Angle);
  VarType Cos = Math::cos(Angle);

  X = (CZ * Sin) + (CX * Cos);
  Z = (CZ * Cos) - (CX * Sin);
}

// ############################################################################################# //
// # Nuclex::Point3::rotateZ()                                                                 # //
// ############################################################################################# //
/** Rotates the point around the global Z axis

    @param  Angle  Rotation angle in RAD
*/
template<typename VarType>
inline void Point3<VarType>::rotateZ(VarType Angle) {
  VarType CX = X, CY = Y;
  VarType Sin = Math::sin(Angle);
  VarType Cos = Math::cos(Angle);

  X = (CX * Cos) - (CY * Sin);
  Y = (CX * Sin) + (CY * Cos);
}

// ############################################################################################# //
// # Nuclex::Point3::rotateAroundAxis()                                                        # //
// ############################################################################################# //
/** Rotates the point around an axis

    @param  Axis   Axis to rotate around
    @param  Angle  Rotation angle in RAD
*/
template<typename VarType>
inline void Point3<VarType>::rotateAroundAxis(const Point3 &Axis, VarType Angle) {
  Point3 NormAxis(Axis);
  NormAxis.normalize();

  VarType Sin = Math::sin(Angle);
  VarType Cos = Math::cos(Angle);

  VarType CX = X, CY = Y, CZ = Z;
		      
  X = ((1 - Cos) * NormAxis.X * NormAxis.X + Cos * CX) +
      ((1 - Cos) * NormAxis.X * NormAxis.Y - Sin * NormAxis.Z * CY) +
      ((1 - Cos) * NormAxis.Z * NormAxis.X + Sin * NormAxis.Y * CZ);

  Y = ((1 - Cos) * NormAxis.X * NormAxis.Y + Sin * NormAxis.Z * CX) +
      ((1 - Cos) * NormAxis.Y * NormAxis.Y + Cos * CY) +
      ((1 - Cos) * NormAxis.Y * NormAxis.Z - Sin * NormAxis.X * CZ);

  Z = ((1 - Cos) * NormAxis.Z * NormAxis.X - Sin * NormAxis.Y * CX) +
      ((1 - Cos) * NormAxis.Y * NormAxis.Z + Sin * NormAxis.X * CY) +
      ((1 - Cos) * NormAxis.Z * NormAxis.Z + Cos * CZ);
}

// ############################################################################################# //
// # Nuclex::Point3::operator +()                                                              # //
// ############################################################################################# //
/** Adds two Points and returns the result

    @param  Summand  Second Point to sum
    @return The sum of both Points
*/
template<typename VarType>
inline Point3<VarType> Point3<VarType>::operator +(const Point3 &Summand) const {
  return Point3(*this) += Summand;
}

// ############################################################################################# //
// # Nuclex::Point3::operator -()                                                              # //
// ############################################################################################# //
/** Negates the Point
*/
template<typename VarType>
inline Point3<VarType> Point3<VarType>::operator -() const {
  return Point3(-X, -Y, -Z);
}

// ############################################################################################# //
// # Nuclex::Point3::operator -()                                                              # //
// ############################################################################################# //
/** Subtracts two Points and returns the result

    @param  Subtrahend  Point to be subtracted
    @return The result of the substraction of Point3 from Point1
*/
template<typename VarType>
inline Point3<VarType> Point3<VarType>::operator -(const Point3 &Subtrahend) const {
  return Point3(*this) -= Subtrahend;
}

// ############################################################################################# //
// # Nuclex::Point3::operator *()                                                              # //
// ############################################################################################# //
/** Multiplies a Point by a value and returns the result

    @param  Factor  Point to multiplicated by
    @return The result of the multiplication of the Point with the value
*/
template<typename VarType>
inline Point3<VarType> Point3<VarType>::operator *(VarType Factor) const {
  return Point3(*this) *= Subtrahend;
}

// ############################################################################################# //
// # Nuclex::Point3::operator *()                                                              # //
// ############################################################################################# //
/** Multiplies two Points and returns the result

    @param  Multiplicand  Point to be multiplicated with
    @return The result of the multiplication of the two Points
*/
template<typename VarType>
inline Point3<VarType> Point3<VarType>::operator *(const Point3 &Multiplicand) const {
  return Point3(*this) *= Multiplicand;
}

// ############################################################################################# //
// # Nuclex::Point3::operator /()                                                              # //
// ############################################################################################# //
/** Divides two Points and returns the result

    @param  Quotient  Value to be divided by
    @return The result of the division of Point1 by the value
*/
template<typename VarType>
inline Point3<VarType> Point3<VarType>::operator /(VarType Quotient) const {
  return Point3(*this) /= Quotient;
}

// ############################################################################################# //
// # Nuclex::Point3::operator /()                                                              # //
// ############################################################################################# //
/** Divides two Points and returns the result

    @param  Quotient  Point to be divided by
    @return The result of the division of Point1 by Point3
*/
template<typename VarType>
inline Point3<VarType> Point3<VarType>::operator /(const Point3 &Quotient) const {
  return Point3(*this) /= Quotient;
}

#define Vector3 Point3

}} // namespace Nuclex::Math

#endif // NUCLEX_MATH_POINT3_H
