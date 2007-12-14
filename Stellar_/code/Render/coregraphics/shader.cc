//------------------------------------------------------------------------------
//  shader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shader.h"

#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::Shader, 'SHDR', Direct3D9::D3D9Shader);
}
#elif __XBOX360__
// FIXME!
namespace CoreGraphics
{
ImplementClass(CoreGraphics::Shader, 'SHDR', Xbox360::Xbox360Shader);
}
#else
#error "Shader class not implemented on this platform!"
#endif
