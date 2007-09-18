//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## AudiereSound.h - Audiere sound                                            //
// ### # #      ###                                                                            //
// # ### #      ###  Implementation of an active sound using the audiere library               //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_AUDIO_AUDIERESOUND_H
#define NUCLEX_AUDIO_AUDIERESOUND_H

#include "AudierePlugin/AudierePlugin.h"
#include "Nuclex/Audio/Sound.h"
#include "Audiere/Audiere.h"

namespace Nuclex {
  namespace Storage { class Stream; }
}

namespace Nuclex { namespace Audio {

//  //
//  Nuclex::Audio::AudiereSound                                                                //
//  //
/// Audiere sound
/** 
*/
class AudiereSound :
  public Sound {
  public:
    /// Constructor
    NUCLEXAUDIERE_API AudiereSound(const audiere::OutputStreamPtr &OutputStream);
    /// Destructor
    NUCLEXAUDIERE_API virtual ~AudiereSound();
    
  //
  // AudiereSound implementation
  //
  public:
    /// Retrieve the AudiereSound's current position
    NUCLEXAUDIERE_API const Point3<real> &getPosition() const { return m_Position; }
    /// Sets the AudiereSound's position
    NUCLEXAUDIERE_API void setPosition(const Point3<real> &Position);

    /// Retrieve the sound's current velocity
    NUCLEXAUDIERE_API const Vector3<real> &getVelocity() const { return m_Velocity; }
    /// Sets the sound's velocity
    NUCLEXAUDIERE_API void setVelocity(const Vector3<real> &Velocity);

  private:
    audiere::OutputStreamPtr m_spOutputStream;        ///< Audiere stream
    Point3<real>     m_Position;                      ///< Sound position
    Vector3<real>    m_Velocity;                      ///< Sound velocity
};

}} // namespace Nuclex::Audio

#endif // NUCLEX_AUDIO_AUDIERESOUND_H
