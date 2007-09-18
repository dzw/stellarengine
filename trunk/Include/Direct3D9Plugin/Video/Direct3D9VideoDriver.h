//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## Direct3D9VideoDriver.h - Direct3D9 video device     //
// ### # #      ###                                                      //
// # ### #      ###  A video device using Direct3D9 for rendering        //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R8             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_DIRECT3D9VIDEODRIVER_H
#define NUCLEX_DIRECT3D9VIDEODRIVER_H

#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "Nuclex/Video/VideoDriver.h"

namespace Nuclex {
  class Kernel;
}

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::Direct3D9VideoDriver                                  //
//  //
/// Direct3D9 video device
/** 
*/
class Direct3D9VideoDriver : 
  public VideoDriver {
  public:
    /// Constructor
    NUCLEXDIRECT3D9_API Direct3D9VideoDriver(Kernel *pKernel, unsigned long nAdapter);
    /// Destructor
    NUCLEXDIRECT3D9_API virtual ~Direct3D9VideoDriver() {}

  //
  // Direct3D9VideoDriver implementation
  //
  public:
    /// Retrieve Direct3D adapter ID of this device
    NUCLEXDIRECT3D9_API inline unsigned long getAdapterID() const { return m_nAdapter; }

  //
  // VideoDriver implementation
  //
  public:
    /// Get device name
    NUCLEXDIRECT3D9_API const string &getName() const;

    /// Get an enumerator over all available display modes
    NUCLEXDIRECT3D9_API shared_ptr<DisplayModeEnumerator> enumDisplayModes() const;

    /// Return maximum texture size supported
    NUCLEXDIRECT3D9_API Point2<size_t> getMaxTextureSize() const;

    /// Check if scissor test is supported
    NUCLEXDIRECT3D9_API bool supportsScissorTest() const;

    /// Create renderer
    NUCLEXDIRECT3D9_API shared_ptr<VideoDevice> createDevice(const DisplayMode &Mode);

  private:
    /// Callback for monitor enumeration
    static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC, LPRECT, LPARAM dwData);
  
    DisplayMode           m_DesktopDisplayMode;       ///< Desktop display mode
    unsigned long         m_nAdapter;                 ///< Adapter number
    D3DCAPS9              m_DeviceCaps;               ///< Device capabilities
    string                m_sName;                    ///< Device name
    Kernel               *m_pKernel;                  ///< Kernel instance
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_DIRECT3D9VIDEODRIVER_H
