//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Direct3D9IndexBuffer.h - IndexBuffer                //
// ### # #      ###                                                      //
// # ### #      ###  IndexBuffer class                                   //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_DIRECT3D9INDEXBUFFER_H
#define NUCLEX_VIDEO_DIRECT3D9INDEXBUFFER_H

#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "Direct3D9Plugin/Video/Direct3D9VideoDevice.h"
#include "Nuclex/Video/IndexBuffer.h"

namespace Nuclex { namespace Video {

class Direct3D9VideoDevice;

//  //
//  Nuclex::Video::Direct3D9IndexBuffer                                  //
//  //
/// Direct3D9 IndexBuffer
/**
*/
class Direct3D9IndexBuffer :
  public IndexBuffer {
  public:
    /// Constructor
    NUCLEXDIRECT3D9_API Direct3D9IndexBuffer(Direct3D9VideoDevice &VideoDevice,
                                             size_t nSize,
                                             VideoDevice::ResourceClass eResourceClass);
    /// Destructor
    NUCLEXDIRECT3D9_API virtual ~Direct3D9IndexBuffer() {}

  //
  // Direct3D9IndexBuffer implementation
  //
  public:
    /// Retrieve Direct3D Index buffer
    NUCLEXDIRECT3D9_API const IDirect3DIndexBuffer9Ptr &getDirect3DIndexBuffer() const {
      return m_spIndexBuffer;
    }

    /// Get Index stride
    NUCLEXDIRECT3D9_API size_t getStride() const { return m_nStride; }

  //
  // IndexBuffer implementation
  //
  public:
    /// Get buffer size
    NUCLEXDIRECT3D9_API size_t getSize() const { return m_nSize; }

    /// Lock Index buffer
    NUCLEXDIRECT3D9_API virtual unsigned short *lock(LockMode eLockMode);
    /// Unlock Index buffer
    NUCLEXDIRECT3D9_API virtual void unlock();
  
  private:
    void onDeviceShutdown(bool);
  
    IDirect3DIndexBuffer9Ptr m_spIndexBuffer;         ///< Direct3D Index buffer
    size_t                   m_nSize;                 ///< Buffer size
    size_t                   m_nStride;               ///< Index stride
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_DIRECT3D9INDEXBUFFER_H
