//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## SceneServer.h - Scene server                        //
// ### # #      ###                                                      //
// # ### #      ###  Scene graph management                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCENE_TERRAINMODEL_H
#define NUCLEX_SCENE_TERRAINMODEL_H

#include "TerrainPlugin/TerrainPlugin.h"
#include "Nuclex/Scene/Model.h"
#include "Nuclex/Storage/ResourceSet.h"

#include <vector>

namespace Nuclex {
  namespace Video { class Texture; }
}

namespace Nuclex { namespace Scene {

//  //
//  Nuclex::Scene::TerrainModel                                                                //
//  //
/// Clipmap Terrain
/** A model which bases its geometry on the clipmap terrain algorithm
*/
class TerrainModel :
  public Model {
  public:
    /// Constructor
    NUCLEXTERRAIN_API TerrainModel(const string &sXML);
    /// Destructor
    NUCLEXTERRAIN_API virtual ~TerrainModel() {}

  //
  // TerrainModel implementation
  //
  public:
    /// 
    NUCLEXTERRAIN_API Point3<float> findImpact(
      const Point3<float> &From, const Point3<float> &To
    );

  //
  // Model implementation
  //
  public:
    NUCLEXTERRAIN_API void render(
      const shared_ptr<Video::VideoDevice::RenderingContext> &spRC
    );
  
  private:
    /// Resources used by the terrain
    Storage::ResourceSet m_TerrainResources;
};

}} // namespace Nuclex::Scene

#endif // NUCLEX_SCENE_TERRAINMODEL_H
