//  //
// ##### ####  #   #                         -= Zipex Library =-                               //
//    ## #   # ## ## ZipArchive.h - Zip archive                                                //
//   ##  ####   ###                                                                            //
//  ##   #      ###  Represents a zip archive and manages its files                            //
// ##    #     ## ##                                                                           //
// ##### #     #   # R1                              (C)2003-2004 Markus Ewald -> License.txt  //
//  //
#ifndef ZIPEX_ZIPARCHIVE_H
#define ZIPEX_ZIPARCHIVE_H

#include "zipex/config.h"
#include "zipex/zippedfile.h"

#include <string>
#include <map>

namespace Zipex {

//  //
//  Zipex::Archive                                                                             //
//  //
/// Zip archive reader class
/** Provides access to zip archive in a file or stream. The archive's
    directory is read at construction time, so it is advisable to
    construct the ZipArchive only once and keep it alive as long as
    you need access to the zip in order to avoid unneccessary performance
    issues.

    Thread safety can be added by providing a stream with a thread-safe
    readDataAt() method. See the documentation of ZPX::Stream for
    further informations.
*/
class ZipArchive {
  public:
    /// Zipped file enumerator
    class FileEnumerator;

    /// Constructor from file
    ZIPEX_API ZipArchive(const std::string &sFilename, const std::string &sPassword = "");
    /// Constructor from stream
    ZIPEX_API ZipArchive(Stream *pStream, const std::string &sPassword = "");
    /// Destructor
    ZIPEX_API ~ZipArchive();

  //
  // ZipArchive implementation
  //
  public:
    /// Get number of variables in file
    ZIPEX_API inline size_t getNumFiles() const { return m_Files.size(); }

    /// Get file
    ZIPEX_API ZippedFile &getFile(const std::string &sFilename);
    /// Get file
    ZIPEX_API const ZippedFile &getFile(const std::string &sFilename) const;

    /// Get file enumerator
    ZIPEX_API FileEnumerator enumFiles();

  private:
    /// Reads the directory from the opened zip archive
    void readZipDirectory();

    /// All files within the zip archive
    typedef std::map<std::string, ZippedFile *> FileMap;

    FileMap      m_Files;                             ///< All zipped files
    std::string  m_sPassword;                         ///< Zip password
    Stream      *m_pStream;                           ///< Stream containing the archive
    bool         m_bDeleteStream;                     ///< Whether to delete the stream
};

//  //
//  Zipex::ZipArchive::FileEnumerator                                                          //
//  //
/** Enumerates a list of files
*/
class ZipArchive::FileEnumerator {
  public:
    /// Constructor
    ZIPEX_API FileEnumerator(FileMap &Files) :
      m_Files(Files),
      m_FileIt(Files.end()),
      m_FileEnd(Files.end()) {}

    /// Copy constructor
    ZIPEX_API FileEnumerator(const FileEnumerator &Other) :
      m_Files(Other.m_Files),
      m_FileIt(Other.m_FileIt),
      m_FileEnd(Other.m_FileEnd) {}

  //
  // FileEnumerator implementation
  //
  public:
    /// Get current variable
    ZIPEX_API ZippedFile &get();

    /// Jump to next variable
    ZIPEX_API bool next();

  private:
    FileMap           &m_Files;                       ///< Variable vector
    FileMap::iterator  m_FileIt;                      ///< Current file
    FileMap::iterator  m_FileEnd;                     ///< End of the list
};

} // namespace Zipex

#endif // ZIPEX_ZIPARCHIVE_H

