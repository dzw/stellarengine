//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## StorageServer.h - Data storage subsystem manager                          //
// ### # #      ###                                                                            //
// # ### #      ###  Manages all data accesses within nuclex and allows for archives           //
// #  ## #   # ## ## to be registered for common resource lookups                              //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_STORAGESERVER_H
#define NUCLEX_STORAGE_STORAGESERVER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Storage/Archive.h"
#include "Nuclex/Storage/Stream.h"
#include "Nuclex/Support/Exception.h"
#include <map>

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::StorageServer                                                             //
//  //
/// Data storage manager
/** The storage server manages a set of archives which can be accessed
    freely. You could, for example, register a storage named "textures"
    to the storage server and use it whenever you need to load a texture
    through a stream. If you change the location of your textures or
    maybe compress them in a zip file, you could easily change the storage
    without requiring any changes to your other code.

    For convenience, the storage server's openStream() method handles
    these registered storages like virtual drives. In the above example,
    you could directly give it a file name like "textures:mytex01.png".

    While storages can be manually created, the storage server also has
    a list of storage factories, which, using the CHOR pattern, can
    create storages from any location you specify using the openArchive()
    method. As an example, you could tell it to open "mydata.zip" and
    expect to receive a zip storage, or tell it to open "myfolder/" and
    expect to receive a directory storage.
*/
class StorageServer {
  public:
    /// Archive factory
    class ArchiveFactory;

    /// Constructor
    NUCLEX_API StorageServer();

  //
  // ArchiveServer implementation
  //
  public:
    /// Retrieve storage factory
    NUCLEX_API shared_ptr<ArchiveFactory> getArchiveFactory(const string &sName);
    /// Add new storage factory
    NUCLEX_API void addArchiveFactory(const string &sName, 
                                      const shared_ptr<ArchiveFactory> &spArchiveFactory);
    /// Remove ArchiveFactory
    NUCLEX_API void removeArchiveFactory(const string &sName);
    /// Remove all ArchiveFactorys
    NUCLEX_API void clearArchiveFactories();

    /// Retrieve storage
    NUCLEX_API const shared_ptr<Archive> &getArchive(const string &sName);
    /// Add new storage
    NUCLEX_API void addArchive(const string &sName, const shared_ptr<Archive> &spArchive);
    /// Remove storage
    NUCLEX_API void removeArchive(const string &sName);
    /// Remove all storages
    NUCLEX_API void clearArchives();

    /// Open existing archive
    NUCLEX_API shared_ptr<Archive> openArchive(const string &sSource) const;
    /// Open data stream
    NUCLEX_API shared_ptr<Stream> openStream(
      const string &sSource,
      Stream::AccessMode eMode = Stream::AM_READ
    ) const;

  private:
    /// Map of active storages
    typedef std::map<string, shared_ptr<Archive> > ArchiveMap;
    /// Responsibility chain of storage factories
    typedef std::map<string, shared_ptr<ArchiveFactory> > ArchiveFactoryMap;

    ArchiveMap         m_Archives;                    ///< Active archives
    ArchiveFactoryMap  m_ArchiveFactories;            ///< Archive factories
};

//  //
//  Nuclex::Storage::StorageServer::ArchiveFactory                                             //
//  //
/** Creates storages
*/
class StorageServer::ArchiveFactory {
  public:
    /// Can't create storage exception
    /** This can be avoided by using canCreateArchive() to check
        whether the factory is able to create a storage on the given
        source string
    */
    NUCLEX_DECLAREEXCEPTION(ResourceException, CantCreateArchiveException);

  //
  // ArchiveFactory implementation
  //
  public:
    /// Check whether storage can be created on specified source
    /** Check to see if this factory can create a storage on the
        specified source. The source string can be just about anything
        so the factory should carefully check whether it can create
        a storage on it.

        @param  sSource  Source specifier string
        @return True, if this factory is able to create a storage on
                the specified source
    */
    NUCLEX_API virtual bool canCreateArchive(const string &sSource) const { return false; }

    /// Create a storage from the specified source
    /** Creates a storage using the specigied source string, which is
        freely interpreted by the matching storage factory

        @param  sSource  Source specifier string
        @return The created storage
    */
    NUCLEX_API virtual shared_ptr<Archive> createArchive(const string &sSource) = 0;
};

}} // namespace Nuclex::Archive

#endif // NUCLEX_STORAGE_STORAGESERVER_H
