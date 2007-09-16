//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## AudiereAudioDevice.cpp - Audiere audio renderer                           //
// ### # #      ###                                                                            //
// # ### #      ###  Implementation of an audio renderer using the audiere library             //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "AudierePlugin/Audio/AudiereAudioDevice.h"
#include "AudierePlugin/Audio/AudiereSound.h"
#include "AudierePlugin/Audio/AudiereRenderingContext.h"
#include "Nuclex/Storage/Stream.h"

using namespace Nuclex;
using namespace Nuclex::Audio;

namespace {

//  //
//  StreamFile                                                                                 //
//  //
class StreamFile : public audiere::File {
  public:
    StreamFile(const shared_ptr<Storage::Stream> &spStream) :
      m_spStream(spStream) {}
      
    virtual ~StreamFile() {}
    
  //
  // File implementation
  //
  public:
    /// Read size bytes from the file, storing them in buffer.
    int ADR_CALL read(void *pBuffer, int nBytes) {
      return m_spStream->readData(pBuffer, nBytes);
    }

    /// Jump to a new position in the file, using the specified seek mode
    bool ADR_CALL seek(int nPosition, SeekMode eSeekMode) {
      switch(eSeekMode) {
        case BEGIN: {
          m_spStream->seekTo(nPosition);
          break;
        }
        case END: {
          m_spStream->seekTo(m_spStream->getSize() + nPosition);
          break;
        }
        case CURRENT: {
          m_spStream->seekTo(m_spStream->getLocation() + nPosition);
          break;
        }
      }
      return true;
    }
          
    /// Get current position within the file.
    int ADR_CALL tell() {
      return m_spStream->getLocation();
    }

  //
  // RefCounted implementation
  //
  public:
    void ADR_CALL ref() {
      ++m_ref_count;
    }

    void ADR_CALL unref() {
      if(--m_ref_count == 0)
        delete this;
    }

  private:
    shared_ptr<Storage::Stream> m_spStream;
    int m_ref_count;
};

// ############################################################################################# //
// # getSampleRateFromSampleFormat()                                                           # //
// ############################################################################################# //
/** Retrieves the sample rate of the specified sample format

    @param  eSampleFormat  Sample format whose sample rate to get
    @return The format's sample rate
*/
inline size_t getSampleRateFromSampleFormat(Sound::SampleFormat eSampleFormat) {
  switch(eSampleFormat) {
    case Sound::SF_11025_8:
    case Sound::SF_11025_16: {
      return 11025;
    }

    case Sound::SF_22050_8:
    case Sound::SF_22050_16: {
      return 22050;
    }

    case Sound::SF_44100_8:
    case Sound::SF_44100_16: {
      return 44100;
    }

    default: {
      return 0;
    }
  }
}

} // namespace

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDevice::AudiereAudioDevice()                       Constructor # //
// ############################################################################################# //
/** Initializes an instance of AudiereAudioDevice

    @param  sName  Audiere device name
    @param  Mode   Frequency and accuracy setting
*/
AudiereAudioDevice::AudiereAudioDevice(const string &sName, const AudioDriver::OutputMode &Mode) :
  m_sDeviceName(sName),
  m_OutputMode(Mode) {}

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDevice::~AudiereAudioDevice()                       Destructor # //
// ############################################################################################# //
/** Destroys an instance of AudiereAudioDevice
*/
AudiereAudioDevice::~AudiereAudioDevice() {
  closeOutput();
}

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDevice::getOutputMode()                                        # //
// ############################################################################################# //
/** Returns the currently selected output mode

    @return  The current output mode
*/
const AudioDriver::OutputMode &AudiereAudioDevice::getOutputMode() const {
  return m_OutputMode;
}

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDevice::setOutputMode()                                        # //
// ############################################################################################# //
/** Selects a output mode either before the output is opened or tries
    to change the current output mode while the output is open.

    @param  OutputMode  Desired display mode
    @todo Peform a output mode switch if the output was opened
*/
void AudiereAudioDevice::setOutputMode(const AudioDriver::OutputMode &OutputMode) {
  m_OutputMode = OutputMode;
}

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDevice::openOutput()                                           # //
// ############################################################################################# //
/** Opens the display window. This will show the rendering output
    window or enter the selected Audio mode.
*/
void AudiereAudioDevice::openOutput() {
  m_spAudiereDevice = audiere::OpenDevice(m_sDeviceName.c_str());
  m_bOpened = true;
}

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDevice::createSound()                                          # //
// ############################################################################################# //
/** Creates a new sound from the specified source.

    @param  spStream    Stream to load the sound from
    @param  sExtension  A hint about the stream file's extension
    @param  bStreamed   Stream sound ?
    @return The new sound object created
*/
shared_ptr<Sound> AudiereAudioDevice::createSound(const shared_ptr<Storage::Stream> &spStream, bool bStreamed,
                                                    const string &sExtension) {
  // Fix for audiere not recognizing mp3 files with id3v2 headers
  audiere::FileFormat eFileFormat = audiere::FF_AUTODETECT;
/*  
  if(spStream->getSize() >= 3) {
    unsigned char id[3];
    spStream->readData(id, 3);
    spStream->seekTo(0);
    
    if((id[0] == 'I') && (id[1] == 'D') && (id[2] == '3'))
      eFileFormat = audiere::FF_MP3;
  }
*/  
  return shared_ptr<Sound>(new AudiereSound(
    audiere::OpenSound(m_spAudiereDevice, new StreamFile(spStream), bStreamed, eFileFormat)
  ));
}

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDevice::closeOutput()                                          # //
// ############################################################################################# //
/** Closes the display if opened
*/
void AudiereAudioDevice::closeOutput() {
  m_bOpened = false;
  m_spAudiereDevice = NULL;
}

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDevice::isOutputOpen()                                         # //
// ############################################################################################# //
/** Returns true if the output is currently open

    @return True if the output is currently open
*/
bool AudiereAudioDevice::isOutputOpen() const {
  return m_bOpened;
}

// ############################################################################################# //
// # Nuclex::Audio::AudiereAudioDevice::getAudioContext()                                      # //
// ############################################################################################# //
/** Retrieves the audio renderers audio context

    @return The audio context
*/
shared_ptr<AudioDevice::RenderingContext> AudiereAudioDevice::renderFrame() {
  return shared_ptr<AudioDevice::RenderingContext>(new AudiereRenderingContext(this));
}
