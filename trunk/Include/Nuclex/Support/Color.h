//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Color.h - RGB color                                 //
// ### # #      ###                                                      //
// # ### #      ###  A color represented by floating point RGBA          //
// #  ## #   # ## ## channels                                            //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_COLOR_H
#define NUCLEX_SUPPORT_COLOR_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Math.h"

namespace Nuclex { namespace Support {

//  //
//  Nuclex::Support::Color                                               //
//  //
/// RGBA color
/** Stores a color as its individual RGBA channels and provides some
    manipulation methods
*/
class Color {
  public:
    /// Black color value
    NUCLEX_API static const Color Black;
    /// Red color value
    NUCLEX_API static const Color Red;
    /// Green color value
    NUCLEX_API static const Color Green;
    /// Blue color value
    NUCLEX_API static const Color Blue;
    /// Yellow color value
    NUCLEX_API static const Color Yellow;
    /// Magenta color value
    NUCLEX_API static const Color Magenta;
    /// Cyan color value
    NUCLEX_API static const Color Cyan;
    /// White color value
    NUCLEX_API static const Color White;

    /// Constructor
    NUCLEX_API inline Color() :
      R(1), G(1), B(1), A(1) {}
    /// RGBA color constructor
    NUCLEX_API inline Color(float R, float G, float B, float A = 1) :
      R(R), G(G), B(B), A(A) {}      

  //
  // Color implementation
  //
  public:
    /// Set RGBA values
    NUCLEX_API inline void set(float R, float G, float B, float A = 1);

    /// Add other point to point
    NUCLEX_API inline Color &operator +=(const Color &Summand);
    /// Subtract other point from point
    NUCLEX_API inline Color &operator -=(const Color &Subtrahend);
    /// Multiply point with other point
    NUCLEX_API inline Color &operator *=(const Color &Multiplicand);
    /// Scale point by value
    NUCLEX_API inline Color &operator *=(float Factor);
    /// Divide point by other point
    NUCLEX_API inline Color &operator /=(const Color &Quotient);
    /// Descale point by value
    NUCLEX_API inline Color &operator /=(float Quotient);

    /// Return point added to other point
    NUCLEX_API inline Color operator +(const Color &Summand) const { return Color(*this) += Summand; }
    /// Return point subtracted by other point
    NUCLEX_API inline Color operator -(const Color &Subtrahend) const { return Color(*this) -= Subtrahend; }
    /// Return point multiplied by other point 
    NUCLEX_API inline Color operator *(const Color &Multiplicand) const { return Color(*this) *= Multiplicand; }
    /// Return point scaled by a factor
    NUCLEX_API inline Color operator *(float Factor) const { return Color(*this) *= Factor; }
    /// Return point divided by other point
    NUCLEX_API inline Color operator /(const Color &Quotient) const { return Color(*this) /= Quotient; }
    /// Return point divided by a factor
    NUCLEX_API inline Color operator /(float Quotient) const { return Color(*this) /= Quotient; }

    /// Check for equality with other point
    NUCLEX_API inline bool operator ==(const Color &Other) const {
      return (R == Other.R) && (G == Other.G) && (B == Other.B) && (A == Other.A);
    }
    /// Check for unequality with other point
    NUCLEX_API inline bool operator !=(const Color &Other) const {
      return (R != Other.R) || (G != Other.G) || (B != Other.B) || (A != Other.A);
    }

  public:
    float R, G, B, A;
};  

// ####################################################################### //
// # Nuclex::Color::set()                                                # // 
// ####################################################################### //
/** Sets the color from individual RGBA values

    @param  R, G, B, A  Red, Green, Blue and Alpha components
*/
inline void Color::set(float R, float G, float B, float A) {
  Color::R = R;
  Color::G = G;
  Color::B = B;
  Color::A = A;
}

// ####################################################################### //
// # Nuclex::Color::operator +=()                                        # // 
// ####################################################################### //
/** Adds the point to another point

    @param  Summand  Point which to add to the point
*/
inline Color &Color::operator +=(const Color &Summand) {
  R += Summand.R;
  G += Summand.G;
  B += Summand.B;
  A += Summand.A;
  return *this;
}

// ####################################################################### //
// # Nuclex::Color::operator -=()                                        # // 
// ####################################################################### //
/** Subtracts another Point from this Point

    @param  Subtrahend  Point which to subtract
*/
inline Color &Color::operator -=(const Color &Subtrahend) {
  R -= Subtrahend.R;
  G -= Subtrahend.G;
  B -= Subtrahend.B;
  A -= Subtrahend.A;
  return *this;
}

// ####################################################################### //
// # Nuclex::Color::operator *=()                                        # // 
// ####################################################################### //
/** Multiplicates the vector by a scaling value

    @param  Factor  Scaling value to multiply factor with
*/
inline Color &Color::operator *=(float Factor) {
  R *= Factor;
  G *= Factor;
  B *= Factor;
  A *= Factor;
  return *this;
}

// ####################################################################### //
// # Nuclex::Color::operator *=()                                        # // 
// ####################################################################### //
/** Multiplicates the vector by another vector

    @param  Multiplicand  Vector with which to multiplicate
*/
inline Color &Color::operator *=(const Color &Multiplicand) {
  R *= Multiplicand.R;
  G *= Multiplicand.G;
  B *= Multiplicand.B;
  A *= Multiplicand.A;
  return *this;
}

// ####################################################################### //
// # Nuclex::Color::operator /=()                                        # // 
// ####################################################################### //
/** Divides the vector by a scaling value

    @param  Quotient  Scaling value to divide vector by
*/
inline Color &Color::operator /=(float Quotient) {
  R /= Quotient;
  G /= Quotient;
  B /= Quotient;
  A /= Quotient;
  return *this;
}

// ####################################################################### //
// # Nuclex::Color::operator /=()                                        # // 
// ####################################################################### //
/** Divides the vector by another vector

    @param  Quotient  Point by which to divide
*/
inline Color &Color::operator /=(const Color &Quotient) {
  R /= Quotient.R;
  G /= Quotient.G;
  B /= Quotient.B;
  A /= Quotient.A;
  return *this;
}

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_COLOR_H
