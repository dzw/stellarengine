//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## FileStream.cpp - File stream                                              //
// ### # #      ###                                                                            //
// # ### #      ###  A nuclex stream which wraps a file in the underlying platform's           //
// #  ## #   # ## ## file system                                                               //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Storage/FileStream.h"

using namespace Nuclex;
using namespace Nuclex::Storage;

// ############################################################################################# //
// # Nuclex::Storage::FileStream::FileStream()                                     Constructor # //
// ############################################################################################# //
/** Initializes an instance of FileStream

    @param  sFile  File to open
    @param  eMode  Access mode for the file
*/
FileStream::FileStream(const string &sFile, AccessMode eMode) :
  m_eAccessMode(eMode),
  m_sFilename(sFile) {

#ifdef NUCLEX_WIN32
  m_hFile = ::CreateFile(
    sFile.c_str(),
    (eMode == AM_READ) ? GENERIC_READ : GENERIC_WRITE,
    0,
    NULL,
    (eMode == AM_WRITE) ? CREATE_ALWAYS : OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL
  );
  if(m_hFile == INVALID_HANDLE_VALUE)
    throw CantOpenResourceException("Nuclex::Storage::FileStream::FileStream()",
                                    string("Can't open file: '") + sFile + "'");
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::FileStream::~FileStream()                                     Destructor # //
// ############################################################################################# //
/** Destroys an instance of CFileStream
*/
FileStream::~FileStream() {
#ifdef NUCLEX_WIN32
  if(m_hFile)
    ::CloseHandle(m_hFile);
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::FileStream::getName()                                                    # //
// ############################################################################################# //
/** Retrieves the name of the stream (for debugging purposes only)
    
    @return The stream's name
*/
string FileStream::getName() const {
  return m_sFilename;
}

// ############################################################################################# //
// # Nuclex::Storage::FileStream::getSize()                                                    # //
// ############################################################################################# //
/** Retrieves the size of the stream
    
    @return The stream's size
*/
size_t FileStream::getSize() const {
#ifdef NUCLEX_WIN32
  return ::GetFileSize(m_hFile, NULL);
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::FileStream::seekTo()                                                     # //
// ############################################################################################# //
/** Changes the current position in the stream
    
    @param  nPos  New seek position
*/
void FileStream::seekTo(size_t nPos) {
#ifdef NUCLEX_WIN32
  ::SetFilePointer(m_hFile, nPos, NULL, FILE_BEGIN);
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::FileStream::getLocation()                                                # //
// ############################################################################################# //
/** Retrieves the current position in the stream

    @return The current location
*/
size_t FileStream::getLocation() const {
#ifdef NUCLEX_WIN32
  return ::SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::FileStream::readData()                                                   # //
// ############################################################################################# //
/** Read data from the stream

    @param  pDest   Destination address
    @param  nBytes  Number of bytes to read
    @return The number of bytes actually read
    @bug Doesn't check the ReadFile() call for errors
*/
size_t FileStream::readData(void *pDest, size_t nBytes) {
#ifdef NUCLEX_WIN32
  DWORD dwBytesRead = 0;
  ::ReadFile(m_hFile, pDest, nBytes, &dwBytesRead, NULL);
  return dwBytesRead;
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::FileStream::writeData()                                                  # //
// ############################################################################################# //
/** Write data to the stream

    @param  pDest   Dest address
    @param  nBytes  Number of bytes to write
    @return The number of bytes actually written
    @bug Doesn't check the WriteFile() call for errors
*/
size_t FileStream::writeData(const void *pDest, size_t nBytes) {
#ifdef NUCLEX_WIN32
  DWORD dwBytesWritten = 0;

  ::WriteFile(m_hFile, pDest, nBytes, &dwBytesWritten, NULL);

  return dwBytesWritten;
#else
  #error Not implemented yet
#endif
}

// ############################################################################################# //
// # Nuclex::Storage::FileStream::getAccessMode()                                              # //
// ############################################################################################# //
/** Retrieves the stream's access mode

    @return The stream's access mode
*/
Stream::AccessMode FileStream::getAccessMode() const {
  return m_eAccessMode;
}

// ############################################################################################# //
// # Nuclex::Storage::FileStream::flush()                                                      # //
// ############################################################################################# //
/** Ensures the data sent to the stream is processed (eg. written
    to disk)
*/
void FileStream::flush() {
#ifdef NUCLEX_WIN32
  ::FlushFileBuffers(m_hFile);
#else
  #error Not implemented yet
#endif
}
