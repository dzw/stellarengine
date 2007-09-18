//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Synchronization.h - Thread synchronization          //
// ### # #      ###                                                      //
// # ### #      ###  Helper classes for thread synchronization           //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_COLLECTION_H
#define NUCLEX_SUPPORT_COLLECTION_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Support/Exception.h"
#include <map>

namespace Nuclex { namespace Support {

//  //
//  Nuclex::Support::Collection                                          //
//  //
/// Collection
/** A collection of named objects which can be manipulated by a
    standard set of accessor methods
*/
template<
  typename ItemType
>
class Collection {
  public:
    typedef typename ItemType ItemType;
    
    class Enumerator { 
      // ...
    };
    
    NUCLEX_API const shared_ptr<ItemType> &get(const string &sName) const {
      ItemMap::const_iterator ItemIt = m_Items.find(sName);
      if(ItemIt == m_Items.end())
        throw FailedException("Nuclex::Support::Collection::get()",
                              string("Item '") + sName + "' not found");

      return ItemIt->second;
    }
    
    NUCLEX_API bool has(const string &sName) const {
      return m_Items.find(sName) != m_Items.end();
    }
    
    NUCLEX_API void add(const string &sName, const shared_ptr<ItemType> &spItem) {
      ItemMap::const_iterator ItemIt = m_Items.find(sName);
      if(ItemIt != m_Items.end())
        throw FailedException("Nuclex::Support::Collection::add()",
                              string("An item with the name '") + sName + "' already exists");

      m_Items.insert(ItemMap::value_type(sName, spItem));
    }
    
    NUCLEX_API void remove(const string &sName) {
      ItemMap::const_iterator ItemIt = m_Items.find(sName);
      if(ItemIt == m_Items.end())
        throw FailedException("Nuclex::Support::Collection::remove()",
                              string("An item with the name '") + sName + "' does not exist");
                              
      m_Items.erase(ItemIt);
    }

    NUCLEX_API void clear() {
      m_Items.clear();
    }
    
    NUCLEX_API shared_ptr<Enumerator> enumerate() const {
      return shared_ptr<Enumerator>(new Enumerator(m_Items));
    }
  
  private:
    typedef std::map<string, shared_ptr<ItemType> > ItemMap;
    
    ItemMap m_Items;
};

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_COLLECTION_H