//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## FileStream.h - File stream                                                //
// ### # #      ###                                                                            //
// # ### #      ###  A nuclex stream which wraps a file in the underlying platform's           //
// #  ## #   # ## ## file system                                                               //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_FILESTREAM_H
#define NUCLEX_STORAGE_FILESTREAM_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Storage/Stream.h"
#include "Nuclex/Storage/StorageServer.h"

#ifdef NUCLEX_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif NUCLEX_WIN32

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::FileStream                                                                //
//  //
/// Nuclex FileStream base class
/** A FileStream is both a data source and a data sink, meaning you can
    retrieve data from it or store data in it or. Where the data will
    end up depends on the FileStream class, it could provide direct access
    to a file on disk or it could store the FileStream in memory.
*/
class FileStream :
  public Stream {
  public:
    /// Constructor
    NUCLEX_API FileStream(const string &sFile, AccessMode eMode);
    /// Destructor
    NUCLEX_API virtual ~FileStream();

  //
  // Stream implementation
  //
  public:
    /// Get stream name
    NUCLEX_API string getName() const;
  
    /// Get stream size
    NUCLEX_API size_t getSize() const;

    /// Seek to position
    NUCLEX_API void seekTo(size_t nPos);

    /// Current location
    NUCLEX_API size_t getLocation() const;

    /// Read data
    NUCLEX_API size_t readData(void *pDest, size_t nBytes);

    /// Write data
    NUCLEX_API size_t writeData(const void *pDest, size_t nBytes);

    /// Retrieve access mode
    NUCLEX_API AccessMode getAccessMode() const;

    /// Flush stream cache
    NUCLEX_API void flush();

  private:
    AccessMode m_eAccessMode;               ///< Stream access mode
#ifdef NUCLEX_WIN32
    HANDLE     m_hFile;                     ///< File handle
#else
    #error Not implemented yet
#endif
    /// Name of the file being accessed
    string m_sFilename;
};

}} // namespace Nuclex::Storage

#endif // NUCLEX_STORAGE_FILESTREAM_H
