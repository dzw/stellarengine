//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## AudiereRenderingContext.cpp - Audiere rendering context                   //
// ### # #      ###                                                                            //
// # ### #      ###  Context class for sound update frames                                     //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "AudierePlugin/Audio/AudiereRenderingContext.h"

using namespace Nuclex;
using namespace Nuclex::Audio;

AudiereRenderingContext::AudiereRenderingContext(AudiereAudioDevice *pAudioDevice) :
  m_pAudioDevice(pAudioDevice) {}
