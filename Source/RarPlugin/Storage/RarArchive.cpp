//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## RarArchive.cpp - Rar archive implementation                               //
// ### # #      ###                                                                            //
// # ### #      ###  Represents a rar archive from which streams can be opened                 //
// #  ## #   # ## ## to read data out of compressed files                                      //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "RarPlugin/Storage/RarArchive.h"
#include "RarPlugin/Storage/RarStream.h"
#include "ScopeGuard/ScopeGuard.h"
#include <map>

using namespace Nuclex;
using namespace Nuclex::Storage;

//  //
//  Nuclex::Storage::RarArchive::RarDirectory                                                  //
//  //
/** A directory in the RAR file.
    Used to increase searching speed and because the unrar functions
    aren't thread-safe (they only manage a single directory index)
*/
struct RarArchive::RarDirectory {
  public:
    typedef std::pair<unsigned long, unsigned long> FileData;
    /// Map of subdirectories
    typedef std::map<string, RarDirectory> RarDirectoryMap;
    /// Set of strings
    typedef std::map<string, FileData> RarFileMap;
    /// Archive (directory) enumerator
    class RarArchiveEnumerator;
    /// Stream (file) enumerator
    class RarStreamEnumerator;

    /// Constructor
    RarDirectory() {}
    /// Copy constructor
    RarDirectory(const RarDirectory &Directory) :
      Directories(Directory.Directories),
      Files(Directory.Files) {}

  //
  // RarDirectory implementation
  //
  public:
    /// Get type of child object
    Archive::ItemType getType(const string &sName) const;

    RarDirectoryMap Directories;                      ///< Subdirectories
    RarFileMap      Files;                            ///< Files in the directory
};

//  //
//  Nuclex::Storage::RarArchive::RarDirectory::RarArchiveEnumerator                            //
//  //
/// Archive enumerator
/** Enumerates a list of storages
*/
class RarArchive::RarDirectory::RarArchiveEnumerator :
  public Archive::ArchiveEnumerator {
  public:
    /// Constructor
    RarArchiveEnumerator(const RarDirectory::RarDirectoryMap &Archives) :
      m_Archives(Archives),
      m_ArchiveIt(Archives.end()),
      m_ArchiveEnd(Archives.end()) {}
    /// Destructor
    virtual ~RarArchiveEnumerator() {}

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
    ArchiveInfo                                    m_ArchiveInfo; ///< Archive informations
    const RarDirectory::RarDirectoryMap           &m_Archives; // Archives
    RarDirectory::RarDirectoryMap::const_iterator  m_ArchiveIt; ///< Current iterator
    RarDirectory::RarDirectoryMap::const_iterator  m_ArchiveEnd; ///< End iterator
};

//  //
//  Nuclex::Storage::RarArchive::RarDirectory::RarStreamEnumerator                             //
//  //
/// Stream enumerator
/** Enumerates a list of streams
*/
class RarArchive::RarDirectory::RarStreamEnumerator :
  public Archive::StreamEnumerator {
  public:
    /// Constructor
    RarStreamEnumerator(const RarDirectory::RarFileMap &Streams) :
      m_Streams(Streams),
      m_StreamIt(Streams.end()),
      m_StreamEnd(Streams.end()) {}
    /// Destructor
    virtual ~RarStreamEnumerator() {}

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
        m_StreamInfo.nSize = m_StreamIt->second.first;
      }

      return (m_StreamIt != m_StreamEnd);  
    }

    /// Get current stream information
    const StreamInfo &get() const { return m_StreamInfo; }

  private:
    StreamInfo                                m_StreamInfo; ///< Stream informations
    const RarDirectory::RarFileMap           &m_Streams; ///< Streams
    RarDirectory::RarFileMap::const_iterator  m_StreamIt; ///< Current iterator
    RarDirectory::RarFileMap::const_iterator  m_StreamEnd; ///< End iterator
};

//  //
//  Nuclex::Storage::RarArchive::SubRarArchive                                                 //
//  //
/// Rar data storage class
/** This storage implementation accesses a .rar file. Rars store
    multiple files in a compressed archive. NCXRar is able to directly
    read from and write to these these files without requiring to
    extract them.
*/
class RarArchive::SubRarArchive :
  public Archive {
  public:
    /// Constructor
    SubRarArchive(const RarArchive &RarArchive, const string &sArchiveName,
                  const RarDirectory &Directory, const string &sPath);
    /// Destructor
    virtual ~SubRarArchive();

  //
  // Archive implementation
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
    const RarArchive   &m_RarArchive;                 ///< Rar storage
    string              m_sArchiveName;               ///< Unrar archive
    const RarDirectory &m_Directory;                  ///< Rar directory
    string              m_sPath;                      ///< Archive path
};  

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::RarArchive()                                     Constructor # // 
// ############################################################################################# //
/** Creates a new instance of RarArchive

    @param  sRARFile  Name of the RAR archive to open
*/
RarArchive::RarArchive(const string &sRARFile) :
  m_sArchiveName(),
  m_spDirectory(new RarDirectory()) {

  RAROpenArchiveDataEx OpenArchiveData;
  ::memset(&OpenArchiveData, 0, sizeof(OpenArchiveData));
  OpenArchiveData.ArcName = const_cast<char *>(sRARFile.c_str());
  OpenArchiveData.OpenMode = RAR_OM_LIST;

  { Mutex::ScopedLock Lock(getRarMutex());

    // Open the archive with RAR
    HANDLE hArchive = ::RAROpenArchiveEx(&OpenArchiveData);
    RarCheck(
      "Nuclex::Storage::RarArchive::RarArchive()",
      string("RAROpenArchiveEx() on ") + sRARFile,
      OpenArchiveData.OpenResult
    );
    
    { ScopeGuard CloseArchive = ::MakeGuard(::RARCloseArchive, hArchive);

      RARHeaderDataEx HeaderData;
      memset(&HeaderData, 0, sizeof(HeaderData));

      for(size_t nIndex = 0;;) {
        int nResult = ::RARReadHeaderEx(hArchive, &HeaderData);
        if(nResult == ERAR_END_ARCHIVE)
          break;
        else
          RarCheck(
            "Nuclex::Storage::RarArchive::RarArchive()",
            string("RARReadHeaderEx() on ") + sRARFile,
            nResult
          );

        // Put the file into the directory structure 
        RarDirectory      *pDirectory = m_spDirectory.get();
        string             sFilename = HeaderData.FileName;
        string::size_type  Pos;
        while(Pos = sFilename.find_first_of('\\'), Pos != static_cast<string::size_type>(string::npos)) {
          string sDirectory = sFilename.substr(0, Pos);
        
          // Insert the directory if it doesn't exist yet
          RarDirectory::RarDirectoryMap::iterator DirectoryIt = pDirectory->Directories.find(sDirectory);
          if(DirectoryIt == pDirectory->Directories.end())
            pDirectory = &pDirectory->Directories.insert(
              RarDirectory::RarDirectoryMap::value_type(sDirectory, RarDirectory())
            ).first->second;
          else
            pDirectory = &DirectoryIt->second;

          sFilename = sFilename.substr(Pos + 1);
        }

        // Add the file to the current directory
        if(!(HeaderData.FileAttr & FILE_ATTRIBUTE_DIRECTORY))
          pDirectory->Files.insert(RarDirectory::RarFileMap::value_type(
            sFilename,
            RarDirectory::FileData(HeaderData.UnpSize, nIndex))
          );

        nResult = ::RARProcessFile(hArchive, RAR_SKIP, NULL, NULL);
        if(nResult != ERAR_END_ARCHIVE)
          RarCheck(
            "Nuclex::Storage::RarArchive::RarArchive()",
            string("RARProcessFile() on ") + sRARFile,
            nResult
          );
        else
          break;

        ++nIndex;
      }

    } // CloseArchive

    m_sArchiveName = sRARFile;
  }
 }

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::~RarArchive()                                     Destructor # //
// ############################################################################################# //
/** Destroys an instance of RarArchive
*/
RarArchive::~RarArchive() {}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::getType()                                                    # //
// ############################################################################################# //
/** Retrieves the type of the specified child object. Returns AT_NONE 
    if the object doesn't exist

    @param  sName  Name of the child object to check
    @return The specified child object's type
*/
Archive::ItemType RarArchive::getType(const string &sName) const {
  return m_spDirectory->getType(sName);
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::enumArchives()                                               # //
// ############################################################################################# //
/** Returns an enumerator over all sub storages of this storage

    @return The new enumerator
*/
shared_ptr<Archive::ArchiveEnumerator> RarArchive::enumArchives() const {
  return shared_ptr<Archive::ArchiveEnumerator>(
    new RarDirectory::RarArchiveEnumerator(m_spDirectory->Directories)
  );
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::openArchive()                                                # //
// ############################################################################################# //
/** Opens an existing storage or creates a new one

    @param  sName         Name of the storage to open
    @param  bAllowCreate  Creation of new storage allowed ?
    @return The opened storage
*/
shared_ptr<Archive> RarArchive::openArchive(const string &sName, bool bAllowCreate) {
  RarDirectory::RarDirectoryMap::iterator DirectoryIt = m_spDirectory->Directories.find(sName);
  if(DirectoryIt == m_spDirectory->Directories.end()) {
    if(bAllowCreate)
      throw NotSupportedException("Nuclex::Storage::RarArchive::openArchive()",
                                  "Creating directories in rar files not supported");
    else
      throw CantOpenResourceException("Nuclex::Storage::RarArchive::openArchive()",
                                      string("The specified file '") + sName + "' could not be found");
  }

  return shared_ptr<Archive>(new SubRarArchive(*this, m_sArchiveName, DirectoryIt->second, sName));
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::deleteArchive()                                              # //
// ############################################################################################# //
/** Deletes an existing storage

    @param  sName  Name of the storage to delete
*/
void RarArchive::deleteArchive(const string &sName) {
  throw NotSupportedException("Nuclex::Storage::RarArchive::deleteArchive()",
                              "Deleting directories in rar files not supported");
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::enumStreams()                                                # //
// ############################################################################################# //
/** Returns an enumerator over all streams of this storage

    @return The new enumerator
*/
shared_ptr<Archive::StreamEnumerator> RarArchive::enumStreams() const {
  return shared_ptr<Archive::StreamEnumerator>(
    new RarDirectory::RarStreamEnumerator(m_spDirectory->Files)
  );
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::openStream()                                                 # //
// ############################################################################################# //
/** Opens an existing stream or creates a new one

    @param  sName  Name of the stream to open
    @param  eMode  Access mode for the stream
    @return The opened stream
*/
shared_ptr<Stream> RarArchive::openStream(const string &sName, Stream::AccessMode eMode) {
  RarDirectory::RarFileMap::iterator FileIt = m_spDirectory->Files.find(sName);
  if(FileIt == m_spDirectory->Files.end())
    throw CantOpenResourceException("Nuclex::Storage::RarArchive::openStream()",
                                    string("The file '") + sName + "' could not be found");

  return shared_ptr<Stream>(new RarStream(*this, m_sArchiveName, FileIt->second.second, sName, FileIt->second.first, eMode));
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::deleteStream()                                               # //
// ############################################################################################# //
/** Deletes an existing stream

    @param  sName  Name of the stream to delete
*/
void RarArchive::deleteStream(const string &sName) {
  throw NotSupportedException("Nuclex::Storage::RarArchive::deleteStream()",
                              "Deleting files in rar files not supported");
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::RarDirectory::getType()                                      # //
// ############################################################################################# //
/** Retrieves the type of a child object

    @param  sName  Name of the object whose type to retrieve
    @return The child object's type
*/
Archive::ItemType RarArchive::RarDirectory::getType(const string &sName) const {
  RarFileMap::const_iterator FileIt = Files.find(sName);
  if(FileIt == Files.end()) {
    RarDirectoryMap::const_iterator DirectoryIt = Directories.find(sName);
    if(DirectoryIt == Directories.end())
      return IT_NONE;

    return IT_ARCHIVE;
  }

  return IT_STREAM;  
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::SubRarArchive::SubRarArchive()                   Constructor # //
// ############################################################################################# //
/** Initializes an instance of SubRarArchive

    @param  pRarArchive  RarArchive this SubRarArchive belongs to
    @param  hArchive     Rar archive handle
    @param  Directory    The directory of this sub storage
    @param  sPath        Path to the sub storage
*/
RarArchive::SubRarArchive::SubRarArchive(const RarArchive &Archive, const string &sArchiveName,
                                         const RarDirectory &Directory, const string &sPath) :
  m_RarArchive(Archive),
  m_sArchiveName(sArchiveName),
  m_Directory(Directory),
  m_sPath(sPath) {

  if(m_sPath[m_sPath.length() - 1] != '/')
    m_sPath += "/";
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::SubRarArchive::~SubRarArchive()                   Destructor # //
// ############################################################################################# //
/** Destroys an instance of SubRarArchive
*/
RarArchive::SubRarArchive::~SubRarArchive() {}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::SubRarArchive::getType()                                     # //
// ############################################################################################# //
/** Get type of child object

    @param  sName  Name of the object whose type to retrieve
    @return The object's type
*/
Archive::ItemType RarArchive::SubRarArchive::getType(const string &sName) const {
  return m_Directory.getType(sName);
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::SubRarArchive::enumArchives()                                # //
// ############################################################################################# //
/** Returns an enumerator over all sub storages of this storage

    @return The new enumerator
*/
shared_ptr<Archive::ArchiveEnumerator> RarArchive::SubRarArchive::enumArchives() const {
  return shared_ptr<Archive::ArchiveEnumerator>(
    new RarDirectory::RarArchiveEnumerator(m_Directory.Directories)
  );
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::SubRarArchive::openArchive()                                 # //
// ############################################################################################# //
/** Opens an existing storage or creates a new one

    @param  sName         Name of the storage to open
    @param  bAllowCreate  Creation of new storage allowed ?
    @return The opened storage
*/
shared_ptr<Archive> RarArchive::SubRarArchive::openArchive(const string &sName, bool bAllowCreate) {
  RarDirectory::RarDirectoryMap::const_iterator DirectoryIt = m_Directory.Directories.find(sName);
  if(DirectoryIt == m_Directory.Directories.end()) {
    if(bAllowCreate)
      throw NotSupportedException("Nuclex::Storage::RarArchive::openArchive()",
                                  "Creating directories in rar files not supported");
    else
      throw CantOpenResourceException("Nuclex::Storage::RarArchive::openArchive()",
                                      string("The specified file '") + sName + "' could not be found");
  }

  return shared_ptr<Archive>(new SubRarArchive(m_RarArchive, m_sArchiveName, DirectoryIt->second, m_sPath + sName));
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::SubRarArchive::deleteArchive()                               # //
// ############################################################################################# //
/** Deletes an existing storage

    @param  sName  Name of the storage to delete
*/
void RarArchive::SubRarArchive::deleteArchive(const string &sName) {
  throw NotSupportedException("Nuclex::Storage::RarArchive::deleteArchive()",
                              "Deleting directories in rar files not supported");
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::SubRarArchive::enumStreams()                                 # //
// ############################################################################################# //
/** Returns an enumerator over all streams of this storage

    @return The new enumerator
*/
shared_ptr<Archive::StreamEnumerator> RarArchive::SubRarArchive::enumStreams() const {
  return shared_ptr<Archive::StreamEnumerator>(
    new RarDirectory::RarStreamEnumerator(m_Directory.Files)
  );
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::SubRarArchive::openStream()                                  # //
// ############################################################################################# //
/** Opens an existing stream or creates a new one

    @param  sName  Name of the stream to open
    @param  eMode  Access mode for the stream
    @return The opened stream
*/
shared_ptr<Stream> RarArchive::SubRarArchive::openStream(const string &sName, Stream::AccessMode eMode) {
  RarDirectory::RarFileMap::const_iterator FileIt = m_Directory.Files.find(sName);
  if(FileIt == m_Directory.Files.end())
    throw CantOpenResourceException("Nuclex::Storage::RarArchive::SubRarArchive::openStream()",
                                    string("The file '") + sName + "' could not be found");

  return shared_ptr<Stream>(new RarStream(m_RarArchive, m_sArchiveName, FileIt->second.second, m_sPath + sName, FileIt->second.first, eMode));
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchive::SubRarArchive::deleteStream()                                # //
// ############################################################################################# //
/** Deletes an existing stream

    @param  sName  Name of the stream to delete
*/
void RarArchive::SubRarArchive::deleteStream(const string &sName) {
  throw NotSupportedException("Nuclex::Storage::RarArchive::SubRarArchive::deleteStream()",
                              "Deleting files in rar files not supported");
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchiveFactory::canCreateArchive()                                    # //
// ############################################################################################# //
/** Check to see if this factory can create a storage on the
    specified source. The source string can be just about anything
    so the factory should carefully check whether it can create
    a storage on it.

    @param  sSource  Source specifier string
    @return True, if this factory is able to create a storage on
            the specified source
*/
bool RarArchiveFactory::canCreateArchive(const string &sSource) const {
#ifdef NUCLEX_WIN32
  DWORD dwFileAttribs = ::GetFileAttributes(sSource.c_str());
  if((dwFileAttribs == -1) || (dwFileAttribs & FILE_ATTRIBUTE_DIRECTORY))
    return false;

  if(sSource.substr(sSource.length() - 4) == ".rar")
    return true;
  else
    return false;
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::RarArchiveFactory::createArchive()                                       # //
// ############################################################################################# //
/** Creates a storage using the specigied source string, which is
    freely interpreted by the matching storage factory

    @param  sSource  Source specifier string
    @return The created storage
*/
shared_ptr<Archive> RarArchiveFactory::createArchive(const string &sSource) {
  if(!canCreateArchive(sSource))
    throw CantCreateArchiveException("Nuclex::Storage::RarArchiveFactory::createArchive()",
                                     string("Can't create RAR storage: '") + sSource + "' is not a RAR archive");

  return shared_ptr<Archive>(new RarArchive(sSource));
}
