#pragma once
#ifndef COREGRAPHICS_INDEXBUFFER_H
#define COREGRAPHICS_INDEXBUFFER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::IndexBuffer
  
    A resource which holds an array of indices into an array of vertices.  
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9indexbuffer.h"
namespace CoreGraphics
{
class IndexBuffer : public Direct3D9::D3D9IndexBuffer
{
    DeclareClass(IndexBuffer);
};
}
#else
#error "IndexBuffer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
