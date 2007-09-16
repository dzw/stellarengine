//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## CoronaImageCodec.cpp - Corona image codec          //
// ### # #      ###                                                      //
// # ### #      ###  Loads images through Corona                         //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R4             (C)2003 Markus Ewald -> License.txt  //
//  //
#include "CoronaPlugin/Video/CoronaImageCodec.h"
#include "CoronaPlugin/Video/CoronaImage.h"
#include "Nuclex/Storage/Stream.h"

using namespace Nuclex;
using namespace Nuclex::Video;

//  //
//  Nuclex::StreamFile                                                   //
//  //
/// A corona file on a nuclex stream
/** Wraps a corona file to a nuclex stream
*/
class StreamFile :
  public corona::File {
  public:
    StreamFile(const shared_ptr<Storage::Stream> &spStream) :
      m_spStream(spStream) {}

  //
  // File implementation
  //
  public:
    /** Read size bytes from the file, storing them in buffer.
       
        @param  pBuffer  Buffer to read into
        @param  nSize    Number of bytes to read
        @return The number of bytes successfully read
    */
    int COR_CALL read(void *pBuffer, int nSize) {
      return m_spStream->readData(pBuffer, nSize);
    }

    /** Write size bytes from buffer to the file.
     
        @param buffer  buffer that contains the data to write
        @param size    number of bytes to write
        @return  number of bytes successfully written
    */
    int COR_CALL write(const void *pBuffer, int nSize) {
      return m_spStream->writeData(pBuffer, nSize);
    }

    /** Jump to a new position in the file, using the specified seek
        mode.  Remember: if mode is END, the position must be negative,
        to seek backwards from the end of the file into its contents.
        If the seek fails, the current position is undefined.

        @param position  position relative to the mode
        @param mode      where to seek from in the file
        @return  true on success, false otherwise
    */
    bool COR_CALL seek(int nPosition, SeekMode eMode) {
      switch(eMode) {
        case BEGIN: {
          m_spStream->seekTo(nPosition);
          break;
        }
        case CURRENT: {
          m_spStream->seekTo(m_spStream->getLocation() + nPosition);
          break;
        }
        case END: {
          m_spStream->seekTo(m_spStream->getSize() + nPosition);
          break;
        }
        default: {
          throw UnexpectedException("Nuclex::Video::StreamFile::seek()",
                                    "corona requested seek offset not supported");
        }
      }
      return true;
    }

    /** Get current position within the file.
       
        @return  current position
    */
    int COR_CALL tell() {
      return m_spStream->getLocation();
    }

  //
  // DLLInterface implementation
  //
  public:
    void COR_CALL destroy() {
      delete this;
    }

  private:
    shared_ptr<Storage::Stream> m_spStream;           ///< The nuclex stream
};

// ####################################################################### //
// # Nuclex::Video::CoronaImageCodec::canLoadImage()                     # //
// ####################################################################### //
/** Checks whether the codec is able to load the image from the
    specified stream. The stream has to be reset after read data
    from it.
 
    @param  spStream    Stream to load image from
    @param  sExtension  Hint for the image file's extension
    @return True if the stream is able to read the image
*/
bool CoronaImageCodec::canLoadImage(const shared_ptr<Storage::Stream> &spStream,
                                      const string &sExtension) {
  // Maybe check using corona's image filedesc structure
  //return new CoronaImage(corona::OpenImage(&StreamFile(spStream)));
  return true;
}

// ####################################################################### //
// # Nuclex::Video::CoronaImageCodec::loadImage()                        # //
// ####################################################################### //
/** Loads an image from the specified stream
 
    @param  spStream    Stream to load image from
    @param  sExtension  Hint for the image file's extension
    @return The loaded image
*/
shared_ptr<Image> CoronaImageCodec::loadImage(const shared_ptr<Storage::Stream> &spStream,
                                                 const string &sExtension) {
  corona::Image *pImage = corona::OpenImage(&StreamFile(spStream));
  if(!pImage)
    throw FailedException("Nuclex::Video::CoronaImageCodec::loadImage()",
                          "An error occured while loading the image");

  return shared_ptr<Image>(new CoronaImage(pImage));
}

// ####################################################################### //
// # Nuclex::Video::CoronaImageCodec::canSaveImage()                     # //
// ####################################################################### //
/** Checks whether the codec is able to save the image using the specified
    format.
 
    @param  sFormat  Desired output format (eg 'png').
    @return True if the format was supported and the image is saved
*/
bool CoronaImageCodec::canSaveImage(const string &sFormat) const {
  return false;
}

// ####################################################################### //
// # Nuclex::Video::CoronaImageCodec::saveImage()                        # //
// ####################################################################### //
/** Saves the image into the stream using the specified format
 
    @param  spImage   Image to save
    @param  spStream  Stream to save image into
    @param  sFormat   Desired output format (eg 'png').
    @return True if the format was supported and the image is saved
*/
void CoronaImageCodec::saveImage(const shared_ptr<Image> &spImage,
                                   const shared_ptr<Storage::Stream> &spStream, const string &sFormat) const {
  throw NotSupportedException("Nuclex::Video::CoronaImageCodec::saveImage()",
                              "Not implemented yet");
}

// ####################################################################### //
// # Nuclex::Video::CoronaImageCodec::enumOutputFormats()                # //
// ####################################################################### //
/** Returns an enumerator over all output format strings which can be
    passed to the codec's saveImage() method.

    @return A new enumerator for all supported output formats
*/
shared_ptr<ImageCodec::OutputFormatEnumerator> CoronaImageCodec::enumOutputFormats() const {
  throw NotSupportedException("Nuclex::Video::CoronaImageCodec::enumOutputFormats()",
                              "Not implemented yet");
}
