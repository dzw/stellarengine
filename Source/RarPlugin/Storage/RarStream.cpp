//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## RarStream.cpp - Rar compressed file implementation                        //
// ### # #      ###                                                                            //
// # ### #      ###  Accesses a compressed file and allows to read data                        //
// #  ## #   # ## ## from it through nuclex' stream interface                                  //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "RarPlugin/Storage/RarStream.h"
#include "ScopeGuard/ScopeGuard.h"

using namespace Nuclex;
using namespace Nuclex::Storage;

// ############################################################################################# //
// # Nuclex::Storage::RarStream::RarStream()                                       Constructor # //
// ############################################################################################# //
/** Initializes an instance RarStream
*/
RarStream::RarStream(const RarArchive &RarArchive, const string &sArchiveName,
                     unsigned long nIndex, const string &sFilename, size_t nSize,
                     AccessMode eMode) :
  m_nLoc(0),
  m_Exception(),
  m_eAccessMode(eMode) {

  if(eMode != AM_READ)
    throw NotSupportedException("Nuclex::Storage::RarStream::RarStream()",
                                "Writing to RAR files not supported");

  { Mutex::ScopedLock Lock(getRarMutex());
  
    m_Memory.resize(nSize);

    RAROpenArchiveDataEx OpenArchiveData;
    ::memset(&OpenArchiveData, 0, sizeof(OpenArchiveData));

    OpenArchiveData.ArcName = const_cast<char *>(sArchiveName.c_str());
    OpenArchiveData.OpenMode = RAR_OM_EXTRACT;

    HANDLE hArchive = ::RAROpenArchiveEx(&OpenArchiveData);
    RarCheck(
      "Nuclex::Storage::RarStream::RarStream()",
      string("RAROpenArchiveEx() on ") + sArchiveName,
      OpenArchiveData.OpenResult
    );
    { ScopeGuard CloseArchive = ::MakeGuard(::RARCloseArchive, hArchive);

      RARHeaderDataEx HeaderData;
      memset(&HeaderData, 0, sizeof(HeaderData));
      
      for(size_t nEntry = 0;;) {
        int nResult = ::RARReadHeaderEx(hArchive, &HeaderData);
        if(nResult != ERAR_END_ARCHIVE)
          RarCheck(
            "Nuclex::Storage::RarStream::RarStream()",
            string("RARReadHeaderEx() on ") + sArchiveName,
            nResult
          );
        else
          break;

        if(nEntry == nIndex)
          break;

        nResult = ::RARProcessFile(hArchive, RAR_SKIP, NULL, NULL);
        // Stream index invalid. Archive probably modified, please reopen archive
        RarCheck(
          "Nuclex::Storage::RarStream::RarStream()",
          string("RARProcessFile() on ") + sArchiveName,
          nResult
        );

        nEntry++;
      }

      ::RARSetCallback(hArchive, RARCallbackProc, reinterpret_cast<LONG>(this));
      int nResult = ::RARProcessFile(hArchive, RAR_TEST, NULL, NULL);
      if(nResult == ERAR_UNKNOWN)
        throw UnexpectedException("Nuclex::RarStream::RarStream()",
                                  m_Exception.what());

      RarCheck("Nuclex::RarStream::RarStream()", (string("RARProcessFile() on ") + HeaderData.FileName + " in " + sArchiveName).c_str(),
                nResult);
    } // CloseArchive

    m_nLoc = 0;
  }
}

// ############################################################################################# //
// # Nuclex::Storage::RarStream::~RarStream()                                       Destructor # //
// ############################################################################################# //
/** Destroys an instance of RarStream
*/
RarStream::~RarStream() {}

// ############################################################################################# //
// # Nuclex::Storage::RarStream::getName()                                                     # //
// ############################################################################################# //
/** Retrieves the name of the stream (for debugging purposes only)
    
    @return The stream's name
*/
string RarStream::getName() const {
  return "compressed file"; //return m_sFilename;
}

// ############################################################################################# //
// # Nuclex::Storage::RarStream::getSize()                                                     # //
// ############################################################################################# //
/** Retrieves the size of the stream
    
    @return The stream's size
*/
size_t RarStream::getSize() const {
  return m_Memory.size();
}

// ############################################################################################# //
// # Nuclex::Storage::RarStream::seekTo()                                                      # //
// ############################################################################################# //
/** Changes the current position in the stream
    
    @param  nPos  New seek position
*/
void RarStream::seekTo(size_t nPos) {
  m_nLoc = nPos;

  if(m_nLoc > m_Memory.size())
    m_nLoc = m_Memory.size();
}

// ############################################################################################# //
// # Nuclex::Storage::RarStream::getLocation()                                                 # //
// ############################################################################################# //
/** Retrieves the current position in the stream

    @return The current location
*/
size_t RarStream::getLocation() const {
  return m_nLoc;
}

// ############################################################################################# //
// # Nuclex::Storage::RarStream::readData()                                                    # //
// ############################################################################################# //
/** Read data from the stream

    @param  pDest   Destination address
    @param  nBytes  Number of bytes to read
    @return The number of bytes actually read
*/
size_t RarStream::readData(void *pDest, size_t nBytes) {
  unsigned long nBytesRead = m_Memory.size() - m_nLoc;
  if(nBytes < nBytesRead)
    nBytesRead = nBytes;

  ::memcpy(pDest, &m_Memory[m_nLoc], nBytesRead);
  m_nLoc += nBytesRead;

  return nBytesRead;
}

// ############################################################################################# //
// # Nuclex::Storage::RarStream::writeData()                                                   # //
// ############################################################################################# //
/** Write data to the stream

    @param  pDest   Dest address
    @param  nBytes  Number of bytes to write
    @return The number of bytes actually written
*/
size_t RarStream::writeData(const void *pDest, size_t nBytes) {
  throw NotSupportedException("Nuclex::Storage::RarStream::writeData()",
                              "Writing to Rar files not supported");
}

// ############################################################################################# //
// # Nuclex::Storage::RarStream::getAccessMode()                                               # //
// ############################################################################################# //
/** Retrieves the stream's access mode

    @return The stream's access mode
*/
Stream::AccessMode RarStream::getAccessMode() const {
  return m_eAccessMode;
}

// ############################################################################################# //
// # Nuclex::Storage::RarStream::flush()                                                       # //
// ############################################################################################# //
/** Ensures the data sent to the stream is processed (eg. written
    to disc)
*/
void RarStream::flush() {}

// ############################################################################################# //
// # Nuclex::Storage::RarStream::RARCallbackProc()                                             # //
// ############################################################################################# //
/** Callback procedure for RAR

    @param  nMessage   Callback reason
    @param  nUserData  User data
    @param  nParam1    The adress of the extracted data chunk
    @param  nParam2    The length of the extracted data chunk
    @return 0 to continue, -1 to abort extraction
*/
int PASCAL RarStream::RARCallbackProc(UINT nMessage, LONG nUserData,
                                      LONG nParam1, LONG nParam2) {
  RarStream *pThis = reinterpret_cast<RarStream *>(nUserData);

  if(nMessage != UCM_PROCESSDATA) {
    pThis->m_Exception = UnexpectedException("Nuclex::Storage::RarStream::RARCallbackProc()",
                                             "Unexpected message received. Only single-volume unpassworded archives supported");
    return -1;
  }

  memcpy(&pThis->m_Memory[pThis->m_nLoc], reinterpret_cast<void *>(nParam1), nParam2);
  pThis->m_nLoc += nParam2;
  
  return 1;
}
