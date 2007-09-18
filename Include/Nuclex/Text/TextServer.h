//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## TextServer.h - Font server                          //
// ### # #      ###                                                      //
// # ### #      ###  Font loading and management                         //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_TEXT_TEXTSERVER_H
#define NUCLEX_TEXT_TEXTSERVER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Support/Exception.h"

#include <map>

// Support::stringFrom<T>(T)
// Support::crcFromString()
// Support::base64FromString()

// Nuclex::Text::TextServer
// Nuclex::Text::FontCodec
// Nuclex::Text::Font
// Nuclex::Text::Translation
//                 +removeTranslatedText()
//                 +addTranslatedText()
//                 +getTranslatedText()
//                 +clearTranslatedTexts()

namespace Nuclex {
  namespace Storage { class Stream; }
}

namespace Nuclex { namespace Text {

class FontCodec;
class Font;

//  //
//  Nuclex::Text::TextServer                                                                   //
//  //
/// Font server
/** The font server manages a set of font codecs as a chain of
    responsibility for loading various font types. Each font codec will
    read its supported font file formats and create font objects from
    these which can then be used to render text onto surfaces.

    A set of these font objects is also managed here to facilitate
    font sharing and to provide a set of standard fonts which can be
    easily changed depending on screen resolution, theme, etc.
*/
class TextServer {
  public:
    NUCLEX_DECLAREEXCEPTION(UnsupportedFormatException, UnsupportedFontFormatException);

    /// Font codec enumerator
    class CodecEnumerator;
    /// Font codec enumerator
    class FontEnumerator;

    /// Constructor
    NUCLEX_API TextServer();                    
    /// Destructor
    NUCLEX_API virtual ~TextServer();

  //
  // TextServer implementation
  //
  public:
    /// Retrieve font codec by name
    NUCLEX_API const shared_ptr<FontCodec> &getCodec(const string &sName) const;
    /// Add font codec
    NUCLEX_API void addCodec(const string &sName, const shared_ptr<FontCodec> &spCodec);
    /// Remove font codec
    NUCLEX_API void removeCodec(const string &sName);
    /// Remove all codecs
    NUCLEX_API void clearCodecs();
    /// Get codec enumerator
    NUCLEX_API shared_ptr<CodecEnumerator> enumCodecs() const;

    /// Load font from stream
    NUCLEX_API shared_ptr<Font> loadFont(
      const shared_ptr<Storage::Stream> &spSource,
      const string &sExtension = "",
      size_t Size = 0
    ) const;
    /// Get font by name
    NUCLEX_API const shared_ptr<Font> &getFont(const string &sName) const;
    /// Add font to list
    NUCLEX_API void addFont(const string &sName, const shared_ptr<Font> &spFont);
    /// Remove font from list
    NUCLEX_API void removeFont(const string &sName);
    /// Add font to list
    NUCLEX_API void clearFonts();
    /// Remove font from list
    NUCLEX_API shared_ptr<FontEnumerator> enumFonts() const;

  private:
    /// A map of font codecs
    typedef std::map<string, shared_ptr<FontCodec> > CodecMap;
    /// A map of fonts
    typedef std::map<string, shared_ptr<Font> > FontMap;

    CodecMap m_Codecs;                                ///< Registered codecs
    FontMap  m_Fonts;                                 ///< Fonts
};

//  //
//  Nuclex::Text::TextServer::CodecEnumerator                                                  //
//  //
/** Enumerates a list of font codecs
*/
class TextServer::CodecEnumerator {
  public:
    /// Destructor
    /** Destroys an instance of CodecEnumerator
    */
    NUCLEX_API virtual ~CodecEnumerator() {}

  //
  // CodecEnumerator implementation
  //
  public:
    /// Advance to next entry
    /** Advances to the next stream. If no more streams are available,
        false is returned. Also required to be the first method called,
        in order to correctly handle enumerators to empty lists.

        @return True if the next stream was reached
    */
    NUCLEX_API virtual bool next() = 0;

    /// Get current stream information
    /** Returns informations about the stream being enumerated.

        @return Informations about the current stream
    */
    NUCLEX_API virtual const shared_ptr<FontCodec> &get() const = 0;
};

//  //
//  Nuclex::Text::TextServer::FontEnumerator                                                   //
//  //
/** Enumerates a list of fonts
*/
class TextServer::FontEnumerator {
  public:
    /// Destructor
    /** Destroys an instance of FontEnumerator
    */
    NUCLEX_API virtual ~FontEnumerator() {}

  //
  // FontEnumerator implementation
  //
  public:
    /// Advance to next entry
    /** Advances to the next stream. If no more streams are available,
        false is returned. Also required to be the first method called,
        in order to correctly handle enumerators to empty lists.

        @return True if the next stream was reached
    */
    NUCLEX_API virtual bool next() = 0;

    /// Get current stream information
    /** Returns informations about the stream being enumerated.

        @return Informations about the current stream
    */
    NUCLEX_API virtual const shared_ptr<Font> &get() const = 0;
};

}} // namespace Nuclex::Text

#endif // NUCLEX_TEXT_TEXTSERVER_H
