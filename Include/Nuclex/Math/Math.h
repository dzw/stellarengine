//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Math.h - Math supporting library                                          //
// ### # #      ###                                                                            //
// # ### #      ###  Provides a lot of math specific classes and functions which               //
// #  ## #   # ## ## are not contained in the standard library                                 //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_MATH_MATH_H
#define NUCLEX_MATH_MATH_H

#include <cmath>

namespace Nuclex {

/// Math supporting library
/** Contains a large amount math classes which are of interest for 3D programming but
    are generally not available through the C++ standard library. The entire math namespace
    is always imported into the Nuclex main namespace and any components are allowed to
    access this supporting library.
*/
namespace Math {

// These template functions enable compilers to automatically select the
// appropriate function in other templates. If you, for example, want to
// use Vector2 with a fixed point decimal type, just add a specialization
// for your type here.

typedef float real;

/// Round floating point to an integer
template<typename TargetType, typename VarType> inline TargetType round(VarType Value) { return (Value >= 0) ? static_cast<TargetType>(Value + 0.5) : static_cast<TargetType>(Value - 0.5); }

/// Compute the mathematical power of Value to n
template<unsigned long N, typename VarType> inline VarType pow(VarType Value) { return (N <= 1) ? Value : (pow<N - 1, VarType>(Value) * Value); }
/// Compute the mathematical power of Value to 2
template<typename VarType> inline VarType pow2(VarType Value) { return Value * Value; }
/// Get the square root of the specified number
template<typename VarType> inline VarType sqrt(VarType Value) { return static_cast<VarType>(std::sqrt(static_cast<double>(Value))); }

/// Calculate the sine of the specified angle
template<typename VarType> inline VarType sin(VarType Angle) { return static_cast<VarType>(std::sin(static_cast<double>(Angle))); }
/// Calculate the cosine of the specified angle
template<typename VarType> inline VarType cos(VarType Angle) { return static_cast<VarType>(std::cos(static_cast<double>(Angle))); }

/// Get the absolute value of a number
template<typename VarType> inline VarType abs(VarType Value) { return (Value < 0) ? -Value : Value; }
/// Returns the value for pi
template<typename VarType> inline VarType pi() { return static_cast<VarType>(3.1415926535897932384626433832795); }

/// Returns the modulo of the next lower multiple (eg. floormod(-1, 10) returns 9)
template<typename VarType> inline VarType floormod(VarType Value, VarType Quotient) { return (Value >= 0) ? (Value % Quotient) : (Value % Quotient + Quotient); }
template<> inline float floormod<float>(float Value, float Quotient) { return (Value >= 0) ? fmod(Value, Quotient) : (fmod(Value, Quotient) + Quotient); }
template<> inline double floormod<double>(double Value, double Quotient) { return (Value >= 0) ? fmod(Value, Quotient) : (fmod(Value, Quotient) + Quotient); }
template<> inline long double floormod<long double>(long double Value, long double Quotient) { return (Value >= 0) ? fmod(Value, Quotient) : (fmod(Value, Quotient) + Quotient); }

/// Returns the sign of the specified number
template<typename VarType> inline VarType sgn(VarType Value) {
  if(Value < 0)
    return -1;
  else if(Value > 0)
    return +1;
  else
    return 0;
}

}} // namespace Nuclex::Math

#endif // NUCLEX_MATH_MATH_H
