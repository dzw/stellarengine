//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Archive.h - Hierarchical stream group                                     //
// ### # #      ###                                                                            //
// # ### #      ###  Groups a set of archives and streams                                      //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_ARCHIVE_H
#define NUCLEX_STORAGE_ARCHIVE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Storage/Stream.h"

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::Archive                                                                   //
//  //
/// Stream archive
/** An archive is a hierarchical grouping of other archives and streams.
    It is roughly equivalent to a folder in a file system and can contain
    other archives (folders) as well as streams (files).
*/
class Archive {
  public:
    NUCLEX_DECLAREEXCEPTION(InvalidArgumentException, StreamNotFoundException);
    NUCLEX_DECLAREEXCEPTION(FailedException, StreamAlreadyOpenException);

    /// Archive types
    enum ItemType {
      IT_NONE = 0,                                    ///< Doesn't exist
      IT_ARCHIVE,                                     ///< Archive child
      IT_STREAM,                                      ///< Stream child
    };

    /// Archive enumerator
    class ArchiveEnumerator;
    /// Stream enumerator
    class StreamEnumerator;

    /// Destructor
    /** Destroys an instance of Archive
    */
    NUCLEX_API inline virtual ~Archive() {}

  //
  // Archive implementation
  //
  public:
    /// Get child type
    /** Retrieves the type of the specified child object. Returns IT_NONE 
        if the object doesn't exist

        @param  sName  Name of the child object to check
        @return The specified child object's type
    */
    NUCLEX_API virtual ItemType getType(const string &sName) const = 0;

    /// Get Archive enumerator
    /** Returns an enumerator over all sub Archives of this Archive

        @return The new enumerator
    */
    NUCLEX_API virtual shared_ptr<ArchiveEnumerator> enumArchives() const = 0;

    /// Open a Archive
    /** Opens an existing Archive or creates a new one

        @param  sName         Name of the Archive to open
        @param  bAllowCreate  Creation of new Archive allowed ?
        @return The opened Archive
    */
    NUCLEX_API virtual shared_ptr<Archive> openArchive(
      const string &sName,
      bool bAllowCreate = false
    ) = 0;

    /// Delete an existing Archive
    /** Deletes an existing Archive

        @param  sName  Name of the Archive to delete
    */
    NUCLEX_API virtual void deleteArchive(const string &sName) = 0;

    /// Get stream enumerator
    /** Returns an enumerator over all streams of this Archive

        @return The new enumerator
    */
    NUCLEX_API virtual shared_ptr<StreamEnumerator> enumStreams() const = 0;

    /// Open a stream
    /** Opens an existing stream or creates a new one

        @param  sName  Name of the stream to open
        @param  eMode  Access mode for the stream
        @return The opened stream
    */
    NUCLEX_API virtual shared_ptr<Stream> openStream(
      const string &sName,
      Stream::AccessMode eMode = Stream::AM_READ
    ) = 0;

    /// Delete an existing stream
    /** Deletes an existing stream

        @param  sName  Name of the stream to delete
    */
    NUCLEX_API virtual void deleteStream(const string &sName) = 0;
};

//  //
//  Nuclex::Storage::Archive::ArchiveEnumerator                          //
//  //
/** Enumerates a list of Archives
*/
class Archive::ArchiveEnumerator {
  public:
    /// Archive informations
    /** Used by ArchiveEnumerator to return informations about
        existing Archives
    */
    struct ArchiveInfo {
      string sName;                                  ///< Archive name
    };

    /// Destructor
    /** Destroys an instance of ArchiveEnumerator
    */
    virtual ~ArchiveEnumerator() {}

  //
  // ArchiveEnumerator implementation
  //
  public:
    /// Advance to next entry
    /** Advances to the next Archive. If no more Archives are available,
        false is returned. Also required to be the first method called,
        in order to correctly handle enumerators to empty lists.

        @return True if the next Archive was reached
    */
    NUCLEX_API virtual bool next() = 0;

    /// Get current stream information
    /** Returns informations about the Archive being enumerated.

        @return Informations about the current Archive
    */
    NUCLEX_API virtual const ArchiveInfo &get() const = 0;
};

//  //
//  Nuclex::Storage::Archive::StreamEnumerator                                                 //
//  //
/** Enumerates a list of streams
*/
class Archive::StreamEnumerator {
  public:
    /// Stream informations
    /** Used by StreamEnumerator to return informations about
        existing streams
    */
    struct StreamInfo {
      string sName;                                   ///< Stream name
      size_t nSize;                                   ///< Stream size
    };

    /// Destructor
    /** Destroys an instance of StreamEnumerator
    */
    NUCLEX_API virtual ~StreamEnumerator() {}

  //
  // StreamEnumerator implementation
  //
  public:
    /// Advance to next entry
    /** Advances to the next stream. If no more streams are available,
        false is returned. Also required to be the first method called,
        in order to correctly handle enumerators to empty lists.

        @return True if the next stream was reached
    */
    NUCLEX_API virtual bool next() = 0;

    /// Get current stream information
    /** Returns informations about the stream being enumerated.

        @return Informations about the current stream
    */
    NUCLEX_API virtual const StreamInfo &get() const = 0;
};

}} // namespace Nuclex::Storage

#endif // NUCLEX_STORAGE_ARCHIVE_H
