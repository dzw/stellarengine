//------------------------------------------------------------------------------
//  memoryindexbufferloaderbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/memoryindexbufferloaderbase.h"

namespace Base
{
ImplementClass(Base::MemoryIndexBufferLoaderBase, 'MILB', Resources::ResourceLoader);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
MemoryIndexBufferLoaderBase::MemoryIndexBufferLoaderBase() :
    indexType(IndexType::None),
    numIndices(0),
    indexDataPtr(0),
    indexDataSize(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Setup all information needed to initialize the IndexBuffer resource.
    The data must remain valid until OnLoadRequested() is called (which
    will invaliate the data).

*/
void
MemoryIndexBufferLoaderBase::Setup(IndexType::Code type, SizeT num, void* ptr, SizeT numBytes)
{
    s_assert((type == IndexType::Index16) || (type == IndexType::Index32));
    s_assert(num > 0);
    s_assert(numBytes == (IndexType::SizeOf(type) * num));
    s_assert(0 != ptr);
    this->indexType = type;
    this->numIndices = num;
    this->indexDataPtr = ptr;
    this->indexDataSize = numBytes;
}

} // namespace Base
