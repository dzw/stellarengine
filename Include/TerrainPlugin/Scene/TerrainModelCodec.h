//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## SceneServer.h - Scene server                        //
// ### # #      ###                                                      //
// # ### #      ###  Scene graph management                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCENE_TERRAINMODELCODEC_H
#define NUCLEX_SCENE_TERRAINMODELCODEC_H

#include "TerrainPlugin/TerrainPlugin.h"
#include "Nuclex/Scene/ModelCodec.h"

#include <map>

namespace Nuclex { namespace Scene {

//  //
//  Nuclex::Scene::TerrainModelCodec                                                    //
//  //
/// X file model codec
/** Model codec which constructs models from directx x files
*/
class TerrainModelCodec :
  public ModelCodec {
  public:
    /// Destructor
    NUCLEXTERRAIN_API virtual ~TerrainModelCodec() {}

  //
  // SceneServer implementation
  //
  public:
    NUCLEXTERRAIN_API bool canLoadModel(
      const shared_ptr<Storage::Stream> &spStream
    ) const;
    
    NUCLEXTERRAIN_API virtual shared_ptr<Model> loadModel(
      const shared_ptr<Storage::Stream> &spStream,
      const shared_ptr<Video::VideoDevice> &spVideoDevice
    ) const;
};

}} // namespace Nuclex::Scene

#endif // NUCLEX_SCENE_TERRAINMODELCODEC_H
