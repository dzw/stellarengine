//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## ArgumentList.h - Nuclex argument list               //
// ### # #      ###                                                      //
// # ### #      ###  Manages a variable-length list of typed arguments   //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCRIPT_ARGUMENTLIST_H
#define NUCLEX_SCRIPT_ARGUMENTLIST_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Storage/Persistable.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Support/Variant.h"
#include "Nuclex/Support/Exception.h"

#include <deque>

namespace Nuclex { namespace Script {

//  //
//  Nuclex::Script::ArgumentList                                         //
//  //
/// Nuclex Argument list
/** This class manages a variable-length list of arguments. Any type
    Scripted by Variant can be stored in the list.

    The argument list also provides a constructor to implicitely parse
    arguments from a string. A comma is used to seperate individual
    arguments. Arguments containing only numbers are seen as integers or
    as doubles, if there's a decimal point. Anything else will be
    treated as strings, with the exception of True and False, which will
    result in a boolean value. Using quotation marks, the value can be 
    forced to be a string and also to contain commas.
    
    Example:
      "String 1", 2, 3.3, "4", False, "5, True"
      will produce 6 values: string, int, double, string, bool, string
*/
class ArgumentList {
  public:
    /// Argument string parse error
    NUCLEX_DECLAREBASEEXCEPTION(ParseErrorException);

    /// Constructor
    NUCLEX_API ArgumentList();
    /// Parse string constructor
    NUCLEX_API ArgumentList(const string &sArgs);
    /// Value constructor
    NUCLEX_API ArgumentList(const Variant &Value);

  //
  // ArgumentList implementation
  //
  public:
    /// Clear all arguments
    NUCLEX_API void clearArguments();
    /// Get number of arguments
    NUCLEX_API size_t getNumArguments() const;

    /// Parse argument string
    NUCLEX_API void parseArgs(const string &sArgs);

    /// Retrieve argument
    NUCLEX_API Variant &getArgument(size_t nIndex);
    /// Retrieve argument
    NUCLEX_API const Variant &getArgument(size_t nIndex) const;
    /// Add argument
    NUCLEX_API void addArgument(const Variant &Argument);

  private:
    /// List of arguments
    typedef std::deque<Variant> ValueDeque;

    ValueDeque m_Arguments;                           ///< Arguments
};

}} // namespace Nuclex::Script

#endif // NUCLEX_SCRIPT_ARGUMENTLIST_H
