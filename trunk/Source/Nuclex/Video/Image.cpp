//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Image.cpp - Image                                                         //
// ### # #      ###                                                                            //
// # ### #      ###  An image stored as pixels                                                 //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Video/Image.h"
#include "Nuclex/Support/Exception.h"
#include "ScopeGuard/ScopeGuard.h"

using namespace Nuclex;
using namespace Nuclex::Video;

// ############################################################################################# //
// # Nuclex::Video::Image::blitTo()                                                            # //
// ############################################################################################# //
/** Copies the image bits onto the given surface

    @param  spDestSurface  Destination surface
    @param  Position       Target position on destination surface
    @param  SourceRegion   Region to copy
*/
void Image::blitTo(const shared_ptr<Surface> &spDestSurface,
                   const Point2<long> &Position,
                   const Box2<long> &SourceRegion) const {
  const Surface::LockInfo &LockedSurface = spDestSurface->lock(LM_WRITE);
  
  { ScopeGuard unlock_Surface = MakeObjGuard(*spDestSurface.get(), &Surface::unlock);
    blitTo(LockedSurface, Position, SourceRegion);
  }
}

// ############################################################################################# //
// # Nuclex::Imagging::DefaultImage::DefaultImage()                                Constructor # //
// ############################################################################################# //
/** Initializes an instance of DefaultImage

    @param  Size     Desired bitmap resolution
    @param  eFormat  Pixelformat of the image
*/
DefaultImage::DefaultImage(const Point2<size_t> &Size, PixelFormat eFormat) :
  m_Size(Size),
  m_eFormat(eFormat),
  m_Memory(Surface::bppFromFormat(eFormat) * Size.X * Size.Y) {
  
  m_LockInfo.pMemory = 0;
}

// ############################################################################################# //
// # Nuclex::Video::DefaultImage::~DefaultImage()                                   Destructor # //
// ############################################################################################# //
/** Destroys an instance of DefaultImage
*/
DefaultImage::~DefaultImage() {}

// ############################################################################################# //
// # Nuclex::Video::DefaultImage::blitTo()                                                     # //
// ############################################################################################# //
/** Copies the image bits onto the given memory adress

    @param  Destination   Destination surface
    @param  Position      Target position on destination surface
    @param  SourceRegion  Region to copy
*/
void DefaultImage::blitTo(const LockInfo &Destination, const Point2<long> &Position,
                          const Box2<long> &SourceRegion) const {
  LockInfo LockInfo;
  LockInfo.Size = m_Size;
  LockInfo.nPitch = m_Size.X * Surface::bppFromFormat(m_eFormat);
  LockInfo.eFormat = m_eFormat;
  LockInfo.eMode = LM_READ;
  LockInfo.pMemory = &m_Memory[0];

  Surface::blit(Destination, LockInfo, Position, SourceRegion);
}

// ############################################################################################# //
// # Nuclex::Video::DefaultImage::lock()                                                       # //
// ############################################################################################# //
/** Locks the specified region on the surface, returning a lockinfo
    structure containing data required to access the surface.

    @param  eLockMode  Lock access mode
    @param  Region     Region to lock
    @return A structure contaning data about the locked region
    @todo Does the region have to be of type long ?
*/
const Surface::LockInfo &DefaultImage::lock(LockMode eLockMode, const Box2<long> &Region) {
  if(m_LockInfo.pMemory)
    throw FailedException("Nuclex::Video::DefaultImage::lock()",
                          "Tried to lock image multiple times");

  Box2<long> ClippedRegion;
  if(!Region)
    ClippedRegion.BR = m_Size;
  else if((Region.TL.X < 0) || (Region.TL.Y < 0) ||
          (Region.BR.X > static_cast<long>(m_Size.X)) || (Region.BR.Y > static_cast<long>(m_Size.Y)))
    throw InvalidArgumentException("Nuclex::Video::DefaultImage::lock()",
                                   "The locking region must not leave the image area");

  m_LockInfo.Size = ClippedRegion.getSize();
  m_LockInfo.nPitch = m_Size.X * Surface::bppFromFormat(m_eFormat);
  m_LockInfo.eFormat = m_eFormat;
  m_LockInfo.eMode = LM_READWRITE;
  m_LockInfo.pMemory = &m_Memory[0] + (ClippedRegion.TL.Y * m_LockInfo.nPitch +
                                       ClippedRegion.TL.X * Surface::bppFromFormat(m_eFormat));

  return m_LockInfo;
}

// ############################################################################################# //
// # Nuclex::Video::DefaultImage::unlock()                                                     # //
// ############################################################################################# //
/** Unlocks the surface again after it has been locked
*/
void DefaultImage::unlock() {
  if(!m_LockInfo.pMemory)
    throw FailedException("Nuclex::Video::DefaultImage::unlock()",
                          "Tried to unlock non-locked image");

  m_LockInfo.pMemory = NULL;
}
