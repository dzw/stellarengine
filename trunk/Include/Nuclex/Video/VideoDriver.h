//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VideoDriver.h - Graphics output device                                    //
// ### # #      ###                                                                            //
// # ### #      ###  Manages a graphical output device like a software renderer                //
// #  ## #   # ## ## or a graphics adapter accessed through a system API                       //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_VIDEODRIVER_H
#define NUCLEX_VIDEO_VIDEODRIVER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/Surface.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Storage/Persistable.h"

namespace Nuclex { namespace Video {

class Texture;
class VideoDevice;

//  //
//  Nuclex::Video::VideoDriver                                                                 //
//  //
/// Video driver
/** Allows to query the capabilities of a video device installed to the system
    and to create instances of this video device for graphics output
*/
class VideoDriver {
  public:
    /// Enumerator for the available display modes
    class DisplayModeEnumerator;

    /// Display mode information
    /** Holds informations about a display mode
    */
    struct DisplayMode : Storage::Persistable {
      /// Constructor
      NUCLEX_API DisplayMode(const Point2<size_t> &Resolution = Point2<size_t>(640, 480),
                             Surface::PixelFormat eFormat = Surface::PF_NONE,
                             bool bFullscreen = false, size_t nRefreshRate = 0) :
        Resolution(Resolution), 
        eFormat(eFormat),
        bFullscreen(bFullscreen),
        nRefreshRate(nRefreshRate) {}

      /// Get name of display mode
      NUCLEX_API string getName() const {
        return lexical_cast<string>(Resolution.X) + "x" + lexical_cast<string>(Resolution.Y) +
               " x " + lexical_cast<string>(Surface::bppFromFormat(eFormat) * 8) + "bpp" +
               (nRefreshRate ? (" @" + lexical_cast<string>(nRefreshRate) + "Hz") : "") +
               (bFullscreen ? "" : " (windowed)");
      }
      
      //
      // Persistable implementation
      //
      public:
        NUCLEX_API void load(const shared_ptr<Storage::Serializer> &spSerializer);
        NUCLEX_API void save(const shared_ptr<Storage::Serializer> &spSerializer) const;

      Point2<size_t>       Resolution;                ///< Output resolution
      Surface::PixelFormat eFormat;                   ///< Color format
      bool                 bFullscreen;               ///< Fullscreen display ?
      size_t               nRefreshRate;              ///< Refresh rate (Hz)
    };

    /// Destructor
    NUCLEX_API virtual ~VideoDriver() {}

  //
  // VideoDriver implementation
  //
  public:
    /// Get device name
    /** Returns a human readable name uniquely describing the 
        device. Should include the graphics card and monitor used.

        @return The device's name
    */
    NUCLEX_API virtual const string &getName() const = 0;

    /// Get an enumerator over all available display modes
    /** Can be used to enumerate all display modes the device supports

        @return A new display mode enumerator
    */
    NUCLEX_API virtual shared_ptr<DisplayModeEnumerator> enumDisplayModes() const = 0;

    /// Return maximum texture size supported
    /** Returns the maximum texture size supported by the device

        @return The maximum texture size of the device
    */
    NUCLEX_API virtual Point2<size_t> getMaxTextureSize() const = 0;

    /// Check if scissor test is supported
    /** Returns true if the device supports scissor testing

        @return True, if scissor testing is supported
    */
    NUCLEX_API virtual bool supportsScissorTest() const = 0;

    /// Create renderer
    /** Creates a renderer on this device. The renderer is an instance
        of the device with an output window. So the device manages vertex
        buffers, textures and other resources, while the renderer performs
        the actual drawing using these resources and handles the output
        window. This allows multiple windows to share the same resources.

        @return The created renderer
    */
    NUCLEX_API virtual shared_ptr<VideoDevice> createDevice(const DisplayMode &Mode) = 0;
};

//  //
//  Nuclex::Video::VideoDriver::DisplayModeEnumerator                                          //
//  //
/** Enumerates over a list of display modes
*/
class VideoDriver::DisplayModeEnumerator {
  public:
    /// Destructor
    /** Destroys an instance of IDisplayModeEnumerator
    */
    NUCLEX_API virtual ~DisplayModeEnumerator() {}

  //
  // DisplayModeEnumerator implementation
  //
  public:
    /// Go to next display mode
    /** Advances to the next display mode in the list

        @return The current display mode being enumerated
    */
    NUCLEX_API virtual bool next() = 0;

    /// Cycle through all display modes
    /** Returns the current display mode and advances to the next one.
        If no more display modes are available, returns NULL

        @return The current display mode being enumerated
    */
    NUCLEX_API virtual const DisplayMode &get() const = 0;
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_VIDEODRIVER_H
