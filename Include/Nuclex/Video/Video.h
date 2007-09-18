//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Video.h - Visualization                                                   //
// ### # #      ###                                                                            //
// # ### #      ###  Everything related to graphics and visualization                          //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_VIDEO_H
#define NUCLEX_VIDEO_VIDEO_H

namespace Nuclex {

/// Visualization
/** Everything related to graphics and visualization lives in this namespace. The nuclex
    video subsystem is managed by the VideoServer, which uses plugins to provide a series
    of video devices which can each be seen as a graphics card interfaces through a specific
    API. This means that a graphics card may appear multiple times, once through OpenGL, once
    through Direct3D for example.
    
    A video device can spawn a renderer, which is an actual instance of the device on which
    resources can be allocated and rendering can be performed.
*/
namespace Video {

/// How many vertices to ideally batch per drawPrimitive() call
const size_t VertexBatchSize = 1024; // STL vector bug. Should be 4096.

/// Maximum number of texture stages to support
const size_t MaxTextureStages = 8;

/// Maximum number of vertex streams to support
const size_t MaxVertexStreams = 4;

} // namespace Video

} // namespace Nuclex

#endif // NUCLEX_VIDEO_VIDEO_H
