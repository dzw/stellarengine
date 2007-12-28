#pragma once
#ifndef COREGRAPHICS_SHADERSERVER_H
#define COREGRAPHICS_SHADERSERVER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::ShaderServer
  
    The ShaderServer object loads the available shaders and can instantiate
    shaders for usage.
    
    (C) 2007 Radon Labs GmbH
*/    
#if __WIN32__
#include "coregraphics/d3d9/d3d9shaderserver.h"
namespace CoreGraphics
{
class ShaderServer : public Direct3D9::D3D9ShaderServer
{
    DeclareClass(ShaderServer);
    DeclareSingleton(ShaderServer);
public:
    /// constructor
    ShaderServer();
    /// destructor
    virtual ~ShaderServer();
};
}
#else
#error "ShaderServer class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
