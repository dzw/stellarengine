//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## FreeTypeFontCodec.h - FreeType font codec                                 //
// ### # #      ###                                                                            //
// # ### #      ###  A font codec which tries to load any file formats supported by            //
// #  ## #   # ## ## the FreeType library and wraps them into nuclex fonts                     //
// #   #  ###  #   # R1                              (C)2002-2005 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_TEXT_FREETYPEFONTCODEC_H
#define NUCLEX_TEXT_FREETYPEFONTCODEC_H

#include "FreeTypePlugin/FreeTypePlugin.h"
#include "Nuclex/Text/FontCodec.h"

namespace Nuclex { namespace Text {

//  //
//  Nuclex::Text::FreeTypeFontCodec                                                            //
//  //
/// Font codec
/** Free type font loader
*/
class FreeTypeFontCodec : public FontCodec {
  public:
    /// Constructor
    NUCLEXFREETYPE_API FreeTypeFontCodec() {}
    /// Destructor
    NUCLEXFREETYPE_API virtual ~FreeTypeFontCodec() {}

  //
  // IFontCodec implementation
  //
  public:
    /// Check whether font can be loaded
    NUCLEXFREETYPE_API bool canLoadFont(
      const shared_ptr<Storage::Stream> &spStream,
      const string &sExtension = ""
    );

    /// Try to load font
    NUCLEXFREETYPE_API shared_ptr<Font> loadFont(
      const shared_ptr<Storage::Stream> &spStream,
      const string &sExtension = "",
      size_t nSize = 0
    );

    /// Check whether font can be saved
    NUCLEXFREETYPE_API bool canSaveFont(
      const shared_ptr<Font> &spFont,
      const string &sFormat = ""
    ) const;

    /// Try to save font
    NUCLEXFREETYPE_API void saveFont(
      const shared_ptr<Font> &spFont,
      const shared_ptr<Storage::Stream> &spStream,
      const string &sFormat = ""
    ) const;
};

}} // namespace Nuclex::Text

#endif // NUCLEX_TEXT_FREETYPEFONTCODEC_H
