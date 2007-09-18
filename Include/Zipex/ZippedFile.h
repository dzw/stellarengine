//  //
// ##### ####  #   #                         -= Zipex Library =-                               //
//    ## #   # ## ## ZippedFile.h - File stored in a zip archive                               //
//   ##  ####   ###                                                                            //
//  ##   #      ###  Controls access to a file that's stored within a zip archive              //
// ##    #     ## ##                                                                           //
// ##### #     #   # R1                              (C)2003-2004 Markus Ewald -> License.txt  //
//  //
#ifndef ZIPEX_ZIPPEDFILE_H
#define ZIPEX_ZIPPEDFILE_H

#include "zipex/config.h"
#include "zipex/stream.h"
#include "zlib/zlib.h"  

#include <string>
#include <vector>

namespace Zipex {

class ZipArchive;

//  //
//  Zipex::ZippedFile                                                                          //
//  //
/// Zipped file
/** Represents a zipped file and provides methods for retrieving
    the file's properties (like size, filename, crc, attributes, etc.).

    Through the Stream interface, it is also possible to read data
    from the zipped file. Since zip archives can be constructed on any
    zipx stream, you can also construct another ZipArchive from a ZippedFile
    to open nested archives.
    If a file's compression method is unsupported, your can still access
    its properties, but you will not be able to read or write to it.
*/
class ZippedFile :
  public Stream {
  friend ZipArchive;
  public:
    enum CompressionMethod {
      CM_STORE     = 0,                               ///< The file is stored (no compression)
      CM_SHRUNK    = 1,                               ///< The file is Shrunk
      CM_REDUCE1   = 2,                               ///< The file is Reduced with compression factor 1
      CM_REDUCE2   = 3,                               ///< The file is Reduced with compression factor 2
      CM_REDUCE3   = 4,                               ///< The file is Reduced with compression factor 3
      CM_REDUCE4   = 5,                               ///< The file is Reduced with compression factor 4
      CM_IMPLODE   = 6,                               ///< The file is Imploded
      CM_TOKENIZE  = 7,                               ///< Reserved for Tokenizing compression algorithm 
      CM_DEFLATE   = 8,                               ///< The file is Deflated 
      CM_DEFLATE64 = 9,                               ///< Enhanced Deflating using Deflate64(tm)
      CM_PKIMPLODE = 10                               ///< PKWARE Date Compression Library Imploding
    };

  //
  // ZippedFile implementation
  //
  public:
    /// Retrieves the file's name
    ZIPEX_API inline const std::string &getFilename() const { return m_sFilename; }
    /// Retrieves the comment for the file
    ZIPEX_API inline const std::string &getComment() const { return m_sComment; }
    /// Retrieves the file's extra field containing user defined data
    ZIPEX_API inline const std::string &getExtraField() const { return m_sExtraField; }

    /// Retrieves the CRC32 of the file's contents
    ZIPEX_API inline unsigned long getCRC32() const { return m_nCRC32; }
    /// Retrieves the file's compressed size
    ZIPEX_API inline size_t getCompressedSize() const { return m_nCompressedSize; }
    /// Retrieves the file's original (uncompressed) size
    ZIPEX_API inline size_t getSize() const { return m_nUncompressedSize; }
    /// Retrieves the time and date when the file was last modified
    ZIPEX_API inline unsigned long getLastModifyTime() const { return m_nLastModifyTime; }
    /// Retrieves the file's internal attributes
    ZIPEX_API inline unsigned long getInternalAttributes() const { return m_nInternalAttributes; }
    /// Retrieves the file's external attributes
    ZIPEX_API inline unsigned long getExternalAttributes() const { return m_nExternalAttributes; }

    /// Decompress and cache the entire file in memory
    ZIPEX_API const void *precacheAll();
    /// Flush memory buffers
    ZIPEX_API void flush();
    
  //
  // Stream implementation
  //
  public:
    /// Seek to position
    ZIPEX_API void seekTo(size_t nPosition);
    /// Get current location
    ZIPEX_API size_t getLocation() const;
    /// Read data into buffer
    ZIPEX_API size_t readData(void *pBuffer, size_t nLength);
    /// Write data from buffer
    ZIPEX_API void writeData(const void *pBuffer, size_t nLength);
    /// Read from specified location
    ZIPEX_API size_t readDataAt(size_t nPosition, void *pBuffer, size_t nLength);
    /// Read from specified location
    ZIPEX_API void writeDataAt(size_t nPosition, const void *pBuffer, size_t nLength);
    
  protected:
    /// Constructor
    ZIPEX_API ZippedFile(Stream *pStream, size_t nOffset, CompressionMethod eCompressionMethod,
                         const std::string &sFilename, const std::string &sComment,
                         const std::string &sExtraField, unsigned long nCRC32,
                         size_t nCompressedSize, size_t nUncompressedSize,
                         unsigned long nLastModifyTime, unsigned long nInternalAttributes,
                         unsigned long nExternalAttributes);

    /// Destructor
    ZIPEX_API virtual ~ZippedFile();
  
  private:
    ZippedFile(const ZippedFile &); // Not allowed
    void operator =(const ZippedFile &); // Not allowed
  
    /// A vector of bytes
    typedef std::vector<unsigned char> ByteVector;

    /// Uncompress data at least up to the specified byte count
    void (ZippedFile::*extractTo)(size_t nBytes);
    /// Uncompress data that has been compressed using deflation
    void inflateTo(size_t nBytes);
    /// Uncompress data that has been stored uncompressed
    void destoreTo(size_t nBytes);
    /// Throws an exception for unsupported compression formats
    void unsupported(size_t);

    std::string        m_sFilename;                   ///< Filename
    std::string        m_sComment;                    ///< Optional file comment
    std::string        m_sExtraField;                 ///< Optional extra data
    unsigned long      m_nCRC32;                      ///< CRC32 of the file's contents
    size_t             m_nCompressedSize;             ///< Compressed file size
    size_t             m_nUncompressedSize;           ///< Original (uncompressed) file size
    unsigned long      m_nLastModifyTime;             ///< Last modification time and date
    unsigned long      m_nInternalAttributes;         ///< Internal file attributes
    unsigned long      m_nExternalAttributes;         ///< External file attributes
    CompressionMethod  m_eCompressionMethod;          ///< Compression method
                      
    size_t             m_nPosition;                   ///< File pointer in uncompressed data
    Stream            *m_pStream;                     ///< Stream to read from
    size_t             m_nOffset;                     ///< Starting offset within stream
    ByteVector         m_Data;                        ///< Decompressed file data

    bool               m_bDecompressorInitialized;    ///< ZLib decompressor initialized ?
    z_stream           m_ZLibStream;                  ///< ZLib decompressor state
    size_t             m_nCompressedPosition;         ///< File pointer in compressed data
    ByteVector         m_DecompressionBuffer;         ///< Decompression buffer
};

} // namespace Zipex

#endif // ZIPEX_ZIPPEDFILE_H
