//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## AudiereAudioRenderer.h - Audiere rendering context                        //
// ### # #      ###                                                                            //
// # ### #      ###  Context class for sound update frames                                     //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_AUDIO_AUDIERERENDERINGCONTEXT_H
#define NUCLEX_AUDIO_AUDIERERENDERINGCONTEXT_H

#include "AudierePlugin/AudierePlugin.h"
#include "AudierePlugin/Audio/AudiereAudioDevice.h"

namespace Nuclex { namespace Audio {

//  //
//  Nuclex::Audio::AudiereRenderingContext                                                     //
//  //
/// NuclexGame renderer base class
/** 
*/
class AudiereRenderingContext : public AudioDevice::RenderingContext {
  public:
    /// Constructor
    NUCLEXAUDIERE_API AudiereRenderingContext(AudiereAudioDevice *pAudioDevice);
    /// Destructor
    NUCLEXAUDIERE_API virtual ~AudiereRenderingContext() {}
    
  //
  // AudioRenderer implementation
  //
  public:
    // setEcho()
    // set...

  private:
    AudiereAudioDevice *m_pAudioDevice;
};

}} // namespace Nuclex::Audio

#endif // NUCLEX_AUDIO_AUDIERERENDERINGCONTEXT_H
