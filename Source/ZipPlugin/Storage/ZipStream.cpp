//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ZipStream.cpp - Zipped file implementation                                //
// ### # #      ###                                                                            //
// # ### #      ###  Accesses a compressed file and allows to read data                        //
// #  ## #   # ## ## from it through nuclex' stream interface                                  //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "ZipPlugin/Storage/ZipStream.h"

using namespace Nuclex;
using namespace Nuclex::Storage;

// ############################################################################################# //
// # Nuclex::Storage::ZipStream::ZipStream()                                       Constructor # //
// ############################################################################################# //
/** Initializes an instance CZipStream
*/
ZipStream::ZipStream(Zipex::ZippedFile &ZipexFile, AccessMode eMode) :
  m_ZipexFile(ZipexFile),
  m_eAccessMode(eMode),
  m_Location(0) {

  if(eMode != AM_READ)
    throw NotSupportedException("Nuclex::Storage::ZipStream::ZipStream()",
                                "Writing to Zip files not supported");
}

// ############################################################################################# //
// # Nuclex::Storage::ZipStream::getSize()                                                     # //
// ############################################################################################# //
/** Retrieves the size of the stream
    
    @return The stream's size
*/
size_t ZipStream::getSize() const {
  return m_ZipexFile.getSize();
}

// ############################################################################################# //
// # Nuclex::Storage::ZipStream::getName()                                                     # //
// ############################################################################################# //
/** Retrieves the name of the stream (for debugging purposes only)
    
    @return The stream's name
*/
string ZipStream::getName() const {
  return "compressed file"; //return m_sFilename;
}

// ############################################################################################# //
// # Nuclex::Storage::ZipStream::seekTo()                                                      # //
// ############################################################################################# //
/** Changes the current position in the stream
    
    @param  nPos  New seek position
*/
void ZipStream::seekTo(size_t nPos) {
  m_Location = nPos;
}

// ############################################################################################# //
// # Nuclex::Storage::ZipStream::getLocation()                                                 # //
// ############################################################################################# //
/** Retrieves the current position in the stream

    @return The current location
*/
size_t ZipStream::getLocation() const {
  return m_Location;
}

// ############################################################################################# //
// # Nuclex::Storage::ZipStream::readData()                                                    # //
// ############################################################################################# //
/** Read data from the stream

    @param  pDest   Destination address
    @param  nBytes  Number of bytes to read
    @return The number of bytes actually read
*/
size_t ZipStream::readData(void *pDest, size_t nBytes) {
  size_t AmountRead = m_ZipexFile.readDataAt(m_Location, pDest, nBytes);
  m_Location += AmountRead;

  return AmountRead;
}

// ############################################################################################# //
// # Nuclex::Storage::ZipStream::writeData()                                                   # //
// ############################################################################################# //
/** Write data to the stream

    @param  pDest   Dest address
    @param  nBytes  Number of bytes to write
    @return The number of bytes actually written
*/
size_t ZipStream::writeData(const void *pDest, size_t nBytes) {
  throw NotSupportedException("Nuclex::Storage::ZipStream::writeData()",
                              "Writing to Zip files not supported");
}

// ############################################################################################# //
// # Nuclex::Storage::ZipStream::getAccessMode()                                               # //
// ############################################################################################# //
/** Retrieves the stream's access mode

    @return The stream's access mode
*/
Stream::AccessMode ZipStream::getAccessMode() const {
  return m_eAccessMode;
}

// ############################################################################################# //
// # Nuclex::Storage::ZipStream::flush()                                                       # //
// ############################################################################################# //
/** Ensures the data sent to the stream is processed (eg. written
    to disk)
*/
void ZipStream::flush() {
  m_ZipexFile.flush();
}
