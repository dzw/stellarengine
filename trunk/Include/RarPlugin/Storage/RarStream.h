//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## RarStream.h - Rar compressed file implementation                          //
// ### # #      ###                                                                            //
// # ### #      ###  Accesses a compressed file and allows to read data                        //
// #  ## #   # ## ## from it through nuclex' stream interface                                  //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_RARSTREAM_H
#define NUCLEX_STORAGE_RARSTREAM_H

#include "RarPlugin/RarPlugin.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/Storage/Stream.h"
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "UnRar/UnRar.h"

namespace Nuclex {
  namespace Storage { class RarArchive; }
}

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::RarStream                                                                 //
//  //
/// Rar data stream class
/** Performs 
*/
class RarStream :
  public Stream {
  public:
    /// Constructor
    NUCLEXRAR_API RarStream(const RarArchive &RarArchive, const string &sArchiveName,
                            unsigned long nIndex, const string &sFilename, size_t nSize,
                            AccessMode eMode);

    /// Destructor
    NUCLEXRAR_API virtual ~RarStream();

  //
  // Stream implementation
  //
  public:
    /// Get stream name
    NUCLEXRAR_API string getName() const;
    /// Get stream size
    NUCLEXRAR_API size_t getSize() const;
    /// Seek to position
    NUCLEXRAR_API void seekTo(size_t nPos);
    /// Current location
    NUCLEXRAR_API size_t getLocation() const;

    /// Read data
    NUCLEXRAR_API size_t readData(void *pDest, size_t nBytes);
    /// Write data
    NUCLEXRAR_API size_t writeData(const void *pDest, size_t nBytes);

    /// Retrieve access mode
    NUCLEXRAR_API AccessMode getAccessMode() const;

    /// Flush stream cache
    NUCLEXRAR_API void flush();
  
  private:
    static int PASCAL RARCallbackProc(UINT nMessage, LONG nUserData,
                                      LONG nParam1, LONG nParam2);

    std::vector<unsigned char> m_Memory;              ///< The uncompressed file
    size_t                     m_nLoc;                ///< Current read location
    std::exception             m_Exception;           ///< Last exception occured
    AccessMode                 m_eAccessMode;         ///< Stream access mode
};

}} // namespace Nuclex::Storage

#endif // NUCLEX_STORAGE_RARSTREAM_H
