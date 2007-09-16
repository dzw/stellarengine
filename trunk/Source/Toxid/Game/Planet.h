//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # Planet.h - Set of maps                                                    //
//   #    ###  #   #                                                                           //
//   #    ###  #   # A set of maps which can be played through                                 //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef TOXID_GAME_PLANET_H
#define TOXID_GAME_PLANET_H

#include "Toxid/Toxid.h"

namespace Toxid {

class WorldScreen;
class LoadingScreen;
class GameScreen;
class Map;

//  //
//  Toxid::Planet                                                                              //
//  //
/// Planet
/** An episode carries a number of maps and controls how these maps are chained together.
*/
class Planet {
  public:
    /// Destructor
    virtual ~Planet() {}
    
  //
  // Planet implementation
  //
  public:
    /// Create a new world map
    virtual shared_ptr<WorldScreen> createWorldScreen() = 0;
    /// Create a loading screen
    virtual shared_ptr<LoadingScreen> createLoadingScreen() = 0;
    /// Create the ingame screen
    virtual shared_ptr<GameScreen> createGameScreen() = 0;
    /// Create a map of the episode
    virtual shared_ptr<Map> createMap(size_t Index) = 0;
};

} // namespace Toxid

#endif // TOXID_GAME_PLANET_H
