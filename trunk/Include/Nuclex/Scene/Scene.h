//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## SceneServer.h - Scene server                        //
// ### # #      ###                                                      //
// # ### #      ###  Scene graph management                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NCX_SCENESERVER_H
#define NCX_SCENESERVER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"

#include <map>

namespace Nuclex {

//  //
//  Nuclex::SceneServer                                                     //
//  //
/// Scene server
/**
*/
class SceneServer {
  public:
    /// Constructor
    NUCLEX_API SceneServer();                    
    /// Destructor
    NUCLEX_API virtual ~SceneServer();

  //
  // SceneServer implementation
  //
  public:
};

} // namespace Nuclex

#endif NCX_SCENESERVER_H
