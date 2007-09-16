//  //
// #   #  ###  #   #              -= Nuclex Project =-                   //
// ##  # #   # ## ## Direct3D9VideoDevice.cpp - Direct3D9 renderer       //
// ### # #      ###                                                      //
// # ### #      ###  Nuclex Direct3D9 renderer                           //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "Direct3D9Plugin/Video/Direct3D9VideoDevice.h"
#include "Direct3D9Plugin/Video/Direct3D9Texture.h"
#include "Direct3D9Plugin/Video/Direct3D9VertexBuffer.h"
#include "Direct3D9Plugin/Video/Direct3D9IndexBuffer.h"
#include "Direct3D9Plugin/Video/Direct3D9VertexShader.h"
#include "Direct3D9Plugin/Video/Direct3D9PixelShader.h"
#include "Direct3D9Plugin/Video/Direct3D9RenderingContext.h"
#include "Direct3D9Plugin/Video/Direct3D9RenderStates.h"
#include "../Resource.h"
#include "DirectX/DXErr9.h"
#include "DirectX/D3DX9Shader.h"
#include "Nuclex/Kernel.h"
#include "Nuclex/Application.h"

using namespace Nuclex;
using namespace Nuclex::Video;

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::Direct3D9VideoDevice()                   Constructor # //
// ############################################################################################# //
/** Initializes an instance of Direct3D9VideoDevice

    @param  pKernel   Kernel to which the renderer belongs
    @param  nAdapter  Direct3D adapter number
    @param  Mode      Initial display mode for the renderer
*/
Direct3D9VideoDevice::Direct3D9VideoDevice(Kernel *pKernel, unsigned long nAdapter,
                                           const VideoDriver::DisplayMode &Mode) :
  m_nAdapter(nAdapter),
  m_pKernel(pKernel),
  m_DisplayMode(Mode),
  m_bUse16BitTextures(false),
  m_spActiveRenderStates(new ActiveRenderStates()) {

  // Query the device's capabilities
  D3DCheck(
    "Nuclex::Video::Direct3D9VideoDriver::Direct3D9VideoDriver",
    "IDirect3D9::GetDeviceCaps()",
    getDirect3D9()->GetDeviceCaps(m_nAdapter, D3DDEVTYPE_HAL, &m_DeviceCaps)
  );

  // Maybe allow unlimited size later on...  
  createDevice();

  m_MaxTextureSize.set(m_DeviceCaps.MaxTextureWidth, m_DeviceCaps.MaxTextureHeight);
  m_VideoMemorySize = m_spDirect3DDevice->GetAvailableTextureMem() / 1048576;
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::~Direct3D9VideoDevice()                   Destructor # //
// ############################################################################################# //
/** Destroys an instance of CDirect3D9VideoDevice
*/
Direct3D9VideoDevice::~Direct3D9VideoDevice() {
  destroyDevice();
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::setDisplayMode()                                     # //
// ############################################################################################# //
/** Selects a display mode either before the rendering output window
    is opened or tries to change the current display mode while the
    rendering output window is open.
    If the display mode does not specify a color depth (PF_NONE), the
    desktop color depth will be used for a rendering window instead
    of going fullscreen.

    @param  DisplayMode  Desired display mode
*/
void Direct3D9VideoDevice::setDisplayMode(const VideoDriver::DisplayMode &DisplayMode) {
  m_DisplayMode = DisplayMode;
  resetDevice();
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::renderFrame()                                        # //
// ############################################################################################# //
/** Initiates the rendering of a new frame by returning a new renderer
    through which all drawing commands for the frame can be issued. The
    frame is completed by releasing the renderer.
    
    @return A new frame renderer
*/
shared_ptr<VideoDevice::RenderingContext> Direct3D9VideoDevice::renderFrame(
  const shared_ptr<RenderTarget> &spDestination
) {
  // If no device is available, rendering makes no sense
  if(!static_cast<bool>(m_spDirect3DDevice))
    return shared_ptr<VideoDevice::RenderingContext>();

  // Make sure the previous frame has been completed
  if(!m_wpRenderingContext.expired())
    throw FailedException("Nuclex::Video::Direct3D9VideoDevice::renderFrame()",
                          "Rendering is already in progress");

  // Now make sure the device is in a usable state
  HRESULT hResult = m_spDirect3DDevice->TestCooperativeLevel();
  switch(hResult) {
  
    case D3DERR_DEVICELOST: {
      ::Sleep(10);
      return shared_ptr<VideoDevice::RenderingContext>();
    }
    case D3DERR_DEVICENOTRESET: {
      resetDevice();

      break;
    }
    default: {
      D3DCheck("Nuclex::Video::Direct3D9VideoDevice::renderFrame()",
               "IDirect3DDevice9::TestCooperativeLevel()", hResult);
      break;
    }
  }
        
  D3DCheck("Nuclex::Video::Direct3D9VideoDevice::renderFrame()",
            "IDirect3DDevice9::ShowCursor()",
            m_spDirect3DDevice->ShowCursor(TRUE));
  
  shared_ptr<VideoDevice::RenderingContext> spRC(
    new Direct3D9RenderingContext(this)
  );
  m_wpRenderingContext = spRC;

  return spRC;
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::createDevice()                                       # //
// ############################################################################################# //
/** Set up and create the direct3d device
*/
void Direct3D9VideoDevice::createDevice() {
  IDirect3D9 *pDirect3D9 = getDirect3D9();

  memset(&m_PresentParameters, 0, sizeof(m_PresentParameters));

  // Use fullscreen display ?
  if(m_DisplayMode.bFullscreen) {

    m_PresentParameters.BackBufferWidth  = m_DisplayMode.Resolution.X;
    m_PresentParameters.BackBufferHeight = m_DisplayMode.Resolution.Y;
    m_PresentParameters.BackBufferFormat = D3DFORMATFromPixelFormat(m_DisplayMode.eFormat);
    //m_PresentParameters.BackBufferCount  = 1;
    m_PresentParameters.FullScreen_RefreshRateInHz =
      m_DisplayMode.nRefreshRate ? m_DisplayMode.nRefreshRate : D3DPRESENT_RATE_DEFAULT;

  } else {
    // Retrieve the current display mode
    D3DDISPLAYMODE DisplayMode;
    D3DCheck("Nuclex::Video::Direct3D9VideoDevice::createDevice()", "IDirect3D9::GetAdapterDisplayMode()",
             pDirect3D9->GetAdapterDisplayMode(m_nAdapter, &DisplayMode));

    m_PresentParameters.BackBufferWidth  = m_DisplayMode.Resolution.X;
    m_PresentParameters.BackBufferHeight = m_DisplayMode.Resolution.Y;
    m_PresentParameters.BackBufferFormat = D3DFORMATFromPixelFormat(m_DisplayMode.eFormat);
    //m_PresentParameters.BackBufferCount  = 1;
    m_PresentParameters.Windowed         = true;
  }

  bool b16Bits = Surface::bppFromFormat(PixelFormatFromD3DFORMAT(
    m_PresentParameters.BackBufferFormat
  )) < 4;
  
  //Nuclex::VideoKernel::message(0, lexical_cast<string>(m_PresentParameters.BackBufferFormat));
  
  m_PresentParameters.hDeviceWindow   = m_pKernel->getApplication().getWindowHandle();
  m_PresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
  m_PresentParameters.SwapEffect      = D3DSWAPEFFECT_DISCARD; // D3DSWAPEFFECT_COPY;
  m_PresentParameters.EnableAutoDepthStencil = true;
  m_PresentParameters.AutoDepthStencilFormat = b16Bits ? D3DFMT_D16 : D3DFMT_D24X8;
  m_PresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
  //m_PresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

  m_pKernel->getApplication().setSize(m_DisplayMode.Resolution);
  m_pKernel->getApplication().show();

  // Create the D3D9 device, effectively entering the selected video mode
  D3DCheck("Nuclex::Video::Direct3D9VideoDevice::createDevice()", "IDirect3D9::CreateDevice()",
           pDirect3D9->CreateDevice(m_nAdapter,
                                    D3DDEVTYPE_HAL,
                                    m_pKernel->getApplication().getWindowHandle(),
                                    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                    &m_PresentParameters,
                                    &m_spDirect3DDevice));
  m_spActiveRenderStates->setDevice(m_spDirect3DDevice);

  // @BUG: For windowed mode, eFormat is not set
  //       Query the d3ddevice for the real mode.
  m_bUse16BitTextures = b16Bits;
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::destroyDevice()                                      # //
// ############################################################################################# //
/** Prepare the selected window for usage by the renderer
*/
void Direct3D9VideoDevice::destroyDevice() {
  m_spDirect3DDevice = NULL;
  m_spActiveRenderStates->setDevice(NULL);
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::resetDevice()                                        # //
// ############################################################################################# //
/** Resets the Direct3D device
*/
void Direct3D9VideoDevice::resetDevice() {
  if(!static_cast<bool>(m_spDirect3DDevice))
    return;

  // Prevent internal driver error on nVidia cards...
  if((m_DisplayMode.Resolution.X <= 0) || (m_DisplayMode.Resolution.Y <= 0))
    return;
      
  // This might have occured due to a window size change (if windowed)
  if(m_PresentParameters.Windowed) {
    m_PresentParameters.BackBufferWidth = m_DisplayMode.Resolution.X;
    m_PresentParameters.BackBufferHeight = m_DisplayMode.Resolution.Y;
  }

  OnDeviceShutdown(true);
  m_spActiveRenderStates->clear();

  D3DCheck(
    "Nuclex::Video::Direct3D9VideoDevice::resetDevice()",
    "IDirect3DDevice9::Reset()",
    m_spDirect3DDevice->Reset(&m_PresentParameters)
  );

  OnDeviceRestore();
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::createTexture()                                      # //
// ############################################################################################# //
/** Creates a new texture of the specified size and format on the
    device. The texture's contents are initially undefined.
    It is recommended to use texture sizes that are powers of 2,
    since, due to hardware limitations, in general, other sizes
    will waste memory up to the next power of 2.

    @param  Size            Resolution of the texture
    @param  eFormat         Pixel format of the texture
    @param  eResourceClass  Resource class in which to create the texture
    @return The new texture
*/
shared_ptr<Texture> Direct3D9VideoDevice::createTexture(const Point2<size_t> &Size,
                                                        Surface::PixelFormat eFormat,
                                                        ResourceClass eResourceClass) {
  return shared_ptr<Texture>(
    new Direct3D9Texture(*this, Size, eFormat, eResourceClass)
  );
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::createTexture()                                      # //
// ############################################################################################# //
/** Creates a new texture of the specified size and format on the
    device. The texture's contents are initially undefined.
    It is recommended to use texture sizes that are powers of 2,
    since, due to hardware limitations, in general, other sizes
    will waste memory up to the next power of 2.

    @param  Size            Resolution of the texture
    @param  eAlphaChannel   The desired kind of alpha channel in the texture
    @param  eResourceClass  Resource class in which to create the texture
    @return The new texture
*/
shared_ptr<Texture> Direct3D9VideoDevice::createTexture(const Point2<size_t> &Size,
                                                        AlphaChannel eAlphaChannel,
                                                        ResourceClass eResourceClass) {
  switch(eAlphaChannel) {
    case AC_NONE: {
      return m_bUse16BitTextures ?
               createTexture(Size, Surface::PF_RGB_5_6_5, eResourceClass) :
               createTexture(Size, Surface::PF_XRGB_8_8_8_8, eResourceClass);
    }
    case AC_KEY: {
      return m_bUse16BitTextures ?
               createTexture(Size, Surface::PF_ARGB_1_5_5_5, eResourceClass) :
               createTexture(Size, Surface::PF_ARGB_8_8_8_8, eResourceClass);
    }
    case AC_BLEND: {
      return m_bUse16BitTextures ?
               createTexture(Size, Surface::PF_ARGB_4_4_4_4, eResourceClass) :
               createTexture(Size, Surface::PF_ARGB_8_8_8_8, eResourceClass);
    }
    default: {
      throw UnexpectedException("Nuclex::Video::Direct3D9VideoDevice::createTexture()",
                                "Unknown alpha channel type requested");
    }
  }
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::createVertexBuffer()                                 # //
// ############################################################################################# //
/** Creates a new vertex buffer using the specified number of vertices
    and vertex format. The vertex buffer's contents are initially undefined
    and have to be filled before using it in a rendering operation.

    @param  Declaration     Vertex declaration descibring the data format
    @param  nSize           Number of vertices
    @param  eResourceClass  Resource class in which to create the buffer
    @return The new vertex buffer
*/
shared_ptr<VertexBuffer> Direct3D9VideoDevice::createVertexBuffer(const VertexDeclaration &Declaration,
                                                                  size_t nSize,
                                                                  ResourceClass eResourceClass) {
  return shared_ptr<VertexBuffer>(
    new Direct3D9VertexBuffer(*this, Declaration, nSize, eResourceClass)
  );
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::createIndexBuffer()                                  # //
// ############################################################################################# //
/** Creates a new index buffer with the specified number of indices.
    The index buffer's contents are initially undefined and have to be
    filled before using it in a rendering operation.

    @param  nSize           Number of elements in the buffer
    @param  eResourceClass  Resource class in which to create the buffer
    @return The new index buffer
*/
shared_ptr<IndexBuffer> Direct3D9VideoDevice::createIndexBuffer(size_t nSize,
                                                                ResourceClass eResourceClass) {
  return shared_ptr<IndexBuffer>(
    new Direct3D9IndexBuffer(*this, nSize, eResourceClass)
  );
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::createRenderStates()                                 # //
// ############################################################################################# //
/** Creates a new render state set using a state block or similar
    techniques where possible.

    @return The new render state set
*/
shared_ptr<RenderStates> Direct3D9VideoDevice::createRenderStates() {
  return shared_ptr<RenderStates>(
    new Direct3D9RenderStates(*this)
  );
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::createVertexShader()                                 # //
// ############################################################################################# //
/** Creates a new vertex shader
    
    @param  pnFunction  Vertex shader function
    @return The new vertex shader
*/
shared_ptr<VertexShader> Direct3D9VideoDevice::createVertexShader(const string &sShader) {
  _COM_SMARTPTR_TYPEDEF(ID3DXConstantTable, IID_ID3DXConstantTable);

  ID3DXBufferPtr        spShader, spErrors;
  ID3DXConstantTablePtr spConstants;

  D3DCheck(
    "Nuclex::Video::Direct3D9VideoDevice::createVertexShader()",
    "D3DXCompileShader()",
    D3DXCompileShader(
      sShader.c_str(), sShader.length(),
      NULL, // Preprocessor defines
      NULL, // Include handler
      "main",
      D3DXGetVertexShaderProfile(m_spDirect3DDevice),
      0,
      &spShader, &spErrors, &spConstants
    )
  );
  
  return shared_ptr<VertexShader>(new Direct3D9VertexShader(
    *this,
    spShader->GetBufferPointer()
  ));
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9VideoDevice::createPixelShader()                                  # //
// ############################################################################################# //
/** Creates a new pixel shader
    
    @param  pnFunction  Pixel shader function
    @return The new pixel shader
*/
shared_ptr<PixelShader> Direct3D9VideoDevice::createPixelShader(const string &sShader) {
  _COM_SMARTPTR_TYPEDEF(ID3DXConstantTable, IID_ID3DXConstantTable);

  ID3DXBufferPtr        spShader, spErrors;
  ID3DXConstantTablePtr spConstants;

  D3DCheck(
    "Nuclex::Video::Direct3D9VideoDevice::createPixelShader()",
    "D3DXCompileShader()",
    D3DXCompileShader(
      sShader.c_str(), sShader.length(),
      NULL, // Preprocessor defines
      NULL, // Include handler
      "main",
      D3DXGetPixelShaderProfile(m_spDirect3DDevice),
      0,
      &spShader, &spErrors, &spConstants
    )
  );
  
  return shared_ptr<PixelShader>(new Direct3D9PixelShader(
    *this,
    spShader->GetBufferPointer()
  ));
}

void Direct3D9VideoDevice::setCursor(const shared_ptr<Image> &spCursorImage) {
  D3DCheck(
    "Nuclex::Video::Direct3D9VideoDevice::setCursor()",
    "IDirect3DDevice9::CreateOffscreenPlainSurface()",
    m_spDirect3DDevice->CreateOffscreenPlainSurface(
      spCursorImage->getSize().X, spCursorImage->getSize().Y,
      D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_spCursor, NULL
    )
  );
  
  { D3DLOCKED_RECT LockedRect;
    D3DCheck(
      "Nuclex::Video::Direct3D9VideoDevice::setCursor()",
      "IDirect3DSurface9::LockRect()",
      m_spCursor->LockRect(&LockedRect, NULL, D3DLOCK_DISCARD)
    );
    
    ScopeGuard Unlock_Surface = MakeObjGuard(*m_spCursor, &IDirect3DSurface9::UnlockRect);
    
    Surface::LockInfo LockedSurface;
    LockedSurface.pMemory = LockedRect.pBits;
    LockedSurface.eFormat = Surface::PF_ARGB_8_8_8_8;
    LockedSurface.nPitch = LockedRect.Pitch;
    LockedSurface.eMode = Surface::LM_OVERWRITEALL;
    LockedSurface.Size = spCursorImage->getSize();
    
    spCursorImage->blitTo(LockedSurface);
  }
  
  D3DCheck(
    "Nuclex::Video::Direct3D9VideoDevice::setCursor()",
    "IDirect3DDevice9::SetCursorProperties()",
    m_spDirect3DDevice->SetCursorProperties(0, 0, m_spCursor)
  );
/*  
  m_spDirect3DDevice->ShowCursor(TRUE);
  m_spDirect3DDevice->SetCursorPosition(10, 10, 0);
*/  
}
