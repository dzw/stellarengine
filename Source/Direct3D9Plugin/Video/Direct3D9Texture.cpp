//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Direct3D9Texture.cpp - Texture                      //
// ### # #      ###                                                      //
// # ### #      ###  Direct3D9 texture implementation                    //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "Direct3D9Plugin/Video/Direct3D9Texture.h"

using namespace Nuclex;
using namespace Nuclex::Video;

// ############################################################################################# //
// # Nuclex::Video::Direct3D9Texture::Direct3D9Texture()                           Constructor # //
// ############################################################################################# //
/** Initializes an instance of CDirect3D9Texture
 
    @param  pTexture  The Direct3D texture
*/
Direct3D9Texture::Direct3D9Texture(Direct3D9VideoDevice &VideoDevice,
                                   const Point2<size_t> &Size,
                                   Surface::PixelFormat eFormat,
                                   VideoDevice::ResourceClass eResourceClass) :
  m_VideoDevice(VideoDevice),
  m_Size(Size),                                   
  m_eFormat(eFormat) {
  
  switch(eResourceClass) {
    case VideoDevice::RC_STATIC: {
      D3DCheck(
        "Nuclex::Video::Direct3D9Texture::Direct3D9Texture()",
        "IDirect3DDevice9::CreateTexture()",
        VideoDevice.getDirect3DDevice()->CreateTexture(
          Size.X, Size.Y, 0, D3DUSAGE_AUTOGENMIPMAP,
          D3DFORMATFromPixelFormat(eFormat), D3DPOOL_MANAGED,
          &m_spTexture, NULL
        )
      );

      break;
    }
    case VideoDevice::RC_TEMPORARY:
    case VideoDevice::RC_DYNAMIC: {

      if(0) { //VideoDevice.getDeviceCaps().Caps2 & D3DCAPS2_DYNAMICTEXTURES)  {
        D3DCheck(
          "Nuclex::Video::Direct3D9Texture::Direct3D9Texture()",
          "IDirect3DDevice9::CreateTexture()",
          VideoDevice.getDirect3DDevice()->CreateTexture(
            Size.X, Size.Y, 0, D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_DYNAMIC,
            D3DFORMATFromPixelFormat(eFormat), D3DPOOL_DEFAULT,
            &m_spTexture, NULL
          )
        );
        
        VideoDevice.OnDeviceShutdown.connect(
          sigc::mem_fun(this, &Direct3D9Texture::onDeviceShutdown)
        );
        VideoDevice.OnDeviceRestore.connect(
          sigc::mem_fun(this, &Direct3D9Texture::onDeviceRestore)
        );

      } else {
      
        D3DCheck(
          "Nuclex::Video::Direct3D9Texture::Direct3D9Texture()",
          "IDirect3DDevice9::CreateTexture()",
          VideoDevice.getDirect3DDevice()->CreateTexture(
            Size.X, Size.Y, 0, D3DUSAGE_AUTOGENMIPMAP,
            D3DFORMATFromPixelFormat(eFormat), D3DPOOL_MANAGED,
            &m_spTexture, NULL
          )
        );
      }

      break;
    }
  }

  m_LockInfo.pMemory = NULL;
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9Texture::precache()                                               # //
// ############################################################################################# //
/** Tries to precache the texture in the video card's memory.
    This method does not guarantee anything, but provides a hint
    at the renderer to prepare this texture for rendering asap
*/
void Direct3D9Texture::precache() {
  m_spTexture->PreLoad();
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9Texture::lock()                                                   # //
// ############################################################################################# //
/** Locks the specified region on the surface, returning a lockinfo
    structure containing data required to access the surface.

    @param  Region     Region to lock
    @param  eLockMode  Lock access mode
    @return A structure contaning data about the locked region
*/
const Surface::LockInfo &Direct3D9Texture::lock(LockMode eLockMode, const Box2<long> &Region) {
  if(m_LockInfo.pMemory)
    throw FailedException("Nuclex::Video::Direct3D9Texture::lock()",
                          "Tried to lock texture multiple times");

  DWORD Flags = 0;
  switch(eLockMode) {
    case LM_READ: {
      Flags |= D3DLOCK_READONLY;
      break;
    }
    case LM_OVERWRITEALL: {
      Flags |= D3DLOCK_DISCARD;
      break;
    }
    case LM_APPENDONLY: {
      Flags |= D3DLOCK_NOOVERWRITE;
      break;
    }
  }

  D3DLOCKED_RECT LockedRect;
  if(!Region) {
    D3DCheck("Nuclex::Video::Direct3D9Texture::lock()", "IDirect3DTexture9::LockRect()",
             m_spTexture->LockRect(0, &LockedRect, NULL, Flags));

    m_LockInfo.Size = m_Size;

  } else {
    if((Region.TL.X < 0) || (Region.TL.Y < 0) ||
       (static_cast<unsigned long>(Region.BR.X) > m_Size.X) ||
       (static_cast<unsigned long>(Region.BR.Y) > m_Size.Y))
      throw InvalidArgumentException("Nuclex::Video::Direct3D9Texture::lock()",
                                     "The locking region must not leave the texture area");

    RECT Rect;

    // fix for nvidia bug
    Rect.left = 0; //Region.TL.X;
    Rect.top = 0; //Region.TL.Y;
    Rect.right = Region.BR.X;
    Rect.bottom = Region.BR.Y;

    D3DCheck(
      "Nuclex::Video::Direct3D9Texture::lock()",
      "IDirect3DTexture9::LockRect()",
      m_spTexture->LockRect(0, &LockedRect, &Rect, Flags)
    );

    m_LockInfo.Size.set(Region.getWidth(), Region.getHeight());
  }

  m_LockInfo.nPitch = LockedRect.Pitch;
//  m_LockInfo.nPitch = -LockedRect.Pitch;
  m_LockInfo.eFormat = m_eFormat;
  m_LockInfo.eMode = eLockMode;
//  m_LockInfo.pMemory = reinterpret_cast<unsigned char *>(LockedRect.pBits) -
//                       (m_LockInfo.nPitch * (m_LockInfo.nHeight - 1));

  m_LockInfo.pMemory = reinterpret_cast<unsigned char *>(LockedRect.pBits) +
                       m_LockInfo.nPitch * Region.TL.Y + // fix for nvidia bug
                       Surface::bppFromFormat(m_eFormat) * Region.TL.X;

  return m_LockInfo;
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9Texture::unlock()                                                 # //
// ############################################################################################# //
/** Unlocks the surface again after it has been locked
*/
void Direct3D9Texture::unlock() {
  if(!m_LockInfo.pMemory)
    throw FailedException("Nuclex::Video::Direct3D9Texture::unlock()",
                          "Tried to unlock non-locked texture");

  D3DCheck(
    "Nuclex::Video::Direct3D9Texture::unlock()",
    "IDirect3DTexture9::UnlockRect()",
    m_spTexture->UnlockRect(0)
  );

  m_LockInfo.pMemory = NULL;
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9Texture::onDeviceShutdown()                                       # //
// ############################################################################################# //
void Direct3D9Texture::onDeviceShutdown(bool) {
  m_spTexture = NULL;
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9Texture::onDeviceRestore()                                        # //
// ############################################################################################# //
void Direct3D9Texture::onDeviceRestore() {
  D3DCheck(
    "Nuclex::Video::Direct3D9Texture::onDeviceRestore()",
    "IDirect3DDevice9::CreateTexture()",
    m_VideoDevice.getDirect3DDevice()->CreateTexture(
      m_Size.X, m_Size.Y, 0, D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_DYNAMIC,
      D3DFORMATFromPixelFormat(m_eFormat), D3DPOOL_DEFAULT,
      &m_spTexture, NULL
    )
  );
  
  // Restore contents...
}
