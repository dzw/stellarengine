//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ZipArchive.cpp - Zip archive implementation                               //
// ### # #      ###                                                                            //
// # ### #      ###  Represents a zip archive from which streams can be opened                 //
// #  ## #   # ## ## to read data out of compressed files                                      //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "ZipPlugin/Storage/ZipArchive.h"
#include "ZipPlugin/Storage/ZipStream.h"
#include "Nuclex/Platform.h"

#ifdef NUCLEX_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif // NUCLEX_WIN32

#include <map>

using namespace Nuclex;
using namespace Nuclex::Storage;

//  //
//  Nuclex::Storage::ZipArchive::ZipDirectory                                                  //
//  //
/** A directory in the ZIP file.
    Used to increase searching speed and because the unzip functions
    aren't thread-safe (they only manage a single directory index)
*/
struct ZipArchive::ZipDirectory {
  public:
    /// Map of subdirectories
    typedef std::map<string, ZipDirectory> ZipDirectoryMap;
    /// Set of strings
    typedef std::map<string, unsigned long> ZipFileMap;
    /// Storage (directory) enumerator
    class ZipArchiveEnumerator;
    /// Stream (file) enumerator
    class ZipStreamEnumerator;

    /// Constructor
    ZipDirectory() {}
    /// Copy constructor
    ZipDirectory(const ZipDirectory &Directory) :
      Directories(Directory.Directories),
      Files(Directory.Files) {}

  //
  // ZipDirectory implementation
  //
  public:
    /// Get type of child object
    Archive::ItemType getType(const string &sName) const;

    ZipDirectoryMap Directories;                      ///< Subdirectories
    ZipFileMap      Files;                            ///< Files in the directory
};

//  //
//  Nuclex::Storage::ZipArchive::ZipDirectory::ZipArchiveEnumerator                            //
//  //
/// Storage enumerator
/** Enumerates a list of storages
*/
class ZipArchive::ZipDirectory::ZipArchiveEnumerator :
  public Archive::ArchiveEnumerator {
  public:
    /// Constructor
    ZipArchiveEnumerator(const ZipDirectory::ZipDirectoryMap &Archives) :
      m_Archives(Archives),
      m_ArchiveIt(Archives.end()),
      m_ArchiveEnd(Archives.end()) {}
      
    /// Destructor
    virtual ~ZipArchiveEnumerator() {}

  //
  // ArchiveEnumerator implementation
  //
  public:
    /// Advance to next entry
    bool next() {
      if(m_ArchiveIt == m_ArchiveEnd)
        m_ArchiveIt = m_Archives.begin();
      else
        ++m_ArchiveIt;

      if(m_ArchiveIt != m_ArchiveEnd)
        m_ArchiveInfo.sName = m_ArchiveIt->first;
      
      return (m_ArchiveIt != m_ArchiveEnd);  
    }

    /// Get current stream information
    const ArchiveInfo &get() const { return m_ArchiveInfo; }

  private:
    ArchiveInfo                                    m_ArchiveInfo;
    const ZipDirectory::ZipDirectoryMap           &m_Archives; // Storages
    ZipDirectory::ZipDirectoryMap::const_iterator  m_ArchiveIt; ///< Current iterator
    ZipDirectory::ZipDirectoryMap::const_iterator  m_ArchiveEnd; ///< End iterator
};

//  //
//  Nuclex::Storage::ZipArchive::ZipDirectory::ZipStreamEnumerator                             //
//  //
/// Stream enumerator
/** Enumerates a list of streams
*/
class ZipArchive::ZipDirectory::ZipStreamEnumerator :
  public Archive::StreamEnumerator {
  public:
    /// Constructor
    ZipStreamEnumerator(const ZipDirectory::ZipFileMap &Streams) :
      m_Streams(Streams),
      m_StreamIt(m_Streams.end()),
      m_StreamEnd(m_Streams.end()) {}
    
    /// Destructor
    virtual ~ZipStreamEnumerator() {}

  //
  // StreamEnumerator implementation
  //
  public:
    /// Advance to next entry
    bool next() {
      if(m_StreamIt == m_StreamEnd)
        m_StreamIt = m_Streams.begin();
      else
        ++m_StreamIt;

      if(m_StreamIt != m_StreamEnd) {
        m_StreamInfo.sName = m_StreamIt->first;
        m_StreamInfo.nSize = m_StreamIt->second;
      }

      return (m_StreamIt != m_StreamEnd);  
    }

    /// Get current stream information
    const StreamInfo &get() const { return m_StreamInfo; }

  private:
    StreamInfo                                m_StreamInfo;
    const ZipDirectory::ZipFileMap           &m_Streams; // Streams
    ZipDirectory::ZipFileMap::const_iterator  m_StreamIt; ///< Current iterator
    ZipDirectory::ZipFileMap::const_iterator  m_StreamEnd; ///< End iterator
};

//  //
//  Nuclex::Storage::ZipArchive::SubZipArchive                                                 //
//  //
/// Zip data storage class
/** This storage implementation accesses a .zip file. Zips store
    multiple files in a compressed archive. NCXZip is able to directly
    read from and write to these these files without requiring to
    extract them.
*/
class ZipArchive::SubZipArchive :
  public Archive {
  public:
    /// Constructor
    SubZipArchive(const ZipArchive &ZipArchive, Zipex::ZipArchive &ZipexArchive,
                  const ZipDirectory &Directory, const string &sPath);
    /// Destructor
    virtual ~SubZipArchive();

  //
  // Storage implementation
  //
  public:
    /// Get child type
    ItemType getType(const string &sName) const;

    /// Get storage enumerator
    shared_ptr<ArchiveEnumerator> enumArchives() const;

    /// Open a storage
    shared_ptr<Archive> openArchive(const string &sName, bool bAllowCreate = false);

    /// Delete an existing storage
    void deleteArchive(const string &sName);

    /// Get stream enumerator
    shared_ptr<StreamEnumerator> enumStreams() const;

    /// Open a stream
    shared_ptr<Stream> openStream(
      const string &sName,
      Stream::AccessMode eMode = Stream::AM_READ
    );

    /// Delete an existing stream
    void deleteStream(const string &sName);

  private:
    const ZipArchive   &m_ZipArchive;                 ///< Zip storage
    Zipex::ZipArchive  &m_ZipexArchive;               ///< Zip archive
    const ZipDirectory &m_Directory;                  ///< Zip directory
    string              m_sPath;                      ///< Storage path
};  

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::ZipArchive()                                     Constructor # //
// ############################################################################################# //
/** Creates a new instance of ZipArchive
*/
ZipArchive::ZipArchive(const string &sZIPFile) :
  m_ZipArchive(convertPath(sZIPFile, PT_NATIVE)),
  m_spDirectory(new ZipDirectory()) {

  Zipex::ZipArchive::FileEnumerator FileEnum = m_ZipArchive.enumFiles();
  while(FileEnum.next()) {
    const Zipex::ZippedFile &File = FileEnum.get();

    // Put the file into the directory structure 
    ZipDirectory *pDirectory = m_spDirectory.get();

    string sFilename = File.getFilename();

    for(;;) {
      string::size_type Pos = sFilename.find_first_of('/');
      if(Pos == static_cast<string::size_type>(string::npos))
        break;

      string sDirectory = sFilename.substr(0, Pos);
        
      // Insert the directory if it doesn't exist yet
      ZipDirectory::ZipDirectoryMap::iterator DirectoryIt =
        pDirectory->Directories.find(sDirectory);

      if(DirectoryIt == pDirectory->Directories.end())
        pDirectory = &pDirectory->Directories.insert(
          ZipDirectory::ZipDirectoryMap::value_type(sDirectory, ZipDirectory())
        ).first->second;
      else
        pDirectory = &DirectoryIt->second;

      sFilename = sFilename.substr(Pos + 1);
    }

    // Add the file to the current directory
    if(sFilename.length())
      pDirectory->Files.insert(
        ZipDirectory::ZipFileMap::value_type(sFilename, File.getSize())
      );
  }
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::~ZipArchive()                                     Destructor # //
// ############################################################################################# //
/** Destroys an instance of ZipArchive
*/
ZipArchive::~ZipArchive() {}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::getType()                                                    # //
// ############################################################################################# //
/** Retrieves the type of the specified child object. Returns AT_NONE 
    if the object doesn't exist

    @param  sName  Name of the child object to check
    @return The specified child object's type
*/
Archive::ItemType ZipArchive::getType(const string &sName) const {
  return m_spDirectory->getType(sName);
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::enumStorages()                                               # //
// ############################################################################################# //
/** Returns an enumerator over all sub storages of this storage

    @return The new enumerator
*/
shared_ptr<Archive::ArchiveEnumerator> ZipArchive::enumArchives() const {
  return shared_ptr<Archive::ArchiveEnumerator>(
    new ZipDirectory::ZipArchiveEnumerator(m_spDirectory->Directories)
  );
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::openArchive()                                                # //
// ############################################################################################# //
/** Opens an existing storage or creates a new one

    @param  sName         Name of the storage to open
    @param  bAllowCreate  Creation of new storage allowed ?
    @return The opened storage
*/
shared_ptr<Archive> ZipArchive::openArchive(const string &sName, bool bAllowCreate) {
  ZipDirectory::ZipDirectoryMap::iterator DirectoryIt = m_spDirectory->Directories.find(sName);
  if(DirectoryIt == m_spDirectory->Directories.end()) {
    if(bAllowCreate)
      throw NotSupportedException("Nuclex::Storage::ZipArchive::openArchive()",
                                  "Creating directories in zip files not supported");
    else
      throw CantOpenResourceException("Nuclex::Storage::ZipArchive::openArchive()",
                                      string("The specified file '") + sName + "' could not be found");
  }

  return shared_ptr<Archive>(
    new SubZipArchive(*this, m_ZipArchive, DirectoryIt->second, sName)
  );
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::deleteArchive()                                              # //
// ############################################################################################# //
/** Deletes an existing storage

    @param  sName  Name of the storage to delete
*/
void ZipArchive::deleteArchive(const string &sName) {
  throw NotSupportedException("Nuclex::Storage::ZipArchive::deleteArchive()",
                              "Deleting directories in zip files not supported");
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::enumStreams()                                                # //
// ############################################################################################# //
/** Returns an enumerator over all streams of this storage

    @return The new enumerator
*/
shared_ptr<Archive::StreamEnumerator> ZipArchive::enumStreams() const {
  return shared_ptr<Archive::StreamEnumerator>(
    new ZipDirectory::ZipStreamEnumerator(m_spDirectory->Files)
  );
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::openStream()                                                 # //
// ############################################################################################# //
/** Opens an existing stream or creates a new one

    @param  sName  Name of the stream to open
    @param  eMode  Access mode for the stream
    @return The opened stream
*/
shared_ptr<Stream> ZipArchive::openStream(const string &sName, Stream::AccessMode eMode) {
  ZipDirectory::ZipFileMap::iterator FileIt = m_spDirectory->Files.find(sName);
  if(FileIt == m_spDirectory->Files.end())
    throw CantOpenResourceException("Nuclex::Storage::ZipArchive::openStream()",
                                    string("The file '") + sName + "' could not be found");

  return shared_ptr<Stream>(new ZipStream(m_ZipArchive.getFile(sName), eMode));
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::deleteStream()                                               # //
// ############################################################################################# //
/** Deletes an existing stream

    @param  sName  Name of the stream to delete
*/
void ZipArchive::deleteStream(const string &sName) {
  throw NotSupportedException("Nuclex::Storage::ZipArchive::deleteStream()",
                              "Deleting files in zip files not supported");
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::ZipDirectory::getType()                                      # //
// ############################################################################################# //
/** Retrieves the type of a child object

    @param  sName  Name of the object whose type to retrieve
    @return The child object's type
*/
Archive::ItemType ZipArchive::ZipDirectory::getType(const string &sName) const {
  ZipFileMap::const_iterator FileIt = Files.find(sName);
  if(FileIt == Files.end()) {
    ZipDirectoryMap::const_iterator DirectoryIt = Directories.find(sName);
    if(DirectoryIt == Directories.end())
      return IT_NONE;

    return IT_ARCHIVE;
  }

  return IT_STREAM;  
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::SubZipArchive::SubZipArchive()                   Constructor # //
// ############################################################################################# //
/** Initializes an instance of SubZipArchive

    @param  ZipArchive  ZipArchive this SubZipArchive belongs to
    @param  UnzipFile   Unzip file handle
    @param  Directory   The directory of this sub storage
    @param  sPath       Path to the sub storage
*/
ZipArchive::SubZipArchive::SubZipArchive(const ZipArchive &ZipArchive, Zipex::ZipArchive &ZipexArchive,
                                         const ZipDirectory &Directory, const string &sPath) :
  m_ZipArchive(ZipArchive),
  m_ZipexArchive(ZipexArchive),
  m_Directory(Directory),
  m_sPath(convertPath(sPath)) {

  if(m_sPath[m_sPath.length() - 1] != '/')
    m_sPath += "/";
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::SubZipArchive::SubZipArchive()                    Destructor # //
// ############################################################################################# //
/** Destroys an instance of CSubZipArchive
*/
ZipArchive::SubZipArchive::~SubZipArchive() {
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::SubZipArchive::getType()                                     # //
// ############################################################################################# //
/** Get type of child object

    @param  sName  Name of the object whose type to retrieve
    @return The object's type
*/
Archive::ItemType ZipArchive::SubZipArchive::getType(const string &sName) const {
  return m_Directory.getType(sName);
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::SubZipArchive::enumArchives()                                # //
// ############################################################################################# //
/** Returns an enumerator over all sub storages of this storage

    @return The new enumerator
*/
shared_ptr<Archive::ArchiveEnumerator> ZipArchive::SubZipArchive::enumArchives() const {
  return shared_ptr<Archive::ArchiveEnumerator>(
    new ZipDirectory::ZipArchiveEnumerator(m_Directory.Directories)
  );
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::SubZipArchive::openArchive()                                 # //
// ############################################################################################# //
/** Opens an existing storage or creates a new one

    @param  sName         Name of the storage to open
    @param  bAllowCreate  Creation of new storage allowed ?
    @return The opened storage
*/
shared_ptr<Archive> ZipArchive::SubZipArchive::openArchive(const string &sName, bool bAllowCreate) {
  ZipDirectory::ZipDirectoryMap::const_iterator DirectoryIt = m_Directory.Directories.find(sName);
  if(DirectoryIt == m_Directory.Directories.end()) {
    if(bAllowCreate)
      throw NotSupportedException("Nuclex::Storage::ZipArchive::openArchive()",
                                  "Creating directories in zip files not supported");
    else
      throw CantOpenResourceException("Nuclex::Storage::ZipArchive::openArchive()",
                                      string("The specified file '") + sName + "' could not be found");
  }

  return shared_ptr<Archive>(
    new SubZipArchive(m_ZipArchive, m_ZipexArchive, DirectoryIt->second, m_sPath + sName)
  );
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::SubZipArchive::deleteArchive()                               # //
// ############################################################################################# //
/** Deletes an existing storage

    @param  sName  Name of the storage to delete
*/
void ZipArchive::SubZipArchive::deleteArchive(const string &sName) {
  throw NotSupportedException("Nuclex::Storage::ZipArchive::deleteArchive()",
                              "Deleting directories in zip files not supported");
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::SubZipArchive::enumStreams()                                 # //
// ############################################################################################# //
/** Returns an enumerator over all streams of this storage

    @return The new enumerator
*/
shared_ptr<Archive::StreamEnumerator> ZipArchive::SubZipArchive::enumStreams() const {
  return shared_ptr<Archive::StreamEnumerator>(
    new ZipDirectory::ZipStreamEnumerator(m_Directory.Files)
  );
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::SubZipArchive::openStream()                                  # //
// ############################################################################################# //
/** Opens an existing stream or creates a new one

    @param  sName  Name of the stream to open
    @param  eMode  Access mode for the stream
    @return The opened stream
*/
shared_ptr<Stream> ZipArchive::SubZipArchive::openStream(const string &sName, Stream::AccessMode eMode) {
  ZipDirectory::ZipFileMap::const_iterator FileIt = m_Directory.Files.find(sName);
  if(FileIt == m_Directory.Files.end())
    throw CantOpenResourceException("Nuclex::Storage::ZipArchive::SubZipArchive::openStream()",
                                    string("The file '") + sName + "' could not be found");

  return shared_ptr<Stream>(
    new ZipStream(m_ZipexArchive.getFile(m_sPath + sName), eMode)
  );
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchive::SubZipArchive::deleteStream()                                # //
// ############################################################################################# //
/** Deletes an existing stream

    @param  sName  Name of the stream to delete
*/
void ZipArchive::SubZipArchive::deleteStream(const string &sName) {
  throw NotSupportedException("Nuclex::Storage::ZipArchive::SubZipArchive::deleteStream()",
                              "Deleting files in zip files not supported");
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchiveFactory::canCreateArchive()                                    # //
// ############################################################################################# //
/** Check to see if this factory can create a storage on the
    specified source. The source string can be just about anything
    so the factory should carefully check whether it can create
    a storage on it.

    @param  sSource  Source specifier string
    @return True, if this factory is able to create a storage on
            the specified source
*/
bool ZipArchiveFactory::canCreateArchive(const string &sSource) const {
#ifdef NUCLEX_WIN32
  DWORD dwFileAttribs = ::GetFileAttributes(sSource.c_str());
  if((dwFileAttribs == -1) || (dwFileAttribs & FILE_ATTRIBUTE_DIRECTORY))
    return false;

  if(sSource.substr(sSource.length() - 4) == ".zip")
    return true;
  else
    return false;
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::ZipArchiveFactory::createArchive()                                       # //
// ############################################################################################# //
/** Creates a storage using the specigied source string, which is
    freely interpreted by the matching storage factory

    @param  sSource  Source specifier string
    @return The created storage
*/
shared_ptr<Archive> ZipArchiveFactory::createArchive(const string &sSource) {
  if(!canCreateArchive(sSource))
    throw CantCreateArchiveException("Nuclex::Storage::ZipArchiveFactory::createArchive()",
                                     string("Can't create Zip storage: '") + sSource + "' is not a Zip");

  return shared_ptr<Archive>(new ZipArchive(sSource));
}
