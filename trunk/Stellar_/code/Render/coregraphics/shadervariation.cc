//------------------------------------------------------------------------------
//  shadervariation.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shadervariation.h"

#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::ShaderVariation, 'SHVR', Direct3D9::D3D9ShaderVariation);
}
#else
#error "ShaderVariation class not implemented on this platform!"
#endif
