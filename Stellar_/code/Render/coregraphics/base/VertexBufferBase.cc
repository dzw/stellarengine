//------------------------------------------------------------------------------
//  vertexbufferbase.cc
//  (C) 2007 by ctuo
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/vertexbufferbase.h"

namespace Base
{
ImplementClass(Base::VertexBufferBase, 'VXBB', Resources::Resource);
using namespace CoreGraphics;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
VertexBufferBase::VertexBufferBase() :
numVertices(0),
vertexByteSize(0)
{
	components.clear();
}

//------------------------------------------------------------------------------
/**
*/
VertexBufferBase::~VertexBufferBase()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
VertexBufferBase::Unload()
{
	//this->vertexLayout = 0;
	Resource::Unload();
}

//------------------------------------------------------------------------------
/**
Make the vertex buffer content accessible by the CPU. The vertex buffer
must have been initialized with the right Access and Usage flags 
(see parent class for details). There are several reasons why a mapping
the resource may fail, this depends on the platform (for instance, the
resource may currently be busy, or selected for rendering).
*/
void*
VertexBufferBase::Map(MapType mapType)
{
	s_error("VertexBufferBase::Map() called!");
	return 0;
}

//------------------------------------------------------------------------------
/**
Give up CPU access on the vertex buffer content.
*/
void
VertexBufferBase::Unmap()
{
	s_error("VertexBufferBase::Unmap() called!");
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
VertexBufferBase::GetNumVertices() const
{
	return this->numVertices;
}

//------------------------------------------------------------------------------
/**
*/
void
VertexBufferBase::Setup(const Array<VertexComponent>& comps)
{
	s_assert(comps.size() > 0);
	s_assert(0 <= this->components.size());
	s_assert(0 == this->vertexByteSize);
	this->components = comps;
	IndexT i;
	for (i = 0; i < comps.size(); i++)
	{
		this->vertexByteSize += comps[i].GetByteSize();
	}
}

//------------------------------------------------------------------------------
/**
*/
void 
VertexBufferBase::SetVertexBuffer()
{
	// empty
}

} // namespace Base
