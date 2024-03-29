#pragma once
#ifndef COREGRAPHICS_RENDERTARGET_H
#define COREGRAPHICS_RENDERTARGET_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::RenderTarget
    
    A render targets wraps up to 4 color buffers and an optional depth/stencil 
    buffer into a C++ object. The special default render target represents the 
    backbuffer and default depth/stencil surface.

    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9rendertarget.h"
namespace CoreGraphics
{
class RenderTarget : public Direct3D9::D3D9RenderTarget
{
    DeclareClass(RenderTarget);
};
}
#else
#error "RenderTarget class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
