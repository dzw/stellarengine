//  //
// ##### ####  #   #                         -= Zipex Library =-                               //
//    ## #   # ## ## zstream.h - Zipped iostream                                               //
//   ##  ####   ###                                                                            //
//  ##   #      ###  iostream implementation for zipped files                                  //
// ##    #     ## ##                                                                           //
// ##### #     #   # R1                              (C)2003-2004 Markus Ewald -> License.txt  //
//  //
#ifndef ZIPEX_ZSTREAM_H
#define ZIPEX_ZSTREAM_H

#include "zipex/zippedfile.h"
#include "zipex/ziparchive.h"
#include <streambuf>

namespace Zipex {

//  //
//  Zipex::basic_zstreambuf                                                                    //
//  //
/// Zipped stream buffer
/** A buffer for zipped streams

    @param  Character  Data type of a character
    @param  Traits     How to handle characters
*/
template<class Character, class Traits = std::char_traits<Character> >
class basic_zstreambuf : public std::basic_streambuf<Character, Traits> {
  public:
    typedef typename std::basic_streambuf<Character, Traits>::int_type int_type;
    typedef typename std::basic_streambuf<Character, Traits>::char_type char_type;
  
    enum {
      BUFFER_SIZE = 64,
      PUT_BACK_SIZE = 16
    };
    
    /// Constructor
    basic_zstreambuf(ZippedFile &File, std::ios_base::openmode mode) :
      m_File(File),
      m_bBinary((mode & std::ios_base::binary) != 0) {
      setg(0, 0, 0);
    }

    /// Destructor
    virtual ~basic_zstreambuf() {}

  protected:
    int_type pbackfail(int_type character) {
      if(eback() != gptr()) {
        gbump(-1);
        if(!traits_type::eq_int_type(traits_type::eof(), character))
            *(gptr()) = traits_type::to_char_type(character);

        return traits_type::not_eof(character);
      } else {
        return traits_type::eof();
      }
    }
  
    int_type underflow() {
      if(fillReadBuffer() > 0)
        return sgetc();
      else
        return traits_type::eof();
    }

  private:
    /// Private copy constructor
    basic_zstreambuf(const basic_zstreambuf &);
    /// Private assignment operator
    basic_zstreambuf &operator =(const basic_zstreambuf &);

    /// Translates newline characters from windows text files
    static inline unsigned int toTextMode(char* buffer, unsigned int size) {
      unsigned int newSize(size);

      // Win32-only: Convert newline characters
#ifdef ZIPEX_WIN32
      char *source = buffer;
      char *destination = buffer;
      char *end = buffer+size;

      while(source < end) {
        if(*source == '\r' && *(source+1) == '\n') {
          ++source;
          --newSize;
        }
          
        if(destination != source)
          *destination = *source;

        ++destination;
        ++source;
      }
#endif // _WIN32

      return newSize;
    }

    /// Loads new data into the read buffer
    int fillReadBuffer() {
      unsigned int putBackSize = 0;
      if(eback() != 0) {
        putBackSize = std::min(unsigned int(PUT_BACK_SIZE), unsigned int(gptr()-eback()));
        memmove(m_pReadBuffer, gptr() - putBackSize, putBackSize);
      }

      char_type *begin = m_pReadBuffer + putBackSize;
      std::size_t readBytes = m_File.readData(begin, (BUFFER_SIZE - putBackSize) * sizeof(char_type));

      if(readBytes > 0) {
        if(!m_bBinary)
          readBytes = toTextMode(begin, readBytes);
              
        setg(begin, begin, begin + readBytes);
      }

      return readBytes;
    }

    ZippedFile &m_File;
    char_type   m_pReadBuffer[BUFFER_SIZE];
    bool        m_bBinary;
};

typedef basic_zstreambuf<char> zstreambuf;
typedef basic_zstreambuf<wchar_t> wzstreambuf;

//  //
//  Zipex::basic_izstream                                                                      //
//  //
/// Zipped stream
/** Implementation of a zipped istream

    @param  Character  Data type of a character
    @param  Traits     How to handle characters
*/
template<class Character, class Traits = std::char_traits<Character> >
class basic_izstream :
  public std::basic_istream<Character, Traits> {
  public:
    /// Constructor
    /** Initializes an izstream reading from a zipped file
        
        @param  File  File from which to read
        @param  mode  Mode in which to open the file
    */
    basic_izstream(ZippedFile &File, 
                   std::ios_base::openmode mode = std::ios_base::in) :
      std::basic_istream<Character, Traits>(0),
      m_spBuffer(new basic_zstreambuf<Character, Traits>(File, mode)) {
      init(m_spBuffer.get());
    }

    /// Constructor
    /** Initializes an izstream, opening a zip archive and then
        accessing a zipped file in the archive
        
        @param  pszFilename    Filename of the zip archive
        @param  pszZippedFile  Zipped file to read from
        @param  mode           Mode in which to open the file
    */
    basic_izstream(const char *pszFilename, const char *pszZippedFile,
                   std::ios_base::openmode mode = std::ios_base::in) :
      std::basic_istream<Character, Traits>(0),
      m_spArchive(new ZipArchive(pszFilename)) {
      m_spBuffer.reset(new basic_zstreambuf<Character, Traits>(
        m_spArchive->getFile(pszZippedFile), mode
      ));
      init(m_spBuffer.get());
    }

  private:
    std::auto_ptr<ZipArchive>                           m_spArchive;
    std::auto_ptr<basic_zstreambuf<Character, Traits> > m_spBuffer;    
};

typedef basic_izstream<char> izstream;
typedef basic_izstream<wchar_t> wizstream;

} // namespace Zipex

#endif // ZIPEX_ZSTREAM_H
