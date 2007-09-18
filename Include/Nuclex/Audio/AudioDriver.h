//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## AudioDriver.h - Audio device                                              //
// ### # #      ###                                                                            //
// # ### #      ###  Sound output device which can play back various sound file formats        //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_AUDIO_AUDIODRIVER_H
#define NUCLEX_AUDIO_AUDIODRIVER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Audio/Sound.h"
#include "Nuclex/Support/Variant.h"

namespace Nuclex { namespace Audio {

class AudioDevice;

//  //
//  Nuclex::Audio::AudioDriver                                                                 //
//  //
/// Audio device
/** 
*/
class AudioDriver {
  public:
    /// Enumerator for the available display modes
    class OutputModeEnumerator;

    /// Display mode information
    /** Holds informations about a wave mode
    */
    struct OutputMode {
      /// Constructor
      NUCLEX_API OutputMode(Sound::SampleFormat eFormat, size_t nChannels) :
        eFormat(eFormat), nChannels(nChannels) {}
      /// Get name of output mode
      NUCLEX_API string getName() {
        string sName;
        
        switch(eFormat) {
          case Sound::SF_11025_8:  sName = " 8 Bits in 11 KHz"; break;
          case Sound::SF_11025_16: sName = "16 Bits in 11 KHz"; break;
          case Sound::SF_22050_8:  sName = " 8 Bits in 22.5 KHz"; break;
          case Sound::SF_22050_16: sName = "16 Bits in 22.5 KHz"; break;
          case Sound::SF_44100_8:  sName = " 8 Bits in 44.1 KHz"; break;
          case Sound::SF_44100_16: sName = "16 Bits in 44.1 KHz"; break;
          default:                 sName = "Unknown"; break;
        }
        
        return sName + " on " + lexical_cast<string>(nChannels) + " channels";
      }

      Sound::SampleFormat eFormat;                    ///< Audio sample format
      size_t              nChannels;                  ///< Number of channels
    };

    /// Destructor
    NUCLEX_API virtual ~AudioDriver() {}

  //
  // AudioDriver implementation
  //
  public:
    /// Create device
    NUCLEX_API virtual shared_ptr<AudioDevice> createDevice() = 0;

    /// Get an enumerator over all available output modes
    /** Can be used to enumerate all output modes the device supports

        @return A new output mode enumerator
    */
    NUCLEX_API virtual shared_ptr<OutputModeEnumerator> enumOutputModes() const = 0;

    /// Get device name
    /** Returns a human readable name uniquely describing the 
        device. Should include the sound card used.

        @return The device's name
    */
    NUCLEX_API virtual const string &getName() const = 0;
};

//  //
//  Nuclex::Audio::AudioDriver::OutputModeEnumerator                                           //
//  //
/** Enumerates over a list of display modes
*/
class AudioDriver::OutputModeEnumerator {
  public:
    /// Destructor
    /** Destroys an instance of IWaveModeEnumerator
    */
    NUCLEX_API virtual ~OutputModeEnumerator() {}

  //
  // OutputModeEnumerator implementation
  //
  public:
    /// Cycle through all output modes
    /** Returns the current output mode and advances to the next one.
        If no more output modes are available, returns NULL

        @return The current output mode being enumerated
    */
    NUCLEX_API virtual const OutputMode *cycle(void) = 0;
};

}} // namespace Nuclex::Audio

#endif // NUCLEX_AUDIO_AUDIODRIVER_H
