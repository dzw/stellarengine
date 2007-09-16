//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## AudioServer.cpp - Audio device management                                 //
// ### # #      ###                                                                            //
// # ### #      ###  Manages audio devices supplied through plugins                            //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Audio/AudioServer.h"
#include "Nuclex/Support/Exception.h"

using namespace Nuclex;
using namespace Nuclex::Audio;

namespace {

//  //
//  Nuclex::Audio::AudioDriverEnumerator                                                       //
//  //
/// Driver enumerator for Audio server
/** Enumerates over all devices registered to the Audio server
*/
class AudioDriverEnumerator :
  public AudioServer::DriverEnumerator {
  typedef std::map<string, shared_ptr<AudioDriver> > DriverMap;
  public:
    /// Constructor
    /** Initializes an instance of AudioDriverEnumerator

        @param  pDriverMap  Drivers to enumerate
    */
    AudioDriverEnumerator(DriverMap &Drivers) :
      m_Drivers(Drivers),
      m_DriverIt(Drivers.begin()),
      m_DriverEnd(Drivers.end()) {}

    /// Destructor
    /** Destroys an instance of AudioDriverEnumerator
    */
    virtual ~AudioDriverEnumerator() {}

  //
  // AudioDriverEnumerator implementation
  //
  public:
    /// Cycle through renderers
    /** Returns the current renderer being enumerated and advances
        to the next. If no more renderers are remaning, NULL is returned

        @return The currently enumerated renderer
    */
    bool next() {
      if(m_DriverIt == m_DriverEnd)
        m_DriverIt = m_Drivers.begin();
      else  
        ++m_DriverIt;
        
      return (m_DriverIt != m_DriverEnd);
    }
    
    const shared_ptr<AudioDriver> &get() const {
      if(m_DriverIt == m_DriverEnd)
        throw FailedException("AudioDriverEnumerator::get()",
                              "Enumerator is in empty cycle position");

      return m_DriverIt->second;
    }

  private:
    DriverMap                 &m_Drivers;             ///< Driver map
    DriverMap::const_iterator  m_DriverIt;            ///< Current renderer
    DriverMap::const_iterator  m_DriverEnd;           ///< List end
};

} // namespace

// ############################################################################################# //
// # Nuclex::Audio::AudioServer::~AudioServer()                                     Destructor # //
// ############################################################################################# //
/** Destroys an instance of AudioServer
*/
AudioServer::~AudioServer() {
  clearDrivers();
}

// ############################################################################################# //
// # Nuclex::Audio::AudioServer::getDrivers()                                                  # //
// ############################################################################################# //
/** Retrieves a Audio device by name

    @param  sName     Name of the Audio device to retrieve
    @return The Audio device or NULL if not found
*/
const shared_ptr<AudioDriver> &AudioServer::getDriver(const string &sName) const {
  DriverMap::const_iterator DriverIt = m_Drivers.find(sName);
  if(DriverIt == m_Drivers.end())
    throw InvalidArgumentException("Nuclex::Audio::AudioServer::getDriver()",
                                   string("Driver not found: '") + sName + "'");

  return DriverIt->second;
}

// ############################################################################################# //
// # Nuclex::Audio::AudioServer::addDriver()                                                   # //
// ############################################################################################# //
/** Add a new Audio device.

    @param  sName     Name of the Audio device
    @param  spDriver  The Audio device to add
*/
void AudioServer::addDriver(const string &sName, const shared_ptr<AudioDriver> &spDriver) {
  DriverMap::iterator DriverIt = m_Drivers.find(sName);
  if(DriverIt != m_Drivers.end())
    DriverIt->second = spDriver;
  else
    m_Drivers.insert(DriverMap::value_type(sName, spDriver));
}

// ############################################################################################# //
// # Nuclex::Audio::AudioServer::removeDriver()                                                # //
// ############################################################################################# //
/** Removes a Audio device previously added using addDriver()

    @param  sName  Name of the Audio device to remove
*/
void AudioServer::removeDriver(const string &sName) {
  DriverMap::iterator DriverIt = m_Drivers.find(sName);
  if(DriverIt != m_Drivers.end())
    m_Drivers.erase(DriverIt);
  else
    throw InvalidArgumentException("Nuclex::Audio::AudioServer::getDriver()",
                                   string("Driver not found: '") + sName + "'");
}

// ############################################################################################# //
// # Nuclex::Audio::AudioServer::clearDrivers()                                                # //
// ############################################################################################# //
/** Removes all Audio devices currently added to the Audio server
*/
void AudioServer::clearDrivers() {
  m_Drivers.clear();
}

// ############################################################################################# //
// # Nuclex::Audio::AudioServer::enumDrivers()                                                 # //
// ############################################################################################# //
/** Returns a new enumerator over all Audio devices registered to
    the Audio server.

    @return A new enumerator over all Audio devices
*/
shared_ptr<AudioServer::DriverEnumerator> AudioServer::enumDrivers() {
  return shared_ptr<AudioServer::DriverEnumerator>(new AudioDriverEnumerator(m_Drivers));
}
