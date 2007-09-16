//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # GameServer.h - Game management                                            //
//   #    ###  #   #                                                                           //
//   #    ###  #   # Manages the overall game state and objects                                //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef TOXID_GAME_GAMESERVER_H
#define TOXID_GAME_GAMESERVER_H

#include "Toxid/Toxid.h"
#include "Toxid/Game/Player.h"
#include "Nuclex/Storage/Stream.h"
#include <map>

namespace Toxid {

class Map;
class Planet;

//  //
//  Toxid::GameServer                                                                          //
//  //
class GameServer {
  public:
    class PlanetEnumerator;
    
    /// Constructor
    GameServer();
    /// Destructor
    ~GameServer();

  //
  // GameServer implementation
  //
  public:
    /// Add episode to list of episodes
    void addPlanet(const string &sName, const shared_ptr<Planet> &spPlanet);
    /// Remove episode from list
    void clearPlanets();
    /// Enumerate all episodes
    shared_ptr<PlanetEnumerator> enumPlanets();

    /// Start a new game
    void startGame(const string &sPlanet);
    /// Load game from stream
    void loadGame(const shared_ptr<Storage::Stream> &spSource);
    /// Save game to stream
    void saveGame(const shared_ptr<Storage::Stream> &spDestination);
    /// Restart the current map
    void restartMap();

    /// Get player
    Player &getPlayer() { return m_Player; }
    /// Get active map (may return NULL)
    const shared_ptr<Map> &getActiveMap() { return m_spActiveMap; }

  private:
    typedef std::map<string, shared_ptr<Planet> > PlanetMap;
    
    PlanetMap      m_Planets;                       ///< All episodes
    Player          m_Player;                         ///< The player
    shared_ptr<Map> m_spActiveMap;                    ///< Current map
};

} // namespace Toxid

#endif // TOXID_GAME_GAMESERVER_H
