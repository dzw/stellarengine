#pragma once
#ifndef BASE_MEMORYINDEXBUFFERLOADERBASE_H
#define BASE_MEMORYINDEXBUFFERLOADERBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::MemoryIndexBufferLoaderBase
    
    Base resource loader class for initializing an index buffer from
    data in memory.
    
    (C) 2007 Radon Labs GmbH
*/
#include "resources/resourceloader.h"
#include "coregraphics/indextype.h"

//------------------------------------------------------------------------------
namespace Base
{
class MemoryIndexBufferLoaderBase : public Resources::ResourceLoader
{
    DeclareClass(MemoryIndexBufferLoaderBase);
public:
    /// constructor
    MemoryIndexBufferLoaderBase();
    /// setup index buffer data, must remain valid until OnLoadRequested() is called!
    void Setup(CoreGraphics::IndexType::Code indexType, SizeT numIndices, void* ptr, SizeT numBytes);

protected:
    CoreGraphics::IndexType::Code indexType;
    SizeT numIndices;
    void* indexDataPtr;
    SizeT indexDataSize;
};
        
} // namespace CoreGraphics
//------------------------------------------------------------------------------
#endif
