//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## RarArchive.h - Rar archive implementation                                 //
// ### # #      ###                                                                            //
// # ### #      ###  Represents a rar archive from which streams can be opened                 //
// #  ## #   # ## ## to read data out of compressed files                                      //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_RARARCHIVE_H
#define NUCLEX_STORAGE_RARARCHIVE_H

#include "RarPlugin/RarPlugin.h"
#include "Nuclex/Storage/Archive.h"
#include "Nuclex/Storage/StorageServer.h"

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::RarArchive                                                                //
//  //
/// Rar data storage class
/** This storage implementation accesses a .zip file. Rars store
    multiple files in a compressed archive. NCXRar is able to directly
    read from and write to these these files without requiring to
    extract them.
*/
class RarArchive :
  public Archive {
  public:
    /// Constructor
    NUCLEXRAR_API RarArchive(const string &sRARFile);
    /// Destructor
    NUCLEXRAR_API virtual ~RarArchive();

  //
  // Archive implementation
  //
  public:
    /// Get child type
    NUCLEXRAR_API ItemType getType(const string &sName) const;

    /// Get storage enumerator
    NUCLEXRAR_API shared_ptr<ArchiveEnumerator> enumArchives() const;
    /// Open a storage
    NUCLEXRAR_API shared_ptr<Archive> openArchive(const string &sName, bool bAllowCreate = false);
    /// Delete an existing storage
    NUCLEXRAR_API void deleteArchive(const string &sName);

    /// Get stream enumerator
    NUCLEXRAR_API shared_ptr<StreamEnumerator> enumStreams() const;
    /// Open a stream
    NUCLEXRAR_API shared_ptr<Stream> openStream(const string &sName, Stream::AccessMode eMode = Stream::AM_READ);
    /// Delete an existing stream
    NUCLEXRAR_API void deleteStream(const string &sName);

  protected:
    /// Rar directory
    struct RarDirectory;

  private:
    /// Rar sub stoage
    class SubRarArchive;
    friend SubRarArchive;
    
    string                      m_sArchiveName;       ///< Archive file name
    std::auto_ptr<RarDirectory> m_spDirectory;        ///< Rar directory
};  

//  //
//  Nuclex::Storage::RarArchiveFactory                                                         //
//  //
/// RarArchive factory
/** Creates RarArchives
*/
class RarArchiveFactory :
  public StorageServer::ArchiveFactory {
  public:
    /// Constructor
    /** Initializes an instance of CRarArchiveFactory
    */
    NUCLEXRAR_API RarArchiveFactory() {}
    /// Destuctor
    /** Destroys an instance of CRarArchiveFactory
    */
    NUCLEXRAR_API virtual ~RarArchiveFactory() {}

  //
  // ArchiveFactory implementation
  //
  public:
    /// Check whether storage can be created on specified source
    NUCLEXRAR_API bool canCreateArchive(const string &sSource) const;

    /// Create a storage from the specified source
    NUCLEXRAR_API shared_ptr<Archive> createArchive(const string &sSource);
};

}} // namespace Nuclex::Storage

#endif // NUCLEX_STORAGE_RARARCHIVE_H
