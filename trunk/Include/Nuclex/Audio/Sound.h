//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Sound.h - Active sound                                                    //
// ### # #      ###                                                                            //
// # ### #      ###  Controls a sound while it is playing                                      //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_AUDIO_SOUND_H
#define NUCLEX_AUDIO_SOUND_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Point3.h"

namespace Nuclex { namespace Audio {

//  //
//  Nuclex::Audio::Sound                                                                       //
//  //
/// Active sound
/** Controls a sound that is playing in 3D space with location und velocity.
*/
class Sound {
  public:
    enum SampleFormat {
      SF_NONE = 0,                                    ///< None or unknown
      SF_11025_8,                                     ///< 11025 Hz 8 Bit
      SF_22050_8,                                     ///< 22050 Hz 8 Bit
      SF_44100_8,                                     ///< 44100 Hz 8 Bit
      SF_11025_16,                                    ///< 11025 Hz 16 Bit
      SF_22050_16,                                    ///< 22050 Hz 16 Bit
      SF_44100_16                                     ///< 44100 Hz 16 Bit
    };
    
    /// Get *bytes* per second from sample format
    NUCLEX_API inline static size_t bpsFromFormat(SampleFormat eFormat);
    /// Destructor
    NUCLEX_API virtual ~Sound() {}
    
  //
  // ISound implementation
  //
  public:
    /// Retrieve the sound's current position
    /** Returns the sound's position

        @return  The sound's current position
    */
    NUCLEX_API virtual const Point3<real> &getPosition() const = 0;
    /// Sets the sound's position
    /** Moves the sound source to the specified position

        @param  Position  New sound source position
    */
    NUCLEX_API virtual void setPosition(const Point3<real> &Position) = 0;

    /// Retrieve the sound's current velocity
    /** Returns the sound's velocity

        @return  The sound's current velocity
    */
    NUCLEX_API virtual const Vector3<real> &getVelocity() const = 0;
    /// Sets the sound's velocity
    /** Moves the sound source to the specified velocity

        @param  Velocity  New sound source velocity
    */
    NUCLEX_API virtual void setVelocity(const Vector3<real> &Velocity) = 0;
};

// ############################################################################################# //
// # Nuclex::Audio::Sound::bpsFromFormat()                                                     # //
// ############################################################################################# //
/** Retrieves the number of *bytes* per second of the specified format

    @param  eFormat  Format of which to retrieve the bps
    @return The number of *bytes* per second in the specified format
*/
inline size_t Sound::bpsFromFormat(SampleFormat eFormat) {
  switch(eFormat) {
    case Sound::SF_11025_8:  return 11025;
    case Sound::SF_22050_8:  return 22050;
    case Sound::SF_44100_8:  return 44100;
    case Sound::SF_11025_16: return 11025 * 2;
    case Sound::SF_22050_16: return 22050 * 2;
    case Sound::SF_44100_16: return 44100 * 2;
    default:                 return 0;
  }
}

}} // namespace Nuclex::Audio

#endif // NUCLEX_AUDIO_SOUND_H
