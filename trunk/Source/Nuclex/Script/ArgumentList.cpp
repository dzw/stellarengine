//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## ArgumentList.cpp - Nuclex argument list             //
// ### # #      ###                                                      //
// # ### #      ###  Manages a variable-length list of typed arguments   //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Script/ArgumentList.h"
#include "Nuclex/Support/Exception.h"

using namespace Nuclex;
using namespace Script;

// ####################################################################### //
// # Nuclex::ArgumentList::ArgumentList()                    Constructor # // 
// ####################################################################### //
/** Creates an instance of an empty argument list
*/
ArgumentList::ArgumentList() {}

// ####################################################################### //
// # Nuclex::ArgumentList::ArgumentList()                    Constructor # // 
// ####################################################################### //
/** Creates an instance of an argument list from an argument string.
    The format of the argument string is explained in the ArgumentList
    class description.

    @param  sArgs  Argument string to parse
*/
ArgumentList::ArgumentList(const string &sArgs) {
  parseArgs(sArgs);
}

// ####################################################################### //
// # Nuclex::ArgumentList::ArgumentList()                    Constructor # // 
// ####################################################################### //
/** Creates an instance of an argument list from an argument string.
    The format of the argument string is explained in the ArgumentList
    class description.

    @param  Value  Value to construct the argument list with
*/
ArgumentList::ArgumentList(const Variant &Value) {
  addArgument(Value);
}

// ####################################################################### //
// # Nuclex::ArgumentList::clearArguments()                              # // 
// ####################################################################### //
/** Clears the argument list. Any numeric or string argument will
    be deleted. Blobs and Objects are Release()d.
    After the method returns, the argument list will be in the
    same state as a newly constructed class instance.
*/
void ArgumentList::clearArguments() {
  m_Arguments.clear();
}

// ####################################################################### //
// # Nuclex::ArgumentList::getNumArguments()                             # // 
// ####################################################################### //
/** Retrieves the number of arguments stored in the argument list.

    @return Number of arguments in the list
*/
size_t ArgumentList::getNumArguments() const {
  return m_Arguments.size();
}

// ####################################################################### //
// # Nuclex::ArgumentList::getArgument()                                 # // 
// ####################################################################### //
/** Retrieves an argument by its index.
    The index must be within range of 0 to NumArguments() - 1.

    @param  nIndex  Index of argument to retrieve
*/
Variant &ArgumentList::getArgument(size_t nIndex) {
  if(nIndex >= m_Arguments.size())
    throw InvalidArgumentException("Nuclex::Script::ArgumentList::getArgument()",
                                   string("Index out of range: ") + lexical_cast<string>(nIndex));
  
  return m_Arguments[nIndex];
}

// ####################################################################### //
// # Nuclex::ArgumentList::Argument()                                    # // 
// ####################################################################### //
/** Retrieves an argument by its index.
    The index must be within range of 0 to NumArguments() - 1.

    @param  nIndex  Index of argument to retrieve
*/
const Variant &ArgumentList::getArgument(size_t nIndex) const {
  if(nIndex >= m_Arguments.size())
    throw InvalidArgumentException("Nuclex::Script::ArgumentList::getArgument()",
                                   string("Index out of range: ") + lexical_cast<string>(nIndex));
  
  return m_Arguments[nIndex];
}

// ####################################################################### //
// # Nuclex::ArgumentList::addArgument()                                 # // 
// ####################################################################### //
/** Appends an argument to the argument list. Numeric values and strings
    will be copied (by value) into the argument list, but Blobs and
    Objects will only be stored as pointer (by reference).

    @param  Argument  Argument to append
*/
void ArgumentList::addArgument(const Variant &Argument) {
  m_Arguments.push_back(Argument);
}

// ############################################################################################# //
// # Nuclex::ArgumentList::parseArgs()                                                         # //
// ############################################################################################# //
/** Parses an argument string and appends its arguments to the
    argument list. For an explanation of the argument string format,
    see the ArgumentList class description.

    @param  sArgs  Argument string to be parsed
*/
void ArgumentList::parseArgs(const string &sArgs) {
  int           nStart = 0;
  unsigned int  nCount     = 0;
  bool          bQuoted    = false;
  bool          bNumeric   = true;
  bool          bDecimal   = false;
  bool          bWasQuoted = false;
  std::string   sParseArgs = sArgs + ",";

  if(!sArgs.length())
    return;
  
  while(const char c = sParseArgs.operator[](nCount)) {
    // In quote: Do not parse anything but quote end
    if(bQuoted) {
      if(c == '"')
        bQuoted = false;

    // Does a quote begin here ?
    } else if(c == '"') {
      bQuoted = true;
      bWasQuoted = true;
      bNumeric = false;
      nStart = nCount;

    // Is it a seperator ?
    } else if(c == ',') { 
      if(bNumeric) {
        if(bDecimal)
          addArgument(::atof(sArgs.substr(nStart, nCount - nStart).c_str()));
        else
          addArgument(::atoi(sArgs.substr(nStart, nCount - nStart).c_str()));
      } else {
        if(bWasQuoted) {
          addArgument(string(sArgs.substr(nStart + 1, nCount - nStart - 2).c_str()));
        } else {
          //std::string sString = String::RemoveLeadingAndEndingSpaces(sArgs.substr(nStart, nCount - nStart));
          string sString = sArgs.substr(nStart, nCount - nStart);
          string sLower;
          std::transform(sString.begin(), sString.end(), sLower.begin(), tolower);
          if(sLower == "true")
            addArgument(true);
          else if(sLower == "false")
            addArgument(false);
          else
            addArgument(sString);
        }
      }
      nStart = nCount + 1;
      bNumeric = true;
      bDecimal = false;
      bWasQuoted = false;

    // Keep checking if it's numeric
    } else {
      // Decimal sign found ?
      if(ispunct(c)) {
        if(bDecimal) // Was a decimal sign found already ?
          bNumeric = false; // Then it is not a numeric type
        else
          bDecimal = true;

      // Is this a number ?
      } else if(!isdigit(c) && c != '-' && c != ' ') {
        bNumeric = false;
      }
    }

    nCount++;
  }
}
