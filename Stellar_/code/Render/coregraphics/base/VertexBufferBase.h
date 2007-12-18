#pragma once
#ifndef BASE_VERTEXBUFFERBASE_H
#define BASE_VERTEXBUFFERBASE_H
//------------------------------------------------------------------------------
/**
	@class Base::VertexBufferBase

	A resource which holds an array of vertices.

	(C) 2006 by ctuo
*/
#include "core/refcounted.h"
#include "coregraphics/vertexcomponent.h"
#include "resources/resource.h"

//------------------------------------------------------------------------------
namespace Base
{
class VertexBufferBase : public Resources::Resource
{
	DeclareClass(VertexBufferBase);
public:
	/// constructor
	VertexBufferBase();
	/// destructor
	virtual ~VertexBufferBase();

	/// unload the resource, or cancel the pending load
	virtual void Unload();
	/// map the vertices for CPU access
	void* Map(MapType mapType);
	/// unmap the resource
	void Unmap();
	/// get the vertex layout
	//const Ptr<CoreGraphics::VertexLayout>& GetVertexLayout() const;
	/// get number of vertices in the buffer
	SizeT GetNumVertices() const;

	/// setup the vertex layout
	void Setup(const Util::Array<CoreGraphics::VertexComponent>& c);
	/// get the vertex stride in number of bytes
	SizeT GetVertexByteSize() const;
	/// 渲染前设置顶点源
	void SetVertexBuffer();
protected:
	/// set vertex layout (set by resource loader)
	//void SetVertexLayout(const Ptr<CoreGraphics::VertexLayout>& vertexLayout);
	/// set number of vertices (set by resource loader)
	void SetNumVertices(SizeT numVertices);

	//Ptr<CoreGraphics::VertexLayout> vertexLayout;
	/// 顶点数
	SizeT numVertices;

	/// FVF size
	SizeT vertexByteSize;
	Util::Array<CoreGraphics::VertexComponent> components;
};

//------------------------------------------------------------------------------
/**
*/
//inline void
//VertexBufferBase::SetVertexLayout(const Ptr<CoreGraphics::VertexLayout>& l)
//{
//	this->vertexLayout = l;
//}

//------------------------------------------------------------------------------
/**
*/
//inline const Ptr<CoreGraphics::VertexLayout>&
//VertexBufferBase::GetVertexLayout() const
//{
//	return this->vertexLayout;
//}

//------------------------------------------------------------------------------
/**
*/
inline void
VertexBufferBase::SetNumVertices(SizeT num)
{
	s_assert(num > 0);
	this->numVertices = num;
}

} // namespace Base
//------------------------------------------------------------------------------
#endif