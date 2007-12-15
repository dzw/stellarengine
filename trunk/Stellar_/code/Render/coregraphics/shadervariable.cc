//------------------------------------------------------------------------------
//  shadervariable.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shadervariable.h"

#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::ShaderVariable, 'SHDV', Direct3D9::D3D9ShaderVariable);
}
#else
#error "ShaderVariable class not implemented on this platform!"
#endif
