//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ImageCodec.h - Image reader and writer                                    //
// ### # #      ###                                                                            //
// # ### #      ###  Abstract codec for reading and writing of image files                     //
// #  ## #   # ## ## from and to streams                                                       //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_IMAGECODEC_H
#define NUCLEX_VIDEO_IMAGECODEC_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/Surface.h"
#include "Nuclex/Support/String.h"

namespace Nuclex { 
  namespace Storage { class Stream; }
  namespace Video { class Image; }
}

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::ImageCodec                                                                  //
//  //
/// Image codec
/** An image codec reads image files (like .png or .jpg) into bitmaps
    and vice versa. The image files are always read or written through
    streams to enforce generality in the sense of usage on memory and
    compressed streams.
*/
class ImageCodec {
  public:
    /// Output format enumerator
    class OutputFormatEnumerator;

    /// Destructor
    /** Destroys an instance of ImageCodec
    */
    NUCLEX_API virtual ~ImageCodec() {}

  //
  // ImageCodec implementation
  //
  public:
    /// Check whether the image can be loaded
    /** Checks whether the codec is able to load the image from the
        specified stream. The stream has to be reset after read data
        from it.
     
        @param  spStream    Stream to load image from
        @param  sExtension  Hint for the image file's extension
        @return True if the stream is able to read the image
    */
    NUCLEX_API virtual bool canLoadImage(
      const shared_ptr<Storage::Stream> &spStream,
      const string &sExtension = ""
    ) { return false; };

    /// Load image
    /** Loads an image from the specified stream
     
        @param  spStream    Stream to load image from
        @param  sExtension  Hint for the image file's extension
        @return The loaded image
    */
    NUCLEX_API virtual shared_ptr<Image> loadImage(
      const shared_ptr<Storage::Stream> &spStream,
      const string &sExtension = ""
    ) = 0;

    /// Check whether the image can be saved
    /** Checks whether the codec is able to save the image using the specified
        format.
     
        @param  sFormat  Desired output format (eg 'png').
        @return True if the format is supported and the image can be saved
    */
    NUCLEX_API virtual bool canSaveImage(const string &sFormat) const { return false; }

    /// Save image
    /** Saves the image into the stream using the specified format
     
        @param  spImage   Image to save
        @param  spStream  Stream to save image into
        @param  sFormat   Desired output format (eg 'png').
    */
    NUCLEX_API virtual void saveImage(
      const shared_ptr<Image> &spImage,
      const shared_ptr<Storage::Stream> &spStream,
      const string &sFormat
    ) const = 0;

    /// Enumerate the supported output formats
    /** Returns an enumerator over all output format strings which can be
        passed to the codec's saveImage() method.

        @return A new enumerator for all supported output formats
    */
    NUCLEX_API virtual shared_ptr<OutputFormatEnumerator> enumOutputFormats() const = 0;
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_IMAGECODEC_H
