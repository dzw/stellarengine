//------------------------------------------------------------------------------
//  vertexbuffer.cc
//  (C) 2007 by ctuo
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/vertexbuffer.h"
#if __WIN32__
namespace CoreGraphics
{
	ImplementClass(CoreGraphics::VertexBuffer, 'VTXB', Direct3D9::D3D9VertexBuffer);
}
#else
#error "VertexBuffer class not implemented on this platform!"
#endif

