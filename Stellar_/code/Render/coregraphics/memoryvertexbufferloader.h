#pragma once
#ifndef COREGRAPHICS_MEMORYVERTEXBUFFERLOADER_H
#define COREGRAPHICS_MEMORYVERTEXBUFFERLOADER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::MemoryVertexBufferLoader
    
    Initialize a vertex buffer object from vertex data in memory.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9memoryvertexbufferloader.h"
namespace CoreGraphics
{
class MemoryVertexBufferLoader : public Direct3D9::D3D9MemoryVertexBufferLoader
{
    DeclareClass(MemoryVertexBufferLoader);
};
}
#else
#error "MemoryVertexBufferLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
