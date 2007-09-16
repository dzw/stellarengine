//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## StorageServer.cpp - Data storage subsystem manager                        //
// ### # #      ###                                                                            //
// # ### #      ###  Manages all data accesses within nuclex and allows for archives           //
// #  ## #   # ## ## to be registered for common resource lookups                              //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Storage/StorageServer.h"
#include "Nuclex/Storage/DirectoryArchive.h"

using namespace Nuclex;
using namespace Nuclex::Storage;

// ############################################################################################# //
// # Nuclex::Storage::StorageServer::StorageServer()                               Constructor # //
// ############################################################################################# //
/** Initializes an instance of StorageServer
*/
StorageServer::StorageServer() {
  // Add built-in archive factory for the platform's file system
  addArchiveFactory("Directory", shared_ptr<ArchiveFactory>(new DirectoryArchiveFactory()));

  // Allows us to interpret plain file names as well
  addArchive("", shared_ptr<Archive>(new DirectoryArchive()));
}

// ############################################################################################# //
// # Nuclex::Storage::StorageServer::getArchiveFactory()                                       # //
// ############################################################################################# //
/** Retrieves an archive factory by the name it was registered with

    @param  sName  Name of the ArchiveFactory to retrieve
    @return The ArchiveFactory
    @see addArchiveFactory, removeArchiveFactory, clearArchiveFactories
*/
shared_ptr<StorageServer::ArchiveFactory> StorageServer::getArchiveFactory(const string &sName) {
  ArchiveFactoryMap::iterator ArchiveFactoryIt = m_ArchiveFactories.find(sName);
  if(ArchiveFactoryIt == m_ArchiveFactories.end())
    throw ItemNotFoundException("Nuclex::Storage::StorageServer::getArchiveFactory()",
                                string("A factory named '") + sName + "' does not exist");

  return ArchiveFactoryIt->second;
}

// ############################################################################################# //
// # Nuclex::Storage::StorageServer::addArchiveFactory()                                       # //
// ############################################################################################# //
/** Add a new ArchiveFactory to be used within the responsibility chain
    when an archive is to be opened by openArchive(). The order in which
    the ArchiveFactories will be tested for compatibility will be
    determined by the order in which they are registered.

    @param  sName             Name of the ArchiveFactory
    @param  spArchiveFactory  Archive factory to add
    @see openArchive
*/
void StorageServer::addArchiveFactory(const string &sName, const shared_ptr<ArchiveFactory> &spArchiveFactory) {
  ArchiveFactoryMap::iterator ArchiveFactoryIt = m_ArchiveFactories.find(sName);
  if(ArchiveFactoryIt != m_ArchiveFactories.end())
    ArchiveFactoryIt->second = spArchiveFactory;
  else
    m_ArchiveFactories.insert(ArchiveFactoryMap::value_type(sName, spArchiveFactory));
}

// ############################################################################################# //
// # Nuclex::Storage::StorageServer::removeArchiveFactory()                                    # //
// ############################################################################################# //
/** Removes an ArchiveFactory that was previously added using
    addArchiveFactory()

    @param  sName  Name of the ArchiveFactory to remove
    @see addArchiveFactory, clearArchiveFactories
*/
void StorageServer::removeArchiveFactory(const string &sName) {
  ArchiveFactoryMap::iterator ArchiveFactoryIt = m_ArchiveFactories.find(sName);
  if(ArchiveFactoryIt != m_ArchiveFactories.end())
    m_ArchiveFactories.erase(ArchiveFactoryIt);
  else
    throw ItemNotFoundException("Nuclex::Storage::StorageServer::removeArchiveFactory()",
                                string("A factory named '") + sName + "' does not exist");
}

// ############################################################################################# //
// # Nuclex::Storage::StorageServer::clearArchiveFactories()                                   # //
// ############################################################################################# //
/** Clears all ArchiveFactories added to the StorageServer
    @see addArchiveFactory, removeArchiveFactory
*/
void StorageServer::clearArchiveFactories() {
  m_ArchiveFactories.clear();
}

// ############################################################################################# //
// # Nuclex::Storage::StorageServer::getArchive()                                              # //
// ############################################################################################# //
/** Retrieves an archive by the name it was registered to

    @param  sName  Name of the archive to retrieve
    @return The archive
    @see addArchive, removeArchive, clearArchives
*/
const shared_ptr<Archive> &StorageServer::getArchive(const string &sName) {
  ArchiveMap::iterator ArchiveIt = m_Archives.find(sName);
  if(ArchiveIt == m_Archives.end())
    throw ItemNotFoundException("Nuclex::Storage::StorageServer::getArchive()",
                                string("An archive named '") + sName + "' does not exist");

  return ArchiveIt->second;
}

// ############################################################################################# //
// # Nuclex::Storage::StorageServer::addArchive()                                              # //
// ############################################################################################# //
/** Add a new archive to the StorageServer. Calls to the openStream()
    method can use the archive name to specify from which archive to
    open the new stream.

    @param  sName      Name of the archive
    @param  spArchive  Archive to add
    @see removeArchive, openStream
*/
void StorageServer::addArchive(const string &sName, const shared_ptr<Archive> &spArchive) {
  ArchiveMap::iterator ArchiveIt = m_Archives.find(sName);
  if(ArchiveIt != m_Archives.end())
    ArchiveIt->second = spArchive;
  else
    m_Archives.insert(ArchiveMap::value_type(sName, spArchive));
}

// ############################################################################################# //
// # Nuclex::Storage::StorageServer::removeArchive()                                           # //
// ############################################################################################# //
/** Removes an archive that was previously added using addArchive()

    @param  sName  Name of the storage class to remove
    @see addArchive, clearArchives
*/
void StorageServer::removeArchive(const string &sName) {
  ArchiveMap::iterator ArchiveIt = m_Archives.find(sName);
  if(ArchiveIt != m_Archives.end())
    m_Archives.erase(ArchiveIt);
  else
    throw ItemNotFoundException("Nuclex::Storage::StorageServer::getArchive()",
                                string("A storage named '") + sName + "' does not exist");
}

// ############################################################################################# //
// # Nuclex::StorageServer::clearArchives()                                                    # //
// ############################################################################################# //
/** Removes all archives added to the StorageServer

    @see addArchive, removeArchive
*/
void StorageServer::clearArchives() {
  m_Archives.clear();
}

// ############################################################################################# //
// # Nuclex::Storage::StorageServer::openArchive()                                             # //
// ############################################################################################# //
/** Opens an existing archive, trying to determine its type by
    querying the registered archive factories as a responsibility chain.

    @param  sSource  Identifier understood by a registered archive
                     factory, could for example be an IP or a filename
    @return A new archive
    @see addArchiveFactory, removeArchiveFactory, openStream
*/
shared_ptr<Archive> StorageServer::openArchive(const string &sSource) const {
  ArchiveFactoryMap::const_iterator FactoryEnd = m_ArchiveFactories.end();
  for(ArchiveFactoryMap::const_iterator FactoryIt = m_ArchiveFactories.begin();
      FactoryIt != FactoryEnd;
      FactoryIt++)
    if(FactoryIt->second->canCreateArchive(sSource))
      return FactoryIt->second->createArchive(sSource);

  throw CantOpenResourceException("Nuclex::Storage::StorageServer::openArchive()",
                                  string("Unsupported archive type or resource not found: '") + sSource + "'");
}

// ############################################################################################# //
// # Nuclex::Storage::StorageServer::openStream()                                              # //
// ############################################################################################# //
/** Creates a new stream from an identifier consisting of an archive
    name seperated by a ':' from a stream name. If no archive name is
    specified (the ':' can be left out in this case), the StorageServer
    looks for an archive with no name, which is, by default, registered
    to a DirectoryArchive located in the applications launch path.Thus
    you can pass a plain relative file name and it will be opened.

    @param  sSource  Identifier descibring the stream to open
    @param  eMode    Access mode for the new stream
    @return A new stream
    @see addArchive, removeArchive, openArchive
*/
shared_ptr<Stream> StorageServer::openStream(const string &sSource, Stream::AccessMode eMode) const {
  ArchiveMap::const_iterator ArchiveIt;

  // Look for the archive class delimiter
  string::size_type Pos = sSource.find(':');

  // Look up the archive class
  if(Pos == static_cast<string::size_type>(string::npos))
    ArchiveIt = m_Archives.find("");
  else
    ArchiveIt = m_Archives.find(sSource.substr(0, Pos));

  // The specified archive has to exist in order to succeed
  if(ArchiveIt == m_Archives.end())
    throw ItemNotFoundException("Nuclex::Storage::StorageServer::openStream()", 
                                string("Unknown archive name: '") + sSource.substr(0, Pos) + "'");

  // Let the archive handle the opening of the stream
  if(Pos == static_cast<string::size_type>(string::npos))
    return ArchiveIt->second->openStream(sSource, eMode);
  else
    return ArchiveIt->second->openStream(sSource.substr(Pos + 1), eMode);
}
