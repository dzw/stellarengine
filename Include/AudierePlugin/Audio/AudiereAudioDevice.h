//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## AudiereAudioDevice.h - Audiere audio renderer                             //
// ### # #      ###                                                                            //
// # ### #      ###  Implementation of an audio renderer using the audiere library             //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_AUDIO_AUDIEREAUDIORENDERER_H
#define NUCLEX_AUDIO_AUDIEREAUDIORENDERER_H

#include "AudierePlugin/AudierePlugin.h"
#include "AudierePlugin/Audio/AudiereAudioDriver.h"
#include "Nuclex/Audio/AudioDevice.h"
#include "Audiere/Audiere.h"

namespace Nuclex { namespace Audio {

class AudiereRenderingContext;

//  //
//  Nuclex::Audio::AudiereAudioDevice                                                          //
//  //
/// Audiere audio renderer
/** 
*/
class AudiereAudioDevice :
  public AudioDevice {
  public:
    /// Constructor
    NUCLEXAUDIERE_API AudiereAudioDevice(
      const string &sName, const AudioDriver::OutputMode &Mode
    );
    /// Destructor
    NUCLEXAUDIERE_API ~AudiereAudioDevice();

  //
  // AudiereAudioDevice implementation
  //
  public:
    /// Get output window handle
    NUCLEXAUDIERE_API HWND getWindow() const;
    /// Set output window handle
    NUCLEXAUDIERE_API void setWindow(HWND hWnd);

  //
  // AudioDevice implementation
  //
  public:
    /// Select an output mode to use
    NUCLEXAUDIERE_API void setOutputMode(const AudioDriver::OutputMode &OutputMode);
    /// Retrieve the currently set output mode
    NUCLEXAUDIERE_API const AudioDriver::OutputMode &getOutputMode() const;

    /// Open the output
    NUCLEXAUDIERE_API void openOutput();
    /// Close the output
    NUCLEXAUDIERE_API void closeOutput();
    /// Check whether the output is open
    NUCLEXAUDIERE_API bool isOutputOpen() const;

    /// Get audio context
    NUCLEXAUDIERE_API shared_ptr<RenderingContext> renderFrame();

    /// Create new sound
    NUCLEXAUDIERE_API shared_ptr<Sound> createSound(
      const shared_ptr<Storage::Stream> &spStream, 
      bool bStreamed = false,
      const string &sExtension = ""
    );

  private:
    audiere::AudioDevicePtr m_spAudiereDevice;        ///< Audiere audio device
    AudioDriver::OutputMode m_OutputMode;             ///< Output mode
    string                  m_sDeviceName;            ///< Audiere device name
    bool                    m_bOpened;                ///< Device opened ?
};

}} // namespace Nuclex::Audio

#endif // NUCLEX_AUDIO_AUDIEREAUDIORENDERER_H
