#pragma once
#ifndef COREGRAPHICS_STREAMSHADERLOADER_H
#define COREGRAPHICS_STREAMSHADERLOADER_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::StreamShaderLoader
    
    Resource loader to setup a Shader object from a stream.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9streamshaderloader.h"
namespace CoreGraphics
{
class StreamShaderLoader : public Direct3D9::D3D9StreamShaderLoader
{
    DeclareClass(StreamShaderLoader);
};
}
#else
#error "StreamShaderLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
    