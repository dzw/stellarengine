//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # MapServer - Map management                                                //
//   #    ###  #   #                                                                           //
//   #    ###  #   # Map management server                                                     //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef TOXID_MAP_MAPSERVER_H
#define TOXID_MAP_MAPSERVER_H

#include "Toxid/Toxid.h"
#include "Nuclex/Support/Collection.h"

namespace Toxid {

class FloorFactory;
class BlockFactory;
class ActorFactory;
class ItemFactory;

//  //
//  Toxid::MapServer                                                                           //
//  //
class MapServer {
  public:
    
  //
  // MapServer implementation
  //
  public:
    Collection<FloorFactory> FloorFactories;
    Collection<BlockFactory> BlockFactories;
    Collection<ActorFactory> ActorFactories;
    Collection<ItemFactory> ItemFactories;

  private:
};

} // namespace Toxid

#endif // TOXID_MAP_MAPSERVER_H
