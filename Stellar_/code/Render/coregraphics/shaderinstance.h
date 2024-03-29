#pragma once
#ifndef COREGRAPHICS_SHADERINSTANCE_H
#define COREGRAPHICS_SHADERINSTANCE_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::ShaderInstance
    
    A shader instance object is created from a shader and contains a local copy
    of the original shader state which can be modified through ShaderVariable
    objects. Shader instance objects are created directly through the 
    shader server.

    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9shaderinstance.h"
namespace CoreGraphics
{
class ShaderInstance : public Direct3D9::D3D9ShaderInstance
{
    DeclareClass(ShaderInstance);
};
}
#else
#error "ShaderInstance class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
