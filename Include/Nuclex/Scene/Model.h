//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## SceneServer.h - Scene server                        //
// ### # #      ###                                                      //
// # ### #      ###  Scene graph management                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCENE_MODEL_H
#define NUCLEX_SCENE_MODEL_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Video/VideoDevice.h"
#include "Nuclex/Storage/Persistable.h"

namespace Nuclex { namespace Scene {

//  //
//  Nuclex::Scene::Model                                                                       //
//  //
/// Model
/** A model template from which model instances can be created
*/
class Model {
  public:
    class Instance;

    /// Destructor
    NUCLEX_API virtual ~Model() {}

  //
  // Model implementation
  //
  public:
    /// Create model instance
    NUCLEX_API virtual shared_ptr<Instance> createInstance() = 0;
    
    // NUCLEX_API virtual shared_ptr<BoneEnumerator> enumBones() = 0;
    // NUCLEX_API virtual shared_ptr<SkinEnumerator> enumSkins() = 0;
};

//  //
//  Nuclex::Scene::Model::Instance                                                             //
//  //
/// Model instance
/** Instance of a model template
*/
class Model::Instance : public Storage::Persistable {
  public:
    /// Destructor
    NUCLEX_API virtual ~Instance() {}
  
  //
  // Instance implementation
  //
  public:
    /// Render the model instance
    NUCLEX_API virtual void render(
      const shared_ptr<Video::VideoDevice::RenderingContext> &spRC
    ) = 0;
};

}} // namespace Nuclex::Scene

#endif // NUCLEX_SCENE_MODEL_H
