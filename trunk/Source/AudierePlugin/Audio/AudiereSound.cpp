//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## AudiereSound.cpp - Audiere sound                                          //
// ### # #      ###                                                                            //
// # ### #      ###  Implementation of an active sound using the audiere library               //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "AudierePlugin/Audio/AudiereSound.h"

using namespace Nuclex;
using namespace Nuclex::Audio;

// ############################################################################################# //
// # Nuclex::Audio::AudiereSound::AudiereSound()                                   Constructor # //
// ############################################################################################# //
/** Initializes an instance of CAudiereSound
*/
AudiereSound::AudiereSound(const audiere::OutputStreamPtr &OutputStream) :
  m_spOutputStream(OutputStream) {
  m_spOutputStream->play();
}

// ############################################################################################# //
// # Nuclex::Audio::AudiereSound::~AudiereSound()                                   Destructor # //
// ############################################################################################# //
/** Destroys an instance of CAudiereSound
*/
AudiereSound::~AudiereSound() {}

// ############################################################################################# //
// # Nuclex::Audio::AudiereSound::setPosition()                                                # //
// ############################################################################################# //
/** Returns the sound's position

    @return  The sound's current position
*/
void AudiereSound::setPosition(const Point3<real> &Position) {
  //m_spOutputStream->setPan(Position.X);
}

// ############################################################################################# //
// # Nuclex::Audio::AudiereSound::setVelocity()                                                # //
// ############################################################################################# //
/** Returns the sound's Velocity

    @return  The sound's current Velocity
*/
void AudiereSound::setVelocity(const Vector3<real> &Velocity) {
  //m_spOutputStream->setPitchShift((m_Position - Velocity).getLength());
}
