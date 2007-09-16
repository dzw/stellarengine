//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # GameServer - Game management                                              //
//   #    ###  #   #                                                                           //
//   #    ###  #   # Game management server                                                    //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Toxid/Game/GameServer.h"

using namespace Toxid;

// ############################################################################################# //
// # Toxid::GameServer::GameServer()                                               Constructor # //
// ############################################################################################# //
/** Initializes an instance of GameServer
*/
GameServer::GameServer() {
}

// ############################################################################################# //
// # Toxid::GameServer::~GameServer()                                               Destructor # //
// ############################################################################################# //
/** Destroys an instance of GameServer
*/
GameServer::~GameServer() {
}

// ############################################################################################# //
// # Toxid::GameServer::addPlanet()                                                           # //
// ############################################################################################# //
/** Adds a new episode into the list of available episodes

    @param  sName      Name of the episode
    @param  spPlanet  Planet to add
*/
void GameServer::addPlanet(const string &sName, const shared_ptr<Planet> &spPlanet) {
  PlanetMap::iterator PlanetIt = m_Planets.find(sName);
  if(PlanetIt != m_Planets.end())
    throw FailedException("Toxid::GameServer::GameServer()",
                          "An episode with this name already exists");
  
  m_Planets.insert(PlanetMap::value_type(sName, spPlanet));
}

// ############################################################################################# //
// # Toxid::GameServer::clearPlanets()                                                        # //
// ############################################################################################# //
/** Removes all episodes from the game server
*/
void GameServer::clearPlanets() {
  m_Planets.clear();
}

// ############################################################################################# //
// # Toxid::GameServer::enumPlanets()                                                         # //
// ############################################################################################# //
/** Returns a new enumerator over all episodes

    @return A new enumerator over all episodes
*/
shared_ptr<GameServer::PlanetEnumerator> GameServer::enumPlanets() {
  throw NotSupportedException("Toxid::GameServer::GameServer()",
                              "Not implemented yet");
}

// ############################################################################################# //
// # Toxid::GameServer::startGame()                                                            # //
// ############################################################################################# //
void GameServer::startGame(const string &sPlanet) {
  throw NotSupportedException("Toxid::GameServer::startGame()",
                              "Not implemented yet");
}

// ############################################################################################# //
// # Toxid::GameServer::loadGame()                                                             # //
// ############################################################################################# //
void GameServer::loadGame(const shared_ptr<Storage::Stream> &spSource) {
  throw NotSupportedException("Toxid::GameServer::loadGame()",
                              "Not implemented yet");
}

// ############################################################################################# //
// # Toxid::GameServer::saveGame()                                                             # //
// ############################################################################################# //
void GameServer::saveGame(const shared_ptr<Storage::Stream> &spDestination) {
  throw NotSupportedException("Toxid::GameServer::saveGame()",
                              "Not implemented yet");
}

// ############################################################################################# //
// # Toxid::GameServer::restartMap()                                                           # //
// ############################################################################################# //
void GameServer::restartMap() {
  throw NotSupportedException("Toxid::GameServer::restartMap()",
                              "Not implemented yet");
}
