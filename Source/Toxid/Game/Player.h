//  //
// ##### #   # ####                            -= Nuclex Library =-                            //
//   #   ## ## #   # Episode.h - Set of maps                                                   //
//   #    ###  #   #                                                                           //
//   #    ###  #   # A set of maps which can be played through                                 //
//   #   ## ## #   #                                                                           //
//   #   #   # ####  R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef TOXID_GAME_PLAYER_H
#define TOXID_GAME_PLAYER_H

#include "Toxid/Toxid.h"
#include <list>

namespace Toxid {

class Item;

//  //
//  Toxid::Player                                                                              //
//  //
/// Episode
/** An episode carries a number of maps and controls how these maps are chained together.
*/
class Player {
  public:

  //
  // Player implementation
  //
  public:

  private:
    typedef std::list<shared_ptr<Item> > ItemList;
    
    ItemList m_Inventory;                             ///< Carried items
};

} // namespace Toxid

#endif // TOXID_GAME_PLAYER_H
