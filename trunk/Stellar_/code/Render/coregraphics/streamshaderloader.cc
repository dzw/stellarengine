//------------------------------------------------------------------------------
//  streamshaderloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/streamshaderloader.h"

#if __WIN32__
namespace CoreGraphics
{
ImplementClass(CoreGraphics::StreamShaderLoader, 'SSDL', Direct3D9::D3D9StreamShaderLoader);
}
#else
#error "StreamShaderLoader class not implemented on this platform!"
#endif
