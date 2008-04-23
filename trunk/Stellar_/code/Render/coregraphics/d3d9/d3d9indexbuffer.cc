//------------------------------------------------------------------------------
//  d3d9indexbuffer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9indexbuffer.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9IndexBuffer, 'D9IB', Base::IndexBufferBase);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
D3D9IndexBuffer::D3D9IndexBuffer() :
    d3d9IndexBuffer(0),
    mapCount(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
D3D9IndexBuffer::~D3D9IndexBuffer()
{
    s_assert(0 == this->d3d9IndexBuffer);
    s_assert(0 == this->mapCount);
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9IndexBuffer::Unload()
{
    s_assert(0 == this->mapCount);
    if (0 != this->d3d9IndexBuffer)
    {
        this->d3d9IndexBuffer->Release();
        this->d3d9IndexBuffer = 0;
    }
    IndexBufferBase::Unload();
}

//------------------------------------------------------------------------------
/**
*/
void*
D3D9IndexBuffer::Map(MapType mapType)
{
    s_assert(0 != this->d3d9IndexBuffer);
    DWORD lockFlags = D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_NOSYSLOCK;
    switch (mapType)
    {
        case MapRead:
            //s_assert((UsageStaging == this->usage) && (AccessRead == this->access));
            lockFlags |= D3DLOCK_READONLY;
            break;

        case MapWrite:
            //s_assert((UsageStaging == this->usage) && (AccessWrite == this->access));
            break;

        case MapReadWrite:
            //s_assert((UsageStaging == this->usage) && (AccessReadWrite == this->access));
            break;

        case MapWriteDiscard:
            //s_assert((UsageDynamic == this->usage) && (AccessWrite == this->access));
            lockFlags |= D3DLOCK_DISCARD;
            break;

        case MapWriteNoOverwrite:
            //s_assert((UsageDynamic == this->usage) && (AccessWrite == this->access));
            lockFlags |= D3DLOCK_NOOVERWRITE;
            break;
    }
    void* ptr = 0;
    HRESULT hr = this->d3d9IndexBuffer->Lock(0, 0, &ptr, lockFlags);
    s_assert(SUCCEEDED(hr));
    this->mapCount++;
    return ptr;
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9IndexBuffer::Unmap()
{
    s_assert(0 != this->d3d9IndexBuffer);
    s_assert(this->mapCount > 0);
    HRESULT hr = this->d3d9IndexBuffer->Unlock();
    s_assert(SUCCEEDED(hr));
    this->mapCount--;
}

} // namespace CoreGraphics