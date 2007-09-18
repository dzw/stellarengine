//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## BitManipulation.h - Bit manipulation functions      //
// ### # #      ###                                                      //
// # ### #      ###  Helper functions for dealing with low-level         //
// #  ## #   # ## ## bit manipulations                                   //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_BITMANIPULATION_H
#define NUCLEX_SUPPORT_BITMANIPULATION_H

#include "Nuclex/Nuclex.h"

namespace Nuclex { namespace Support {

/// Statically returns the maximum of the supplied values
template<size_t Left, size_t Right> struct StaticMax { enum { Result = (Left > Right) ? Left : Right }; };

/// Creates an unique data type for true and false
template<bool b> struct Bool2Type { enum { Value = b }; };

//  //
//  Nuclex::Support::BitRange                                            //
//  //
/// Compile time bit mask builder
/** Constructs a bit mask with all bits set from the indices indicated
    by First up to Last (exclusive). To build the value 255, for example,
    you would use BitMask<0, 8>::Value

    Standard C++ enum bit allocation regulations apply. The user is
    responsible to assign the resulting value to a data type large
    enough to hold the requested bits
*/
template<size_t First, size_t Last>
struct BitRange {
  private:
    // This little trick is required because the compiler would evaluate
    // both branches if we use the ternary operator instead
    template<bool> struct MaskBuilder {
      enum { Value = BitRange<First + 1, Last>::Mask | (1 << First) };
    };
    template<> struct MaskBuilder<false> {
      enum { Value = 0 };
    };

  public:
    enum { 
      First = First,                                  ///< The first bit that is set
      Last  = Last,                                   ///< The last bit that is set
      Count = Last - First,                           ///< The number of bits that are set
      Mask  = MaskBuilder<(First < Last)>::Value      ///< The bit mask
    };
};

#pragma warning(push)
#pragma warning(disable : 4293 4333)

//  //
//  Nuclex::Support::Shift                                               //
//  //
/// Bit shifter
/** Performs a bit shift with the direction indicated by sign or the
    value to shift by. A shift by -2 bits, for example, would result in
    a left shift by +2 bits.
*/
template<typename VarType, int Places>
struct Shift {
  // A shift like >> -1 would translate to "shift right by 0xffffffff places",
  // so we use this helper to perform a right shift for positive values or a
  // left shift for negative values.
  inline VarType operator()(VarType Value) { return operator()<(Places >= 0)>(Value); }

  private:
    // Statically select which shift operator to use
    template<bool bRight> inline VarType operator()(VarType Value);
    template<> inline VarType operator()<false>(VarType Value) { return Value << (-Places); }
    template<> inline VarType operator()<true>(VarType Value) { return Value >> Places; }
};

#pragma warning(pop)

//  //
//  Nuclex::Support::Repeat                                              //
//  //
/// Bit pattern repeater
/** Allows to repeat a runtime-specified bit pattern of compile-time
    known size until it reaches a length that is known at compile-time.
    The resulting bit pattern can actually be longer (up to twice as long)
    than the specified number of required bits
*/
template<typename VarType, size_t BitCount, size_t RequiredBits>
struct Repeat {
  /// Repeats the specified bit pattern
  inline VarType operator()(VarType Bits) {
    // We try to gain speed by recursively duplicating the bit pattern,
    // reducing the number of operations needed to log(n)
    return operator()<(BitCount < RequiredBits)>(Bits);
  }
  
  private:
    /// Makes sure that there is at least one bit to repeat
    template<bool SizeNotZero> struct BitCountChecker;
    template<> struct BitCountChecker<true> {};
    BitCountChecker<BitCount != 0> ERROR_BitCount_must_not_be_set_to_zero;
    
    /// Performs the actual repetition and recursively initiates the next
    template<bool bRepeat> inline VarType operator()(VarType Bits);
    template<> inline VarType operator()<true>(VarType Bits) {
      return Repeat<VarType, BitCount * 2, RequiredBits>()(
        Bits | (Bits << static_cast<VarType>(BitCount))
      );
    }
    template<> inline VarType operator()<false>(VarType Bits) {
      return Bits;
    }
};

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_BITMANIPULATION_H
