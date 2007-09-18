//  //
// ##### ####  #   #                         -= Zipex Library =-                               //
//    ## #   # ## ## Exception.h - Zipex exceptions                                            //
//   ##  ####   ###                                                                            //
//  ##   #      ###  A set of exceptions for common errors which can occur while               //
// ##    #     ## ## working with zip archives                                                 //
// ##### #     #   # R1                              (C)2003-2004 Markus Ewald -> License.txt  //
//  //
#ifndef ZIPEX_EXCEPTION_H
#define ZIPEX_EXCEPTION_H

#include "zipex/config.h"

#include <stdexcept>
#include <string>

namespace Zipex {

//  //
//  Zipex::ArchiveCorruptedError                                                               //
//  //
/// Archive has been corrupted
/** Will be thrown if zipex is unable to read a zip archive because of
    invalid or missing data. Part of the archive may have been
    overwritten or damaged due to a transmission error.
*/
class ArchiveCorruptedError :
  public std::runtime_error {
  public:
    explicit ArchiveCorruptedError(const std::string &sMessage) :
      runtime_error(sMessage) {}
      
    virtual ~ArchiveCorruptedError() throw() {}
};  

//  //
//  Zipex::FileOpenError                                                                       //
//  //
/// The file could not be opened
/** This error is thrown when a file could not be opened because it
    either doesn't exist or because it is in use by another application.

    If you try to retrieve an nonexisting zipped file from an archive,
    this error is also thrown.
*/
class FileOpenError :
  public std::runtime_error {
  public:
    explicit FileOpenError(const std::string &sMessage) :
      runtime_error(sMessage) {}
      
    virtual ~FileOpenError() throw() {}
};  

//  //
//  Zipex::UnsupportedFormatError                                                              //
//  //
/// The data format is not supported
/** Thrown if a zip archive uses a compression method which is not
    supported by zlib or if an archive has an external, fixed
    dictionary, which also isn't supported by zipex.

    Unlike the ArchiveCorruptedError, the archive may be perfectly valid
    but makes use of features not supported by zipex.
*/
class UnsupportedFormatError :
  public std::runtime_error {
  public:
    explicit UnsupportedFormatError(const std::string &sMessage) :
      runtime_error(sMessage) {}

    virtual ~UnsupportedFormatError() throw() {}
};  

//  //
//  Zipex::InternalError                                                                       //
//  //
/// An internal error occured
/** This exception should not happen and you should not try to handle
    it generally. It will be thrown when an internal error within zipex
    or Zlib occurs and is likely the result of a programming error.
*/
class InternalError :
  public std::runtime_error {
  public:
    explicit InternalError(const std::string &sMessage) :
      runtime_error(sMessage) {}

    virtual ~InternalError() throw() {}
};  

} // namespace Zipex

#endif // ZIPEX_EXCEPTION_H
