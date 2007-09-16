//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## Direct3D9VideoDriver.cpp - Direct3D9 video device   //
// ### # #      ###                                                      //
// # ### #      ###  A video device using Direct3D9 for rendering        //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R8             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "Direct3D9Plugin/Video/Direct3D9VideoDriver.h"
#include "Direct3D9Plugin/Video/Direct3D9VideoDevice.h"
#include "DirectX/DXErr9.h"
//#include <multimon.h>

using namespace Nuclex;
using namespace Nuclex::Video;

namespace {

const D3DFORMAT SupportedD3DFormats[] = {
//  D3DFMT_R3G3B2,
  D3DFMT_R5G6B5,
  D3DFMT_R8G8B8,
  D3DFMT_X1R5G5B5,
  D3DFMT_X8R8G8B8,
  D3DFMT_A1R5G5B5,
  D3DFMT_A4R4G4B4,
  D3DFMT_A8R8G8B8
};
const size_t NumSupportedD3DFormats = 
  sizeof(SupportedD3DFormats) / sizeof(*SupportedD3DFormats);

}

//  //
//  Direct3D9DisplayModeEnumerator                                       //
//  //
/// Enumerator for display modes
/** Enumerates over all display modes available on a video device
*/
class Direct3D9DisplayModeEnumerator :
  public VideoDriver::DisplayModeEnumerator {
  public:
    /// Constructor
    /** Initializes an instance of Direct3D9DisplayModeEnumerator
    */
    Direct3D9DisplayModeEnumerator(unsigned long nAdapter) :
      m_nAdapter(nAdapter),
      m_nCurrentFormat(0),
      m_nCurrentMode(0),
      m_nNumModes(0) {

      return;
    }

    /// Destructor
    /** Destroys an instance of Direct3D9DisplayModeEnumerator
    */
    virtual ~Direct3D9DisplayModeEnumerator() {}

    /// Cycle through renderers
    /** Returns the current renderer being enumerated and advances
        to the next. If no more renderers are remaning, NULL is returned

        @return The currently enumerated renderer
    */
    const VideoDriver::DisplayMode &get() const {
      return m_DisplayMode;
    }

    /// Cycle through renderers
    /** Returns the current renderer being enumerated and advances
        to the next. If no more renderers are remaning, NULL is returned

        @return The currently enumerated renderer
    */
    bool next() {
      D3DDISPLAYMODE DisplayMode;

      getDirect3D9()->GetAdapterDisplayMode(m_nAdapter, &DisplayMode);

      while(m_nCurrentMode >= m_nNumModes) {
        m_nCurrentFormat++;
        if(m_nCurrentFormat > NumSupportedD3DFormats)
          return false;

        m_nCurrentMode = 0;
        m_nNumModes = getDirect3D9()->GetAdapterModeCount(m_nAdapter, SupportedD3DFormats[m_nCurrentFormat - 1]);
      }

      D3DCheck("Nuclex::Direct3D9DisplayModeEnumerator::cycle()", "IDirect3D9::EnumAdapterModes()",
               getDirect3D9()->EnumAdapterModes(m_nAdapter, SupportedD3DFormats[m_nCurrentFormat - 1], 
                                                m_nCurrentMode, &DisplayMode));
      
      m_DisplayMode.Resolution.X = DisplayMode.Width;
      m_DisplayMode.Resolution.Y = DisplayMode.Height;
      m_DisplayMode.nRefreshRate = DisplayMode.RefreshRate;
      m_DisplayMode.eFormat = PixelFormatFromD3DFORMAT(DisplayMode.Format);
      m_DisplayMode.bFullscreen = true;
      
      m_nCurrentMode++;
      return true;
    }

  private:
    VideoDriver::DisplayMode m_DisplayMode;           ///< Currently enumerated display mode
    unsigned long            m_nAdapter;              ///< Direct3D video adapter id
    unsigned long            m_nCurrentMode;          ///< Current video mode
    unsigned long            m_nNumModes;             ///< Total number of video modes
    unsigned long            m_nCurrentFormat;        ///< Current format being enumerated
};

// ####################################################################### //
// # Nuclex::Direct3D9VideoDriver::Direct3D9VideoDriver()    Constructor # // 
// ####################################################################### //
/** Initializes an instance of Direct3D9VideoDriver

    @param  pKernel   Kernel to which the device belongs
    @param  nAdapter  Direct3D Adapter number of the device
    @bug Does not display the monitor's name correctly 
*/
Direct3D9VideoDriver::Direct3D9VideoDriver(Kernel *pKernel, unsigned long nAdapter) :
  m_nAdapter(nAdapter),
  m_pKernel(pKernel) {
  IDirect3D9 *pDirect3D9 = getDirect3D9();

  // Query the device's capabilities
  D3DCheck("Nuclex::Direct3D9VideoDriver::Direct3D9VideoDriver", "IDirect3D9::GetDeviceCaps()",
           pDirect3D9->GetDeviceCaps(m_nAdapter, D3DDEVTYPE_HAL, &m_DeviceCaps));

  // Build a string uniquely describing the device
  D3DADAPTER_IDENTIFIER9 AdapterIdentifier;
  D3DCheck("Nuclex::Direct3D9VideoDriver::Direct3D9VideoDriver", "IDirect3D9::GetDeviceCaps()",
           pDirect3D9->GetAdapterIdentifier(m_nAdapter, 0, &AdapterIdentifier));

  HMONITOR hMonitor = pDirect3D9->GetAdapterMonitor(m_nAdapter);


  MONITORINFOEX MonitorInfoEx;
  memset(&MonitorInfoEx, 0, sizeof(MonitorInfoEx));
  MonitorInfoEx.cbSize = sizeof(MonitorInfoEx);
  GetMonitorInfo(hMonitor, &MonitorInfoEx);
  
  string MonitorName = MonitorInfoEx.szDevice;
  string::size_type Pos = MonitorName.find_first_of("0123456789");
  if(Pos == string::npos) {
    Pos = MonitorName.find_last_of("\\/");
    if(Pos == string::npos)
      Pos = 0;
    else
      ++Pos;
  }
  
  m_sName = string("Direct3D9 ") + AdapterIdentifier.Description +
            " on Monitor " + MonitorName.substr(Pos);
  if(MonitorName.substr(Pos) == "1")
    m_sName += " (primary)";

  D3DDISPLAYMODE D3DDisplayMode;
  pDirect3D9->GetAdapterDisplayMode(nAdapter, &D3DDisplayMode);
  
  m_DesktopDisplayMode.Resolution.X = D3DDisplayMode.Width;
  m_DesktopDisplayMode.Resolution.Y = D3DDisplayMode.Height;
  m_DesktopDisplayMode.eFormat = PixelFormatFromD3DFORMAT(D3DDisplayMode.Format);
  m_DesktopDisplayMode.nRefreshRate = D3DDisplayMode.RefreshRate;
  m_DesktopDisplayMode.bFullscreen = false;
}

// ####################################################################### //
// # Nuclex::Direct3D9VideoDriver::getMaxTextureSize()                   # // 
// ####################################################################### //
/** Returns the maximum texture size supported by the device

    @return The maximum texture size of the device
*/
Point2<size_t> Direct3D9VideoDriver::getMaxTextureSize() const {
  return Point2<size_t>(m_DeviceCaps.MaxTextureWidth, m_DeviceCaps.MaxTextureHeight);
}

// ####################################################################### //
// # Nuclex::Direct3D9VideoDriver::supportsScissorTest()                 # // 
// ####################################################################### //
/** Returns true if the device supports scissor testing

    @return True, if scissor testing is supported
*/
bool Direct3D9VideoDriver::supportsScissorTest() const {
  return (m_DeviceCaps.RasterCaps & D3DPRASTERCAPS_SCISSORTEST) != 0;
}

// ####################################################################### //
// # Nuclex::Direct3D9VideoDriver::createDevice()                        # // 
// ####################################################################### //
/** Creates a renderer on this device. The renderer is an instance
    of the device with an output window. So the device manages vertex
    buffers, textures and other resources, while the renderer performs
    the actual drawing using these resources and handles the output
    window. This allows multiple windows to share the same resources.

    @return The created device
*/
shared_ptr<VideoDevice> Direct3D9VideoDriver::createDevice(const DisplayMode &Mode) {
  return shared_ptr<VideoDevice>(new Direct3D9VideoDevice(m_pKernel, m_nAdapter, Mode));
}

// ####################################################################### //
// # Nuclex::Direct3D9VideoDriver::enumDisplayModes()                    # // 
// ####################################################################### //
/** Can be used to enumerate all display modes the device supports

    @return A new display mode enumerator
*/
shared_ptr<VideoDriver::DisplayModeEnumerator> Direct3D9VideoDriver::enumDisplayModes() const {
  return shared_ptr<VideoDriver::DisplayModeEnumerator>(new Direct3D9DisplayModeEnumerator(m_nAdapter));
}

// ####################################################################### //
// # Nuclex::Direct3D9VideoDriver::getName()                                # // 
// ####################################################################### //
/** Returns a human readable name uniquely describing the 
    device. Should include the graphics card and monitor used.

    @return The device's name
*/
const string &Direct3D9VideoDriver::getName() const {
  return m_sName;
}

BOOL CALLBACK Direct3D9VideoDriver::MonitorEnumProc(HMONITOR hMonitor, HDC, LPRECT, LPARAM dwData) {
  Direct3D9VideoDriver *pThis = reinterpret_cast<Direct3D9VideoDriver *>(dwData);
/*  
  ++pThis->m_MonitorIndex;
  if(hMonitor == pThis->m_hSearchedMonitor)
    return FALSE;
  else
*/  
    return TRUE;
}

