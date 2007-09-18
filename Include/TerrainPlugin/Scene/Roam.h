//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## ROAM.h                                              //
// ### # #      ###                                                      //
// # ### #      ###                                                      //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2005 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCENE_ROAM_H
#define NUCLEX_SCENE_ROAM_H

#include "TerrainPlugin/TerrainPlugin.h"

namespace Nuclex { namespace Scene {

// ROAM 2.0 problems
//
// Diamonds take up way too much memory
// No memory-wise locality of reference if diamonds are reused
// Using tetrahedrons instead of diamonds is even worse
// Paging is quite difficult to realize
// Morphing is even harder ?

// Clipmapping problems
//
// No direct hardware support for toroidal vertex buffer updates
// Extreme viewing distances require very large paging chunks
   

// Terrain storage format
//
// Vertex array - 3 times the memory of a height map, any contour is possible
//                Paging is again very difficult
// Color-keyed heightmap sandwitches - Hard to generate


// ROAM 2.0 adjustments
//
// Each diamond obtains a pointer to a diamond above it
//   -> Combined splitting and merging of all levels + averaged error metric ?
//   -> Independent splitting and merging ?
//
// Just normal heightmap sandwiches ?
//   -> Stitching could produce artifacts like T-vertices
//
// Tetrahedrons seem to be the best approach. Maybe combine with RuSTiC.
//   -> Even more memory overhead, rustic becomes increasingly important

// Custom
//
// Collapsible geomipmapping of irregular regions

// Required Features
//   + continuous terrain
//     - extremely huge viewing distances
//     - paging in background
//   + realistic grass
//     - impostored foliage
//     - near-region grass as actual vertex blades
//     - far-region grass using post-processing and a texture
//   + sun shading
//     - non-lightmap terrain shading
//     - allow stepless changing sun position
//   + overhangs
//     - caves (manual shading adjustment is ok)
//     - any number of regions on top of each other
//   + forest rendering
//     - large amounts of trees using impostors, walkable
//     - also viewable from the top
//   + weather effects
//     - realistical rain and snow
//     - maybe even sand storms

namespace ROAM {

struct Node {
  
  
};

}

}} // namespace Nuclex::Scene

#endif // NUCLEX_SCENE_ROAM_H
