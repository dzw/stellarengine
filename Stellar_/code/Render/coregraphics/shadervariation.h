#pragma once
#ifndef COREGRAPHICS_SHADERVARIATION_H
#define COREGRAPHICS_SHADERVARIATION_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::ShaderVariation
  
    A variation of a shader implements a specific feature set identified
    by a feature mask.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9shadervariation.h"
namespace CoreGraphics
{
class ShaderVariation : public Direct3D9::D3D9ShaderVariation
{
    DeclareClass(ShaderVariation);
};
}
#else
#error "ShaderVariation class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
