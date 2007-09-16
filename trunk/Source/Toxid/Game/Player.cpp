//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # Episode.h - Set of maps                                                   //
//   #    ###  #   #                                                                           //
//   #    ###  #   # A set of maps which can be played through                                 //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef TOXID_GAME_EPISODE_H
#define TOXID_GAME_EPISODE_H

#include "Toxid/Toxid.h"

namespace Toxid {

class WorldScreen;
class LoadingScreen;
class GameScreen;
class Map;

//  //
//  Toxid::Episode                                                                             //
//  //
/// Episode
/** An episode carries a number of maps and controls how these maps are chained together.
*/
class Episode {
  public:
    /// Destructor
    virtual ~Episode() {}
    
  //
  // Episode implementation
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

#endif // TOXID_GAME_EPISODE_H
