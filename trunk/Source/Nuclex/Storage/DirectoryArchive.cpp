//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## DirectoryArchive.cpp - Directory archive                                  //
// ### # #      ###                                                                            //
// # ### #      ###  A nuclex archive which wraps a directory in the underlying platform's     //
// #  ## #   # ## ## file system                                                               //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Storage/DirectoryArchive.h"
#include "Nuclex/Storage/FileStream.h"
#include "Nuclex/Platform.h"

#ifdef NUCLEX_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif NUCLEX_WIN32

using namespace Nuclex;
using namespace Nuclex::Storage;

namespace {

//  //
//  DirectoryArchiveEnumerator                                                                 //
//  //
/// Archive enumerator
/** Enumerates a list of archives
*/
class DirectoryArchiveEnumerator :
  public Archive::ArchiveEnumerator {
  public:
    /// Constructor
    DirectoryArchiveEnumerator(const DirectoryArchive &DirectoryArchive);
    /// Destructor
    NUCLEX_API ~DirectoryArchiveEnumerator();

  //
  // ArchiveEnumerator implementation
  //
  public:
    /// Advance to next entry
    bool next();

    /// Get current stream information
    /** Returns informations about the storage being enumerated.

        @return Informations about the current storage
    */
    const ArchiveInfo &get() const { return m_ArchiveInfo; }

  private:
    const DirectoryArchive &m_DirectoryArchive;       ///< Owning storage
    ArchiveInfo             m_ArchiveInfo;            ///< Archive informations
#ifdef NUCLEX_WIN32
    HANDLE                  m_hSearchHandle;          ///< Win32 search handle
    WIN32_FIND_DATA         m_FindFileData;           ///< Win32 file find data
#else
    #error Not implemented yet
#endif
};

//  //
//  DirectoryStreamEnumerator                                                                  //
//  //
/// Stream enumerator
/** Enumerates a list of Streams
*/
class DirectoryStreamEnumerator :
  public Archive::StreamEnumerator {
  public:
    /// Constructor
    DirectoryStreamEnumerator(const DirectoryArchive &DirectoryArchive);
    /// Destructor
    ~DirectoryStreamEnumerator();

  //
  // StreamEnumerator implementation
  //
  public:
    /// Advance to next entry
    bool next();

    /// Get current stream information
    const StreamInfo &get() const { return m_StreamInfo; }

  private:
    const DirectoryArchive &m_DirectoryArchive;       ///< Owning storage
    StreamInfo              m_StreamInfo;             ///< Stream informations
#ifdef NUCLEX_WIN32
    HANDLE                  m_hSearchHandle;          ///< Win32 search handle
    WIN32_FIND_DATA         m_FindFileData;           ///< Win32 file find data
#else
    #error Not implemented yet
#endif
};

} // namespace

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchive::DirectoryArchive()                         Constructor # //
// ############################################################################################# //
/** Creates a new instance of DirectoryArchive.

    @param  sPath  Base path for stream accesses
*/
DirectoryArchive::DirectoryArchive(const string &sPath) :
  m_sPath(sPath) {
  char pszPath[MAX_PATH];

#ifdef NUCLEX_WIN32
  if(sPath.length()) {
    for(string::size_type Pos = 0; Pos < m_sPath.length(); Pos++)
      if(m_sPath[Pos] == '/')
        m_sPath[Pos] = '\\';

    DWORD dwResult = ::GetFullPathName(m_sPath.c_str(), sizeof(pszPath), pszPath, NULL);
    if((dwResult != 0) && (dwResult < sizeof(pszPath)))
      m_sPath = pszPath;
   
    DWORD dwFileAttribs = ::GetFileAttributes(m_sPath.c_str());
    if(dwFileAttribs == -1)
      throw CantOpenResourceException(
        "Nuclex::Storage::DirectoryArchive::openArchive()",
        string("The specified directory '") + m_sPath + "' does not exist"
      );
    
    else if(!(dwFileAttribs & FILE_ATTRIBUTE_DIRECTORY))
      throw ResourceException(
        "Nuclex::Storage::DirectoryArchive::openArchive()",
        string("The specified path, '") + m_sPath + "', is not a directory"
      );

  } else {
    char *pszFilenameInPath = NULL;

    // Locate the application's directory
    ::GetModuleFileName(NULL, pszPath, sizeof(pszPath));
    ::GetFullPathName(pszPath, sizeof(pszPath), pszPath, &pszFilenameInPath);

    m_sPath = std::string(pszPath, pszFilenameInPath - pszPath);
  }

  if(m_sPath[m_sPath.length() - 1] != '\\')
    m_sPath += "\\";
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchive::~DirectoryArchive()                         Destructor # //
// ############################################################################################# //
/** Destroys an instance of CDirArchive
*/
DirectoryArchive::~DirectoryArchive() {}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchive::getPath()                                              # //
// ############################################################################################# //
/** Retrieves the base directory used for stream accesses

    @return Current base directory
*/
const string &DirectoryArchive::getPath() const {
  return m_sPath;
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchive::getType()                                              # //
// ############################################################################################# //
/** Retrieves the type of the specified child object. Returns AT_NONE 
    if the object doesn't exist

    @param  sName  Name of the child object to check
    @return The specified child object's type
*/
Archive::ItemType DirectoryArchive::getType(const string &sName) const {
#ifdef NUCLEX_WIN32
  DWORD dwFileAttribs = ::GetFileAttributes((m_sPath + sName).c_str());
  if(dwFileAttribs == -1)
    return Archive::IT_NONE;
  else if(dwFileAttribs & FILE_ATTRIBUTE_DIRECTORY)
    return Archive::IT_ARCHIVE;
  else
    return Archive::IT_STREAM;
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchive::enumArchives()                                         # //
// ############################################################################################# //
/** Returns an enumerator over all sub storages of this storage

    @return The new enumerator
*/
shared_ptr<Archive::ArchiveEnumerator> DirectoryArchive::enumArchives() const {
  return shared_ptr<Archive::ArchiveEnumerator>(new DirectoryArchiveEnumerator(*this));
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchive::openArchive()                                          # //
// ############################################################################################# //
/** Opens an existing storage or creates a new one

    @param  sName         Name of the storage to open
    @param  bAllowCreate  Creation of new storage allowed ?
    @return The opened storage
*/
shared_ptr<Archive> DirectoryArchive::openArchive(const string &sName, bool bAllowCreate) {
#ifdef NUCLEX_WIN32
  // Does the specified path already exist ?
  DWORD dwFileAttribs = ::GetFileAttributes((m_sPath + sName).c_str());
  if(dwFileAttribs == -1) {
    if(!bAllowCreate)
      throw CantOpenResourceException("Nuclex::Storage::DirectoryArchive::openArchive()",
                                      string("The specified directory '") + sName + "' does not exist");

    if(!::CreateDirectory((m_sPath + sName).c_str(), NULL))
      throw UnexpectedException("Nuclex::Storage::DirectoryArchive::openArchive()",
                                string("Could not create directory '") + sName + "'");

  } else if(!(dwFileAttribs & FILE_ATTRIBUTE_DIRECTORY)) {
    throw ResourceException("Nuclex::Storage::DirectoryArchive::openArchive()",
                            string("Cannot create directory '") + sName + "': a file with the same name exists");
  }
#else
  #error Not implemented yet
#endif

  return shared_ptr<Archive>(new DirectoryArchive(m_sPath + sName));
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchive::deleteArchive()                                        # //
// ############################################################################################# //
/** Deletes an existing storage

    @param  sName  Name of the storage to delete
*/
void DirectoryArchive::deleteArchive(const string &sName) {
  if(!::RemoveDirectory((m_sPath + sName).c_str()))
    throw ResourceException("Nuclex::Storage::DirectoryArchive::deleteArchive()",
                            string("Could not delete directory '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchive::enumStreams()                                          # //
// ############################################################################################# //
/** Returns an enumerator over all streams of this storage

    @return The new enumerator
*/
shared_ptr<Archive::StreamEnumerator> DirectoryArchive::enumStreams() const {
  return shared_ptr<Archive::StreamEnumerator>(new DirectoryStreamEnumerator(*this));
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchive::openStream()                                           # //
// ############################################################################################# //
/** Opens an existing stream or creates a new one

    @param  sName  Name of the stream to open
    @param  eMode  Access mode for the stream
    @return The opened stream
*/
shared_ptr<Stream> DirectoryArchive::openStream(const string &sName, Stream::AccessMode eMode) {
  return shared_ptr<Stream>(new FileStream(m_sPath + sName, eMode));
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchive::deleteStream()                                         # //
// ############################################################################################# //
/** Deletes an existing stream

    @param  sName  Name of the stream to delete
*/
void DirectoryArchive::deleteStream(const string &sName) {
  if(!::DeleteFile(sName.c_str()))
    throw ResourceException("Nuclex::Storage::DirectoryArchive::deleteStream()",
                            string("Error deleting file '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchiveEnumerator::DirectoryArchiveEnumerator()     Constructor # //
// ############################################################################################# //
/** Initializes an instance of DirectoryArchiveEnumerator

    @param  sPath  Path whose directories to enumerate
*/
DirectoryArchiveEnumerator::DirectoryArchiveEnumerator(const DirectoryArchive &DirectoryArchive) :
#ifdef NUCLEX_WIN32
  m_hSearchHandle(INVALID_HANDLE_VALUE),
#endif
  m_DirectoryArchive(DirectoryArchive) {
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchiveEnumerator::~DirectoryArchiveEnumerator()     Destructor # //
// ############################################################################################# //
/** Destroys an instance of DirectoryArchiveEnumerator
*/
DirectoryArchiveEnumerator::~DirectoryArchiveEnumerator() {
#ifdef NUCLEX_WIN32
  if(m_hSearchHandle != INVALID_HANDLE_VALUE)
    ::FindClose(m_hSearchHandle);
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchiveEnumerator::next()                                       # //
// ############################################################################################# //
/** Returns informations for the current storage and advances
    to the next one. If there are no more storages available,
    NULL is returned

    @return Informations about the current storage
*/
bool DirectoryArchiveEnumerator::next() {
#ifdef NUCLEX_WIN32
  if(m_hSearchHandle == INVALID_HANDLE_VALUE) {

    m_hSearchHandle = ::FindFirstFile(convertPath(m_DirectoryArchive.getPath() + "*").c_str(), &m_FindFileData);
    if(m_hSearchHandle == INVALID_HANDLE_VALUE)
      return false;

  } else {

    int nResult = ::FindNextFile(m_hSearchHandle, &m_FindFileData);
    if(!nResult || nResult == ERROR_NO_MORE_FILES) {
      ::FindClose(m_hSearchHandle);
      m_hSearchHandle = INVALID_HANDLE_VALUE;
    }
  
  }

  // Skip all but directories
  while(!(m_FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ||
          string(m_FindFileData.cFileName) == "." ||
          string(m_FindFileData.cFileName) == "..") {
    int nResult = ::FindNextFile(m_hSearchHandle, &m_FindFileData);
    if(!nResult || nResult == ERROR_NO_MORE_FILES) {
      ::FindClose(m_hSearchHandle);
      m_hSearchHandle = INVALID_HANDLE_VALUE;

      return false;
    }
  }

  m_ArchiveInfo.sName = m_FindFileData.cFileName;

  return true;

#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryStreamEnumerator::DirectoryStreamEnumerator()       Constructor # //
// ############################################################################################# //
/** Creates an instance of DirectoryStreamEnumerator

    @param  sPath  Path of which to enumerate the files
*/
DirectoryStreamEnumerator::DirectoryStreamEnumerator(const DirectoryArchive &DirectoryArchive) :
  m_DirectoryArchive(DirectoryArchive),
#ifdef NUCLEX_WIN32
  m_hSearchHandle(INVALID_HANDLE_VALUE) {

  string sMask = m_DirectoryArchive.getPath() + "*";
  for(unsigned int nChar = 0; nChar < sMask.length(); nChar++)
    if(sMask[nChar] == '/')
      sMask[nChar] = '\\';
  
  m_hSearchHandle = ::FindFirstFile(sMask.c_str(), &m_FindFileData);
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryStreamEnumerator::~DirectoryStreamEnumerator()       Destructor # //
// ############################################################################################# //
/** Destroys an instance of DirectoryStreamEnumerator
*/
DirectoryStreamEnumerator::~DirectoryStreamEnumerator() {
#ifdef NUCLEX_WIN32
  if(m_hSearchHandle != INVALID_HANDLE_VALUE)
    ::FindClose(m_hSearchHandle);
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryStreamEnumerator::next()                                        # //
// ############################################################################################# //
/** Returns informations for the current Stream and advances
    to the next one. If there are no more Streams available,
    NULL is returned

    @return Informations about the current Stream
*/
bool DirectoryStreamEnumerator::next() {
#ifdef NUCLEX_WIN32
  if(m_hSearchHandle == INVALID_HANDLE_VALUE) {

    m_hSearchHandle = ::FindFirstFile(convertPath(m_DirectoryArchive.getPath() + "*").c_str(), &m_FindFileData);
    if(m_hSearchHandle == INVALID_HANDLE_VALUE)
      return false;

  } else {

    int nResult = ::FindNextFile(m_hSearchHandle, &m_FindFileData);
    if(!nResult || nResult == ERROR_NO_MORE_FILES) {
      ::FindClose(m_hSearchHandle);
      m_hSearchHandle = INVALID_HANDLE_VALUE;
      
      return false;
    }
  
  }

  // Skip all but directories
  while(m_FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
    int nResult = ::FindNextFile(m_hSearchHandle, &m_FindFileData);
    if(!nResult || nResult == ERROR_NO_MORE_FILES) {
      ::FindClose(m_hSearchHandle);
      m_hSearchHandle = INVALID_HANDLE_VALUE;

      return false;
    }
  }

  m_StreamInfo.sName = m_FindFileData.cFileName;
  m_StreamInfo.nSize = m_FindFileData.nFileSizeLow;

  return true;

#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchiveFactory::canCreateArchive()                              # //
// ############################################################################################# //
/** Check to see if this factory can create a storage on the
    specified source. The source string can be just about anything
    so the factory should carefully check whether it can create
    a storage on it.

    @param  sSource  Source specifier string
    @return True, if this factory is able to create a storage on
            the specified source
*/
bool DirectoryArchiveFactory::canCreateArchive(const string &sSource) const {
#ifdef NUCLEX_WIN32
  DWORD dwFileAttribs = ::GetFileAttributes(sSource.c_str());
  if((dwFileAttribs != -1) && (dwFileAttribs & FILE_ATTRIBUTE_DIRECTORY))
    return true;
  else
    return false;
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::DirectoryArchiveFactory::createArchive()                                 # //
// ############################################################################################# //
/** Creates a storage using the specigied source string, which is
    freely interpreted by the matching storage factory

    @param  sSource  Source specifier string
    @return The created storage
*/
shared_ptr<Archive> DirectoryArchiveFactory::createArchive(const string &sSource) {
  if(!canCreateArchive(sSource))
    throw CantCreateArchiveException("Nuclex::DirectoryArchiveFactory::createArchive()",
                                     string("Can't create directory storage: '") + sSource + "' is not a directory");

  return shared_ptr<Archive>(new DirectoryArchive(sSource));
}
