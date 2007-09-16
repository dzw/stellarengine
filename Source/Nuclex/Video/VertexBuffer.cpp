//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## VertexBuffer.cpp - Set of vertices                                        //
// ### # #      ###                                                                            //
// # ### #      ###  Stores a set of custom-format vertices which can be rendered              //
// #  ## #   # ## ## in a single or in multiple steps                                          //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Video/VertexBuffer.h"

using namespace Nuclex;
using namespace Nuclex::Video;

// Just here to ensure it compiles without additional headers


// Mögliche Designs:
//
// Vertextypen mit Type-Flags
// Jeder Vertex ist eine Struktur seiner Elemente
//
// Typen-Streams

// 
//   - An object which provides memory (a contrast to takes memory)
//   - The structure contained therein is decided at run-time
//
// Einfach VertexBuffer-Template, dann
//
//  template <typename VARTYPE>
//  class VertexBuffer {
//    public:
//      VertexBuffer(void *pUnsafe) :
//        m_pData(reinterpret_cast<VARTYPE *>(pUnsafe));
//
//    private:
//      VARTYPE *m_pData;
//  };
