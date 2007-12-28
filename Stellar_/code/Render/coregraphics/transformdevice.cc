//------------------------------------------------------------------------------
//  transformdevice.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/transformdevice.h"

namespace CoreGraphics
{
ImplementClass(CoreGraphics::TransformDevice, 'TRDV', Base::TransformDeviceBase);
ImplementSingleton(CoreGraphics::TransformDevice);

//------------------------------------------------------------------------------
/**
*/
TransformDevice::TransformDevice()
{
    ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
TransformDevice::~TransformDevice()
{
    DestructSingleton;
}

} // namespace CoreGraphics
