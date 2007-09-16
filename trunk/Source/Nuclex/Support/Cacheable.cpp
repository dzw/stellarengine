//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Cacheable.cpp - Cacheable object                    //
// ### # #      ###                                                      //
// # ### #      ###  Attribute for cacheable objects                     //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Support/Cacheable.h"

using namespace Nuclex;
using namespace Nuclex::Support;

Cacheable::CacheID Cacheable::s_nNextUniqueID = 0;

// ####################################################################### //
// # Nuclex::Support::Cacheable::createUniqueID()                        # // 
// ####################################################################### //
/** Creates a new unique id
*/
size_t Cacheable::createUniqueID() {
  return ++s_nNextUniqueID;
}
