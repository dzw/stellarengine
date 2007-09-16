//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## XMLSerializer.cpp - XML object serializer                                 //
// ### # #      ###                                                                            //
// # ### #      ###  A serializer which formats serialized objects into XML tags               //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Storage/XMLSerializer.h"
#include <sstream>

using namespace Nuclex;
using namespace Nuclex::Storage;

namespace {
  
// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::colorFromHex()                                            # //
// ############################################################################################# //
/// Convert hex string to long
/** Conerts a hexadecimal number contained in a string into a plain long

    @param  sHex  String containing a hexadecimal number
    @return The hexadecimal number as a long
*/
long longFromHex(const string &sHex) {
  long Value = 0;
  
  string::size_type Pos = 0;
  while(Pos < sHex.length()) {
    Value *= 16;

    switch(sHex[Pos]) {
      case '0': Value += 0; break;
      case '1': Value += 1; break;
      case '2': Value += 2; break;
      case '3': Value += 3; break;
      case '4': Value += 4; break;
      case '5': Value += 5; break;
      case '6': Value += 6; break;
      case '7': Value += 7; break;
      case '8': Value += 8; break;
      case '9': Value += 9; break;
      case 'a': case 'A': Value += 10; break;
      case 'b': case 'B': Value += 11; break;
      case 'c': case 'C': Value += 12; break;
      case 'd': case 'D': Value += 13; break;
      case 'e': case 'E': Value += 14; break;
      case 'f': case 'F': Value += 15; break;
    }
    
    ++Pos;
  }
  
  return Value;
} 
 
}

//  //
//  Nuclex::Storage::XMLSerializer::XMLNode                                                    //
//  //
/// XML node
/** Wraps an xml node in the tinyXML library
*/
class XMLSerializer::XMLNode :
  public Serializer {
    /// 
    class NodeScopeEnumerator :
      public Serializer::ScopeEnumerator {
      public:
        /// Constructor
        NodeScopeEnumerator(TiXmlNode *pTopNode, const string &sName) :
          m_pTopNode(pTopNode),
          m_CurrentScope(sName, shared_ptr<Serializer>()),
          m_pCurrentNode(NULL) {}
        
      //
      // Enumerator implementation
      //
      public:
        /// Advance to next entry
        bool next() { 
          if(!m_pCurrentNode)
            m_pCurrentNode = m_pTopNode->FirstChild(m_CurrentScope.first);
          else
            m_pCurrentNode = m_pCurrentNode->NextSibling(m_CurrentScope.first);

          if(m_pCurrentNode)
            m_CurrentScope.second = shared_ptr<Serializer>(new XMLNode(m_pCurrentNode));
          else
            m_CurrentScope.second = shared_ptr<Serializer>();

          return (m_pCurrentNode != NULL);
        }

        /// Get current stream information
        const std::pair<string, shared_ptr<Serializer> > &get() const {
          return m_CurrentScope;
        }
        
      private:
        TiXmlNode                                  *m_pTopNode; ///< Node whose childs to enumerate
        TiXmlNode                                  *m_pCurrentNode; ///< Current child node
        std::pair<string, shared_ptr<Serializer> >  m_CurrentScope; ///< Associated serializer scope
    };

  public:
    /// Constructor
    XMLNode(TiXmlNode *pXMLNode) :
      m_pXMLNode(pXMLNode) {}

    /// Destructor
    virtual ~XMLNode() {}
  
  //
  // XMLNode implementation
  //
  public:
    /// Retrieves the wrapped tinyXML node
    TiXmlNode *getXMLNode() const { return m_pXMLNode; }
    
  //
  // Serializer implementation
  //
  public:
    /// Enumerate nested scopes
    shared_ptr<ScopeEnumerator> enumScopes(const string &sName = "") {
      return shared_ptr<ScopeEnumerator>(
        new NodeScopeEnumerator(m_pXMLNode, sName)
      );
    }
    
    /// Enter nested scope
    shared_ptr<Serializer> openScope(const string &sName, bool bOptional = false);

    /// Begin nested scope
    shared_ptr<Serializer> createScope(const string &sName);
    
    /// Retrieve a value from the xml tree
    Variant retrieveValue(const string &sName, const Variant &Default);
    /// Retrieve a value from the xml tree or return a default value
    Variant retrieveValue(const string &sName);
    /// Store a value in the xml tree
    void storeValue(const string &sName, const Variant &Value);

  private:    
    TiXmlNode *m_pXMLNode;                            ///< The wrapped tinyXML node
};

// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::colorFromHex()                                            # //
// ############################################################################################# //
/** Converts a hexadecimal color code into a nuclex color

    @param  sHexColor  Hexadecimal color code
    @return The corresponding nuclex color
*/
Color XMLSerializer::colorFromHex(const string &sHexColor) {
  string::size_type End = sHexColor.find_last_not_of(" \t\n");
  if(End == string::npos)
    return Color();
    
  return Color(
    static_cast<float>((End >= 6) ? longFromHex(sHexColor.substr(End - 5, 2)) : 0x00) / 255.0f,
    static_cast<float>((End >= 4) ? longFromHex(sHexColor.substr(End - 3, 2)) : 0x00) / 255.0f,
    static_cast<float>((End >= 2) ? longFromHex(sHexColor.substr(End - 1, 2)) : 0x00) / 255.0f,
    static_cast<float>((End >= 8) ? longFromHex(sHexColor.substr(End - 7, 2)) : 0x00) / 255.0f
  );
}

// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::XMLSerializer()                               Constructor # //
// ############################################################################################# //
/** Initializes an instance of XMLSerializer

    @param  sXML  Initial contents of the serializer
*/
XMLSerializer::XMLSerializer(const string &sXML) :
  m_XMLDocument("document") {
  std::stringstream XMLStream(sXML, std::ios::in | std::ios::binary);
  XMLStream >> m_XMLDocument;

/* Maybe a bit faster (no string copy created)

  m_XMLDocument.Parse(sXML.c_str());
  if(m_XMLDocument.Error())
    throw UnsupportedFormatException("Nuclex::Storage::XMLSerializer::XMLSerializer()",
                                     string("Error while parsing XML string: ") + m_XMLDocument.ErrorDesc());
*/

  m_spDocumentNode = shared_ptr<XMLNode>(new XMLNode(&m_XMLDocument));
}

// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::getXML()                                                  # //
// ############################################################################################# //
/** Returns the xml code of the data serialized so far

    @return The current xml string
*/
string XMLSerializer::getXML() const {
  std::stringstream XMLStream(std::ios::out | std::ios::binary);
  XMLStream << m_XMLDocument;
  return XMLStream.str();
}

// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::enumScopes()                                              # //
// ############################################################################################# //
/** Returns an enumerator over all subscopes of the global xml scope

    @param  sName  Name of the subscopes over which to enumerate
    @return The new enumerator
*/
shared_ptr<Serializer::ScopeEnumerator> XMLSerializer::enumScopes(const string &sName) {
  return m_spDocumentNode->enumScopes(sName);
}
    
// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::openScope()                                               # //
// ############################################################################################# //
/** Opens the scope with the specified name

    @param  sName      Name of the scope to open
    @param  bOptional  Whether to return an empty scope when the scope doesn't exist
*/    
shared_ptr<Serializer> XMLSerializer::openScope(const string &sName, bool bOptional) {
  return m_spDocumentNode->openScope(sName, bOptional);
}

// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::createScope()                                             # //
// ############################################################################################# //
/** Creates a new scope und the global scope

    @param  sName  Name of the scope to create
    @return The created scope
*/
shared_ptr<Serializer> XMLSerializer::createScope(const string &sName) {
  return m_spDocumentNode->createScope(sName);
}

// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::retrieveValue()                                           # //
// ############################################################################################# //
/** Returns the stored value whit the specified name from the global scope

    @param  sName  Name of the value to return
    @return The value stored under the specified name
*/
Variant XMLSerializer::retrieveValue(const string &sName) {
  return m_spDocumentNode->retrieveValue(sName);
}

// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::retrieveValue()                                           # //
// ############################################################################################# //
/** Returns the stored value whit the specified name from the global scope or
    a default value, if the specified name could not be found.

    @param  sName    Name of the value to return
    @param  Default  Default value to return when the named value could not be found
    @return The value stored under the specified name or the default value
*/
Variant XMLSerializer::retrieveValue(const string &sName, const Variant &Default) {
  return m_spDocumentNode->retrieveValue(sName, Default);
}

// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::storeValue()                                              # //
// ############################################################################################# //
/** Stores a value under the specified name into the global scope

    @param  sName  Name under which to store the value
    @param  Value  Value to store
*/
void XMLSerializer::storeValue(const string &sName, const Variant &Value) {
  m_spDocumentNode->storeValue(sName, Value);
}

// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::XMLNode::openScope()                                      # //
// ############################################################################################# //
/** Opens the specified scope

    @param  sName      Name of the scope to open
    @param  bOptional  Whether to return an empty scope if the scope cannot be found
*/
shared_ptr<Serializer> XMLSerializer::XMLNode::openScope(const string &sName, bool bOptional) {
  TiXmlNode *pChild = m_pXMLNode->FirstChild(sName);
  if(!pChild) {
    if(bOptional)
      return shared_ptr<Serializer>();
    else
      throw ResourceException("Nuclex::Storage::XMLSerializer::XMLNode::openScope()",
                              string("The node '") + m_pXMLNode->Value() + "' does not have a child named '" + sName + "'");
  }

  return shared_ptr<Serializer>(new XMLNode(pChild));
}

// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::XMLNode::createScope()                                    # //
// ############################################################################################# //
/** Creates a nested scope under the current scope

    @param  sName  Name of the scope to create
    @return The created scope
*/
shared_ptr<Serializer> XMLSerializer::XMLNode::createScope(const string &sName) {
  TiXmlNode *pChild = m_pXMLNode->LinkEndChild(new TiXmlElement(sName));
  
  return shared_ptr<Serializer>(new XMLNode(pChild));
}
    
// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::XMLNode::retrieveValue()                                  # //
// ############################################################################################# //
/** Retrieves the value with the specified name from the current scope or returns
    a default value if the specified name cannot be found
    
    @param  sName    Name of the value to return
    @param  Default  Default value to return of the name does not exist
    @return The requested value or the specified default value
*/
Variant XMLSerializer::XMLNode::retrieveValue(const string &sName, const Variant &Default) {
  // inefficient. make better.
  try {
    return retrieveValue(sName);
  }
  catch(const ResourceException &) {
    return Default;
  }
}
    
// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::XMLNode::retrieveValue()                                  # //
// ############################################################################################# //
/** Retrieves the value with the specified name from the current scope

    @param  sName  Name of the value to return
    @return The value stored under the specified name in the current scope
*/
Variant XMLSerializer::XMLNode::retrieveValue(const string &sName) {
  if(sName.length() && sName[0] == '_') {
    TiXmlElement *pElement = m_pXMLNode->ToElement();
    if(!pElement)
      throw UnexpectedException(
        "Nuclex::Storage::XMLSerializer::XMLNode::retrieveValue()",
        string("Node '") + m_pXMLNode->Value() + "' is not an XML element"
      );

    const string *psAttribute = pElement->Attribute(sName.substr(1));
    if(psAttribute)
      return Variant(*psAttribute);
    else
      throw ResourceException(
        "Nuclex::Storage::XMLSerializer::XMLNode::retrieveValue()",
        string("Attribute '") + sName.substr(1) +
          "' not found in element '" + m_pXMLNode->Value() + "'"
      );
  } else {
    TiXmlNode *pChild = m_pXMLNode->FirstChild(sName);
    if(!pChild)
      throw ResourceException(
        "Nuclex::Storage::XMLSerializer::XMLNode::retrieveValue()",
        string("The node '") + m_pXMLNode->Value() +
          "' does not have a child named '" + sName + "'"
      );

    TiXmlNode *pTextChild = pChild->FirstChild();
    if(!pTextChild)
      throw ResourceException(
        "Nuclex::Storage::XMLSerializer::XMLNode::retrieveValue()",
        string("The element '") + sName + "' of '" +
          m_pXMLNode->Value() + "' does not contain a value"
      );
    
    TiXmlText *pText = pTextChild->ToText();
    if(!pText)
      throw ResourceException(
        "Nuclex::Storage::XMLSerializer::XMLNode::retrieveValue()",
        string("The element '") + sName + "' of '" +
           m_pXMLNode->Value() + "' does not contain a value"
      );
    
    return Variant(string(pText->Value()));
  }
}

// ############################################################################################# //
// # Nuclex::Storage::XMLSerializer::XMLNode::storeValue()                                     # //
// ############################################################################################# //
/** Stores the value under the specified name in the current scope

    @param  sName  Name under which to store the value
    @param  Value  Value to store
*/
void XMLSerializer::XMLNode::storeValue(const string &sName, const Variant &Value) {
  if(sName.length() && sName[0] == '_') {
    TiXmlElement *pElement = m_pXMLNode->ToElement();
    if(!pElement)
      throw UnexpectedException("Nuclex::Storage::XMLSerializer::XMLNode::storeValue()",
                                string("Node '") + m_pXMLNode->Value() + "' is not an XML element");
    
    pElement->SetAttribute(sName.substr(1), Value.to<string>());
  } else {
    TiXmlNode *pChild = m_pXMLNode->LinkEndChild(new TiXmlElement(sName));
    pChild->LinkEndChild(new TiXmlText(Value));
  }
}
