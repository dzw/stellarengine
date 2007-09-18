//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VideoServer.h - Manager for all visualization devices                     //
// ### # #      ###                                                                            //
// # ### #      ###  Manages all available visualization devices                               //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_VIDEOSERVER_H
#define NUCLEX_VIDEO_VIDEOSERVER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/VideoDriver.h"
#include "Nuclex/Video/Image.h"
#include "Nuclex/Support/Exception.h"
#include <map>

namespace Nuclex {
  namespace Storage { class Stream; }
  namespace Video { class ImageCodec; }
}

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::VideoServer                                                                 //
//  //
/// Video server
/** Manages anything visual. The video server manages a list of VideoDrivers, which the
    user can query for capabilities and use to create VideoDevices to render and display
    3D graphics. Further, a list of ImageCodecs is kept, each of which provide the
    VideoServer with additional file formats from which images and textures can be read.

    Both lists can be extended manually by the application which links the engine itself,
    or through the usage of plugins, which are dynamically loaded by the engine at
    startup time.
*/
class VideoServer {
  public:
    /// An unsupported image file format was used
    NUCLEX_DECLAREEXCEPTION(UnsupportedFormatException, UnsupportedImageFormatException);

    /// Enumerator over a list of video devices
    class DriverEnumerator;
    /// Image codec enumerator
    class ImageCodecEnumerator;
    /// Image codec enumerator
    class ImageEnumerator;

    /// Constructor
    NUCLEX_API VideoServer();
    /// Destructor
    NUCLEX_API virtual ~VideoServer() {}

  //
  // VideoServer implementation
  //
  public:
    /// Retrieve driver
    NUCLEX_API const shared_ptr<VideoDriver> &getDriver(const string &sName) const;
    /// Add driver
    NUCLEX_API void addDriver(const string &sName, const shared_ptr<VideoDriver> &spDriver);
    /// Remove driver
    NUCLEX_API void removeDriver(const string &sName);
    /// Remove all drivers
    NUCLEX_API void clearDrivers();
    /// Get renderer enumerator
    NUCLEX_API shared_ptr<DriverEnumerator> enumDrivers();

    /// Retrieve image codec by name
    NUCLEX_API const shared_ptr<ImageCodec> &getImageCodec(const string &sName) const;
    /// Add image codec
    NUCLEX_API void addImageCodec(const string &sName, const shared_ptr<ImageCodec> &spCodec);
    /// Remove image codec
    NUCLEX_API void removeImageCodec(const string &sName);
    /// Remove all codecs
    NUCLEX_API void clearImageCodecs();
    /// Get codec enumerator
    NUCLEX_API shared_ptr<ImageCodecEnumerator> enumImageCodecs() const;
    
    /// Retrieve image  by name
    NUCLEX_API const shared_ptr<Image> &getImage(const string &sName) const;
    /// Add image 
    NUCLEX_API void addImage(const string &sName, const shared_ptr<Image> &spImage);
    /// Remove image 
    NUCLEX_API void removeImage(const string &sName);
    /// Remove all s
    NUCLEX_API void clearImages();
    /// Get  enumerator
    NUCLEX_API shared_ptr<ImageEnumerator> enumImages() const;

    /// Create a new image from scratch
    NUCLEX_API shared_ptr<Image> createImage(
      const Point2<size_t> &Size,
      Video::Surface::PixelFormat eFormat
    );

    /// Load image from stream
    NUCLEX_API shared_ptr<Image> loadImage(
      const shared_ptr<Storage::Stream> &spSource,
      const string &sExtension = ""
    );

  private:
    /// Map of video devices
    typedef std::map<string, shared_ptr<VideoDriver> > DriverMap;
    /// A map of Image codecs
    typedef std::map<string, shared_ptr<ImageCodec> > ImageCodecMap;
    /// A map of Images
    typedef std::map<string, shared_ptr<Image> > ImageMap;

    DriverMap     m_Drivers;                          ///< Available drivers
    ImageCodecMap m_ImageCodecs;                      ///< Registered codecs
    ImageMap      m_Images;                           ///< Registered Images
};

//  //
//  Nuclex::Video::VideoServer::DriverEnumerator                                               //
//  //
/** Enumerates all registered devices
*/
class VideoServer::DriverEnumerator {
  public:
    /// Destructor
    /** Destroys an instance of DeviceIterator
    */
    NUCLEX_API virtual ~DriverEnumerator() {}

  //
  // DriverEnumerator implementation
  //
  public:
    NUCLEX_API virtual const shared_ptr<VideoDriver> &get() const = 0;
    NUCLEX_API virtual bool next() = 0;
};

//  //
//  Nuclex::Video::VideoServer::ImageCodecEnumerator                                           //
//  //
/** Enumerates over a list of image codecs
*/
class VideoServer::ImageCodecEnumerator {
  public:
    /// Destructor
    /** Destroys an instance of ImageCodecEnumerator
    */
    NUCLEX_API virtual ~ImageCodecEnumerator() {}

  //
  // ImageCodecEnumerator implementation
  //
  public:
    NUCLEX_API virtual const shared_ptr<ImageCodec> &get() const = 0;
    NUCLEX_API virtual bool next() = 0;
};

//  //
//  Nuclex::Video::VideoServer::ImageEnumerator                                                //
//  //
/** Enumerates over a list of Images
*/
class VideoServer::ImageEnumerator {
  public:
    /// Destructor
    /** Destroys an instance of ImageEnumerator
    */
    NUCLEX_API virtual ~ImageEnumerator() {}

  //
  // ImageEnumerator implementation
  //
  public:
    NUCLEX_API virtual const shared_ptr<Image> &get() const = 0;
    NUCLEX_API virtual bool next() = 0;
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_VIDEOSERVER_H
