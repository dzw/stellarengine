#pragma once
#ifndef COREGRAPHICS_DISPLAYDEVICE_H
#define COREGRAPHICS_DISPLAYDEVICE_H
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::DisplayDevice
  
    A DisplayDevice object represents the display where the RenderDevice
    presents the rendered frame. 
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "coregraphics/d3d9/d3d9displaydevice.h"
namespace CoreGraphics
{
class DisplayDevice : public Direct3D9::D3D9DisplayDevice
{
    DeclareClass(DisplayDevice);
    DeclareSingleton(DisplayDevice);
public:
    /// constructor
    DisplayDevice();
    /// destructor
    virtual ~DisplayDevice();
};
} // namespace CoreGraphics
#else
#error "CoreGraphics::DisplayDevice not implemented on this platform!"
#endif
//------------------------------------------------------------------------------
#endif
