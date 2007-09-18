//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## CoronaImageCodec.h - Corona image codec             //
// ### # #      ###                                                      //
// # ### #      ###  Loads images through Corona                         //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R4             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_CORONAIMAGECODEC_H
#define NUCLEX_VIDEO_CORONAIMAGECODEC_H

#include "CoronaPlugin/CoronaPlugin.h"
#include "Nuclex/Video/ImageCodec.h"
#include "Corona/Corona.h"

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::CoronaImageCodec                                      //
//  //
/// Corona Image loader
/** Loads images using the Corona library
*/
class CoronaImageCodec :
  public ImageCodec {
  public:
    /// Unexpected Corona library function failure
    //NUCLEX_DECLAREEXCEPTION(UnexpectedException, CoronaFailureException);

  //
  // ImageCodec implementation
  //
  public:
    /// Check whether the image can be loaded
    NUCLEXCORONA_API bool canLoadImage(
      const shared_ptr<Storage::Stream> &spStream,
      const string &sExtension = ""
    );

    /// Load image
    NUCLEXCORONA_API shared_ptr<Image> loadImage(
      const shared_ptr<Storage::Stream> &spStream,
      const string &sExtension = ""
    );

    /// Check whether the image can be saved
    NUCLEXCORONA_API bool canSaveImage(const string &sFormat) const;

    /// Save image
    NUCLEXCORONA_API void saveImage(
      const shared_ptr<Image> &spImage,
      const shared_ptr<Storage::Stream> &spStream,
      const string &sFormat
    ) const;
   
    /// Enumerate the supported output formats
    NUCLEXCORONA_API shared_ptr<OutputFormatEnumerator> enumOutputFormats() const;
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_CORONAIMAGECODEC_H
