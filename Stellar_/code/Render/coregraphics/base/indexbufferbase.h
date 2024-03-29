#pragma once
#ifndef BASE_INDEXBUFFERBASE_H
#define BASE_INDEXBUFFERBASE_H
//------------------------------------------------------------------------------
/**
    @class Base::IndexBufferBase
  
    A resource which holds an array of indices into an array of vertices.
    
    (C) 2007 by ctuo
*/
#include "core/refcounted.h"
#include "coregraphics/indextype.h"
#include "resources/resource.h"

//------------------------------------------------------------------------------
namespace Base
{
class IndexBufferBase : public Resources::Resource
{
    DeclareClass(IndexBufferBase);
public:
    /// constructor
    IndexBufferBase();
    /// destructor
    virtual ~IndexBufferBase();

    /// map index buffer for CPU access
    void* Map(MapType mapType);
    /// unmap the resource
    void Unmap();
    /// get the index type (Index16 or Index32)
    CoreGraphics::IndexType::Code GetIndexType() const;
    /// get number of indices
    SizeT GetNumIndices() const;

protected:
    /// set the index type (Index16 or Index32)
    void SetIndexType(CoreGraphics::IndexType::Code type);
    /// set number of indices
    void SetNumIndices(SizeT num);

    CoreGraphics::IndexType::Code indexType;
    SizeT numIndices;
};

//------------------------------------------------------------------------------
/**
*/
inline void
IndexBufferBase::SetIndexType(CoreGraphics::IndexType::Code type)
{
    this->indexType = type;
}

//------------------------------------------------------------------------------
/**
*/
inline CoreGraphics::IndexType::Code
IndexBufferBase::GetIndexType() const
{
    return this->indexType;
}

//------------------------------------------------------------------------------
/**
*/
inline void
IndexBufferBase::SetNumIndices(SizeT num)
{
    s_assert(num > 0);
    this->numIndices = num;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
IndexBufferBase::GetNumIndices() const
{
    return this->numIndices;
}

} // namespace Base
//------------------------------------------------------------------------------
#endif
