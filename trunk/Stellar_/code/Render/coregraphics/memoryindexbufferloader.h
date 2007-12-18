#pragma once
#ifndef COREGRAPHICS_MEMORYINDEXBUFFERLOADER_H
#define COREGRAPHICS_MEMORYINDEXBUFFERLOADER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::MemoryIndexBufferLoader
    
    Initialize an index buffer object from index data in memory.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9memoryindexbufferloader.h"
namespace CoreGraphics
{
class MemoryIndexBufferLoader : public Direct3D9::D3D9MemoryIndexBufferLoader
{
    DeclareClass(MemoryIndexBufferLoader);
};
}
#else
#error "MemoryIndexBufferLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
