//------------------------------------------------------------------------------
//  memoryvertexbufferloaderbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/memoryvertexbufferloaderbase.h"

namespace Base
{
ImplementClass(Base::MemoryVertexBufferLoaderBase, 'MVLB', Resources::ResourceLoader);

//------------------------------------------------------------------------------
/**
*/
MemoryVertexBufferLoaderBase::MemoryVertexBufferLoaderBase() :
    numVertices(0),
    vertexDataPtr(0),
    vertexDataSize(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Setup all information needed to initialize the VertexBuffer resource.
    The data must remain valid until OnLoadRequested() is called (which
    will invaliate the data).
*/
void
MemoryVertexBufferLoaderBase::Setup(const Util::Array<CoreGraphics::VertexComponent>& components, SizeT num, void* ptr, SizeT numBytes)
{
    s_assert(num > 0);
    s_assert(0 != ptr);
    this->vertexComponents = components;
    this->numVertices = num;
    this->vertexDataPtr = ptr;
    this->vertexDataSize = numBytes;
}

} // namespace Base
