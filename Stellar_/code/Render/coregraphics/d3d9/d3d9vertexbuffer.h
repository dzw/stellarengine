#pragma once
#ifndef DIRECT3D9_D3D9VERTEXBUFFER_H
#define DIRECT3D9_D3D9VERTEXBUFFER_H
//------------------------------------------------------------------------------
/** 
	@class Direct3D9::D3D9VertexBuffer

	D3D9 implementation of VertexBuffer.

	(C) 2007 by ctuo
*/    
#include "coregraphics/base/vertexbufferbase.h"

//------------------------------------------------------------------------------
namespace Direct3D9
{
class D3D9VertexBuffer : public Base::VertexBufferBase
{
	DeclareClass(D3D9VertexBuffer);
public:
	/// constructor
	D3D9VertexBuffer();
	/// destructor
	virtual ~D3D9VertexBuffer();

	/// unload the resource, or cancel the pending load
	virtual void Unload();
	/// map the vertices for CPU access
	void* Map(MapType mapType);
	/// unmap the resource
	void Unmap();

	/// setup the vertex layout
	void Setup(const Util::Array<CoreGraphics::VertexComponent>& c);
	/// get pointer to d3d9 vertex declaration
	IDirect3DVertexDeclaration9* GetD3D9VertexDeclaration() const;
	/// set source stream
	void SetVertexBuffer();

	/// get pointer to d3d9 vertex buffer object
	IDirect3DVertexBuffer9* GetD3D9VertexBuffer() const;

private:
	friend class D3D9MemoryVertexBufferLoader;

	/// set d3d9 vertex buffer pointer
	void SetD3D9VertexBuffer(IDirect3DVertexBuffer9* ptr);
	/// set d3d9 vertex declaration pointer
	void SetD3D9VertexDeclaration(IDirect3DVertexDeclaration9* ptr);

	IDirect3DVertexBuffer9* d3d9VertexBuffer;
	int mapCount;

	IDirect3DVertexDeclaration9* d3d9VertexDeclaration;
};

//------------------------------------------------------------------------------
/**
*/
inline void
D3D9VertexBuffer::SetD3D9VertexBuffer(IDirect3DVertexBuffer9* ptr)
{
	s_assert(0 != ptr);
	s_assert(0 == this->d3d9VertexBuffer);
	this->d3d9VertexBuffer = ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline IDirect3DVertexBuffer9*
D3D9VertexBuffer::GetD3D9VertexBuffer() const
{
	s_assert(0 != this->d3d9VertexBuffer);
	s_assert(0 == this->mapCount);
	return this->d3d9VertexBuffer;
}

//------------------------------------------------------------------------------
/**
*/
inline void
D3D9VertexBuffer::SetD3D9VertexDeclaration(IDirect3DVertexDeclaration9* ptr)
{
	s_assert(0 != ptr);
	s_assert(0 == this->d3d9VertexDeclaration);
	this->d3d9VertexDeclaration = ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline IDirect3DVertexDeclaration9*
D3D9VertexBuffer::GetD3D9VertexDeclaration() const
{
	s_assert(0 != this->d3d9VertexDeclaration);
	return this->d3d9VertexDeclaration;
}

} // namespace Direct3D9
//------------------------------------------------------------------------------
#endif

