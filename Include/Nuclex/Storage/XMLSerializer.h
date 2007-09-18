//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## XMLSerializer.h - XML object serializer                                   //
// ### # #      ###                                                                            //
// # ### #      ###  A serializer which formats serialized objects into XML tags               //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_XMLSERIALIZER_H
#define NUCLEX_STORAGE_XMLSERIALIZER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Storage/Serializer.h"
#include "Nuclex/Support/Color.h"

#define TIXML_USE_STL
#include "TinyXML/tinyxml.h"

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::XMLSerializer                                                             //
//  //
/// Structured serializer base class
/** Base class for structured data serializers
*/
class XMLSerializer :
  public Serializer {
  public:
    /// Get color from hex code
    NUCLEX_API static Color colorFromHex(const string &sHexColor);

    /// Constructor
    NUCLEX_API XMLSerializer(const string &sXML = "");
    
    /// Destructor
    /** Destroys an instance of Serializer
    */
    NUCLEX_API virtual ~XMLSerializer() {}

  //
  // XMLSerializer implementation
  //
  public:
    NUCLEX_API string getXML() const;

  //
  // Serializer implementation
  //
  public:
    /// Enumerate nested scopes
    NUCLEX_API shared_ptr<ScopeEnumerator> enumScopes(const string &sName = "");
    
    /// Enter nested scope
    /** Enters an existing block of nested data.
        If the method is called in succession with the same name, it
        will enter multiple blocks sharing that name one after another.

        @param  sName      Name of the block to enter
        @param  bOptional  Whether to throw an exception if scope not found
        @return True if the block was entered
    */
    NUCLEX_API shared_ptr<Serializer> openScope(const string &sName, bool bOptional = false);

    /// Begin nested scope
    /** Begins a new block of nested data. Always creates a new block,
        even if a block with the same name already exists.
    */
    NUCLEX_API shared_ptr<Serializer> createScope(const string &sName);
    
  private:
    class XMLNode;
  
    /// Retrieve a value from the xml tree
    Variant retrieveValue(const string &sName);
    /// Retrieve a value from the xml tree or return a default value
    Variant retrieveValue(const string &sName, const Variant &Default);
    /// Store a value in the xml tree
    void storeValue(const string &sName, const Variant &Value);
    
    TiXmlDocument       m_XMLDocument;                ///< XML document
    shared_ptr<XMLNode> m_spDocumentNode;             ///< Document node
};

}} // namespace Nuclex::Storage

#endif // NUCLEX_STORAGE_XMLSERIALIZER_H
