//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Direct3D9VertexBuffer.h - VertexBuffer              //
// ### # #      ###                                                      //
// # ### #      ###  VertexBuffer class                                  //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_DIRECT3D9VERTEXBUFFER_H
#define NUCLEX_VIDEO_DIRECT3D9VERTEXBUFFER_H

#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "Direct3D9Plugin/Video/Direct3D9VideoDevice.h"
#include "Nuclex/Video/VertexBuffer.h"
#include "SigC++/SigC++.h"

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::Direct3D9VertexBuffer                                                       //
//  //
/// NuclexGame VertexBuffer class
/** A VertexBuffer class
*/
class Direct3D9VertexBuffer :
  public VertexBuffer,
  public sigc::trackable {
  public:
    /// Constructor
    NUCLEXDIRECT3D9_API Direct3D9VertexBuffer(
      Direct3D9VideoDevice &VideoDevice, const VertexDeclaration &Declaration,
      size_t nSize, VideoDevice::ResourceClass eResourceClass
    );
    /// Destructor
    NUCLEXDIRECT3D9_API virtual ~Direct3D9VertexBuffer() {}

  //
  // Direct3D9VertexBuffer implementation
  //
  public:
    /// Retrieve Direct3D vertex buffer
    NUCLEXDIRECT3D9_API const IDirect3DVertexBuffer9Ptr &getDirect3DVertexBuffer() const {
      return m_spVertexBuffer;
    }

    /// Get vertex stride
    NUCLEXDIRECT3D9_API size_t getStride() const { return m_nStride; }

  //
  // VertexBuffer implementation
  //
  public:
    /// Get buffer size
    NUCLEXDIRECT3D9_API size_t getSize() const { return m_nSize; }

    /// Lock vertex buffer
    NUCLEXDIRECT3D9_API virtual void *lock(LockMode eLockMode) {
      return lock(eLockMode, 0, 0);
    }
    /// Lock vertex buffer
    NUCLEXDIRECT3D9_API virtual void *lock(
      LockMode eLockMode,
      size_t StartVertex, size_t VertexCount
    );
    /// Unlock vertex buffer
    NUCLEXDIRECT3D9_API virtual void unlock();
  
  private:
    void onDeviceShutdown(bool);
    void onDeviceRestore();

    Direct3D9VideoDevice      &m_VideoDevice;         ///< Owner
    IDirect3DVertexBuffer9Ptr  m_spVertexBuffer;      ///< Direct3D Vertex buffer
    size_t                     m_nSize;               ///< Buffer size in vertices
    size_t                     m_nStride;             ///< Vertex stride in bytes
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_DIRECT3D9VERTEXBUFFER_H
