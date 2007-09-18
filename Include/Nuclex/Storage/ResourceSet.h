//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## ResourceSet.h - A set of resources                                        //
// ### # #      ###                                                                            //
// # ### #      ###  A set of resources loaded from a serialized resource list                 //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_RESOURCESET_H
#define NUCLEX_STORAGE_RESOURCESET_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Storage/Persistable.h"
#include <deque>

namespace Nuclex {
  namespace Storage { class StorageServer; class Archive; }
  namespace Video { class VideoServer; class Image; }
  namespace Text { class TextServer; class Font; }
}

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::ResourceSet                                                               //
//  //
/// Structured serializer base class
/** Base class for structured data serializers
*/
class ResourceSet :
  public Persistable {
  public:
    /// Constructor
    NUCLEX_API ResourceSet(
      const string &sName,
      const shared_ptr<Storage::StorageServer> &spStorageServer,
      const shared_ptr<Video::VideoServer> &spVideoServer,
      const shared_ptr<Text::TextServer> &spTextServer
    );

    /// Destructor
    /** Destroys an instance of Serializer
    */
    NUCLEX_API virtual ~ResourceSet();

  //
  // ResourceSet implementation
  //
  public:
    /// Add archive to pool
    NUCLEX_API void addArchive(const string &sName, const string &sSource);
    /// Add bitmap to pool
    NUCLEX_API void addImage(const string &sName, const string &sSource);
    /// Add font to pool
    NUCLEX_API void addFont(const string &sName, const string &sSource);

    /// Remove archive from pool
    NUCLEX_API void removeArchive(const string &sName);
    /// Remove bitmap from pool
    NUCLEX_API void removeImage(const string &sName);
    /// Remove font from pool
    NUCLEX_API void removeFont(const string &sName);
  
    /// Get pooled archive
    NUCLEX_API const shared_ptr<Storage::Archive> &getArchive(const string &sName) const;
    /// Get pooled bitmap
    NUCLEX_API const shared_ptr<Video::Image> &getImage(const string &sName) const;
    /// Get pooled font
    NUCLEX_API const shared_ptr<Text::Font> &getFont(const string &sName) const;

  //
  // Persistable implementation
  //
  public:
    /// Load object from serializer
    NUCLEX_API void load(const shared_ptr<Serializer> &spSerializer);
    /// Save object to serializer
    NUCLEX_API void save(const shared_ptr<Serializer> &spSerializer) const;
    
  private:
    typedef std::deque<std::pair<string, string> > StringPairDeque;
    
    ResourceSet(const ResourceSet &);
    virtual void operator =(const ResourceSet &) { throw -1; }

    string m_sName;
    
    StringPairDeque m_Archives;
    StringPairDeque m_Images;
    StringPairDeque m_Fonts;
    
    shared_ptr<Storage::StorageServer> m_spStorageServer; ///< The storage server
    shared_ptr<Video::VideoServer> m_spVideoServer; ///< The image server
    shared_ptr<Text::TextServer> m_spTextServer; ///< The text server
};

}} // namespace Nuclex::Storage

#endif // NUCLEX_STORAGE_RESOURCESET_H
