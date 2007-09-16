//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## TextureCache.cpp - Texture cache                                          //
// ### # #      ###                                                                            //
// # ### #      ###  Caches images and smaller textures onto intermediary                      //
// #  ## #   # ## ## textures to reduce temporary objects and texture switching                //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Video/TextureCache.h"
#include "ScopeGuard/ScopeGuard.h"

using namespace Nuclex;
using namespace Nuclex::Video;

// ############################################################################################# //
// # Nuclex::Video::TextureCache::TextureCache()                                   Constructor # //
// ############################################################################################# //
/** Initializes a texture cache. The texture cache will immediately create its first
    cache texture, thus taking up some video memory
    
    @param  spVideoDevice  Video device for which the cache is intended
*/
TextureCache::TextureCache(const shared_ptr<VideoDevice> &spVideoDevice) :
  m_spVideoDevice(spVideoDevice),
  m_Resolution(spVideoDevice->getMaxTextureSize()) {

  // Calculate the optimal cache texture size for the chosen device
  if(spVideoDevice->getVideoMemorySize() < 64) 
    m_Resolution = Point2<size_t>(
      std::min<size_t>(m_Resolution.X, 512),
      std::min<size_t>(m_Resolution.Y, 512)
    ); // Max of 1 MB per texture
  else if(spVideoDevice->getVideoMemorySize() < 256)
    m_Resolution = Point2<size_t>(
      std::min<size_t>(m_Resolution.X, 1024),
      std::min<size_t>(m_Resolution.Y, 1024)
    ); // Max of 4 MB per texture
  else
    m_Resolution = Point2<size_t>(
      std::min<size_t>(m_Resolution.X, 2048),
      std::min<size_t>(m_Resolution.Y, 2048)
    ); // Max of 16 MB per texture

  // Create the first shared texture here - saves us an additional check when caching
  appendSharedTexture();
}

// ############################################################################################# //
// # Nuclex::Video::TextureCache::cache()                                                      # //
// ############################################################################################# //
/** Caches a texture. If the texture is larger than a quarter of the cache's texture
    resolution, cache returns an uncached.
    
    @param  spSurface  Surface to be cached
    @return A cache slot specifying which texture to use and the location of the
            cached surface on that texture
*/
TextureCache::CacheSlot TextureCache::cache(const shared_ptr<Texture> &spTexture) {
  // Look for the texture in the cache
  CacheEntryMap::iterator CacheEntryIt = m_CacheEntries.find(spTexture->getUniqueID());
  if(CacheEntryIt == m_CacheEntries.end()) {
  
    // Textures larger than one forth of the cache are just passed through
    if((spTexture->getSize().X > m_Resolution.X / 2) ||
       (spTexture->getSize().Y > m_Resolution.Y / 2))
      return CacheSlot(spTexture, Box2<float>(0,0, 1,1));
      
    // Everything else gets placed in the cache
    else
      CacheEntryIt = addToCache(spTexture);
  }

  // Return a valid cache slot
  return CacheSlot(CacheEntryIt->second.spCacheTexture, CacheEntryIt->second.Location);
}

// ############################################################################################# //
// # Nuclex::Video::TextureCache::cache()                                                      # //
// ############################################################################################# //
/** Caches a surface

    @param  spSurface  Surface to be cached
    @return A cache slot specifying which texture to use and the location of the
            cached surface on that texture
*/
TextureCache::CacheSlot TextureCache::cache(
  Cacheable::CacheID ID, const Point2<size_t> Size, UpdateCacheSlot UpdateCallback
) {
  // Look for the object in the cache
  CacheEntryMap::iterator CacheEntryIt = m_CacheEntries.find(ID);
  if(CacheEntryIt == m_CacheEntries.end()) {
  
    // Textures larger than one forth of the cache are just passed through
    if((Size.X > m_Resolution.X) || (Size.Y > m_Resolution.Y)) {
      throw FailedException(
        "Nuclex::Video::TextureCache::cache()",
        "Object is too large to fit in the cache"
      );
      
    // Everything else gets placed in the cache
    } else {
      // Find a free location for the surface within the current cache texture
      Point2<size_t> Location = m_spCurrentTexture->Packer.placeRectangle(Size);
      if(Location == m_Resolution) {
        // If it doesn't fit anymore, try using a new cache texture
        appendSharedTexture();
        Location = m_spCurrentTexture->Packer.placeRectangle(Size);
      }

      { Surface::LockInfo LockedSurface = m_spCurrentTexture->spTexture->lock(
          Surface::LM_WRITE, Box2<long>(Location, Location + Size)
        );
      
        ScopeGuard Unlock_Dest = ::MakeObjGuard(
          *m_spCurrentTexture->spTexture.get(), &Surface::unlock
        );
        UpdateCallback(LockedSurface);
      }

      // Add the cache entry into the internet list and return its the iterator
      CacheEntryIt = m_CacheEntries.insert(CacheEntryMap::value_type(
        ID,
        CacheEntry(
          shared_ptr<Surface>(),
          m_spCurrentTexture->spTexture,
          Box2<float>(
            Point2<float>(Location, StaticCastTag()) /
              Point2<float>(m_Resolution, StaticCastTag()),
            Point2<float>(Location + Size, StaticCastTag()) /
              Point2<float>(m_Resolution, StaticCastTag())
          )
        )
      )).first;
    }
  }

  // Return a valid cache slot
  return CacheSlot(CacheEntryIt->second.spCacheTexture, CacheEntryIt->second.Location);
}

// ############################################################################################# //
// # Nuclex::Video::TextureCache::cache()                                                      # //
// ############################################################################################# //
/** Caches a surface

    @param  spSurface  Surface to be cached
    @return A cache slot specifying which texture to use and the location of the
            cached surface on that texture
*/
TextureCache::CacheSlot TextureCache::cache(const shared_ptr<Surface> &spSurface) {
  // Look for the surface in the cache
  CacheEntryMap::iterator CacheEntryIt = m_CacheEntries.find(spSurface->getUniqueID());
  if(CacheEntryIt == m_CacheEntries.end()) {
  
    // If it is too large, we have no option but to give up and throw
    if((spSurface->getSize().X > m_Resolution.X) || (spSurface->getSize().Y > m_Resolution.Y))
      throw FailedException(
        "Nuclex::Video::TextureCache::cache()",
        "Surface is too large to fit in the cache"
      );

    // If it fits, place it in the cache
    else
      CacheEntryIt = addToCache(spSurface);
  }

  // Return a valid cache slot
  return CacheSlot(CacheEntryIt->second.spCacheTexture, CacheEntryIt->second.Location);
}

// ############################################################################################# //
// # Nuclex::Video::TextureCache::flush()                                                      # //
// ############################################################################################# //
/** Cleans the cache, throwing out any objects not needed anymore

    @todo Cache textures with a use_count of 1 after cleaning should be destroyed
    @todo Maybe even the textures should be removed and the cache list be cleared ?
*/
void TextureCache::flush() {
  m_CacheEntries.clear();
  m_CacheTextures.clear();
  appendSharedTexture();
}

// ############################################################################################# //
// # Nuclex::Video::TextureCache::addToCache()                                                 # //
// ############################################################################################# //
/** Adds the specified surface into the cache and returns an iterator to the newly
    generated cache entry for the surface
    
    @param  spSurface  Surface to be cached
    @return An iterator to the CacheEntry containing the surface
*/
TextureCache::CacheEntryMap::iterator TextureCache::addToCache(
  const shared_ptr<Surface> &spSurface
) {
  // Find a free location for the surface within the current cache texture
  Point2<size_t> Location = m_spCurrentTexture->Packer.placeRectangle(spSurface->getSize());
  if(Location == m_Resolution) {
    // If it doesn't fit anymore, try using a new cache texture
    appendSharedTexture();
    Location = m_spCurrentTexture->Packer.placeRectangle(spSurface->getSize());
  }
  
  // Copy the surface pixels onto the cache texture
  spSurface->blitTo(m_spCurrentTexture->spTexture, Location);

  // Add the cache entry into the internet list and return its the iterator
  return m_CacheEntries.insert(CacheEntryMap::value_type(
    spSurface->getUniqueID(),
    CacheEntry(
      spSurface,
      m_spCurrentTexture->spTexture,
      Box2<float>(
        Point2<float>(Location, StaticCastTag()) / Point2<float>(m_Resolution, StaticCastTag()),
        Point2<float>(Location + spSurface->getSize(), StaticCastTag()) /
          Point2<float>(m_Resolution, StaticCastTag())
      )
    )
  )).first;
}

// ############################################################################################# //
// # Nuclex::Video::TextureCache::appendSharedTexture()                                        # //
// ############################################################################################# //
/** Appends a new shared texture to the cache. Called when the current cache texture is
    full and another texture is required to place a new object on
*/
void TextureCache::appendSharedTexture() {
  m_CacheTextures.push_back(
    shared_ptr<SharedTexture>(new SharedTexture(
      m_spVideoDevice->createTexture(m_Resolution, VideoDevice::AC_BLEND)
    ))
  );

  m_spCurrentTexture = m_CacheTextures.back();
}

// ############################################################################################# //
// # Nuclex::Video::TextureCache::SharedTexture::RectanglePacker::RectanglePacker()            # //
// ############################################################################################# //
/** Initializes a rectangle packer

    @param  Size  Total size of packing area
*/
TextureCache::SharedTexture::RectanglePacker::RectanglePacker(const Point2<size_t> &Size) :
  m_Size(Size),
  m_lCurrentLine(0),
  m_lMaxHeight(0),
  m_lXPosition(0) {}

// ############################################################################################# //
// # Nuclex::Video::TextureCache::SharedTexture::RectanglePacker::placeRectangle()             # //
// ############################################################################################# //
/** Places a rectangle in the packing area. If the packing area is too full to place the
    rectangle, the size of the packing area will be returned to indicate that a new
    shared texture should be created by the cache
    
    @param  Size  Size of the rectangle to put in the packing area
    @return The location at which the rectangle was placed
*/
Point2<size_t> TextureCache::SharedTexture::RectanglePacker::placeRectangle(
  const Point2<size_t> &Size
) {
  // Filtering out rectangles that are too large should be handled by our owner
  assert((Size.X <= m_Size.X) && (Size.Y <= m_Size.Y));

  // Do we have to start a new line ?
  if((m_lXPosition + Size.X) > m_Size.X) {
    size_t NewLine = m_lCurrentLine + m_lMaxHeight;

    // Check if it will fit then
    if((NewLine + Size.Y) > m_Size.Y)
      return m_Size;

    // If it fits in the new line, make the advancement to the next line
    m_lCurrentLine = NewLine;
    m_lMaxHeight = 0;
    m_lXPosition = 0;
  }
  
  // Save the placement of the rectangle and advance to the right
  Point2<long> Position(static_cast<long>(m_lXPosition), static_cast<long>(m_lCurrentLine));
  m_lXPosition += Size.X;
  if(Size.Y > m_lMaxHeight)
    m_lMaxHeight = Size.Y;

  return Position;
}
