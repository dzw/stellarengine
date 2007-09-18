//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## SceneServer.h - Scene server                        //
// ### # #      ###                                                      //
// # ### #      ###  Scene graph management                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SCENE_SCENESERVER_H
#define NUCLEX_SCENE_SCENESERVER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Video/VideoDevice.h"
#include "Nuclex/Audio/AudioDevice.h"
#include "Nuclex/Math/MatrixStack.h"
#include "Nuclex/Support/TimeSpan.h"
#include "SigC++/SigC++.h"

#include <map>

namespace Nuclex {
  namespace Video { class VideoServer; }
}

namespace Nuclex { namespace Scene {

class ModelCodec;
class Model;

//  //
//  Nuclex::Scene::SceneServer                                                                 //
//  //
/// Scene server
/**
*/
class SceneServer {
  public:
    /// An unsupported model file format was used
    NUCLEX_DECLAREEXCEPTION(UnsupportedFormatException, UnsupportedModelFormatException);

    /// Timing models
    /** To make a real-time simulation (such as a game) progress independently of the
        underlying machine's speed, movement has to be performed based on time. There
        are two major methods to achieve this, both of which are supported by nuclex
        as well as a third, hybrid method especially adequate for games with complex
        physics engines.

        @param  TM_DELTA         Scale all movements by the time passed since the last update was
                                 performed. Not suited for multiplayer games, since rounding
                                 differences due to varying machine speeds will make the game go
                                 out of sync. Physics simulations risk to "explode" due to the
                                 possibility of very large delta times.
        @param  TM_CAPPED_DELTA  Same as TM_DELTA, except that delta values which exceed a
                                 specific capping value are split into multiple smaller updates,
                                 thus eliminating the risk of physics simulations to cause
                                 an explosion. Not suited for multiplayer games as well.
        @param  TM_STEPS         Use a fixed stepping size and execute updates once whenever
                                 the amount of time specified as step size has passed. Especially
                                 suited for multiplayer games since all machines will perform
                                 updates with the exact same stepping value, causing equal
                                 rounding errors everywhere.
    */
    enum TimeModel {
      TM_DELTA,
      TM_CAPPED_DELTA,
      TM_STEPS
    };

    struct FrameContext;
    class ModelCodecEnumerator;

    typedef SigC::Signal1<void, const TimeSpan &> TimeFrameSignal;
    typedef SigC::Signal1<void, const FrameContext &> RenderFrameSignal;
  
    /// Constructor
    NUCLEX_API SceneServer();                    
    /// Destructor
    NUCLEX_API virtual ~SceneServer();

  //
  // SceneServer implementation
  //
  public:
    /// Retrieve model codec
    NUCLEX_API const shared_ptr<ModelCodec> &getModelCodec(const string &sName) const;
    /// Add model codec
    NUCLEX_API void addModelCodec(
      const string &sName, const shared_ptr<ModelCodec> &spModelCodec
    );
    /// Remove model codec
    NUCLEX_API void removeModelCodec(const string &sName);
    /// Remove all model codecs
    NUCLEX_API void clearModelCodecs();
    /// Get model codec enumerator
    NUCLEX_API shared_ptr<ModelCodecEnumerator> enumModelCodecs() const;
    /// Load model from stream
    NUCLEX_API shared_ptr<Model> loadModel(
      const shared_ptr<Storage::Stream> &spStream,
      const shared_ptr<Video::VideoDevice> &spVideoDevice
    );

    /// Returns the currently selected time model
    NUCLEX_API TimeModel getTimeModel() const { return m_eTimeModel; }
    /// Selects a different time model
    NUCLEX_API void setTimeModel(TimeModel eTimeModel) { m_eTimeModel = eTimeModel; }

    /// Obtains the desired time step size
    /** Depending on the selected time model, the time step size will either
        define the amount of time after which a step will be performed or the
        maximum step size after which a step will be broken down into shorter steps
        @see TimeModel
    */
    NUCLEX_API const TimeSpan &getTimeStepSize() const { return m_TimeStepSize; }
    /// Changes the desired time step size
    /** Depending on the selected time model, the time step size will either
        define the amount of time after which a step will be performed or the
        maximum step size after which a step will be broken down into shorter steps
        @see TimeModel
    */
    NUCLEX_API void setTimeStepSize(const TimeSpan &StepSize) { m_TimeStepSize = StepSize; }

    NUCLEX_API void resetTimer() {
      m_bResetTimer = true;
    }

    /// Performs a rendering frame
    NUCLEX_API void renderFrame(
      const shared_ptr<Video::VideoDevice> &spVideoDevice,
      const shared_ptr<Audio::AudioDevice> &spAudioDevice = shared_ptr<Audio::AudioDevice>()
    );
    /// Performs a time frame
    NUCLEX_API void timeFrame();

    /// Signal for each time frame
    TimeFrameSignal OnTimeFrame;
    /// Signal for each rendering frame
    RenderFrameSignal OnRenderFrame;

    // Model
    //   AnimatedModel / DynamicModel
    //
    // Where to implement ModelTemplate / ModelInstance differentiation ?  
    //
    //   StaticModel
    //     (Or use Model for StaticModel ?)
    //
    // World geometry also a model ?
    //   or Environment, World, BuildingBlock, Chunk, Region, ... ?


    // Model
    //   Instance
    //
    // AnimatedModel : Model
    //   Instance : Model::Instance
    //
    // ModelTemplate -> Model
    // Model -> ModelInstance
    // 

    //

  private:
    typedef std::map<string, shared_ptr<ModelCodec> > ModelCodecMap;
    
    ModelCodecMap m_ModelCodecs;
    TimeModel     m_eTimeModel;
    TimeSpan      m_TimeStepSize;
    TimeSpan      m_PreviousStepTime;
    bool          m_bResetTimer;
};

//  //
//  Nuclex::Scene::SceneServer::FrameContext                                                   //
//  //
/// Rendering frame context
/** Encloses informations required for rendering a frame
*/
struct SceneServer::FrameContext {
  FrameContext(
    shared_ptr<Video::VideoDevice::RenderingContext> &spVideoRC,
    shared_ptr<Audio::AudioDevice::RenderingContext> &spAudioRC
  ) :
    spVideoRC(spVideoRC),
    spAudioRC(spAudioRC) {}
    
  const shared_ptr<Video::VideoDevice::RenderingContext> &spVideoRC;
  const shared_ptr<Audio::AudioDevice::RenderingContext> &spAudioRC;
  MatrixStack<float> WorldMatrices;
};

}} // namespace Nuclex::Scene

#endif // NUCLEX_SCENE_SCENESERVER_H
