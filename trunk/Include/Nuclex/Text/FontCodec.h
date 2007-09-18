//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## FontCodec.h - Font codec                            //
// ### # #      ###                                                      //
// # ### #      ###  Defines the interface for font codecs               //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_TEXT_FONTCODEC_H
#define NUCLEX_TEXT_FONTCODEC_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"

namespace Nuclex {
  namespace Storage { class Stream; }
}
namespace Nuclex { namespace Text {

class Font;

//  //
//  Nuclex::Text::FontCodec                                                                    //
//  //
/// Font loader
/** 
*/
class FontCodec {
  public:
    /// Destructor
    /** Destroys an instance of Font
    */
    NUCLEX_API virtual ~FontCodec() {}

  //
  // FontCodec implementation
  //
  public:
    /// Check whether font can be loaded
    /** Checks whether the codec is able to load the requested font
     
        @param  spStream    Stream to load font from
        @param  sExtension  Hint for the font file's extension
        @return True if the font can be loaded
    */
    NUCLEX_API virtual bool canLoadFont(
      const shared_ptr<Storage::Stream> &spStream,
      const string &sExtension = ""
    ) = 0;

    /// Try to load font
    /** Tries to load the font from a stream
     
        @param  spStream    Stream to load font from
        @param  sExtension  Hint for the font file's extension
        @param  Size        Desired size of the text (may be ignored)
        @return The loaded font
    */
    NUCLEX_API virtual shared_ptr<Font> loadFont(
      const shared_ptr<Storage::Stream> &spStream,
      const string &sExtension = "",
      size_t Size = 0
    ) = 0;

    /// Check whether font can be saved
    /** Checks whether the codec is able to save the font in the requested format
     
        @param  spFont    Font to save
        @param  sFormat   Desired output format (eg. 'ttf')
        @return True if the format was supported and the font can be saved
    */
    NUCLEX_API virtual bool canSaveFont(
      const shared_ptr<Font> &spFont,
      const string &sFormat = ""
    ) const = 0;

    /// Try to save font
    /** Tries to save the font into the stream using the specified format
     
        @param  spFont    Font to save
        @param  spStream  Stream to save font into
        @param  sFormat   Desired output format (eg. 'ttf')
    */
    NUCLEX_API virtual void saveFont(
      const shared_ptr<Font> &spFont,
      const shared_ptr<Storage::Stream> &spStream,
      const string &sFormat = ""
    ) const = 0;
};

}} // namespace Nuclex::Text

#endif // NUCLEX_TEXT_FONTCODEC_H
