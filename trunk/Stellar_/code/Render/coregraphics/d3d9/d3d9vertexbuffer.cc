//------------------------------------------------------------------------------
//  d3d9vertexbuffer.cc
//  (C) 2007 by ctuo
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9vertexbuffer.h"
#include "coregraphics/d3d9/d3d9types.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9VertexBuffer, 'D9VB', Base::VertexBufferBase);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
D3D9VertexBuffer::D3D9VertexBuffer() :
d3d9VertexBuffer(0),
mapCount(0),
d3d9VertexDeclaration(0)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
D3D9VertexBuffer::~D3D9VertexBuffer()
{
	s_assert(0 == this->d3d9VertexBuffer);
	s_assert(0 == this->mapCount);
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9VertexBuffer::Unload()
{
	s_assert(0 == this->mapCount);
	if (0 != this->d3d9VertexBuffer)
	{
		this->d3d9VertexBuffer->Release();
		this->d3d9VertexBuffer = 0;
	}
	VertexBufferBase::Unload();
}

//------------------------------------------------------------------------------
/**
*/
void*
D3D9VertexBuffer::Map(MapType mapType)
{
	s_assert(0 != this->d3d9VertexBuffer);
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
	HRESULT hr = this->d3d9VertexBuffer->Lock(0, 0, &ptr, lockFlags);
	s_assert(SUCCEEDED(hr));
	this->mapCount++;
	return ptr;
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9VertexBuffer::Unmap()
{
	s_assert(0 != this->d3d9VertexBuffer);
	s_assert(this->mapCount > 0);
	HRESULT hr = this->d3d9VertexBuffer->Unlock();
	s_assert(SUCCEEDED(hr));
	this->mapCount--;
}

void 
D3D9VertexBuffer::Setup(const Util::Array<CoreGraphics::VertexComponent>& c)
{
	s_assert(0 == this->d3d9VertexDeclaration);

	// call parent class
	Base::VertexBufferBase::Setup(c);

	// create a D3D9 vertex declaration object
	const SizeT maxElements = 32;
	s_assert(this->components.size() < maxElements);
	D3DVERTEXELEMENT9 decl[maxElements] = { 0 };
	IndexT curOffset = 0;
	IndexT compIndex;
	for (compIndex = 0; compIndex < this->components.size(); compIndex++)
	{
		const VertexComponent& component = this->components[compIndex];
		decl[compIndex].Stream = 0;
		decl[compIndex].Offset = (WORD) curOffset;
		decl[compIndex].Type   = D3D9Types::AsD3D9VertexDeclarationType(component.GetFormat());
		decl[compIndex].Method = D3DDECLMETHOD_DEFAULT;
		decl[compIndex].Usage  = (BYTE) D3D9Types::AsD3D9VertexDeclarationUsage(component.GetSemanticName());
		decl[compIndex].UsageIndex = (BYTE) component.GetSemanticIndex();
		curOffset += component.GetByteSize();
	}
	decl[compIndex].Stream = 0xff;
	decl[compIndex].Type = (WORD) D3DDECLTYPE_UNUSED;

	//IDirect3DDevice9* d3d9Dev = D3D9RenderDevice::Instance()->GetDirect3DDevice();
	//HRESULT hr = d3d9Dev->CreateVertexDeclaration(decl, &this->d3d9VertexDeclaration);
	//s_assert(SUCCEEDED(hr));
	s_assert(0 != this->d3d9VertexDeclaration);
}

} // namespace CoreGraphics