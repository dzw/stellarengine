//------------------------------------------------------------------------------
//  d3d9memoryvertexbufferloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/vertexlayoutserver.h"
#include "coregraphics/d3d9/d3d9memoryvertexbufferloader.h"
#include "coregraphics/d3d9/d3d9types.h"
#include "coregraphics/d3d9/d3d9renderdevice.h"
#include "coregraphics/d3d9/d3d9vertexbuffer.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9MemoryVertexBufferLoader, 'DMVL', Base::MemoryVertexBufferLoaderBase);

using namespace Resources;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
    This will create a D3D9 vertex buffer and vertex declaration object
    from the data provided in the Setup() method and setup our resource
    object (which must be a D3D9VertexBuffer object). The data pointer provided
    to Setup() will be invalidated inside OnLoadRequested(). Resource usage
    will be set to UsageImmutable and resource access to AccessNone.
*/
bool
D3D9MemoryVertexBufferLoader::OnLoadRequested()
{
    s_assert(this->GetState() == Resource::Initial);
    s_assert(this->resource.isvalid());
    s_assert(!this->resource->IsAsyncEnabled());
    s_assert(0 != this->vertexDataPtr);

    IDirect3DDevice9* d3d9Device = D3D9RenderDevice::Instance()->GetDirect3DDevice();
    s_assert(0 != d3d9Device);

    // create a d3d9 vertex buffer object
    IDirect3DVertexBuffer9* d3dVertexBuffer = 0;
    HRESULT hr = d3d9Device->CreateVertexBuffer(this->vertexDataSize,       // Length
                                                0,                          // Usage
                                                0,                          // FVF
                                                D3DPOOL_MANAGED,            // Pool
                                                &d3dVertexBuffer,           // ppVertexBuffer
                                                NULL);                      // pSharedHandle
    s_assert(SUCCEEDED(hr));
    s_assert(0 != d3dVertexBuffer);

    // copy vertex data to vertex buffer
    void* dstPtr = 0;
    hr = d3dVertexBuffer->Lock(0, 0, &dstPtr, D3DLOCK_NOSYSLOCK);
    s_assert(SUCCEEDED(hr));
    s_assert(0 != dstPtr);
    Memory::Copy(this->vertexDataPtr, dstPtr, this->vertexDataSize);
    hr = d3dVertexBuffer->Unlock();
    s_assert(SUCCEEDED(hr));

    // setup the vertex layout (contains the D3D9 vertex declaration)
    //Ptr<VertexLayout> vertexLayout = VertexLayoutServer::Instance()->CreateSharedVertexLayout(this->vertexComponents);
    //s_assert((this->numVertices * vertexLayout->GetVertexByteSize()) == this->vertexDataSize);

    // setup our resource object
    s_assert(this->resource->IsA(D3D9VertexBuffer::RTTI));
    const Ptr<D3D9VertexBuffer>& res = this->resource.downcast<D3D9VertexBuffer>();
    s_assert(!res->IsLoaded());
    //res->SetUsage(D3D9VertexBuffer::UsageImmutable);
    //res->SetAccess(D3D9VertexBuffer::AccessNone);
    //res->SetVertexLayout(vertexLayout);
	res->Setup(this->vertexComponents);
    res->SetNumVertices(this->numVertices);
    res->SetD3D9VertexBuffer(d3dVertexBuffer);

    // invalidate setup data (because we don't own our data)
    this->vertexDataPtr = 0;
    this->vertexDataSize = 0;

    this->SetState(Resource::Loaded);
    return true;
}

} // namespace CoreGraphics
