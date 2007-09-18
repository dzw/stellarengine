//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## AudioDevice.h - Audio device                                              //
// ### # #      ###                                                                            //
// # ### #      ###  Instance of an audio device which can be used for actual                  //
// #  ## #   # ## ## sound output and sound file format decoding                               //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_AUDIO_AUDIODEVICE_H
#define NUCLEX_AUDIO_AUDIODEVICE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Audio/AudioDriver.h"
#include "Nuclex/Audio/Sound.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/Support/Cacheable.h"

namespace Nuclex {
  namespace Storage { class Stream; }
}

namespace Nuclex { namespace Audio {

//  //
//  Nuclex::Audio::AudioDevice                                                                 //
//  //
/// Audio renderer
/** 
*/
class AudioDevice :
  public Cacheable {
  public:
    /// Invalid or not supported Audio mode
    NUCLEX_DECLAREEXCEPTION(InvalidArgumentException, UnsupportedAudioModeException);

    /// Audio rendering context
    class RenderingContext;

    /// Destructor
    NUCLEX_API virtual ~AudioDevice() {}
    
  //
  // AudioDevice implementation
  //
  public:
    /// Retrieve the currently set output mode
    /** Returns the currently selected output mode

        @return  The current output mode
    */
    NUCLEX_API virtual const AudioDriver::OutputMode &getOutputMode() const = 0;
    /// Select an output mode to use
    /** Selects an output mode either before the rendering output
        is opened or tries to change the current output mode while the
        rendering output is open.

        @param  OutputMode  Desired output mode
    */
    NUCLEX_API virtual void setOutputMode(const AudioDriver::OutputMode &OutputMode) = 0;

    /// Open the wave output
    /** Opens the wave output. This will access the wave output device
        using the selected wave mode.
    */
    NUCLEX_API virtual void openOutput() = 0;
    /// Close the wave output
    /** Closes the wave output if opened
    */
    NUCLEX_API virtual void closeOutput() = 0;
    /// Check whether the wave output is open
    /** Returns true if the wave output is currently open

        @return True if the wave output is currently open
    */
    NUCLEX_API virtual bool isOutputOpen() const = 0;

    /// Get audio context
    /** Retrieves the renderer's audio context
    */
    NUCLEX_API virtual shared_ptr<RenderingContext> renderFrame() = 0;

    /// Create new sound
    /** Creates a new sound from the specified source.

        @param  spSource    Stream to load the sound from
        @param  sExtension  A hint about the stream file's extension
        @param  bStreamed   Stream sound ?
        @return The new sound object created
    */
    NUCLEX_API virtual shared_ptr<Sound> createSound(
      const shared_ptr<Storage::Stream> &spSource, 
      bool bStreamed = false,
      const string &sExtension = ""
    ) = 0;

    // doppler factor
    // rolloff
    // frequency/pitch shift

};

//  //
//  Nuclex::Audio::AudioDevice::RenderingContext                                               //
//  //
/** 
*/
class AudioDevice::RenderingContext {
  public:
    /// Destructor
    NUCLEX_API virtual ~RenderingContext() {}
    
  //
  // AudioRenderer implementation
  //
  public:
    // TODO: Add environmental settings
    // -Echo
    // -Pitch
    // etc.
};

}} // namespace Nuclex::Audio

#endif // NUCLEX_AUDIO_AUDIODEVICE_H
