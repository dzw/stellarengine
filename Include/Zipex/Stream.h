//  //
// ##### ####  #   #                         -= Zipex Library =-                               //
//    ## #   # ## ## Stream.h - Stream base class                                              //
//   ##  ####   ###                                                                            //
//  ##   #      ###  An efficient binary stream through which data can be read and written     //
// ##    #     ## ## without knowing the actual means of storage                               //
// ##### #     #   # R1                              (C)2003-2004 Markus Ewald -> License.txt  //
//  //
#ifndef ZIPEX_STREAM_H
#define ZIPEX_STREAM_H

#include "zipex/config.h"

namespace Zipex {

//  //
//  Zipex::Stream                                                                              //
//  //
/// Zipex stream base
/** Instead of using iostreams, Zipex introduces yet another stream base,
    in order to (optionally) provide thread safety. Having a file pointer
    within the stream, like the iostreams do, moves the responsibility of
    thread synchronization into the components which use the stream.

    Althought threads are out of the scope of this library, it was
    implemented with complete support for multithreading in mind. Instead
    of locking the stream, seeking to the desired read position and then
    performing the read, this stream provides methods which perform both
    steps in a single operation, readDataAt() and writeDataAt().

    If you need to access zipped files from within multiple threads, simply
    build a stream which either uses thread-safe file access routines or
    enters a critical section for the duration of the seek and read
    operations. You control whether the reads are performed simultaneously
    or sequentially.
*/
class Stream {
  public:
    /// Destructor
    virtual ~Stream() {}

    /// Templated read method (for convenience)
    template<typename VarType>
    ZIPEX_API inline void read(VarType &Value) {
      readData(reinterpret_cast<void *>(&Value), sizeof(VarType));
    }
    /// Templated write method (for convenience)
    template<typename VarType>
    ZIPEX_API inline void write(const VarType &Value) {
      writeData(reinterpret_cast<const void *>(&Value), sizeof(VarType));
    }

    /// Templated read method (for convenience)
    template<typename VarType>
    ZIPEX_API inline void readAt(size_t nPosition, VarType &Value) {
      readDataAt(nPosition, reinterpret_cast<void *>(&Value), sizeof(VarType));
    }
    /// Templated write method (for convenience)
    template<typename VarType>
    ZIPEX_API inline void writeAt(size_t nPosition, const VarType &Value) {
      writeDataAt(nPosition, reinterpret_cast<const void *>(&Value), sizeof(VarType));
    }

  //
  // Stream implementation
  //
  public:
    /// Get stream size
    /** Returns the length of the stream in bytes

        @return The length of the stream in bytes
    */
    ZIPEX_API virtual size_t getSize() const = 0;
    /// Seek to position
    /** Seeks to the specified position, relative to the file's beginning

        @param  nPosition  Position to seek to
    */
    ZIPEX_API virtual void seekTo(size_t nPosition) = 0;
    /// Get current location
    /** Retrieves the current file-pointer location

        @return The current file-pointer location
    */
    ZIPEX_API virtual size_t getLocation() const = 0;
    /// Read data into buffer
    /** Reads data from the stream into a user-provided memory buffer

        @param  pBuffer  Adress at which to store the data to be read
        @param  nLength  Number of bytes to read
        @return The number of bytes actually read
    */
    ZIPEX_API virtual size_t readData(void *pBuffer, size_t nLength) = 0;
    /// Write data from buffer
    /** Writes data from a user-provided memory buffer into the stream

        @param  pBuffer  Adress from which to take the data to be written
        @param  nLength  Number of bytes to write
    */
    ZIPEX_API virtual void writeData(const void *pBuffer, size_t nLength) = 0;

    /// Read from specified location
    /** Reads data from a specified location

        @param  nPosition  Position, relative to the beginning of the stream, at which to read
        @param  pBuffer  Adress at which to store the data to be read
        @param  nLength  Number of bytes to read
        @return The number of bytes actually read
    */
    ZIPEX_API virtual size_t readDataAt(size_t nPosition, void *pBuffer, size_t nLength) {
      seekTo(nPosition);
      return readData(pBuffer, nLength);
    }

    /// Read from specified location
    /** Writes data from a user-provided memory buffer into the stream

        @param  nPosition  Position, relative to the beginning of the stream, at which to write
        @param  pBuffer  Adress from which to take the data to be written
        @param  nLength  Number of bytes to write
    */
    ZIPEX_API virtual void writeDataAt(size_t nPosition, const void *pBuffer, size_t nLength) {
      seekTo(nPosition);
      writeData(pBuffer, nLength);
    }
};

} // namespace Zipex

#endif // ZIPEX_STREAM_H
