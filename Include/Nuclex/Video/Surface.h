//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Surface.h - Bitmap surface                                                //
// ### # #      ###                                                                            //
// # ### #      ###  A two-dimensional array of pixels which forms an image,                   //
// #  ## #   # ## ## supporting multiple pixel formats                                         //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_SURFACE_H
#define NUCLEX_VIDEO_SURFACE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Box2.h"
#include "Nuclex/Support/Color.h"
#include "Nuclex/Support/Cacheable.h"

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::Surface                                                                     //
//  //
/// Bitmap surface
/**
*/
class Surface :
  public Cacheable {
  public:
    typedef size_t PixelType;

    /// Lock access mode
    enum LockMode {
      LM_READ = 1,                                    ///< Read-only lock
      LM_WRITE = 2,                                   ///< Write-only lock
      LM_READWRITE = 3,                               ///< Read-and-write lock
      LM_OVERWRITEALL = 6,                            ///< Write-everything lock
      LM_APPENDONLY = 10,                             ///< Write-without-overwriting lock
    };

    /// Pixel format
    enum PixelFormat {
      PF_NONE = 0,                                    ///< None or unknown
      PF_A_8,                                         ///< 8 Bit Alpha
      PF_RGB_3_3_2,                                   ///< 8 Bit RGB-332
      PF_RGB_5_6_5,                                   ///< 16 Bit RGB-565
      PF_RGB_8_8_8,                                   ///< 24 Bit RGB-888
      PF_XRGB_1_5_5_5,                                ///< 16 Bit RGB-555
      PF_XRGB_8_8_8_8,                                ///< 32 Bit RGB-888
      PF_ARGB_1_5_5_5,                                ///< 16 Bit ARGB-1555
      PF_ARGB_4_4_4_4,                                ///< 16 Bit ARGB-4444
      PF_ARGB_8_8_8_8                                 ///< 32 Bit ARGB-8888
    };
    
    /// Lock informations
    /** Provides data for accessing the locked surface
    */
    struct LockInfo {
      Point2<size_t>  Size;                  ///< Region size
      long            nPitch;                ///< Image line width in bytes
      PixelFormat     eFormat;               ///< Image color format
      LockMode        eMode;                 ///< Locking mode
      void           *pMemory;               ///< Locked surface memory
    };

    template<typename PixelFormat> class Accessor;

    /// Get string from pixel format
    NUCLEX_API static string stringFromPixelFormat(PixelFormat ePixelFormat);
    /// Get pixel format from string
    NUCLEX_API static PixelFormat pixelFormatFromString(const string &sPixelFormat);

    /// Convert a DWORD RGBA color into a pixel color
    NUCLEX_API inline static PixelType pixelFromColor(const Color &TheColor, PixelFormat eDestFormat);
    /// Convert a pixel color into a DWORD RGBA color
    NUCLEX_API inline static Color colorFromPixel(PixelType nPixel, PixelFormat eSourceFormat);
    /// Get *bytes* per pixel from pixel format
    NUCLEX_API inline static size_t bppFromFormat(PixelFormat eFormat);
    /// Perform blit on linear memory
    NUCLEX_API static void blit(const LockInfo &Destination, const LockInfo &Source,
                                const Point2<long> &Position = Point2<long>(),
                                const Box2<long> &SourceRegion = Box2<long>());


    /// Destructor
    /** Destroys an instance of Surface
    */
    NUCLEX_API virtual ~Surface() {}

  //
  // Surface implementation
  //
  public:
    /// Get surface size
    /** Returns the size of the surface

        @return The surface's size
    */
    NUCLEX_API virtual const Point2<size_t> &getSize() const = 0;

    /// Get surface color format
    /** Returns the color format of the surface

        @return The surface's color format
    */
    NUCLEX_API virtual PixelFormat getFormat() const = 0;

    /// Lock surface region
    /** Locks the specified region on the surface, returning a lockinfo
        structure containing data required to access the surface.

        @param  eLockMode  Lock access mode
        @param  Region     Region to lock
        @return A structure contaning data about the locked region
    */
    NUCLEX_API virtual const LockInfo &lock(LockMode eLockMode, 
                                            const Box2<long> &Region = Box2<long>()) = 0;
                                        
    /// Unlock surface
    /** Unlocks the surface again after it has been locked
    */
    NUCLEX_API virtual void unlock() = 0;

    /// Copy image bits region
    /** Copies a region of the image bits onto the given memory adress

        @param  Destination   Locked destination surface
        @param  Position      Target position on destination surface
        @param  SourceRegion  Region to copy
    */
    NUCLEX_API virtual void blitTo(
      const LockInfo &Destination,
      const Point2<long> &Position = Point2<long>(),
      const Box2<long> &SourceRegion = Box2<long>()
    );

    /// Copy image bits (conveniency method)
    NUCLEX_API virtual void blitTo(
      const shared_ptr<Surface> &spDestSurface,
      const Point2<long> &Position = Point2<long>(),
      const Box2<long> &SourceRegion = Box2<long>()
    );
};

// ############################################################################################# //
// # Nuclex::Video::Surface::bppFromFormat()                                                   # //
// ############################################################################################# //
/** Retrieves the number of *bytes* per pixel of the specified format

    @param  eFormat  Format of which to retrieve the bpp
    @return The number of *bytes* per pixel in the specified format
*/
inline size_t Surface::bppFromFormat(PixelFormat eFormat) {
  switch(eFormat) {
    case Surface::PF_A_8:         return 1;
    case Surface::PF_RGB_3_3_2:   return 1;
    case Surface::PF_RGB_5_6_5:   return 2;
    case Surface::PF_RGB_8_8_8:   return 3;
    case Surface::PF_XRGB_1_5_5_5: return 2;
    case Surface::PF_XRGB_8_8_8_8: return 4;
    case Surface::PF_ARGB_1_5_5_5: return 2;
    case Surface::PF_ARGB_4_4_4_4: return 2;
    case Surface::PF_ARGB_8_8_8_8: return 4;
    default: return 0;
  }
}

template<typename SurfacePixelFormat>
class Surface::Accessor {
  public:
    /// Constructor
    /** Initializes an instance of Accessor
        
        @param  pMemory    Memory buffer on which to operate
        @param  nElements  Number of elements contained in the buffer
        @param  nPadding   Padding, in bytes, between two elements
    */
    inline Accessor(const shared_ptr<Surface> &spSurface, LockMode eLockMode,
                    const Box2<long> &Region = Box2<long>()) :
      m_spSurface(spSurface),
      m_bLocked(false) {
      
      m_LockInfo = m_spSurface->lock(eLockMode, Region);
      m_bLocked = true;
    }
    
    inline ~Accessor() {
      if(m_bLocked)
        m_spSurface->unlock();
    }
    
    inline typename SurfacePixelFormat::PixelType &operator()(size_t nX, size_t nY) {
      return reinterpret_cast<typename SurfacePixelFormat::PixelType *>(
               reinterpret_cast<unsigned char *>(m_LockInfo.pMemory) +
                 nY * m_LockInfo.nPitch
             )[nX];
    }

  private:
    LockInfo             m_LockInfo;              ///< 
    bool                 m_bLocked;
    shared_ptr<Surface>  m_spSurface;             ///< Vertex buffer
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_SURFACE_H
