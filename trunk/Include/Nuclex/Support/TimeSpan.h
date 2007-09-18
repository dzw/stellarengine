//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## TimeSpan.h - Accurate time span                     //
// ### # #      ###                                                      //
// # ### #      ###  Stores a time span with up to microsecond           //
// #  ## #   # ## ## accuracy using integers                             //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_TIMESPAN_H
#define NUCLEX_SUPPORT_TIMESPAN_H

#include "Nuclex/Nuclex.h"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace Nuclex { namespace Support {

//  //
//  Nuclex::Support::TimeSpan                                            //
//  //
/// Time span
/** Stores a time span by millisecond and microsecond fraction, avoiding
    possible floating point inaccuracies.
    
    This class is designed to store the difference between two points in
    time, not to describe a point in time itself. Thus, negative time
    spans are logically impossible, causing all arithmetical operators
    to automatically assume an overflow if a negative time span would
    result and adjust the time accordingly.
*/
class TimeSpan {
  public:
    /// Get time since application launch
    NUCLEX_API static TimeSpan getRunningTime();
  
    /// Constructor
    NUCLEX_API TimeSpan(size_t Microseconds = 0) :
      Microseconds(Microseconds) {}

  //
  // TimeSpan implementation
  //
  public:
    /// Get number of seconds
    NUCLEX_API float toSeconds() const { return (Microseconds * 0.000001f); }
    
    /// Sum with other time
    NUCLEX_API TimeSpan &operator +=(const TimeSpan &Summand);
    /// Subtract other time from this
    NUCLEX_API TimeSpan &operator -=(const TimeSpan &Subtrahend);
    /// Get division remainder
    NUCLEX_API TimeSpan &operator %=(const TimeSpan &Quotient);
    /// Return sum with other time
    NUCLEX_API TimeSpan operator +(const TimeSpan &Summand) const { return TimeSpan(*this) += Summand; }
    /// Return other time subtracted from this
    NUCLEX_API TimeSpan operator -(const TimeSpan &Subtrahend) const { return TimeSpan(*this) -= Subtrahend; }
    /// Return division remainder                 
    NUCLEX_API TimeSpan operator %(const TimeSpan &Quotient) const { return TimeSpan(*this) %= Quotient; }
    
    /// Check if time span is shorter than another time span
    NUCLEX_API bool operator <(const TimeSpan &Other) const;
    /// Check if time span is long than another time span
    NUCLEX_API bool operator >(const TimeSpan &Other) const;
    /// Check if time span is shorter or equal to another time span
    NUCLEX_API bool operator <=(const TimeSpan &Other) const { return !(*this > Other); }
    /// Check if time span is longer or equal to another time span
    NUCLEX_API bool operator >=(const TimeSpan &Other) const { return !(*this < Other); }
    /// Check for equality with other time
    NUCLEX_API bool operator ==(const TimeSpan &Other) const;
    /// Check for unequality with other time
    NUCLEX_API bool operator !=(const TimeSpan &Other) const;
    
    /// Output time into stream
    NUCLEX_API friend inline std::ostream &operator <<(std::ostream &o, const TimeSpan &t) {
      std::stringstream s;
      s << std::setfill('0') << std::setw(3) << t.Microseconds % 1000;
      
      return o << t.Microseconds / 1000 << "." << s.str();
    }
    
    /// Scale value by number of seconds passed in time span
    template<typename VarType>
    friend inline VarType operator *(VarType v, const TimeSpan &t) {
      return v * toSeconds();
    }
    
  public:
    size_t Microseconds;                              ///< Elapsed Microseconds
};

// ############################################################################################# //
// # Nuclex::TimeSpan::operator +=()                                                           # //
// ############################################################################################# //
/** Sums the time span with another time span

    @param  Summand  Other time span which to add to the time span
    @return The time span, summed with the other time span
*/
inline TimeSpan &TimeSpan::operator +=(const TimeSpan &Summand) {
  Microseconds += Summand.Microseconds;
  return *this;
}

// ############################################################################################# //
// # Nuclex::TimeSpan::operator -=()                                                           # //
// ############################################################################################# //
/** Sums the another time span from the time span

    @param  Subtrahend  Other time span which to subtract from the time span
    @return The time span, after  with the other time span
*/
inline TimeSpan &TimeSpan::operator -=(const TimeSpan &Subtrahend) {
  Microseconds -= Subtrahend.Microseconds;
  return *this;
}

// ############################################################################################# //
// # Nuclex::TimeSpan::operator %=()                                                           # //
// ############################################################################################# //
/** Calculates the division remainder

    @param  Quotient  Quotient by which to return the division remainder
    @return The time span, summed with the other time span
*/
inline TimeSpan &TimeSpan::operator %=(const TimeSpan &Quotient) {
  Microseconds %= Quotient.Microseconds;
  return *this;
}

// ############################################################################################# //
// # Nuclex::TimeSpan::operator <()                                                            # //
// ############################################################################################# //
/** Returns true of the time span is shorter than the other timespan

    @param  Other  Other time span
    @return True if the time span is shorter than the other timespan
*/
inline bool TimeSpan::operator <(const TimeSpan &Other) const {
  
  return Microseconds < Other.Microseconds;
}

// ############################################################################################# //
// # Nuclex::TimeSpan::operator >()                                                            # //
// ############################################################################################# //
/** Returns true of the time span is longer than the other timespan

    @param  Other  Other time span
    @return True if the time span is longer than the other timespan
*/
inline bool TimeSpan::operator >(const TimeSpan &Other) const {
  return Microseconds > Other.Microseconds;
}

// ############################################################################################# //
// # Nuclex::TimeSpan::operator ==()                                                           # //
// ############################################################################################# //
/** Returns true of the time span is equal to the other timespan

    @param  Other  Other time span
    @return True if the time span is equal than the other timespan
*/
inline bool TimeSpan::operator ==(const TimeSpan &Other) const {
  return (Microseconds == Other.Microseconds);
}

// ############################################################################################# //
// # Nuclex::TimeSpan::operator !=()                                                           # //
// ############################################################################################# //
/** Returns true of the time span is not equal to the other timespan

    @param  Other  Other time span
    @return True if the time span is not equal than the other timespan
*/
inline bool TimeSpan::operator !=(const TimeSpan &Other) const {
  return (Microseconds != Other.Microseconds);
}

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_TIMESPAN_H
