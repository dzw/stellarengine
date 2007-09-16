//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## XMLSerializer.cpp - XML object serializer                                 //
// ### # #      ###                                                                            //
// # ### #      ###  A serializer which formats serialized objects into XML tags               //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Storage/ResourceSet.h"
#include "Nuclex/Storage/StorageServer.h"
#include "Nuclex/Video/VideoServer.h"
#include "Nuclex/Text/TextServer.h"
#include "Nuclex/Text/Font.h"

using namespace Nuclex;
using namespace Nuclex::Storage;

// ############################################################################################# //
// # Nuclex::Storage::ResourceSet::ResourceSet()                                   Constructor # //
// ############################################################################################# //
ResourceSet::ResourceSet(
  const string &sName,
  const shared_ptr<Storage::StorageServer> &spStorageServer,
  const shared_ptr<Video::VideoServer> &spVideoServer,
  const shared_ptr<Text::TextServer> &spTextServer
) :
  m_sName(sName),
  m_spStorageServer(spStorageServer),
  m_spVideoServer(spVideoServer),
  m_spTextServer(spTextServer) {}

// ############################################################################################# //
// # Nuclex::Storage::ResourceSet::~ResourceSet()                                   Destructor # //
// ############################################################################################# //
ResourceSet::~ResourceSet() {}

// ############################################################################################# //
// # Nuclex::Storage::ResourceSet::load()                                                      # //
// ############################################################################################# //
void ResourceSet::load(const shared_ptr<Serializer> &spSerializer) {
  // Mount archives
  shared_ptr<Storage::Serializer::ScopeEnumerator> spArchiveEnum =
    spSerializer->enumScopes("archive");

  while(spArchiveEnum->next()) {
    m_spStorageServer->addArchive(
      spArchiveEnum->get().second->get<string>("_name"),
      m_spStorageServer->openArchive(
        spArchiveEnum->get().second->get<string>("_source")
      )
    );
  }

  // Load bitmaps
  shared_ptr<Storage::Serializer::ScopeEnumerator> spImageEnum =
    spSerializer->enumScopes("bitmap");
  
  while(spImageEnum->next()) {
    m_spVideoServer->addImage(
      spImageEnum->get().second->get<string>("_name"),
      m_spVideoServer->loadImage(
        m_spStorageServer->openStream(spImageEnum->get().second->get<string>("_stream"))
      )
    );
  }

  // Load fonts
  shared_ptr<Storage::Serializer::ScopeEnumerator> spFontEnum =
    spSerializer->enumScopes("font");

  while(spFontEnum->next()) {
    m_spTextServer->addFont(
      spFontEnum->get().second->get<string>("_name"),
      m_spTextServer->loadFont(
        m_spStorageServer->openStream(spFontEnum->get().second->get<string>("_stream")),
        "",
        spFontEnum->get().second->get<size_t>("_size")
      )
    );
  }
}

// ############################################################################################# //
// # Nuclex::Storage::ResourceSet::save()                                                      # //
// ############################################################################################# //
void ResourceSet::save(const shared_ptr<Serializer> &spSerializer) const {
  for(StringPairDeque::const_iterator ArchiveIt = m_Archives.begin();
      ArchiveIt != m_Archives.end();
      ++ArchiveIt) {
     shared_ptr<Serializer> spScope = spSerializer->createScope("archive");
     spScope->set<string>("_name", ArchiveIt->first);
     spScope->set<string>("_source", ArchiveIt->second);
  }
  
  for(StringPairDeque::const_iterator ImageIt = m_Images.begin();
      ImageIt != m_Images.end();
      ++ImageIt) {
     shared_ptr<Serializer> spScope = spSerializer->createScope("Image");
     spScope->set<string>("_name", ImageIt->first);
     spScope->set<string>("_source", ImageIt->second);
  }

  for(StringPairDeque::const_iterator FontIt = m_Fonts.begin();
      FontIt != m_Fonts.end();
      ++FontIt) {
     shared_ptr<Serializer> spScope = spSerializer->createScope("Font");
     spScope->set<string>("_name", FontIt->first);
     spScope->set<string>("_source", FontIt->second);
     spScope->set<size_t>("_size", getFont(FontIt->first)->getHeight());
  }
}

// ############################################################################################# //
// # Nuclex::Storage::ResourceSet::addArchive()                                                # //
// ############################################################################################# //
void ResourceSet::addArchive(const string &sName, const string &sSource) {
  shared_ptr<Storage::Archive> spArchive = m_spStorageServer->openArchive(sSource);
  m_spStorageServer->addArchive(sName, spArchive);
  m_Archives.push_back(std::pair<string, string>(sName, sSource));
}

// ############################################################################################# //
// # Nuclex::Storage::ResourceSet::addImage()                                                  # //
// ############################################################################################# //
void ResourceSet::addImage(const string &sName, const string &sSource) {
  shared_ptr<Storage::Stream> spStream = m_spStorageServer->openStream(sSource);
  shared_ptr<Video::Image> spImage = m_spVideoServer->loadImage(spStream);
  m_spVideoServer->addImage(sName, spImage);
  m_Images.push_back(std::pair<string, string>(sName, sSource));
}

// ############################################################################################# //
// # Nuclex::Storage::ResourceSet::addFont()                                                   # //
// ############################################################################################# //
void ResourceSet::addFont(const string &sName, const string &sSource) {
  shared_ptr<Storage::Stream> spStream = m_spStorageServer->openStream(sSource);
  shared_ptr<Text::Font> spFont = m_spTextServer->loadFont(spStream);
  m_spTextServer->addFont(sName, spFont);
  m_Fonts.push_back(std::pair<string, string>(sName, sSource));
}

// ############################################################################################# //
// # Nuclex::Storage::ResourceSet::removeArchive()                                             # //
// ############################################################################################# //
void ResourceSet::removeArchive(const string &sName) {

}

// ############################################################################################# //
// # Nuclex::Storage::ResourceSet::removeImage()                                               # //
// ############################################################################################# //
void ResourceSet::removeImage(const string &sName) {

}

// ############################################################################################# //
// # Nuclex::Storage::ResourceSet::removeFont()                                                # //
// ############################################################################################# //
void ResourceSet::removeFont(const string &sName) {

}

// ############################################################################################# //
// # Nuclex::Storage::ResourceSet::getArchive()                                                # //
// ############################################################################################# //
const shared_ptr<Storage::Archive> &ResourceSet::getArchive(const string &sName) const {
  return m_spStorageServer->getArchive(sName);
}

// ############################################################################################# //
// # Nuclex::Storage::ResourceSet::getImage()                                                  # //
// ############################################################################################# //
const shared_ptr<Video::Image> &ResourceSet::getImage(const string &sName) const {
  return m_spVideoServer->getImage(sName);
}

// ############################################################################################# //
// # Nuclex::Storage::ResourceSet::getFont()                                                   # //
// ############################################################################################# //
const shared_ptr<Text::Font> &ResourceSet::getFont(const string &sName) const {
  return m_spTextServer->getFont(sName);
}
