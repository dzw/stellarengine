//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Cacheable.h - Cacheable object                      //
// ### # #      ###                                                      //
// # ### #      ###  Attribute for cacheable objects                     //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_CACHEABLE_H
#define NUCLEX_SUPPORT_CACHEABLE_H

#include "Nuclex/Nuclex.h"

namespace Nuclex { namespace Support {

//  //
//  Nuclex::Support::Cacheable                                           //
//  //
/// Cacheable object attribute
/**
*/
class Cacheable {
  public:
    /// Inique ID type
    typedef size_t CacheID;
    
    /// Creates a new unique ID
    NUCLEX_API static size_t createUniqueID();

    /// Constructor
    NUCLEX_API Cacheable() :
      m_nUniqueID(createUniqueID()) {}
    /// Destruktor
    NUCLEX_API virtual ~Cacheable() {}
    
  //
  // Cacheable implementation
  //
  public:  
    /// Get this object's unique ID
    /** Every object is assigned an unique id at construction time. This
        id can be used for optimization in some places (eg. a codec could
        detect successive calls to canLoadXY() and loadXY() with the
        same stream and only load once)

        @return The object's unique id
    */
    NUCLEX_API inline CacheID getUniqueID() const { return m_nUniqueID; }
    
    /// Updates the object's unique ID
    /** Should be called when the object has been modified in a way that
        it does not represent its previous state. When something is blitted
        onto a texture, for example, the texture should receive a new unique id
    */
    NUCLEX_API inline void modified() { m_nUniqueID = createUniqueID(); }

  private:
    static CacheID s_nNextUniqueID;

    CacheID m_nUniqueID;
};

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_CACHEABLE_H
