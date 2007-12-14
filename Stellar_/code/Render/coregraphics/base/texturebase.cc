//------------------------------------------------------------------------------
//  texturebase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/texturebase.h"

namespace Base
{
ImplementClass(Base::TextureBase, 'TXRB', Resources::Resource);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
TextureBase::TextureBase() :
    type(InvalidType),
    width(0),
    height(0),
    depth(0),
    numMipLevels(0),
    pixelFormat(PixelFormat::InvalidPixelFormat)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TextureBase::~TextureBase()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
TextureBase::Map(IndexT mipLevel, MapType mapType, MapInfo& outMapInfo)
{
    s_error("TextureBase::Map() called!");
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
TextureBase::Unmap(IndexT mipLevel)
{
    s_error("TextureBase::Unmap() called!");
}

//------------------------------------------------------------------------------
/**
*/
bool
TextureBase::MapCubeFace(CubeFace face, IndexT mipLevel, MapType mapType, MapInfo& outMapInfo)
{
    s_error("TextureBase::MapCubeFace() called!");
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
TextureBase::UnmapCubeFace(CubeFace cubeFace, IndexT mipLevel)
{
    s_error("TextureBase::UnmapCubeFace() called!");
}

} // namespace Base
