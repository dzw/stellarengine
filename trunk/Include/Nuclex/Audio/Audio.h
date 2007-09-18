//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Audio.h - Audio playback system                                           //
// ### # #      ###                                                                            //
// # ### #      ###  Everything related to audio devices and playback of sounds                //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_AUDIO_AUDIO_H
#define NUCLEX_AUDIO_AUDIO_H

namespace Nuclex {

/// Audio playback
/** Nuclex uses a simplistic audio model. There's an audio server which manages a set of
    audio devices. Each device is a sound output device installed on the system seen through
    a specific api or library. Support for different file formats (eg. ogg, mp3) has been
    tightly integrated into the audio devices. This decision represents the fact that every
    good audio library already provides built-in decoding of common file formats.
*/
namespace Audio {

// Nothing yet

} // namespace Audio

} // namespace Nuclex

#endif // NUCLEX_AUDIO_AUDIO_H
