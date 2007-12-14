#pragma once
#ifndef COREGRAPHICS_TEXTURE_H
#define COREGRAPHICS_TEXTURE_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::Texture
    
    Front-end class for texture objects.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9texture.h"
namespace CoreGraphics
{
class Texture : public Direct3D9::D3D9Texture
{
    DeclareClass(Texture);
};
}
#else
#error "Texture class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
