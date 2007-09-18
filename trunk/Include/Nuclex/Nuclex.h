//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Nuclex.h - Nuclex base header                                             //
// ### # #      ###                                                                            //
// # ### #      ###  Root header to be included by all nuclex headers                          //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_NUCLEX_H
#define NUCLEX_NUCLEX_H

#include "Nuclex/Config.h"
#include "Boost/shared_ptr.hpp" 
#include "Boost/weak_ptr.hpp" 
#include <string>

/// Nuclex
/** This is the root namespace of the entire nuclex framework. Subsystems
    within nuclex will be located in nested namespaces, closely
    resembling the directory structure used for the nuclex sources.
  
    There are two utility namespaces which will automatically be imported
    into the nuclex namespace: 'Support' and 'Math'.
*/
namespace Nuclex {
  // Nuclex incorporates boost's shared_ptr<> and weak_ptr<>,
  // internally both are seen as part of the engine
  using ::boost::shared_ptr;
  using ::boost::weak_ptr;
  
  // STL strings fully suffice for nuclex' string implementation
  using ::std::wstring;
  using ::std::string;

  // The utility namespaces are considered a part of nuclex
  // and only exist to keep consistency with other modules
  namespace Math {} using namespace Math;
  namespace Support {} using namespace Support;

  struct StaticCastTag {};
}  

#endif // NUCLEX_NUCLEX_H
