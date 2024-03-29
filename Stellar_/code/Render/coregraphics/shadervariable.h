#pragma once
#ifndef COREGRAPHICS_SHADERVARIABLE_H
#define COREGRAPHICS_SHADERVARIABLE_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::ShaderVariable
    
    Provides direct access to a shader's global variable.
    The fastest way to change the value of a shader variable is to
    obtain a pointer to a shader variable once, and use it repeatedly
    to set new values.

    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9shadervariable.h"
namespace CoreGraphics
{
class ShaderVariable : public Direct3D9::D3D9ShaderVariable
{
    DeclareClass(ShaderVariable);
};
}
#else
#error "ShaderVariable class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
