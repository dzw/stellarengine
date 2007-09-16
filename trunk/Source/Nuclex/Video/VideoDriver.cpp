//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VideoDriver.cpp - Graphics output device                                  //
// ### # #      ###                                                                            //
// # ### #      ###  Manages a graphical output device like a software renderer                //
// #  ## #   # ## ## or a graphics adapter accessed through a system API                       //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Video/VideoDriver.h"

using namespace Nuclex;
using namespace Nuclex::Video;
using namespace Nuclex::Storage;

// ############################################################################################# //
// # VideoDriver::DisplayMode::load()                                                          # //
// ############################################################################################# //
/// Load display mode
/** Loads a stored display mode from the serializer scope

    @param  spScope  Scope to load the display mode from
*/
void VideoDriver::DisplayMode::load(const shared_ptr<Serializer> &spScope) {
  Video::VideoDriver::DisplayMode DM;
  
  shared_ptr<Storage::Serializer> spResolution = spScope->openScope("Resolution");
  Resolution.X = spResolution->get<size_t>("_Width");
  Resolution.Y = spResolution->get<size_t>("_Height");
  
  bFullscreen = spScope->get<bool>("Fullscreen");
  nRefreshRate = spScope->get<size_t>("RefreshRate");
  eFormat = Surface::pixelFormatFromString(spScope->get<string>("Format"));
}

// ############################################################################################# //
// # saveDisplayMode()                                                                         # //
// ############################################################################################# //
/// Save display mode
/** Saves a display mode into a serializer scope

    @param  spScope  Scope to save the display mode into
*/
void Video::VideoDriver::DisplayMode::save(const shared_ptr<Storage::Serializer> &spScope) const {
  shared_ptr<Storage::Serializer> spResolution = spScope->createScope("Resolution");
  spResolution->set<size_t>("_Width", Resolution.X);
  spResolution->set<size_t>("_Height", Resolution.Y);
  
  spScope->set<bool>("Fullscreen", bFullscreen);
  spScope->set<size_t>("RefreshRate", nRefreshRate);
  spScope->set<string>("Format", Surface::stringFromPixelFormat(eFormat));
}
