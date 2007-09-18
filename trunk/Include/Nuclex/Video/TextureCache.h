//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## TextureCache.h - Texture cache                                            //
// ### # #      ###                                                                            //
// # ### #      ###  Caches images and smaller textures onto intermediary                      //
// #  ## #   # ## ## textures to reduce temporary objects and texture switching                //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_TEXTURECACHE_H
#define NUCLEX_VIDEO_TEXTURECACHE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/Video.h"
#include "Nuclex/Video/VideoDevice.h"
#include "Nuclex/Video/Texture.h"
#include "Nuclex/Video/Image.h"
#include "SigC++/sigc++.h"
#include <map>

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::TextureCache                                                                //
//  //
/// Dynamic texture cache
/** Caches multiple smaller textures on large cache textures to reduce the number of
    required texture switches and drawPrimitive() calls for frame.
    
    @todo If a new font is used, the frame rate may crash down temporarily due to locking the
          cache texture up to 50 times in a row. Maybe the lock could be kept active until
          the cache texture is required for rendering ?
          After profiling, it seems lock is no problem at all, compared to the time taken
          for copying the image pixels :-/
*/
class TextureCache {
  public:
    typedef std::pair<shared_ptr<Texture>, Box2<float> > CacheSlot;
    typedef sigc::slot<void, const Surface::LockInfo &> UpdateCacheSlot;

    /// Constructor
    NUCLEX_API TextureCache(const shared_ptr<VideoDevice> &spVideoDevice);

  //
  // TextureCache implementation
  //
  public:
    /// Maximum size an image is allowed to have in order to fit in the cache
    NUCLEX_API Point2<size_t> getMaxResolution() const { return m_Resolution; }
    /// Retrieves the number of cache textures used
    NUCLEX_API size_t getTextureCount() const { return m_CacheTextures.size(); }
    
    /// @todo For debugging purpose. Remove it!
    NUCLEX_API const shared_ptr<Texture> &getTexture(size_t Index) const {
      return m_CacheTextures.at(Index)->spTexture;
    }

    /// Cache a texture
    NUCLEX_API CacheSlot cache(const shared_ptr<Texture> &spTexture);
    /// Cache an arbitrary size image
    NUCLEX_API CacheSlot cache(const shared_ptr<Surface> &spSurface);
    /// Cache a random thing ;)
    NUCLEX_API CacheSlot cache(
      Cacheable::CacheID ID, const Point2<size_t> Size, UpdateCacheSlot UpdateCallback
    );

    /// Try to clean the cache
    NUCLEX_API void flush();
    
  private:
    /// Surface with cached images
    struct SharedTexture {
      /// Optimized rectangle allocator
      struct RectanglePacker {
        /// Constructor
        RectanglePacker(const Point2<size_t> &Size);
        /// Allocate space for a rectangle
        Point2<size_t> placeRectangle(const Point2<size_t> &Size);

        private:
          Point2<size_t> m_Size;                      ///< Total packing area size
          size_t         m_lCurrentLine;              ///< Current packing line
          size_t         m_lMaxHeight;                ///< Largest rectangle in current line
          size_t         m_lXPosition;                ///< Offset within current line
      };

      /// Constructor
      SharedTexture(const shared_ptr<Texture> &spTexture) :
        Packer(spTexture->getSize()),
        spTexture(spTexture) {}

      RectanglePacker     Packer;                     ///< Rectangle allocator
      shared_ptr<Texture> spTexture;                  ///< Texture with cached images
    };

    /// An entry in the texture cache
    struct CacheEntry {
      /// Initializes a texture cache entry
      CacheEntry(const weak_ptr<void> &wpSurface, const shared_ptr<Texture> &spCacheTexture,
                 Box2<float> Location) :
        wpSurface(wpSurface), spCacheTexture(spCacheTexture), Location(Location) {}

      weak_ptr<void>      wpSurface;                  ///< The object itself
      shared_ptr<Texture> spCacheTexture;             ///< Used cache texture
      Box2<float>         Location;                   ///< Location of the cached object
    };
    
    /// Vector of shared textures
    typedef std::vector<shared_ptr<SharedTexture> > SharedTextureVector;
    /// Map of cache entries by id
    typedef std::map<Cacheable::CacheID, CacheEntry> CacheEntryMap;

    /// Appends a new shared cache texture to the cache
    void appendSharedTexture();
    /// Caches an arbitrary size surface
    CacheEntryMap::iterator addToCache(const shared_ptr<Surface> &spSurface);

    shared_ptr<VideoDevice>   m_spVideoDevice;        ///< Owner of the VertexDrawer
    SharedTextureVector       m_CacheTextures;        ///< The cache's textures
    shared_ptr<SharedTexture> m_spCurrentTexture;     ///< Current cache texture
    Point2<size_t>            m_Resolution;           ///< Cache texture resolution
    CacheEntryMap             m_CacheEntries;         ///< Texture cache entries
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_VIDEOSERVER_H
