//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Storage.h - Storage system                                                //
// ### # #      ###                                                                            //
// # ### #      ###  Everything related to data storage and access                             //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_STORAGE_H
#define NUCLEX_STORAGE_STORAGE_H

namespace Nuclex {

/// Storage system
/** All accesses to stored data in Nuclex are performed through streams. A stream is
    roughly equivalent to a simplified version of the C++ iostream interface, because
    it doesn't carry all the formatting and conversion features with it. The most
    common type of a stream is the FileStream, which does nothing more than reading
    data out of a file.

    Nuclex also provides an abstraction for the containers of streams, which are
    labelled as Archives. If you think of a stream as a file, you can think of an
    archive as a directory. Archives provide a good way to keep several streams
    together - for example, all graphics files in a simple game. The storage server
    allows you to 'mount' archives to make them globally accessable, thus, allowing
    you to change storage media at will without the need to rewrite any code.
    
    If an object in your application needs to store its state in a file or somewhere
    else, as is the case when you wish to implement save game functionality, you can
    use a Serializer. A Serializer formats data and can aid you in avoiding versioning
    troubles, adjusting byte ordering and supporting multiple file formats where needed.
    
    Plugins can enable the StorageServer to support new archive types (like zip or rar
    archives), provide access to new storage media (like an ftp server) and mount
    their own archives.
*/
namespace Storage {

// Nothing yet

} // namespace Storage

} // namespace Nuclex

#endif // NUCLEX_STORAGE_STORAGE_H