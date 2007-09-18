//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## DirectoryArchive.h - Directory archive                                    //
// ### # #      ###                                                                            //
// # ### #      ###  A nuclex archive which wraps a directory in the underlying platform's     //
// #  ## #   # ## ## file system                                                               //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_DIRECTORYARCHIVE_H
#define NUCLEX_STORAGE_DIRECTORYARCHIVE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Storage/Archive.h"
#include "Nuclex/Storage/StorageServer.h"

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::DirectoryArchive                                                          //
//  //
/// Directory archive
/** Wraps a directory on the platform's file system to a nuclex archive.
    Creates FileStreams which directly read out of files when a stream
    is requested from the DirectoryArchive.
*/
class DirectoryArchive :
  public Archive {
  public:
    /// Path constructor
    NUCLEX_API DirectoryArchive(const string &sPath = "");
    /// Destructor
    NUCLEX_API virtual ~DirectoryArchive();

  //
  // DirectoryArchive implementation
  //
  public:
    /// Get path
    NUCLEX_API const string &getPath() const;

  //
  // Archive implementation
  //
  public:
    /// Get child type
    ItemType getType(const string &sName) const;

    /// Get storage enumerator
    NUCLEX_API shared_ptr<ArchiveEnumerator> enumArchives() const;

    /// Open a storage
    NUCLEX_API shared_ptr<Archive> openArchive(
      const string &sName,
      bool bAllowCreate = false
    );

    /// Delete an existing storage
    NUCLEX_API void deleteArchive(const string &sName);

    /// Get stream enumerator
    NUCLEX_API shared_ptr<StreamEnumerator> enumStreams() const;

    /// Open a stream
    NUCLEX_API shared_ptr<Stream> openStream(
      const string &sName,
      Stream::AccessMode eMode = Stream::AM_READ
    );

    /// Delete an existing stream
    NUCLEX_API void deleteStream(const string &sName);

  private:
    string m_sPath;                                   ///< Directory path
};

//  //
//  Nuclex::Storage::DirectoryArchiveFactory                                                   //
//  //
/// DirectoryArchive factory
/** Factory for DirectoryArchives. Used by the StorageServer as a
    Chain Of Responsibility when an unknown archive is to be opened.
*/
class DirectoryArchiveFactory :
  public StorageServer::ArchiveFactory {
  public:
    /// Constructor
    /** Initializes an instance of DirectoryArchiveFactory
    */
    NUCLEX_API DirectoryArchiveFactory() {}

    /// Destuctor
    /** Destroys an instance of CDirArchiveFactory
    */
    NUCLEX_API virtual ~DirectoryArchiveFactory() {}

  //
  // ArchiveFactory implementation
  //
  public:
    /// Check whether storage can be created on specified source
    NUCLEX_API bool canCreateArchive(const string &sSource) const;

    /// Create a storage from the specified source
    NUCLEX_API shared_ptr<Archive> createArchive(const string &sSource);
};

}} // namespace Nuclex::Storage

#endif // NUCLEX_STORAGE_DIRECTORYARCHIVE_H
