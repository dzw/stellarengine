//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## SceneServer.h - Scene server                        //
// ### # #      ###                                                      //
// # ### #      ###  Scene graph management                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCENE_XMODEL_H
#define NUCLEX_SCENE_XMODEL_H

#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "Nuclex/Scene/Model.h"

#include <vector>

namespace Nuclex {
  namespace Video { class Texture; }
}

namespace Nuclex { namespace Scene {

//  //
//  Nuclex::Scene::XModel                                                                      //
//  //
/// X file model
/** DirectX X file model
*/
class XModel : public Model {
  public:
    class Instance;

    typedef std::vector<std::pair<D3DMATERIAL9, shared_ptr<Video::Texture> > > MaterialVector;

    /// Constructor
    NUCLEXDIRECT3D9_API XModel(const ID3DXMeshPtr &spMesh,  const MaterialVector &Materials);
    /// Destructor
    NUCLEXDIRECT3D9_API virtual ~XModel() {}

  //
  // Model implementation
  //
  public:
    /// Create model instance
    NUCLEXDIRECT3D9_API shared_ptr<Model::Instance> createInstance() {
      return shared_ptr<Model::Instance>();
    }
  
  private:
    ID3DXMeshPtr   m_spMesh;
    MaterialVector m_Materials;
};

//  //
//  Nuclex::Scene::XModel::Instance                                                            //
//  //
class XModel::Instance : public Model::Instance {
  public:
    NUCLEXDIRECT3D9_API Instance() {}
    NUCLEXDIRECT3D9_API virtual ~Instance() {}

  //
  // Model::Instance implementation
  //
  public:
    NUCLEXDIRECT3D9_API void render(
      const shared_ptr<Video::VideoDevice::RenderingContext> &spRC
    );

  //
  // Persistable implementation
  //
  public:
    /// Load object from serializer
    NUCLEXDIRECT3D9_API void load(const shared_ptr<Storage::Serializer> &spSerializer) {}

    /// Save object to serializer
    NUCLEXDIRECT3D9_API void save(const shared_ptr<Storage::Serializer> &spSerializer) const {}
};

}} // namespace Nuclex::Scene

#endif // NUCLEX_SCENE_XMODEL_H
