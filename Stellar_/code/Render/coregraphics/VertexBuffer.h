#pragma once
#ifndef COREGRAPHICS_VERTEXBUFFER_H
#define COREGRAPHICS_VERTEXBUFFER_H
//------------------------------------------------------------------------------
/**
@class CoreGraphics::VertexBuffer

A resource which holds an array of vertices.

(C) 2007 by ctuo
*/    
#if __WIN32__
#include "coregraphics/d3d9/d3d9vertexbuffer.h"
namespace CoreGraphics
{
class VertexBuffer : public Direct3D9::D3D9VertexBuffer
{
	DeclareClass(VertexBuffer);
};
}
#else
#error "VertexBuffer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif