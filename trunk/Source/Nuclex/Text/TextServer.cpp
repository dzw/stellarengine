//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## TextServer.cpp - Font server                        //
// ### # #      ###                                                      //
// # ### #      ###  Implements an font loading server                   //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Text/TextServer.h"
#include "Nuclex/Text/FontCodec.h"
#include "Nuclex/Support/Exception.h"

using namespace Nuclex;
using namespace Nuclex::Text;

namespace {

//  //
//  FontCodecEnumerator                                                  //
//  //
/// Font codec enumerator
/** Enumerates a list of font codecs
*/
class FontCodecEnumerator :
  public TextServer::CodecEnumerator {
  typedef std::map<string, shared_ptr<FontCodec> > CodecMap;
  public:
    /// Constructor
    /** Initializes an instance of FontCodecEnumerator

        @param  CodecMap  Codecs to enumerate
    */
    FontCodecEnumerator(const CodecMap &Codecs) :
      m_Codecs(Codecs),
      m_CodecIt(Codecs.end()),
      m_CodecEnd(Codecs.end()) {}

    /// Destructor
    /** Destroys an instance of FontCodecEnumerator
    */
    virtual ~FontCodecEnumerator() {}

  //
  // CodecEnumerator implementation
  //
  public:
    /// Advance to next entry
    bool next() {
      if(m_CodecIt == m_CodecEnd)
        m_CodecIt = m_Codecs.begin();
      else
        ++m_CodecIt;
        
      return (m_CodecIt != m_CodecEnd);
    }

    /// Get current stream information
    const shared_ptr<FontCodec> &get() const { return m_CodecIt->second; }

  private:
    const CodecMap           &m_Codecs;               ///< Codecs
    CodecMap::const_iterator  m_CodecIt;              ///< Current codec
    CodecMap::const_iterator  m_CodecEnd;             ///< End iterator
};

//  //
//  FontEnumerator                                                       //
//  //
/// Font enumerator
/** Enumerates a list of fonts
*/
class FontEnumerator :
  public TextServer::FontEnumerator {
  typedef std::map<string, shared_ptr<Font> > FontMap;
  public:
    /// Constructor
    /** Initializes an instance of FontEnumerator

        @param  FontMap  Fonts to enumerate
    */
    FontEnumerator(const FontMap &Fonts) :
      m_Fonts(Fonts),
      m_FontIt(Fonts.begin()),
      m_FontEnd(Fonts.end()) {}

    /// Destructor
    /** Destroys an instance of FontEnumerator
    */
    virtual ~FontEnumerator() {}

  //
  // FontEnumerator implementation
  //
  public:
    /// Advance to next entry
    bool next() {
      if(m_FontIt == m_FontEnd)
        m_FontIt = m_Fonts.begin();
      else
        ++m_FontIt;
        
      return (m_FontIt != m_FontEnd);
    }

    /// Get current stream information
    const shared_ptr<Font> &get() const { return m_FontIt->second; }

  private:
    const FontMap           &m_Fonts;                 ///< Fonts
    FontMap::const_iterator  m_FontIt;                ///< Current font
    FontMap::const_iterator  m_FontEnd;               ///< End iterator
};

} // namespace

// ############################################################################################# //
// # Nuclex::Text::TextServer::TextServer()                                        Constructor # //
// ############################################################################################# //
/** Creates a new instance of TextServer
*/
TextServer::TextServer() {}

// ############################################################################################# //
// # Nuclex::Text::TextServer::~TextServer()                                        Destructor # //
// ############################################################################################# //
/** Destroys an instance of TextServer
*/
TextServer::~TextServer() {}

// ############################################################################################# //
// # Nuclex::Text::TextServer::getCodec()                                                      # //
// ############################################################################################# //
/** Retrieves a codec class by name

    @param  sName  Name of the Loader class to retrieve
    @return Loader class or NULL if not found
*/
const shared_ptr<FontCodec> &TextServer::getCodec(const string &sName) const {
  CodecMap::const_iterator CodecIt = m_Codecs.find(sName);
  if(CodecIt != m_Codecs.end())
    return CodecIt->second;
  else
    throw ItemNotFoundException("Nuclex::Text::TextServer::getCodec()",
                                string("Font codec not found: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::Text::TextServer::addCodec()                                                      # //
// ############################################################################################# //
/** Add a new codec class.

    @param  sName    Name of the codec class
    @param  spCodec  Pointer to the codec
*/
void TextServer::addCodec(const string &sName, const shared_ptr<FontCodec> &spCodec) {
  CodecMap::iterator CodecIt = m_Codecs.find(sName);
  if(CodecIt != m_Codecs.end())
    CodecIt->second = spCodec;
  else
    m_Codecs.insert(CodecMap::value_type(sName, spCodec));
}

// ############################################################################################# //
// # Nuclex::Text::TextServer::removeCodec()                                                   # //
// ############################################################################################# //
/** Removes a Codec class previously added using addCodec()

    @param  sName  Name of the codec class to remove
*/
void TextServer::removeCodec(const string &sName) {
  CodecMap::iterator CodecIt = m_Codecs.find(sName);
  if(CodecIt != m_Codecs.end())
    m_Codecs.erase(CodecIt);
  else
    throw ItemNotFoundException("Nuclex::Text::TextServer::removeCodec()",
                                string("Font codec not found: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::Text::TextServer::clearCodecs()                                                   # //
// ############################################################################################# //
/** Removes all Loader class currently added to the graphics server
*/
void TextServer::clearCodecs() {
  m_Codecs.clear();
}

// ############################################################################################# //
// # Nuclex::Text::TextServer::enumCodecs()                                                    # //
// ############################################################################################# //
/** Returns a new enumerator over all codecs registered to
    the video server.

    @return A new enumerator over all registered codecs
*/
shared_ptr<TextServer::CodecEnumerator> TextServer::enumCodecs() const {
  return shared_ptr<TextServer::CodecEnumerator>(
    new ::FontCodecEnumerator(m_Codecs)
  );
}

// ############################################################################################# //
// # Nuclex::Text::TextServer::loadFont()                                                      # //
// ############################################################################################# //
/** Loads a font from a stream
*/
shared_ptr<Font> TextServer::loadFont(const shared_ptr<Storage::Stream> &spSource, const string &sExtension,
                                      size_t nSize) const {
  CodecMap::const_iterator CodecEnd = m_Codecs.end();
  for(CodecMap::const_iterator CodecIt = m_Codecs.begin();
      CodecIt != CodecEnd;
      CodecIt++) {
    if(CodecIt->second->canLoadFont(spSource, sExtension))
      return CodecIt->second->loadFont(spSource, sExtension, nSize);
  }

  throw UnsupportedFontFormatException("Nuclex::Text::TextServer::loadFont()",
                                       "Unsupported font file format");
}

// ############################################################################################# //
// # Nuclex::Text::TextServer::getFont()                                                       # //
// ############################################################################################# //
/** Retrieves a Font class by name

    @param  sName  Name of the Loader class to retrieve
    @return Loader class or NULL if not found
*/
const shared_ptr<Font> &TextServer::getFont(const string &sName) const {
  FontMap::const_iterator FontIt = m_Fonts.find(sName);
  if(FontIt != m_Fonts.end())
    return FontIt->second;
  else
    throw ItemNotFoundException("Nuclex::Text::TextServer::getFont()",
                                string("Font not found: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::Text::TextServer::addFont()                                                       # //
// ############################################################################################# //
/** Add a new Font class.

    @param  sName   Name of the Font class
    @param  spFont  Pointer to the Font
*/
void TextServer::addFont(const string &sName, const shared_ptr<Font> &spFont) {
  FontMap::iterator FontIt = m_Fonts.find(sName);
  if(FontIt != m_Fonts.end())
    FontIt->second = spFont;
  else
    m_Fonts.insert(FontMap::value_type(sName, spFont));
}

// ############################################################################################# //
// # Nuclex::Text::TextServer::removeFont()                                                    # //
// ############################################################################################# //
/** Removes a Font class previously added using addFont()

    @param  sName  Name of the Font class to remove
*/
void TextServer::removeFont(const string &sName) {
  FontMap::iterator FontIt = m_Fonts.find(sName);
  if(FontIt != m_Fonts.end())
    m_Fonts.erase(FontIt);
  else
    throw ItemNotFoundException("Nuclex::Text::TextServer::removeFont()",
                                string("Font not found: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::Text::TextServer::clearFonts()                                                    # //
// ############################################################################################# //
/** Removes all Loader class currently added to the graphics server
*/
void TextServer::clearFonts() {
  m_Fonts.clear();
}

// ############################################################################################# //
// # Nuclex::Text::TextServer::enumFonts()                                                     # //
// ############################################################################################# //
/** Returns a new enumerator over all Fonts registered to
    the video server.

    @return A new enumerator over all registered Fonts
*/
shared_ptr<TextServer::FontEnumerator> TextServer::enumFonts() const {
  return shared_ptr<TextServer::FontEnumerator>(
    new ::FontEnumerator(m_Fonts)
  );
}
