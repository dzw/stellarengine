//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## Direct3D9VideoDevice.h - Direct3D9 renderer         //
// ### # #      ###                                                      //
// # ### #      ###  Nuclex Direct3D9 renderer                           //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R4             (C)2003 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_VIDEO_DIRECT3D9VIDEODEVICE_H
#define NUCLEX_VIDEO_DIRECT3D9VIDEODEVICE_H

#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "Direct3D9Plugin/Video/Direct3D9VideoDriver.h"
#include "Direct3D9Plugin/Video/Direct3D9RenderingContext.h"
#include "Direct3D9Plugin/Video/Direct3D9RenderStates.h"
#include "Nuclex/Video/VideoDevice.h"
#include "Nuclex/Video/Surface.h"

#include <deque>

namespace Nuclex {
  class Kernel;
}

namespace Nuclex { namespace Video {

//  //
//  Nuclex::Video::Direct3D9VideoDevice                                  //
//  //
/// Direct3D9 video device
/** Implementation of a video device for win32 platforms using
    the microsoft direct3d9 interfaces 
*/
class Direct3D9VideoDevice :
  public VideoDevice {
  friend Direct3D9RenderingContext;
  public:
    typedef SigC::Signal0<void> RestoreSignal;
    typedef SigC::Signal1<void, bool> ShutdownSignal;

    /// Constructor
    NUCLEXDIRECT3D9_API Direct3D9VideoDevice(
      Kernel *pKernel,
      unsigned long nAdapter,
      const VideoDriver::DisplayMode &Mode
    );
    /// Destructor
    NUCLEXDIRECT3D9_API ~Direct3D9VideoDevice();

  //
  // Direct3D9VideoDevice implementation
  //
  public:
    /// Get output window handle
    NUCLEXDIRECT3D9_API HWND getWindow() const;
    /// Set output window handle
    NUCLEXDIRECT3D9_API void setWindow(HWND hWnd);
    
    NUCLEXDIRECT3D9_API const IDirect3DDevice9Ptr &getDirect3DDevice() const {
      return m_spDirect3DDevice;
    }
    NUCLEXDIRECT3D9_API const D3DCAPS9 &getDeviceCaps() const {
      return m_DeviceCaps;
    }

    /// On device shutdown
    ShutdownSignal OnDeviceShutdown;
    /// On device reset
    RestoreSignal OnDeviceRestore;

  //
  // VideoDevice implementation
  //
  public:
    /// Retrieve the currently set display mode
    NUCLEXDIRECT3D9_API const VideoDriver::DisplayMode &getDisplayMode() { return m_DisplayMode; }
    /// Select a display mode to use
    NUCLEXDIRECT3D9_API void setDisplayMode(const VideoDriver::DisplayMode &DisplayMode);

    /// Get video context
    NUCLEXDIRECT3D9_API shared_ptr<RenderingContext> renderFrame(
      const shared_ptr<RenderTarget> &spDestination = shared_ptr<RenderTarget>()
    );

    /// Create a new texture
    NUCLEXDIRECT3D9_API shared_ptr<Texture> createTexture(
      const Point2<size_t> &Size,
      Surface::PixelFormat eFormat,
      ResourceClass eResourceClass = RC_DYNAMIC
    );

    /// Create a new texture
    NUCLEXDIRECT3D9_API shared_ptr<Texture> createTexture(
      const Point2<size_t> &Size,
      AlphaChannel eAlphaChannel,
      ResourceClass eResourceClass = RC_DYNAMIC
    );

    /// Create a new vertex buffer
    NUCLEXDIRECT3D9_API shared_ptr<VertexBuffer> createVertexBuffer(
      const VertexDeclaration &Declaration,
      size_t nSize,
      ResourceClass eResourceClass = RC_DYNAMIC
    );

    /// Create a new index buffer
    NUCLEXDIRECT3D9_API shared_ptr<IndexBuffer> createIndexBuffer(
      size_t nSize,
      ResourceClass eResourceClass = RC_DYNAMIC
    );

    /// Create a new render state set
    NUCLEXDIRECT3D9_API shared_ptr<RenderStates> createRenderStates();

    /// Create a new vertex shader
    NUCLEXDIRECT3D9_API shared_ptr<VertexShader> createVertexShader(
      const string &sShader
    );
    /// Create a new pixel shader
    NUCLEXDIRECT3D9_API shared_ptr<PixelShader> createPixelShader(
      const string &sShader
    );

    /// Retrieve the maximum texture size supported
    NUCLEXDIRECT3D9_API const Point2<size_t> &getMaxTextureSize() const {
      return m_MaxTextureSize;
    }

    NUCLEXDIRECT3D9_API size_t getVideoMemorySize() const { return m_VideoMemorySize; }
    
    NUCLEXDIRECT3D9_API void setCursor(const shared_ptr<Image> &spCursor);

  protected:
    /// Create Direct3D device
    void createDevice();
    /// Destroy Direct3D device
    void destroyDevice();
    /// Reset Direct3D device
    void resetDevice();

  protected:
    class ActiveRenderStates;
    std::auto_ptr<ActiveRenderStates> m_spActiveRenderStates; ///< Currently set render states

    unsigned long               m_nAdapter;            ///< Owner
    bool                        m_bUse16BitTextures;   ///< Whether to use 16 bit textures
    weak_ptr<RenderingContext>  m_wpRenderingContext;  ///< Current rendering context
    VideoDriver::DisplayMode    m_DisplayMode;         ///< Current display mode
    D3DPRESENT_PARAMETERS       m_PresentParameters;   ///< Associated present parameters
    IDirect3DDevice9Ptr         m_spDirect3DDevice;    ///< The Direct3D device
    D3DCAPS9                    m_DeviceCaps;          ///< Rendering device caps
    Point2<size_t>              m_MaxTextureSize;      ///< Max texture size supported
    size_t                      m_VideoMemorySize;     ///< Total amount of video memory available
    Kernel                     *m_pKernel;             ///< Kernel instance
    IDirect3DSurface9Ptr        m_spCursor;            ///< Current cursor
};

//  //
//  Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates              //
//  //
/// Currently active render states
/** Stores the current settings of all render states in the device.
    This is an internal class used to facilitate setting of RenderStates
    through state blocks or by direct modification.
*/
class Direct3D9VideoDevice::ActiveRenderStates :
  public RenderStates {
  public:
    ActiveRenderStates &operator +=(const Direct3D9RenderStates &Other) {
      Other.activateStateBlock(m_spDirect3DDevice);
      RenderStates::operator =(Other);
      
      // Change all S_SET to S_ACTIVE, because activated through state block
      
      return *this;
    }
  
    /// Assign another state set
    RenderStates &operator +=(const RenderStates &Other) {
      const Direct3D9RenderStates *pD3D9RS = dynamic_cast<const Direct3D9RenderStates *>(&Other);
      if(pD3D9RS)
        return operator +=(*pD3D9RS);
      else
        return RenderStates::operator +=(Other);
    }
    
  //
  // ActiveRenderStateSet implementation
  //
  public:
    /// Select the Direct3D device to be used
    void setDevice(const IDirect3DDevice9Ptr &spDevice) { m_spDirect3DDevice = spDevice; }

  //
  // RenderStateSet implementation
  //
  public:
    /// Enable or disable alpha blending
    void enableAlphaBlend(bool bEnable) {
      if((m_bLightingEnabled.getStatus() != RenderState<bool>::S_ACTIVE) ||
          (static_cast<bool>(m_bAlphaBlendEnabled) != bEnable)) {
        m_bAlphaBlendEnabled = bEnable;
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setAlphaBlendEnable()",
                 "IDirect3DDevice9::SetRenderState()",
                 m_spDirect3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, bEnable));
        m_bAlphaBlendEnabled.setStatus(RenderState<bool>::S_ACTIVE);
      }
    }

    /// Enable or disable lighting
    void enableLighting(bool bEnable) {
      if((m_bLightingEnabled.getStatus() != RenderState<bool>::S_ACTIVE) ||
          (static_cast<bool>(m_bLightingEnabled) != bEnable)) {
        m_bLightingEnabled = bEnable;
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setLightingEnable()",
                 "IDirect3DDevice9::SetRenderState()",
                 m_spDirect3DDevice->SetRenderState(D3DRS_LIGHTING, bEnable));
        m_bLightingEnabled.setStatus(RenderState<bool>::S_ACTIVE);
      }
    }

    /// Enable z buffering or depth sorting
    void enableDepthBuffer(bool bEnable) {
      if((m_bDepthBufferEnabled.getStatus() != RenderState<bool>::S_ACTIVE) ||
          (static_cast<bool>(m_bDepthBufferEnabled) != bEnable)) {
        m_bDepthBufferEnabled = bEnable;
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setZEnableEnable()",
                 "IDirect3DDevice9::SetRenderState()",
                 m_spDirect3DDevice->SetRenderState(D3DRS_ZENABLE, bEnable));
        m_bDepthBufferEnabled.setStatus(RenderState<bool>::S_ACTIVE);
      }
    }
    
    /// Set blend mode to use for the source surface (eg. a texture to be rendered)
    void setSourceBlendMode(BlendMode eMode) {
      if((m_eSourceBlendMode.getStatus() != RenderState<BlendMode>::S_ACTIVE) ||
          (static_cast<BlendMode>(m_eSourceBlendMode) != eMode)) {
        m_eSourceBlendMode = eMode;
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setSourceBlendMode()",
                 "IDirect3DDevice9::SetRenderState()",
                 m_spDirect3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLENDFromBlendMode(eMode)));
        m_eSourceBlendMode.setStatus(RenderState<BlendMode>::S_ACTIVE);
      }
    }  

    /// Set blend mode to use for the destination surface (eg. a texture to be rendered)
    void setDestinationBlendMode(BlendMode eMode) {
      if((m_eDestinationBlendMode.getStatus() != RenderState<BlendMode>::S_ACTIVE) ||
          (static_cast<BlendMode>(m_eDestinationBlendMode) != eMode)) {
        m_eDestinationBlendMode = eMode;
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setDestinationBlendMode()",
                 "IDirect3DDevice9::SetRenderState()",
                 m_spDirect3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLENDFromBlendMode(eMode)));
        m_eDestinationBlendMode.setStatus(RenderState<BlendMode>::S_ACTIVE);
      }
    }

    /// Set blend mode to use for the destination surface (eg. a texture to be rendered)
    void setCullMode(CullMode eMode) {
      if((m_eCullMode.getStatus() != RenderState<CullMode>::S_ACTIVE) ||
          (static_cast<CullMode>(m_eCullMode) != eMode)) {
        m_eCullMode = eMode;
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setCullMode()",
                 "IDirect3DDevice9::SetRenderState()",
                 m_spDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULLFromCullMode(eMode)));
        m_eCullMode.setStatus(RenderState<CullMode>::S_ACTIVE);
      }
    }

    /// Set polygon fill mode
    void setFillMode(FillMode eMode) {
      if((m_eFillMode.getStatus() != RenderState<FillMode>::S_ACTIVE) ||
          (static_cast<FillMode>(m_eFillMode) != eMode)) {
        m_eFillMode = eMode;
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setFillMode()",
                 "IDirect3DDevice9::SetRenderState()",
                 m_spDirect3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILLMODEFromFillMode(eMode)));
        m_eFillMode.setStatus(RenderState<FillMode>::S_ACTIVE);
      }
    }
     
    /// Set color blend operation
    void setColorOperation(size_t Stage, BlendOperation eOperation) {
      if((m_peColorOperation[Stage].getStatus() != RenderState<BlendOperation>::S_ACTIVE) ||
          (static_cast<BlendOperation>(m_peColorOperation[Stage]) != eOperation)) {
        m_peColorOperation[Stage] = eOperation;
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setColorOperation()",
                 "IDirect3DDevice9::SetTextureStageState()",
                 m_spDirect3DDevice->SetTextureStageState(Stage, D3DTSS_COLOROP, D3DTEXTUREOPFromBlendOperation(eOperation)));
        m_peColorOperation[Stage].setStatus(RenderState<BlendOperation>::S_ACTIVE);
      }
    }
    
    /// Set color blend operation
    void setColorArgument(size_t Stage, size_t Argument, BlendArgument eArgument) {
      if((m_ppeColorArgument[Stage][Argument].getStatus() != RenderState<BlendArgument>::S_ACTIVE) ||
          (static_cast<BlendArgument>(m_ppeColorArgument[Stage][Argument]) != eArgument)) {
        m_ppeColorArgument[Stage][Argument] = eArgument;
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setColorArgument()",
                 "IDirect3DDevice9::SetTextureStageState()",
                 m_spDirect3DDevice->SetTextureStageState(Stage, D3DTSS_COLORARG1, D3DTEXTURESTAGESTATETYPEFromBlendArgument(eArgument)));
        m_ppeColorArgument[Stage][Argument].setStatus(RenderState<BlendArgument>::S_ACTIVE);
      }
    }      

    /// Set alpha blend operation
    void setAlphaOperation(size_t Stage, BlendOperation eOperation) {
      if((m_peAlphaOperation[Stage].getStatus() != RenderState<BlendOperation>::S_ACTIVE) ||
          (static_cast<BlendOperation>(m_peAlphaOperation[Stage]) != eOperation)) {
        m_peAlphaOperation[Stage] = eOperation;
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setAlphaOperation()",
                 "IDirect3DDevice9::SetTextureStageState()",
                 m_spDirect3DDevice->SetTextureStageState(Stage, D3DTSS_ALPHAOP, D3DTEXTUREOPFromBlendOperation(eOperation)));
        m_peAlphaOperation[Stage].setStatus(RenderState<BlendOperation>::S_ACTIVE);
      }
    }
    
    /// Set alpha blend operation
    void setAlphaArgument(size_t Stage, size_t Argument, BlendArgument eArgument) {
      if((m_ppeAlphaArgument[Stage][Argument].getStatus() != RenderState<BlendArgument>::S_ACTIVE) ||
          (static_cast<BlendArgument>(m_ppeAlphaArgument[Stage][Argument]) != eArgument)) {
        m_ppeAlphaArgument[Stage][Argument] = eArgument;
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setAlphaArgument()",
                 "IDirect3DDevice9::SetTextureStageState()",
                 m_spDirect3DDevice->SetTextureStageState(Stage, D3DTSS_ALPHAARG1, D3DTEXTURESTAGESTATETYPEFromBlendArgument(eArgument)));
        m_ppeAlphaArgument[Stage][Argument].setStatus(RenderState<BlendArgument>::S_ACTIVE);
      }
    }      

    /// Set minification filter type
    void setMinificationFilter(size_t Stage, TextureFilter eFilter) {
      if((m_peMinificationFilter[Stage].getStatus() != RenderState<TextureFilter>::S_ACTIVE) ||
          (static_cast<TextureFilter>(m_peMinificationFilter[Stage]) != eFilter)) {
        m_peMinificationFilter[Stage] = eFilter;
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setMinificationFilter()",
                 "IDirect3DDevice9::SetTextureStageState()",
                 m_spDirect3DDevice->SetSamplerState(Stage, D3DSAMP_MINFILTER, D3DTEXTUREFILTERTYPEFromTextureFilter(eFilter)));
        m_peMinificationFilter[Stage].setStatus(RenderState<TextureFilter>::S_ACTIVE);
      }
    }

    /// Set magnification filter type
    void setMagnificationFilter(size_t Stage, TextureFilter eFilter) {
      if((m_peMagnificationFilter[Stage].getStatus() != RenderState<TextureFilter>::S_ACTIVE) ||
          (static_cast<TextureFilter>(m_peMagnificationFilter[Stage]) != eFilter)) {
        m_peMagnificationFilter[Stage] = eFilter;
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setMagnificationFilter()",
                 "IDirect3DDevice9::SetTextureStageState()",
                 m_spDirect3DDevice->SetSamplerState(Stage, D3DSAMP_MAGFILTER, D3DTEXTUREFILTERTYPEFromTextureFilter(eFilter)));
        m_peMagnificationFilter[Stage].setStatus(RenderState<TextureFilter>::S_ACTIVE);
      }
    }

    /// Set texture addressing mode
    void setTextureAddressMode(size_t Stage, Dimension eDimension, TextureAddressMode eMode) {
      if((m_ppeTextureAddressMode[Stage][eDimension].getStatus() != RenderState<TextureFilter>::S_ACTIVE) ||
          (static_cast<TextureAddressMode>(m_ppeTextureAddressMode[Stage][eDimension]) != eMode)) {
        m_ppeTextureAddressMode[Stage][eDimension] = eMode;
        
        D3DCheck("Nuclex::Video::Direct3D9VideoDevice::ActiveRenderStates::setMagnificationFilter()",
                 "IDirect3DDevice9::SetTextureStageState()",
                 m_spDirect3DDevice->SetSamplerState(Stage, D3DSAMPLERSTATETYPEFromDimension(eDimension), D3DTEXTUREADDRESSFromTextureAddressMode(eMode)));
        m_ppeTextureAddressMode[Stage][eDimension].setStatus(RenderState<TextureAddressMode>::S_ACTIVE);
      }
    }
    
  private:
    IDirect3DDevice9Ptr m_spDirect3DDevice;           ///< The direct3d device to use
};

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_DIRECT3D9VIDEODEVICE_H
