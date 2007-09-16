//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Direct3D9VertexBuffer.cpp - VertexBuffer            //
// ### # #      ###                                                      //
// # ### #      ###  VertexBuffer class                                  //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "Direct3D9Plugin/Video/Direct3D9VertexBuffer.h"
#include "Nuclex/Video/VertexDeclaration.h"

using namespace Nuclex;
using namespace Nuclex::Video;

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VertexBuffer::Direct3D9VertexBuffer()                 Constructor # //
// ############################################################################################# //
/** Initializes an instance of CDirect3D9VertexBuffer
 
    @param  pVertexBuffer  The Direct3D VertexBuffer
    @param  nStride        Offset, in bytes, from one vertex to the next
*/
Direct3D9VertexBuffer::Direct3D9VertexBuffer(Direct3D9VideoDevice &VideoDevice,
                                             const VertexDeclaration &Declaration,
                                             size_t nSize,
                                             VideoDevice::ResourceClass eResourceClass) :
  m_VideoDevice(VideoDevice),
  m_nSize(nSize),
  m_nStride(Declaration.getSize()) {

  switch(eResourceClass) {
    case VideoDevice::RC_STATIC: {
      D3DCheck(
        "Nuclex::Video::Direct3D9VertexBuffer::Direct3D9VertexBuffer()",
        "IDirect3DDevice9::CreateVertexBuffer()",
        VideoDevice.getDirect3DDevice()->CreateVertexBuffer(
          nSize * m_nStride, 0,
          0, D3DPOOL_MANAGED,
          &m_spVertexBuffer, NULL
        )
      );
      
      break;
    }
    case VideoDevice::RC_DYNAMIC:
    case VideoDevice::RC_TEMPORARY: {
      D3DCheck(
        "Nuclex::Video::Direct3D9VertexBuffer::Direct3D9VertexBuffer()",
        "IDirect3DDevice9::CreateVertexBuffer()",
        VideoDevice.getDirect3DDevice()->CreateVertexBuffer(
          nSize * Declaration.getSize(), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
          0, D3DPOOL_DEFAULT,
          &m_spVertexBuffer, NULL
        )
      );
      
      VideoDevice.OnDeviceShutdown.connect(
        sigc::mem_fun(this, &Direct3D9VertexBuffer::onDeviceShutdown)
      );
      VideoDevice.OnDeviceRestore.connect(
        sigc::mem_fun(this, &Direct3D9VertexBuffer::onDeviceRestore)
      );

      break;
    }   
  }
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VertexBuffer::lock()                                              # //
// ############################################################################################# //
/** Locks the vertex buffer

    @param  eLockMode  Locking mode
    @return The vertex buffer pointer
    @todo Allow specifying a range of bytes to lock
*/
void *Direct3D9VertexBuffer::lock(LockMode eLockMode, size_t StartVertex, size_t VertexCount) {
  DWORD Flags = 0;
  switch(eLockMode) {
    case LM_READONLY: {
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

  void *pBuffer = NULL;
  D3DCheck(
    "Nuclex::Video::Direct3D9VertexBuffer::lock()",
    "IDirect3DVertexBuffer9::Lock()",
    m_spVertexBuffer->Lock(StartVertex * m_nStride, VertexCount * m_nStride, &pBuffer, Flags)
  );

  return pBuffer;
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VertexBuffer::unlock()                                            # //
// ############################################################################################# //
/** Unlocks the vertex buffer
*/
void Direct3D9VertexBuffer::unlock() {
  D3DCheck(
    "Nuclex::Video::Direct3D9VertexBuffer::unlock()",
    "IDirect3DVertexBuffer9::Unlock()",
    m_spVertexBuffer->Unlock()
  );
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VertexBuffer::onDeviceShutdown()                                  # //
// ############################################################################################# //
void Direct3D9VertexBuffer::onDeviceShutdown(bool) {
  m_spVertexBuffer = NULL;
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VertexBuffer::onDeviceRestore()                                   # //
// ############################################################################################# //
void Direct3D9VertexBuffer::onDeviceRestore() {
  D3DCheck(
    "Nuclex::Video::Direct3D9VertexBuffer::onDeviceRestore()",
    "IDirect3DDevice9::CreateVertexBuffer()",
    m_VideoDevice.getDirect3DDevice()->CreateVertexBuffer(
      m_nSize * m_nStride, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
      0, D3DPOOL_DEFAULT,
      &m_spVertexBuffer, NULL
    )
  );
  
  // Restore contents...
}
