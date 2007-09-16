//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## SceneServer.h - Scene server                        //
// ### # #      ###                                                      //
// # ### #      ###  Scene graph management                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#include "TerrainPlugin/Scene/TerrainModel.h"
#include "Direct3D9Plugin/Video/Direct3D9VideoDevice.h"
#include "Nuclex/Storage/XMLSerializer.h"
#include "Nuclex/Storage/ResourceSet.h"
#include "Nuclex/Video/VideoServer.h"
#include "Nuclex/Video/Image.h"
#include "Nuclex/Video/Texture.h"
#include "Nuclex/Kernel.h"

using namespace Nuclex;
using namespace Nuclex::Scene;
using namespace Nuclex::Video;
using namespace Nuclex::Storage;

// ############################################################################################# //
// # Nuclex::Scene::TerrainModel::TerrainModel()                     Constructor # //
// ############################################################################################# //
TerrainModel::TerrainModel(const string &sTerrainDescription) :
  m_TerrainResources(
    "Terrain",
    Kernel::getInstance().getStorageServer(),
    Kernel::getInstance().getVideoServer(),
    Kernel::getInstance().getTextServer()
  ) {

  XMLSerializer TerrainDescription(sTerrainDescription);

  shared_ptr<Serializer> spTerrain = TerrainDescription.openScope("terrain");
  
  { shared_ptr<Serializer> spResources = spTerrain->openScope("resources", true);
    if(spResources)
      m_TerrainResources.load(spResources);
  }

  { shared_ptr<Serializer> spSoils = spTerrain->openScope("soils", true);
    if(spSoils) {
      shared_ptr<Serializer::ScopeEnumerator> spSoilsEnum = spSoils->enumScopes("soil");
      
      while(spSoilsEnum->next())
        Kernel::logMessage(Kernel::MT_INFO, spSoilsEnum->get().second->get<string>("_name"));
    }
  }
}

// ############################################################################################# //
// # Nuclex::Scene::TerrainModel::render()                                              # //
// ############################################################################################# //
void TerrainModel::render(const shared_ptr<Video::VideoDevice::RenderingContext> &spVR) {
}
