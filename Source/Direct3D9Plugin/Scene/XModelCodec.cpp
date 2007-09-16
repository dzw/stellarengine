//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## SceneServer.h - Scene server                        //
// ### # #      ###                                                      //
// # ### #      ###  Scene graph management                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#include "Direct3D9Plugin/Scene/XModelCodec.h"
#include "Direct3D9Plugin/Video/Direct3D9VideoDevice.h"
#include "Direct3D9Plugin/Scene/XModel.h"
#include "Nuclex/Storage/FileStream.h"
#include "Nuclex/Storage/Stream.h"
#include "Nuclex/Video/VideoServer.h"
#include "Nuclex/Video/Image.h"
#include "Nuclex/Video/Texture.h"

using namespace Nuclex;
using namespace Nuclex::Video;
using namespace Nuclex::Scene;

// ############################################################################################# //
// # Nuclex::Scene::XModelCodec::canLoadModel()                                                # //
// ############################################################################################# //
bool XModelCodec::canLoadModel(const shared_ptr<Storage::Stream> &spStream) const {
  if(spStream->getSize() < 4)
    return false;

  spStream->seekTo(0);
  return (spStream->read<long>() == 543584120);
}

// ############################################################################################# //
// # Nuclex::Scene::XModelCodec::loadModel()                                                   # //
// ############################################################################################# //
shared_ptr<Model> XModelCodec::loadModel(
  const shared_ptr<Storage::Stream> &spStream,
  const shared_ptr<Video::VideoDevice> &spVideoDevice
) const {
  if(!canLoadModel(spStream))
    throw InvalidArgumentException(
      "Nuclex::Scene::XModelCodec::loadModel()",
      "Unsupported file format"
    );

  // Make sure this is a D3D9 video device
  Direct3D9VideoDevice *pD3D9VD = dynamic_cast<Direct3D9VideoDevice *>(spVideoDevice.get());
  if(!pD3D9VD)
    throw FailedException(
      "Nuclex::Scene::XModelCodec::loadModel()",
      "DirectX X files can only be loaded when using the Direct3D renderer"
    );

  // Load the entire stream containing the X file into memory
  std::vector<unsigned char> XFile(spStream->getSize());
  spStream->seekTo(0);
  spStream->readData(&XFile[0], XFile.size());

  // Construct a mesh from the X file
  DWORD MaterialCount = 0;
  ID3DXBufferPtr spMaterialBuffer;
  ID3DXMeshPtr spMesh;
  D3DCheck(
    "Nuclex::Scene::XModelCodec::loadModel()",
    "D3DXLoadMeshFromXInMemory()",
    D3DXLoadMeshFromXInMemory(
      &XFile[0],
      XFile.size(),
      D3DXMESH_SYSTEMMEM,
      pD3D9VD->getDirect3DDevice(),
      NULL,
      &spMaterialBuffer,
      NULL,
      &MaterialCount,
      &spMesh
    )
  );

  // Prepare all materials associated with the mesh
  D3DXMATERIAL *pMaterials = static_cast<D3DXMATERIAL *>(spMaterialBuffer->GetBufferPointer());
  std::vector<std::pair<D3DMATERIAL9, shared_ptr<Texture> > > Materials(MaterialCount);
  for(DWORD MaterialIndex = 0; MaterialIndex < MaterialCount; ++MaterialIndex) {
    Materials[MaterialIndex].first = pMaterials[MaterialIndex].MatD3D;

    // Does this material use a texture ?
    if(pMaterials[MaterialIndex].pTextureFilename) {

      // Load the image file containing the texture
      shared_ptr<Image> spImage(m_spVideoServer->loadImage(
        shared_ptr<Storage::Stream>(new Storage::FileStream(
          pMaterials[MaterialIndex].pTextureFilename,
          Storage::Stream::AM_READ
        ))
      ));

      // Create a texture in the video device and blit the image onto it
      Materials[MaterialIndex].second = spVideoDevice->createTexture(
        spImage->getSize(),
        VideoDevice::AC_BLEND,
        VideoDevice::RC_STATIC
      );

      spImage->blitTo(Materials[MaterialIndex].second);
    }
  }

  // Success, we can now construct an XModel
  return shared_ptr<Model>(new XModel(spMesh, Materials));
}
