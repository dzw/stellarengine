#pragma once
#ifndef BASE_MEMORYVERTEXBUFFERLOADERBASE_H
#define BASE_MEMORYVERTEXBUFFERLOADERBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::MemoryVertexBufferLoaderBase
    
    Base resource loader class for initializing an vertex buffer from
    data in memory.
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/resourceloader.h"
#include "coregraphics/vertexcomponent.h"

//------------------------------------------------------------------------------
namespace Base
{
class MemoryVertexBufferLoaderBase : public Resources::ResourceLoader
{
    DeclareClass(MemoryVertexBufferLoaderBase);
public:
    /// constructor
    MemoryVertexBufferLoaderBase();
    /// setup vertex buffer data, must remain valid until OnLoadRequested() is called!
    void Setup(const Util::Array<CoreGraphics::VertexComponent>& vertexComponents, SizeT numVertices, void* ptr, SizeT numBytes);

protected:
    Util::Array<CoreGraphics::VertexComponent> vertexComponents;
    SizeT numVertices;
    void* vertexDataPtr;
    SizeT vertexDataSize;
};

} // namespace Base
//------------------------------------------------------------------------------
#endif
