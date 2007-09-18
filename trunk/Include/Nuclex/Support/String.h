//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## String.h - Nuclex string class                      //
// ### # #      ###                                                      //
// # ### #      ###  Nuclex string class, extends std::string with a     //
// #  ## #   # ## ## few additional methods                              //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_STRING_H
#define NUCLEX_SUPPORT_STRING_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"

#include <string>
#include <sstream>
#include <vector>

namespace Nuclex {
  namespace Storage { class Stream; }
}

namespace Nuclex { namespace Support {

// ####################################################################### //
// # Nuclex::Support::lexical_cast()                                     # //
// ####################################################################### //
/// Performs a lexical cast to another data type
/** A lexical cast forms a string from the provided source data
    and tries to interpret it as the target data type
    
    @param  from  Value to be converted
    @return The converted value
*/
template<typename TargetType, typename SourceType>
inline TargetType lexical_cast(SourceType from) {
  std::stringstream s;
  s << from;
  
  TargetType to;
  s >> to;
  
  return to;
}

// ####################################################################### //
// # Nuclex::Support::wlexical_cast()                                    # //
// ####################################################################### //
/// Performs a unicode lexical cast to another data type
/** A unicode lexical cast forms a string from the provided
    source data and tries to interpret it as the target data type
    
    @param  from  Value to be converted
    @return The converted value
*/
template<typename TargetType, typename SourceType>
inline TargetType wlexical_cast(SourceType from) {
  std::wstringstream s;
  s << from;
  
  TargetType to;
  s >> to;
  
  return to;
}

/// Converts an ascii string into an unicode string
NUCLEX_API std::wstring unicodeFromAscii(const std::string &sAscii);
/// Converts an unicode string into an ascii string
NUCLEX_API std::string asciiFromUnicode(const std::wstring &sUnicode);

/// Performs a wildcard match on an ascii string
NUCLEX_API bool wildcardMatch(const string &sString, const string &sWildcard);
/// Performs a wildcard match on an unicode string
NUCLEX_API bool wildcardMatch(const wstring &sString, const wstring &sWildcard);

/// Read string out of a stream
NUCLEX_API string stringFromStream(const shared_ptr<Storage::Stream> &spStream);

/// Retrieves the CRC32 of a memory block
NUCLEX_API unsigned_32 getCRC32(const void *pData, size_t Length);

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_STRING_H
