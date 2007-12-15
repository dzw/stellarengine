//------------------------------------------------------------------------------
//  texture.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/texture.h"

#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::Texture, 'TEXR', Direct3D9::D3D9Texture);
}
#else
#error "Texture class not implemented on this platform!"
#endif
