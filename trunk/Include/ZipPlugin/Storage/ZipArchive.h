//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ZipArchive.h - Zip archive implementation                                 //
// ### # #      ###                                                                            //
// # ### #      ###  Represents a zip archive from which streams can be opened                 //
// #  ## #   # ## ## to read data out of compressed files                                      //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_ZIPARCHIVE_H
#define NUCLEX_STORAGE_ZIPARCHIVE_H

#include "ZipPlugin/ZipPlugin.h"
#include "Nuclex/Storage/StorageServer.h"
#include "Zipex/Zipex.h"

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::ZipArchive                                                                //
//  //
/// Zip data storage class
/** This storage implementation accesses a .zip archive. Zip archives
    store multiple files in a single compressed file. NuclexZip is able
    to directly read from and write to these these files without
    requiring to extract them.
*/
class ZipArchive :
  public Archive {
  public:
    /// Constructor
    NUCLEXZIP_API ZipArchive(const string &sZIPFile);
    /// Destructor
    NUCLEXZIP_API virtual ~ZipArchive();

  //
  // Storage implementation
  //
  public:
    /// Get child type
    NUCLEXZIP_API ItemType getType(const string &sName) const;

    /// Get storage enumerator
    NUCLEXZIP_API shared_ptr<ArchiveEnumerator> enumArchives() const;
    /// Open a storage
    NUCLEXZIP_API shared_ptr<Archive> openArchive(const string &sName, bool bAllowCreate = false);
    /// Delete an existing storage
    NUCLEXZIP_API void deleteArchive(const string &sName);

    /// Get stream enumerator
    NUCLEXZIP_API shared_ptr<StreamEnumerator> enumStreams() const;
    /// Open a stream
    NUCLEXZIP_API shared_ptr<Stream> openStream(const string &sName, Stream::AccessMode eMode = Stream::AM_READ);
    /// Delete an existing stream
    NUCLEXZIP_API void deleteStream(const string &sName);

  private:
    /// Zip directory
    struct ZipDirectory;

    /// Zip sub stoage
    class SubZipArchive;
    friend SubZipArchive;
    
    Zipex::ZipArchive m_ZipArchive;                   ///< Zipex archive
    std::auto_ptr<ZipDirectory> m_spDirectory;        ///< Zip directory
};  

//  //
//  Nuclex::Storage::ZipArchiveFactory                                                         //
//  //
/// ZipArchive factory
/** Creates ZipArchives and checks whether a given source identifier
    can be opened as a zip file
*/
class ZipArchiveFactory :
  public StorageServer::ArchiveFactory {
  public:
    /// Destuctor
    /** Destroys an instance of ZipStorageFactory
    */
    NUCLEXZIP_API virtual ~ZipArchiveFactory() {}

  //
  // StorageFactory implementation
  //
  public:
    /// Check whether storage can be created on specified source
    NUCLEXZIP_API bool canCreateArchive(const string &sSource) const;

    /// Create a storage from the specified source
    NUCLEXZIP_API shared_ptr<Archive> createArchive(const string &sSource);
};

}} // namespace Nuclex::Storage

#endif // NUCLEX_STORAGE_ZIPARCHIVE_H
