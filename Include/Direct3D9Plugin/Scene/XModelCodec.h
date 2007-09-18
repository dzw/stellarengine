//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## SceneServer.h - Scene server                        //
// ### # #      ###                                                      //
// # ### #      ###  Scene graph management                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCENE_XMODELCODEC_H
#define NUCLEX_SCENE_XMODELCODEC_H

#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "Nuclex/Scene/ModelCodec.h"

#include <map>

namespace Nuclex { namespace Scene {

//  //
//  Nuclex::Scene::XModelCodec                                           //
//  //
/// X file model codec
/** Model codec which constructs models from directx x files
*/
class XModelCodec :
  public ModelCodec {
  public:
    /// Destructor
    NUCLEXDIRECT3D9_API XModelCodec(const shared_ptr<Video::VideoServer> &spVideoServer) :
      m_spVideoServer(spVideoServer) {}
    /// Destructor
    NUCLEXDIRECT3D9_API virtual ~XModelCodec() {}

  //
  // SceneServer implementation
  //
  public:
    NUCLEXDIRECT3D9_API bool canLoadModel(
      const shared_ptr<Storage::Stream> &spStream
    ) const;
    
    NUCLEXDIRECT3D9_API virtual shared_ptr<Model> loadModel(
      const shared_ptr<Storage::Stream> &spStream,
      const shared_ptr<Video::VideoDevice> &spVideoDevice
    ) const;
  
  private:
    shared_ptr<Video::VideoServer> m_spVideoServer;
};

}} // namespace Nuclex::Scene

#endif // NUCLEX_SCENE_XMODELCODEC_H
