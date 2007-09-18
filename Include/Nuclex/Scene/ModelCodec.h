//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## SceneServer.h - Scene server                        //
// ### # #      ###                                                      //
// # ### #      ###  Scene graph management                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCENE_MODELCODEC_H
#define NUCLEX_SCENE_MODELCODEC_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"

#include <map>

namespace Nuclex {
  namespace Video { class VideoDevice; class VideoServer; }
  namespace Storage { class Stream; }
}

namespace Nuclex { namespace Scene {

class Model;

//  //
//  Nuclex::Scene::ModelCodec                                                                  //
//  //
/// Model codec
/** Codec for model loaders
*/
class ModelCodec {
  public:
    /// Destructor
    NUCLEX_API virtual ~ModelCodec() {}

  //
  // SceneServer implementation
  //
  public:
    NUCLEX_API virtual bool canLoadModel(
      const shared_ptr<Storage::Stream> &spStream
    ) const = 0;
    
    NUCLEX_API virtual shared_ptr<Model> loadModel(
      const shared_ptr<Storage::Stream> &spStream,
      const shared_ptr<Video::VideoDevice> &spVideoDevice
    ) const = 0;
};

}} // namespace Nuclex::Scene

#endif // NUCLEX_SCENE_MODELCODEC_H
