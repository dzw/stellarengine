//------------------------------------------------------------------------------
//  renderdevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/renderdevice.h"

namespace CoreGraphics
{
#if __WIN32__
ImplementClass(CoreGraphics::RenderDevice, 'RDVC', Direct3D9::D3D9RenderDevice);
ImplementSingleton(CoreGraphics::RenderDevice);
#else
#error "RenderDevice class not implemented on this platform!"
#endif

//------------------------------------------------------------------------------
/**
*/
RenderDevice::RenderDevice()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
RenderDevice::~RenderDevice()
{
    DestructSingleton;
}

} // namespace CoreGraphics
