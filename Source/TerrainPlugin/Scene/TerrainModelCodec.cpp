//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## SceneServer.h - Scene server                        //
// ### # #      ###                                                      //
// # ### #      ###  Scene graph management                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#include "TerrainPlugin/Scene/TerrainModelCodec.h"
#include "TerrainPlugin/Scene/TerrainModel.h"
#include "Nuclex/Storage/Stream.h"

using namespace Nuclex;
using namespace Nuclex::Video;
using namespace Nuclex::Scene;

// ############################################################################################# //
// # Nuclex::Scene::TerrainModelCodec::canLoadModel()                                          # //
// ############################################################################################# //
bool TerrainModelCodec::canLoadModel(const shared_ptr<Storage::Stream> &spStream) const {
  if(spStream->getSize() < 4)
    return false;

  spStream->seekTo(0);
  return (spStream->read<long>() == 1836597052);
}

// ############################################################################################# //
// # Nuclex::Scene::TerrainModelCodec::loadModel()                                             # //
// ############################################################################################# //
shared_ptr<Model> TerrainModelCodec::loadModel(
  const shared_ptr<Storage::Stream> &spStream,
  const shared_ptr<Video::VideoDevice> &spVideoDevice
) const {
  if(!canLoadModel(spStream))
    throw InvalidArgumentException(
      "Nuclex::Scene::TerrainModelCodec::loadModel()",
      "Unsupported file format"
    );

  return shared_ptr<Model>(); //new TerrainModel(stringFromStream(spStream)));
  
}
