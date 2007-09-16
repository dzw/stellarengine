//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VideoServer.cpp - Manager for all visualization devices                   //
// ### # #      ###                                                                            //
// # ### #      ###  Manages all available visualization devices                               //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Video/VideoServer.h"
#include "Nuclex/Video/VideoDriver.h"
#include "Nuclex/Video/ImageCodec.h"
#include "Nuclex/Storage/Stream.h"
#include "Nuclex/Support/Exception.h"

using namespace Nuclex;
using namespace Nuclex::Video;

namespace {

//  //
//  VideoDriverEnumerator                                                                      //
//  //
/// Device enumerator for video server
/** Enumerates over all devices registered to the video server
*/
class VideoDriverEnumerator :
  public VideoServer::DriverEnumerator {
  typedef std::map<string, shared_ptr<VideoDriver> > DriverMap;
  public:
    /// Constructor
    /** Initializes an instance of VideoDriverEnumerator

        @param  DriverMap  Drivers to enumerate
    */
    VideoDriverEnumerator(DriverMap &Drivers) :
      m_Drivers(Drivers),
      m_DriverIt(Drivers.end()),
      m_DriverEnd(Drivers.end()) {}

    /// Destructor
    /** Destroys an instance of CRendererIterator
    */
    virtual ~VideoDriverEnumerator() {}

  //
  // DriverEnumerator implementation
  //
  public:
    bool next() {
      if(m_DriverIt == m_DriverEnd)
        m_DriverIt = m_Drivers.begin();
      else  
        ++m_DriverIt;
        
      return (m_DriverIt != m_DriverEnd);
    }

    const shared_ptr<VideoDriver> &get() const {
      if(m_DriverIt == m_DriverEnd)
        throw FailedException("VideoDriverEnumerator::get()",
                              "Enumerator is in empty cycle position");

      return m_DriverIt->second;
    }

  private:
    DriverMap                 &m_Drivers;             ///< Driver map
    DriverMap::const_iterator  m_DriverIt;            ///< Current renderer
    DriverMap::const_iterator  m_DriverEnd;           ///< List end
};

}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::VideoServer()                                     Constructor # //
// ############################################################################################# //
/** Creates an instance of VideoServer
*/
VideoServer::VideoServer() {}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::getDriver()                                                   # //
// ############################################################################################# //
/** Retrieves a video Driver by name

    @param  sName     Name of the video Driver to retrieve
    @return The video Driver or NULL if not found
*/
const shared_ptr<VideoDriver> &VideoServer::getDriver(const string &sName) const {
  DriverMap::const_iterator DriverIt = m_Drivers.find(sName);
  if(DriverIt == m_Drivers.end())
    throw InvalidArgumentException("Nuclex::Video::VideoServer::getDriver()",
                                   string("Item not found: '") + sName + "'");
  else
    return DriverIt->second;
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::addDriver()                                                   # //
// ############################################################################################# //
/** Add a new video Driver.

    @param  sName     Name of the video Driver
    @param  spDriver  The video Driver to add
*/
void VideoServer::addDriver(const string &sName, const shared_ptr<VideoDriver> &spDriver) {
  DriverMap::iterator DriverIt = m_Drivers.find(sName);
  if(DriverIt != m_Drivers.end())
    DriverIt->second = spDriver;
   else
    m_Drivers.insert(DriverMap::value_type(sName, spDriver));
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::removeDriver()                                                # //
// ############################################################################################# //
/** Removes a video Driver previously added using addDriver()

    @param  sName  Name of the video Driver to remove
*/
void VideoServer::removeDriver(const string &sName) {
  DriverMap::iterator DriverIt = m_Drivers.find(sName);
  if(DriverIt == m_Drivers.end())
    throw InvalidArgumentException("Nuclex::Video::VideoServer::getDriver()",
                                   string("Item not found: '") + sName + "'");
  else
    m_Drivers.erase(DriverIt);
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::clearDrivers()                                                # //
// ############################################################################################# //
/** Removes all video Drivers currently added to the video server
*/
void VideoServer::clearDrivers() {
  m_Drivers.clear();
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::enumDrivers()                                                 # //
// ############################################################################################# //
/** Returns a new enumerator over all video Drivers registered to
    the video server.

    @return A new enumerator over all video Drivers
*/
shared_ptr<VideoServer::DriverEnumerator> VideoServer::enumDrivers() {
  return shared_ptr<VideoServer::DriverEnumerator>(new VideoDriverEnumerator(m_Drivers));
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::getImageCodec()                                               # //
// ############################################################################################# //
/** Retrieves a codec class by name

    @param  sName  Name of the Loader class to retrieve
    @return Loader class or NULL if not found
*/
const shared_ptr<ImageCodec> &VideoServer::getImageCodec(const string &sName) const {
  ImageCodecMap::const_iterator CodecIt = m_ImageCodecs.find(sName);
  if(CodecIt != m_ImageCodecs.end())
    return CodecIt->second;
  else
    throw ItemNotFoundException("Nuclex::Video::VideoServer::getImageCodec()",
                                string("Codec not found: '") + sName + "'");

}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::addImageCodec()                                               # //
// ############################################################################################# //
/** Add a new codec class.

    @param  sName    Name of the codec class
    @param  spCodec  Pointer to the codec
*/
void VideoServer::addImageCodec(const string &sName, const shared_ptr<ImageCodec> &spCodec) {
  ImageCodecMap::iterator CodecIt = m_ImageCodecs.find(sName);
  if(CodecIt != m_ImageCodecs.end())
    CodecIt->second = spCodec;
  else
    m_ImageCodecs.insert(ImageCodecMap::value_type(sName, spCodec));
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::removeImageCodec()                                            # //
// ############################################################################################# //
/** Removes a Codec class previously added using addCodec()

    @param  sName  Name of the codec class to remove
*/
void VideoServer::removeImageCodec(const string &sName) {
  ImageCodecMap::iterator CodecIt = m_ImageCodecs.find(sName);
  if(CodecIt != m_ImageCodecs.end())
    m_ImageCodecs.erase(CodecIt);
  else
    throw ItemNotFoundException("Nuclex::Video::VideoServer::removeImageCodec()",
                                string("Codec not found: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::clearImageCodecs()                                            # //
// ############################################################################################# //
/** Removes all Loader class currently added to the graphics server
*/
void VideoServer::clearImageCodecs() {
  m_ImageCodecs.clear();
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::enumImageCodecs()                                             # //
// ############################################################################################# //
/** Returns a new enumerator over all Loaders registered to
    the video server.

    @return A new enumerator for all registered loaders
*/
shared_ptr<VideoServer::ImageCodecEnumerator> VideoServer::enumImageCodecs() const {
  throw NotSupportedException("Nuclex::Video::VideoServer::enumImageCodecs()",
                              "Not implemented yet");
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::getImage()                                                    # //
// ############################################################################################# //
/** Retrieves a Image by name

    @param  sName  Name of the Loader class to retrieve
    @return Loader class or NULL if not found
*/
const shared_ptr<Image> &VideoServer::getImage(const string &sName) const {
  ImageMap::const_iterator It = m_Images.find(sName);
  if(It != m_Images.end())
    return It->second;
  else
    throw ItemNotFoundException("Nuclex::Video::VideoServer::getImage()",
                                string("Image not found: '") + sName + "'");

}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::addImage()                                                    # //
// ############################################################################################# //
/** Add a new Image

    @param  sName     Name of the Image
    @param  spImage  Pointer to the Image
*/
void VideoServer::addImage(const string &sName, const shared_ptr<Image> &spImage) {
  ImageMap::iterator It = m_Images.find(sName);
  if(It != m_Images.end())
    It->second = spImage;
  else
    m_Images.insert(ImageMap::value_type(sName, spImage));
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::removeImage()                                                 # //
// ############################################################################################# //
/** Removes a Image previously added using addImage()

    @param  sName  Name of the Image to remove
*/
void VideoServer::removeImage(const string &sName) {
  ImageMap::iterator It = m_Images.find(sName);
  if(It != m_Images.end())
    m_Images.erase(It);
  else
    throw ItemNotFoundException("Nuclex::Video::VideoServer::removeImage()",
                                string("Image not found: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::clearImages()                                                 # //
// ############################################################################################# //
/** Removes all Images currently added to the Video server
*/
void VideoServer::clearImages() {
  m_Images.clear();
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::enumImages()                                                  # //
// ############################################################################################# //
/** Returns a new enumerator over all Loaders registered to
    the video server.

    @return A new enumerator for all registered loaders
*/
shared_ptr<VideoServer::ImageEnumerator> VideoServer::enumImages() const {
  throw NotSupportedException("Nuclex::Video::VideoServer::enumImages()",
                              "Not implemented yet");
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::createImage()                                                 # //
// ############################################################################################# //
/** Creates a new Video with the requested properties

    @param  Size     Desired Video resolution
    @param  eFormat  Pixel format
    @return A new, empty Video
*/
shared_ptr<Image> VideoServer::createImage(const Point2<size_t> &Size, Surface::PixelFormat eFormat) {
  return shared_ptr<Image>(new DefaultImage(Size, eFormat));
}

// ############################################################################################# //
// # Nuclex::Video::VideoServer::loadImage()                                                   # //
// ############################################################################################# //
/** Loads an Video from a stream

    @param  spSource    Source stream from which to load
    @param  sExtension  Optional hint for the codec to determine the file type
    @return The loaded Video
*/
shared_ptr<Image> VideoServer::loadImage(const shared_ptr<Storage::Stream> &spSource, const string &sExtension) {
  ImageCodecMap::const_iterator CodecEnd = m_ImageCodecs.end();
  for(ImageCodecMap::const_iterator CodecIt = m_ImageCodecs.begin();
      CodecIt != CodecEnd;
      CodecIt++)
    if(CodecIt->second->canLoadImage(spSource, sExtension))
      return CodecIt->second->loadImage(spSource, sExtension);

  // If no codec could load the Video, raise an error
  throw UnsupportedImageFormatException(
    "Nuclex::Video::VideoServer::loadImage()",
    "Unsupported image file format for '" + spSource->getName() + "'"
  );
}
