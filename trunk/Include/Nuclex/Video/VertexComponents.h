//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VertexDeclaration.h - Vertex declaration                                  //
// ### # #      ###                                                                            //
// # ### #      ###  Defines the components of a vertex and their memory layout                //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_VERTEXCOMPONENTS_H
#define NUCLEX_VIDEO_VERTEXCOMPONENTS_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/VertexDeclaration.h"
#include "Nuclex/Math/Point3.h"

namespace Nuclex { namespace Video { namespace VertexComponents {

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

}}} // namespace Nuclex::Video::VertexComponents

#endif // NUCLEX_VIDEO_VERTEX_H
