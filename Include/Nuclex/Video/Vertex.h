//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VertexDeclaration.h - Vertex declaration                                  //
// ### # #      ###                                                                            //
// # ### #      ###  Defines the components of a vertex and their memory layout                //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_VERTEX_H
#define NUCLEX_VIDEO_VERTEX_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/VertexDeclaration.h"
#include "Nuclex/Math/Point3.h"

/*

Problems to solve:
Offset could be determined at compile time

*/

namespace Nuclex { namespace Video {

namespace VertexComponents {

//  //
//  Nuclex::Video::VertexComponents::Position                                                  //
//  //
template<bool bPretransformed = false> struct Position;
template<> struct Position<false> {
  enum {
    Usage = VertexDeclaration::U_POSITION,
    Method = VertexDeclaration::M_DEFAULT,
    Type = VertexDeclaration::T_FLOAT3,
    Count = 1
  };

  Point3<float>::POD Pos;
};
template<> struct Position<true> {
  enum {
    Usage = VertexDeclaration::U_POSITIONT,
    Method = VertexDeclaration::M_DEFAULT,
    Type = VertexDeclaration::T_FLOAT4,
    Count = 1
  };
  
  Point3<float>::POD Pos;
  float RHW;
};

//  //
//  Nuclex::Video::VertexComponents::TextureCoordinates                                        //
//  //
template<size_t Count = 1>
struct TextureCoordinates {
  enum {
    Usage = VertexDeclaration::U_TEXCOORD,
    Method = VertexDeclaration::M_DEFAULT,
    Type = VertexDeclaration::T_FLOAT2,
    Count = Count
  };

  Point2<float>::POD TexCoords[Count];
};

//  //
//  Nuclex::Video::VertexComponents::DiffuseColor                                              //
//  //
struct DiffuseColor {
  enum {
    Usage = VertexDeclaration::U_COLOR,
    Method = VertexDeclaration::M_DEFAULT,
    Type = VertexDeclaration::T_COLOR,
    Count = 1
  };

  unsigned long Diffuse;
};

} // namespace VertexComponents

//  //
//  Nuclex::Video::Vertex1                                                                     //
//  //
/// Vertex
/** 
*/
template<typename Component1>
struct Vertex1 : Component1 {
  static const VertexDeclaration &getDeclaration() {
    static VertexDeclaration Decl = VertexDeclaration() 
      << Element(0, Component1::Type, Component1::Method, Component1::Usage, 0);
    return Decl;
  }
};

template<typename Component1, typename Component2>
struct Vertex2 : Component1, Component2 {
  static const VertexDeclaration &getDeclaration() {
    static VertexDeclaration Decl = VertexDeclaration()
      << Element(0, Component1::Type, Component1::Method, Component1::Usage, 0)
      << Element(0, Component2::Type, Component2::Method, Component2::Usage, 0);
    return Decl;
  }
};

template<typename Component1, typename Component2, typename Component3>
struct Vertex3 : Component1, Component2, Component3 {
  static const VertexDeclaration &getDeclaration() {
    static VertexDeclaration Decl = VertexDeclaration()
      << Element(0, Component1::Type, Component1::Method, Component1::Usage, 0)
      << Element(0, Component2::Type, Component2::Method, Component2::Usage, 0)
      << Element(0, Component3::Type, Component3::Method, Component3::Usage, 0);
    return Decl;
  }
};

template<typename Component1, typename Component2, typename Component3, typename Component4>
struct Vertex4 : Component1, Component2, Component3, Component4 {
  static const VertexDeclaration &getDeclaration() {
    static VertexDeclaration Decl = VertexDeclaration()
      << Element(0, Component1::Type, Component1::Method, Component1::Usage, 0)
      << Element(0, Component2::Type, Component2::Method, Component2::Usage, 0)
      << Element(0, Component3::Type, Component3::Method, Component3::Usage, 0)
      << Element(0, Component4::Type, Component4::Method, Component4::Usage, 0);
    return Decl;
  }
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_VERTEX_H
