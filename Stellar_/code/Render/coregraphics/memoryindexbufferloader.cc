//------------------------------------------------------------------------------
//  memoryindexbufferloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/memoryindexbufferloader.h"

#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::MemoryIndexBufferLoader, 'MIBL', Direct3D9::D3D9MemoryIndexBufferLoader);
}
#else
#error "MemoryIndexBufferLoader class not implemented on this platform!"
#endif
