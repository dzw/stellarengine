//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## SceneServer.h - Scene server                        //
// ### # #      ###                                                      //
// # ### #      ###  Scene graph management                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#include "Direct3D9Plugin/Scene/XModel.h"

using namespace Nuclex;
using namespace Nuclex::Scene;
using namespace Nuclex::Video;

// ############################################################################################# //
// # Nuclex::Scene::XModel::XModel()                                               Constructor # //
// ############################################################################################# //
XModel::XModel(const ID3DXMeshPtr &spMesh, const MaterialVector &Materials) :
  m_spMesh(spMesh),
  m_Materials(Materials) {}
  

// ############################################################################################# //
// # Nuclex::Scene::XModel::Instance::render()                                                 # //
// ############################################################################################# //
void XModel::Instance::render(const shared_ptr<Video::VideoDevice::RenderingContext> &spVR) {
/*
  for(size_t MaterialIndex = 0; MaterialIndex < m_Materials.size(); ++MaterialIndex) {
    spVR->setTexture(0, m_Materials[MaterialIndex].second);
    D3DCheck(
      "Nuclex::Scene::XModel::render()",
      "ID3DXMesh::DrawSubSet()",
      m_spMesh->DrawSubset(MaterialIndex)
    );
  }
*/  
}
