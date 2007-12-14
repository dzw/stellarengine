//------------------------------------------------------------------------------
//  shaderinstance.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shaderinstance.h"

#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::ShaderInstance, 'SINS', Direct3D9::D3D9ShaderInstance);
}
#elif __XBOX360__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::ShaderInstance, 'SINS', Xbox360::Xbox360ShaderInstance);
}
#else
#error "ShaderInstance class not implemented on this platform!"
#endif
