//------------------------------------------------------------------------------
//  rendertarget.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/rendertarget.h"
#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::RenderTarget, 'RTGT', Direct3D9::D3D9RenderTarget);
}
#else
#error "RenderTarget class not implemented on this platform!"
#endif
