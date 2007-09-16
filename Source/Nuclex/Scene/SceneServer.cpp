//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## SceneNode.cpp - Base class for nodes in the scene graph                   //
// ### # #      ###                                                                            //
// # ### #      ###  Provides the basic definition of a scene graph node using a quad          //
// #  ## #   # ## ## linked tree architecture                                                  //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Scene/SceneServer.h"
#include "Nuclex/Scene/ModelCodec.h"
#include "Nuclex/Storage/Stream.h"

using namespace Nuclex;
using namespace Nuclex::Scene;

SceneServer::SceneServer() :
  m_eTimeModel(TM_DELTA),
  m_TimeStepSize(10000),
  m_bResetTimer(true) {}

SceneServer::~SceneServer() {}

// ############################################################################################# //
// # Nuclex::Scene::SceneServer::getModelCodec()                                               # //
// ############################################################################################# //
/** Retrieves a video device by name

    @param  sName     Name of the video device to retrieve
    @return The video device or NULL if not found
*/
const shared_ptr<ModelCodec> &SceneServer::getModelCodec(const string &sName) const {
  ModelCodecMap::const_iterator ModelCodecIt = m_ModelCodecs.find(sName);
  if(ModelCodecIt == m_ModelCodecs.end())
    throw InvalidArgumentException(
      "Nuclex::Scene::SceneServer::getModelCodec()",
      string("Item not found: '") + sName + "'"
    );

  return ModelCodecIt->second;
}

// ############################################################################################# //
// # Nuclex::Scene::SceneServer::addModelCodec()                                               # //
// ############################################################################################# //
/** Add a new video device.

    @param  sName     Name of the video device
    @param  spModelCodec  The video device to add
*/
void SceneServer::addModelCodec(const string &sName, const shared_ptr<ModelCodec> &spModelCodec) {
  ModelCodecMap::iterator ModelCodecIt = m_ModelCodecs.find(sName);
  if(ModelCodecIt != m_ModelCodecs.end())
    ModelCodecIt->second = spModelCodec;
  else
    m_ModelCodecs.insert(ModelCodecMap::value_type(sName, spModelCodec));
}

// ############################################################################################# //
// # Nuclex::Scene::SceneServer::removeModelCodec()                                            # //
// ############################################################################################# //
/** Removes a video device previously added using addModelCodec()

    @param  sName  Name of the video device to remove
*/
void SceneServer::removeModelCodec(const string &sName) {
  ModelCodecMap::iterator ModelCodecIt = m_ModelCodecs.find(sName);
  if(ModelCodecIt == m_ModelCodecs.end())
    throw InvalidArgumentException(
      "Nuclex::Scene::SceneServer::getModelCodec()",
      string("Item not found: '") + sName + "'"
    );

  m_ModelCodecs.erase(ModelCodecIt);
}

// ############################################################################################# //
// # Nuclex::Scene::SceneServer::clearModelCodecs()                                            # //
// ############################################################################################# //
/** Removes all video devices currently added to the video server
*/
void SceneServer::clearModelCodecs() {
  m_ModelCodecs.clear();
}

// ############################################################################################# //
// # Nuclex::Scene::SceneServer::enumModelCodecs()                                             # //
// ############################################################################################# //
/** Returns a new enumerator over all video devices registered to
    the video server.

    @return A new enumerator over all video devices
*/
shared_ptr<SceneServer::ModelCodecEnumerator> SceneServer::enumModelCodecs() const {
  throw NotSupportedException(
    "Nuclex::Scene::SceneServer::enumModelCodecs()",
    "Not implemented yet"
  );
//  return shared_ptr<SceneServer::ModelCodecEnumerator>(new ModelCodecEnumerator(m_ModelCodecs));
}

// ############################################################################################# //
// # Nuclex::Scene::SceneServer::loadModel()                                                   # //
// ############################################################################################# //
/** Loads an Scene from a stream

    @param  spSource       Source stream from which to load
    @param  spVideoDevice  Video device to load model on
    @return The loaded model
*/
shared_ptr<Model> SceneServer::loadModel(
  const shared_ptr<Storage::Stream> &spSource,
  const shared_ptr<Video::VideoDevice> &spVideoDevice
) {
                                         
  ModelCodecMap::const_iterator CodecEnd = m_ModelCodecs.end();
  for(ModelCodecMap::const_iterator CodecIt = m_ModelCodecs.begin();
      CodecIt != CodecEnd;
      CodecIt++)
    if(CodecIt->second->canLoadModel(spSource))
      return CodecIt->second->loadModel(spSource, spVideoDevice);

  // If no codec could load the Scene, raise an error
  throw UnsupportedModelFormatException(
    "Nuclex::Scene::SceneServer::loadModel()",
    string("Unsupported model file format for '") + spSource->getName() + "'"
  );
}

// ############################################################################################# //
// # Nuclex::Scene::SceneServer::renderFrame()                                                 # //
// ############################################################################################# //
void SceneServer::renderFrame(
  const shared_ptr<Video::VideoDevice> &spVideoDevice,
  const shared_ptr<Audio::AudioDevice> &spAudioDevice
) {
  shared_ptr<Video::VideoDevice::RenderingContext> spVideoRC;
  shared_ptr<Audio::AudioDevice::RenderingContext> spAudioRC;
  
  // Render a new frame
  spVideoRC = spVideoDevice->renderFrame();
  
  if(spAudioDevice)
    spAudioRC = spAudioDevice->renderFrame();

  if(spVideoRC || spAudioRC)
    OnRenderFrame(FrameContext(spVideoRC, spAudioRC));
}

// ############################################################################################# //
// # Nuclex::Scene::SceneServer::timeFrame()                                                   # //
// ############################################################################################# //
void SceneServer::timeFrame() {
  TimeSpan CurrentTime = TimeSpan::getRunningTime();

  if(m_bResetTimer) {
    m_bResetTimer = false;
    m_PreviousStepTime = CurrentTime;

    return;
  }

  switch(m_eTimeModel) {
    case TM_DELTA: {
      OnTimeFrame(CurrentTime - m_PreviousStepTime);
      m_PreviousStepTime = CurrentTime;

      break;
    }
    case TM_CAPPED_DELTA: {
      TimeSpan DeltaTime = CurrentTime - m_PreviousStepTime;
      while(DeltaTime > m_TimeStepSize) {
        OnTimeFrame(m_TimeStepSize);
        DeltaTime -= m_TimeStepSize;
      }

      if(DeltaTime > 0)
        OnTimeFrame(DeltaTime);

      break;
    }
    case TM_STEPS: {
      while((CurrentTime - m_PreviousStepTime) > m_TimeStepSize) {
        OnTimeFrame(m_TimeStepSize);
        m_PreviousStepTime += m_TimeStepSize;
      }

      break;
    }
  }
}