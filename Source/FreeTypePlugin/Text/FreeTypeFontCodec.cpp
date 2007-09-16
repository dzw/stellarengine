//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## FreeTypeFontCodec.cpp - FreeType font codec                               //
// ### # #      ###                                                                            //
// # ### #      ###  A font codec which tries to load any file formats supported by            //
// #  ## #   # ## ## the FreeType library and wraps them into nuclex fonts                     //
// #   #  ###  #   # R1                              (C)2002-2005 Markus Ewald -> License.txt  //
//  //
#include "FreeTypePlugin/Text/FreeTypeFontCodec.h"
#include "FreeTypePlugin/Text/FreeTypeFont.h"
#include "Nuclex/Storage/Stream.h"
#include "ScopeGuard/ScopeGuard.h"

using namespace Nuclex;
using namespace Nuclex::Text;

//  //
//  FreeTypeFontWithMemory                                                                     //
//  //
/// Helper class for FreeType fonts
/** This helper class keeps the memory from which the font file was loaded alive until
    the font has been destroyed.
*/
class FreeTypeFontWithMemory : public FreeTypeFont {
  public:  
    /// Create a new FreeTypeFont
    FreeTypeFontWithMemory(
      FT_Face FTFace, unsigned long nSize, std::vector<unsigned char> &Memory
    ) :
      FreeTypeFont(FTFace, nSize) {

      // Overtake ownership of the font file's memory buffer
      m_Memory.swap(Memory);
    }

  private:
    /// Memory buffer used to store the font file
    std::vector<unsigned char> m_Memory;
};

// ############################################################################################# //
// # Nuclex::Text::FreeTypeFontCodec::canLoadFont()                                            # //
// ############################################################################################# //
/** Tries to load the font from a stream
 
    @param  spStream    Stream to load font from
    @param  sExtension  Hint für the font file's extension
    @return True if the font can be loaded
    @bug Check if it's a loadable font
*/
bool FreeTypeFontCodec::canLoadFont(
  const shared_ptr<Storage::Stream> &spStream, const string &sExtension
) {
  // Doh!
  return true; 
}

// ############################################################################################# //
// # Nuclex::Text::FreeTypeFontCodec::loadFont()                                               # //
// ############################################################################################# //
/** Tries to load the font from a stream
 
    @param  spStream    Stream to load font from
    @param  sExtension  Hint für the font file's extension
    @param  nSize       Desired size of the font
    @return The font or NULL
*/
shared_ptr<Font> FreeTypeFontCodec::loadFont(
  const shared_ptr<Storage::Stream> &spStream, const string &sExtension, size_t nSize
) {
  // Allocate memory to read the image file into
  std::vector<unsigned char> Memory(spStream->getSize());
  
  // Read the entire image into the allocated memory block
  spStream->readData(&Memory[0], spStream->getSize());

  FT_Open_Args OpenArgs;
  OpenArgs.flags = FT_OPEN_MEMORY;
  OpenArgs.memory_base = &Memory[0];
  OpenArgs.memory_size = spStream->getSize();

  FT_Face Face;
  FT_Error Error = ::FT_Open_Face(getFreeTypeLibrary(), &OpenArgs, 0, &Face);
  if(Error)
    throw UnexpectedException(
      "Nuclex::Text::FreeTypeFontCodec::loadFont()",
      string("FreeType reported an error while loading the font: ") +
        getFreeTypeErrorDescription(Error)
    );

  return shared_ptr<Font>(new FreeTypeFontWithMemory(Face, nSize, Memory));
}

// ############################################################################################# //
// # Nuclex::Text::FreeTypeFontCodec::canSaveFont()                                            # //
// ############################################################################################# //
/** Checks whether the codec is able to save the font in the requested format
 
    @param  spFont    Font to save
    @param  sFormat   Desired output format (eg. 'ttf')
    @return True if the format was supported and the font can be saved
*/
bool FreeTypeFontCodec::canSaveFont(
  const shared_ptr<Font> &spFont, const string &sFormat
) const {
  return false;
}

// ############################################################################################# //
// # Nuclex::Text::FreeTypeFontCodec::saveFont()                                               # //
// ############################################################################################# //
/** Tries to save the font into the stream using the specified format
 
    @param  spFont    Font to save
    @param  spStream  Stream to save font into
    @param  sFormat   Desired output format (eg 'ttf')
    @return True if the format was supported and the font saved
*/
void FreeTypeFontCodec::saveFont(
  const shared_ptr<Font> &spFont, const shared_ptr<Storage::Stream> &spStream,
  const string &sFormat
) const {
  throw NotSupportedException(
    "Nuclex::Text::FreeTypeFontCodec::saveFont()", "Saving fonts is not supported"
  );
}
