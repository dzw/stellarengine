//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Stream.h - Data source & sink                                             //
// ### # #      ###                                                                            //
// # ### #      ###  Binary data stream for input and output                                   //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_STREAM_H
#define NUCLEX_STORAGE_STREAM_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/Support/Cacheable.h"

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::Stream                                                                    //
//  //
/// Nuclex stream base class
/** A stream is both a data source and a data sink, meaning you can
    retrieve data from it or store data into it. Where the data will
    end up depends on the stream class, it could provide direct access
    to a file on disk or it could store the stream in memory amongst
    other possibilities.
*/
class Stream :
  public Cacheable  {
  public:
    /// Wrong access mode
    NUCLEX_DECLAREEXCEPTION(FailedException, WrongAccessModeException);

    /// Stream access modes
    enum AccessMode {
      AM_NONE = 0,                                    ///< No access (query only)
      AM_READ,                                        ///< Read access
      AM_WRITE,                                       ///< Write access
      AM_APPEND                                       ///< Append write access
    };

    /// Destructor
    /** Destroys an instance of Stream
    */
    NUCLEX_API virtual ~Stream() {}

  //
  // Stream implementation
  //
  public:
    /// Templated read method
    template<typename VarType> inline void read(VarType &Value) {
      readData(reinterpret_cast<void *>(&Value), sizeof(VarType));
    }

    /// Templated read method
    template<typename VarType> inline VarType read() {
      VarType Var;
      read(Var);
      return Var;
    }

    /// Templated write method 
    template<typename VarType> inline void write(const VarType &Value) {
      writeData(reinterpret_cast<const void *>(&Value), sizeof(VarType));
    }

    /// Get stream name
    /** Retrieves the name of the stream (for debugging purposes only)
        
        @return The stream's name
    */
    NUCLEX_API virtual string getName() const = 0;
    
    /// Get stream size
    /** Retrieves the size of the stream
        
        @return The stream's size
    */
    NUCLEX_API virtual size_t getSize() const = 0;

    /// Seek to position
    /** Changes the current position in the stream
        
        @param  Pos  New seek position
    */
    NUCLEX_API virtual void seekTo(size_t Pos) = 0;

    /// Current location
    /** Retrieves the current position in the stream

        @return The current location
    */
    NUCLEX_API virtual size_t getLocation() const = 0;

    /// Read data
    /** Read data from the stream

        @param  pDest   Destination address
        @param  nBytes  Number of bytes to read
        @return The number of bytes actually read
    */
    NUCLEX_API virtual size_t readData(void *pDest, size_t nBytes) = 0;

    /// Write data
    /** Write data to the stream

        @param  pSource  Source address
        @param  nBytes   Number of bytes to write
        @return The number of bytes actually written
    */
    NUCLEX_API virtual size_t writeData(const void *pSource, size_t nBytes) = 0;

    /// Retrieve access mode
    /** Retrieves the stream's access mode

        @return The stream's access mode
    */
    NUCLEX_API virtual AccessMode getAccessMode() const = 0;

    /// Flush stream cache
    /** Ensures the data sent to the stream is processed (eg. written
        to disc)
    */
    NUCLEX_API virtual void flush() = 0;
};

}} // namespace Nuclex::Storage

#endif // NUCLEX_STORAGE_STREAM_H
