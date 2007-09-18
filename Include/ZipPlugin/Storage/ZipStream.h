//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ZipStream.h - Zipped file implementation                                  //
// ### # #      ###                                                                            //
// # ### #      ###  Accesses a compressed file and allows to read data                        //
// #  ## #   # ## ## from it through nuclex' stream interface                                  //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_ZIPSTREAM_H
#define NUCLEX_STORAGE_ZIPSTREAM_H

#include "ZipPlugin/ZipPlugin.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/Storage/Stream.h"
#include "Zipex/Zipex.h"

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::ZipStream                                                                 //
//  //
/// Zip data stream class
/** Performs 
*/
class ZipStream :
  public Stream {
  public:
    /// Constructor
    NUCLEXZIP_API ZipStream(Zipex::ZippedFile &ZipexFile, AccessMode eMode);

    /// Destructor
    NUCLEXZIP_API virtual ~ZipStream() {}

  //
  // Stream implementation
  //
  public:
    /// Get stream name
    NUCLEXZIP_API string getName() const;
    /// Get stream size
    NUCLEXZIP_API size_t getSize() const;
    /// Seek to position
    NUCLEXZIP_API void seekTo(size_t nPos);
    /// Current location
    NUCLEXZIP_API size_t getLocation() const;

    /// Read data
    NUCLEXZIP_API size_t readData(void *pDest, size_t nBytes);
    /// Write data
    NUCLEXZIP_API size_t writeData(const void *pSource, size_t nBytes);

    /// Retrieve access mode
    NUCLEXZIP_API AccessMode getAccessMode() const;

    /// Flush stream cache
    NUCLEXZIP_API void flush();
  
  private:
    Zipex::ZippedFile  &m_ZipexFile;
    size_t              m_Location;
    AccessMode          m_eAccessMode;                ///< Stream access mode
};

}} // namespace Nuclex::Storage

#endif // NUCLEX_STORAGE_ZIPSTREAM_H
