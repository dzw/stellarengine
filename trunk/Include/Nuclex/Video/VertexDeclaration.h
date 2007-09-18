//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VertexDeclaration.h - Vertex declaration                                  //
// ### # #      ###                                                                            //
// # ### #      ###  Defines the components of a vertex and their memory layout                //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_VERTEXDECLARATION_H
#define NUCLEX_VIDEO_VERTEXDECLARATION_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Exception.h"
#include <vector>

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::VertexDeclaration                                                           //
//  //
/// Vertex Declaration
/** Defines the structure of a vertex stored in a vertex buffer.
    Required for the graphics adapter to extract the data it needs
    for rendering from each vertex
*/
class VertexDeclaration {
  public:
    /// How a vertex component will be used
    enum Usage {
      U_POSITION = 0,
      U_BLENDWEIGHT,
      U_BLENDINDICES,
      U_NORMAL,
      U_PSIZE,
      U_TEXCOORD,
      U_TANGENT,
      U_BINORMAL,
      U_TESSFACTOR,
      U_POSITIONT,
      U_COLOR,
      U_FOG,
      U_DEPTH,
      U_SAMPLE
    };

    /// Method to use for processing the vertex
    enum Method {
      M_DEFAULT = 0,
      M_PARTIALU,
      M_PARTIALV,
      M_CROSSUV,                                      ///< Normal
      M_UV,
      M_LOOKUP,                                       ///< Lookup a displacement map
      M_LOOKUPPRESAMPLED                              ///< Lookup a pre-sampled displacement map
    };

    /// Data type of a vertex component
    enum Type {
      T_NONE = 0,                                     ///< When the type field in a decl is unused.
      T_FLOAT1,                                       ///< 1D float expanded to (value, 0., 0., 1.)
      T_FLOAT2,                                       ///< 2D float expanded to (value, value, 0., 1.)
      T_FLOAT3,                                       ///< 3D float expanded to (value, value, value, 1.)
      T_FLOAT4,                                       ///< 4D float
      T_COLOR,                                        ///< 4D packed unsigned bytes mapped to 0. to 1. range. Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A)
      T_UBYTE4,                                       ///< 4D unsigned byte
      T_SHORT2,                                       ///< 2D signed short expanded to (value, value, 0., 1.)
      T_SHORT4                                        ///< 4D signed short
    };

    /// A vertex component
    struct Element {
      /// Constructor
      /** Initializes an instance of element

          @param  nOffset      Offset in bytes within the stream to read from
          @param  eType        Variable type of the input data
          @param  eMethod      Predefined operations to be performed by the tesselator
          @param  eUsage       How the element will be used
          @param  cUsageIndex  Additional usage index (if you have multiple elements with
                               the same type)
      */
      inline Element(unsigned short nOffset,
                     Type eType, Method eMethod, Usage eUsage,
                     unsigned char cUsageIndex) :
        nOffset(nOffset),
        eType(eType),
        eMethod(eMethod),
        eUsage(eUsage),
        cUsageIndex(cUsageIndex) {}
      /// Copy constructor
      /** Initializes an instance of Element as copy of an existing instance

          @param  Other  Element to copy
      */
      inline Element(const Element &Other) :
        nOffset(Other.nOffset),
        eType(Other.eType),
        eMethod(Other.eMethod),
        eUsage(Other.eUsage),
        cUsageIndex(Other.cUsageIndex) {}

      unsigned short nOffset;                         ///< Offset in the stream in bytes
      Type           eType;                           ///< Data type
      Method         eMethod;                         ///< Processing method
      Usage          eUsage;                          ///< Semantics
      unsigned char  cUsageIndex;                     ///< Semantic index
    };  

    /// Constructor
    NUCLEX_API inline VertexDeclaration() :
      m_nCurrentOffset(0),
      m_nSize(0) {}

    /// Destructor
    NUCLEX_API inline ~VertexDeclaration() {}

    /// Build position element
    NUCLEX_API inline static Element Position(unsigned char cUsageIndex = 0, unsigned short nOffset = 0) {
      return Element(nOffset, T_FLOAT3, M_DEFAULT, U_POSITION, cUsageIndex);
    }
    /// Build pretransformed position element
    NUCLEX_API inline static Element PretransformedPosition(unsigned char cUsageIndex = 0, unsigned short nOffset = 0) {
      return Element(nOffset, T_FLOAT4, M_DEFAULT, U_POSITIONT, cUsageIndex);
    }
    /// Build normal vector element
    NUCLEX_API inline static Element Normal(unsigned char cUsageIndex = 0, unsigned short nOffset = 0) {
      return Element(nOffset, T_FLOAT3, M_DEFAULT, U_NORMAL, cUsageIndex);
    }
    /// Build color element
    NUCLEX_API inline static Element Color(unsigned char cUsageIndex = 0, unsigned short nOffset = 0) {
      return Element(nOffset, T_COLOR, M_DEFAULT, U_COLOR, cUsageIndex);
    }
    /// Build texture coordinate element
    NUCLEX_API inline static Element TexCoord(unsigned char cUsageIndex = 0, unsigned short nOffset = 0) {
      return Element(nOffset, T_FLOAT2, M_DEFAULT, U_TEXCOORD, cUsageIndex);
    }

  //
  // VertexDeclaration implementation
  //
  public:
    /// Append a component to the vertex declaration
    NUCLEX_API inline VertexDeclaration &operator <<(const Element &ElementToAdd) {
      Element e(ElementToAdd);

      unsigned short nSize;
      switch(e.eType) {
        case T_NONE: break;
        case T_FLOAT1: { nSize =  4; break; }
        case T_FLOAT2: { nSize =  8; break; }
        case T_FLOAT3: { nSize = 12; break; }
        case T_FLOAT4: { nSize = 16; break; }
        case T_COLOR:  { nSize =  4; break; }
        case T_UBYTE4: { nSize =  4; break; }
        case T_SHORT2: { nSize =  4; break; }
        case T_SHORT4: { nSize =  8; break; }
        default: throw UnexpectedException(
          "Nuclex::VertexDeclaration::operator <<()", "Unknown element type"
        );
      }

      if(e.nOffset == 0) {
        e.nOffset = m_nCurrentOffset;
        m_nCurrentOffset += nSize;
      }
      if(e.nOffset + nSize > m_nSize)
        m_nSize = e.nOffset + nSize;

      m_Elements.push_back(e);

      return *this;
    }

    /// Total number of bytes for a vertex
    NUCLEX_API inline unsigned short getSize() const { return m_nSize; }

  private:
    /// A vector of declaration elements
    typedef std::vector<Element> ElementVector;

    /// The elements of the vertex declaration
    ElementVector m_Elements;
    /// Accumulated vertex offset
    unsigned short m_nCurrentOffset;
    /// Total space required for a single vertex in this declaration
    unsigned short m_nSize;
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_VERTEXDECLARATION_H
