//------------------------------------------------------------------------------
//  shaderserver.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/shaderserver.h"

namespace CoreGraphics
{
#if __WIN32__
ImplementClass(CoreGraphics::ShaderServer, 'SHSV', Direct3D9::D3D9ShaderServer);
ImplementSingleton(CoreGraphics::ShaderServer);
#else
#error "Texture class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
ShaderServer::ShaderServer()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShaderServer::~ShaderServer()
{
    DestructSingleton;
}

} // namespace CoreGraphics
