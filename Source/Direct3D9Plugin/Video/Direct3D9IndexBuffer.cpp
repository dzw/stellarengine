//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Direct3D9IndexBuffer.cpp - IndexBuffer              //
// ### # #      ###                                                      //
// # ### #      ###  IndexBuffer class                                   //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "Direct3D9Plugin/Video/Direct3D9IndexBuffer.h"

using namespace Nuclex;
using namespace Nuclex::Video;

// ####################################################################### //
// # Nuclex::Direct3D9IndexBuffer::Direct3D9IndexBuffer()    Constructor # // 
// ####################################################################### //
/** Initializes an instance of CDirect3D9IndexBuffer
 
    @param  pIndexBuffer  The Direct3D IndexBuffer
*/
Direct3D9IndexBuffer::Direct3D9IndexBuffer(Direct3D9VideoDevice &VideoDevice,
                                           size_t nSize,
                                           VideoDevice::ResourceClass eResourceClass) {

/*
  IDirect3DIndexBuffer9 *pD3DIndexBuffer;
  D3DCheck("Nuclex::Video::Direct3D9VideoDevice::createIndexBuffer()", "IDirect3DDevice9::CreateIndexBuffer()",
           m_spDirect3DDevice->CreateIndexBuffer(nSize * 2, 0, D3DFMT_INDEX16,
                                                 D3DPOOL_MANAGED, &pD3DIndexBuffer, NULL));

  return shared_ptr<IndexBuffer>(new Direct3D9IndexBuffer(pD3DIndexBuffer));
*/
/*
  D3DINDEXBUFFER_DESC IndexBufferDescription;
  D3DCheck("Nuclex::Direct3D9IndexBuffer::Direct3D9IndexBuffer()", "IDirect3DIndexBuffer9::GetDesc()",
           pIndexBuffer->GetDesc(&IndexBufferDescription));

  m_nSize = IndexBufferDescription.Size;
  m_nStride = 0; // @bug: So geht das nicht
*/  
}

// ####################################################################### //
// # Nuclex::Direct3D9IndexBuffer::lock()                                # // 
// ####################################################################### //
/** Locks the Index buffer

    @param  eLockMode  Locking mode
    @return The Index buffer pointer
    @todo Allow specifying a range of bytes to lock
*/
unsigned short *Direct3D9IndexBuffer::lock(LockMode eLockMode) {
  void *pBuffer = NULL;
  DWORD Flags = 0;

  if(eLockMode == LM_OVERWRITEALL)
    Flags |= D3DLOCK_DISCARD;
  else if(eLockMode == LM_APPENDONLY)
    Flags |= D3DLOCK_NOOVERWRITE;

  D3DCheck("Nuclex::Direct3D9IndexBuffer::lock()", "IDirect3DIndexBuffer9::Lock()",
           m_spIndexBuffer->Lock(0, m_nSize, &pBuffer, Flags));

  return reinterpret_cast<unsigned short *>(pBuffer);
}

// ####################################################################### //
// # Nuclex::Direct3D9IndexBuffer::unlock()                              # // 
// ####################################################################### //
/** Unlocks the Index buffer
*/
void Direct3D9IndexBuffer::unlock(void) {
  D3DCheck("Nuclex::Direct3D9IndexBuffer::unlock()", "IDirect3DIndexBuffer9::Unlock()",
           m_spIndexBuffer->Unlock());
}
