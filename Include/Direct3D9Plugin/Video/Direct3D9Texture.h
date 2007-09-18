//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Direct3D9Texture.h - Texture                        //
// ### # #      ###                                                      //
// # ### #      ###  Direct3D9 texture implementation                    //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_DIRECT3D9TEXTURE_H
#define NUCLEX_VIDEO_DIRECT3D9TEXTURE_H

#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "Direct3D9Plugin/Video/Direct3D9VideoDevice.h"
#include "Nuclex/Video/Texture.h"
#include "DirectX/D3D9.h"
#include "SigC++/Sigc++.h"

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::Direct3D9Texture                                      //
//  //
/// Direct3D9 texture
/**
*/
class Direct3D9Texture :
  public Texture,
  public sigc::trackable {
  public:
    /// Constructor
    NUCLEXDIRECT3D9_API Direct3D9Texture(Direct3D9VideoDevice &VideoDevice,
                                         const Point2<size_t> &Size,
                                         Surface::PixelFormat eFormat,
                                         VideoDevice::ResourceClass eResourceClass);
    /// Destructor
    NUCLEXDIRECT3D9_API virtual ~Direct3D9Texture() {}

  //
  // Direct3D9Texture implementation
  //
  public:
    /// Retrieve Direct3D texture
    NUCLEXDIRECT3D9_API const IDirect3DTexture9Ptr &getDirect3DTexture() const {
      return m_spTexture;
    }

  //
  // Texture implementation
  //
  public:
    /// Precache the texture
    NUCLEXDIRECT3D9_API void precache();

  //
  // Surface implementation
  //
  public:
    /// Get lockablesurface width
    NUCLEXDIRECT3D9_API const Point2<size_t> &getSize() const { return m_Size; }

    /// Get lockablesurface color format
    NUCLEXDIRECT3D9_API Surface::PixelFormat getFormat() const { return m_eFormat; }

    /// Lock surface region
    NUCLEXDIRECT3D9_API const LockInfo &lock(LockMode eLockMode, 
                                             const Box2<long> &Region = Box2<long>());
    /// Unlock surface
    NUCLEXDIRECT3D9_API void unlock();

  private:
    void onDeviceShutdown(bool bTemporary);
    void onDeviceRestore();

    Direct3D9VideoDevice &m_VideoDevice;              ///< Owner
    IDirect3DTexture9Ptr  m_spTexture;                ///< Direct3D texture
    Surface::LockInfo     m_LockInfo;                 ///< Locking info
    Point2<size_t>        m_Size;                     ///< Texture size
    Surface::PixelFormat  m_eFormat;                  ///< Texture pixel format
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_DIRECT3D9TEXTURE_H
