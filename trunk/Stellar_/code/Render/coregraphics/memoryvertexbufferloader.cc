//------------------------------------------------------------------------------
//  memoryvertexbufferloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/memoryvertexbufferloader.h"

#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::MemoryVertexBufferLoader, 'MVBL', Direct3D9::D3D9MemoryVertexBufferLoader);
}
#else
#error "MemoryVertexBufferLoader class not implemented on this platform!"
#endif
