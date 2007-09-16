//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Direct3D9RenderStateSet.cpp - Render state set      //
// ### # #      ###                                                      //
// # ### #      ###  A set of render states                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "Direct3D9Plugin/Video/Direct3D9RenderStates.h"
#include "Direct3D9Plugin/Video/Direct3D9VideoDevice.h"
#include "ScopeGuard/ScopeGuard.h"

using namespace Nuclex;
using namespace Nuclex::Video;

// ############################################################################################# //
// # Nuclex::Video::Direct3D9RenderStates::Direct3D9RenderStates()                 Constructor # //
// ############################################################################################# //
Direct3D9RenderStates::Direct3D9RenderStates(Direct3D9VideoDevice &VideoDevice) {
  VideoDevice.OnDeviceShutdown.connect(
    sigc::mem_fun(this, &Direct3D9RenderStates::onDeviceShutdown)
  );
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9RenderStates::clear()                                             # //
// ############################################################################################# //
void Direct3D9RenderStates::clear() {
  RenderStates::clear();
  m_spStateBlock = NULL;
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9RenderStates::activateStateBlock()                                # //
// ############################################################################################# //
void Direct3D9RenderStates::activateStateBlock(const IDirect3DDevice9Ptr &spDevice) const {
  if(hasChanged()) {
    IDirect3DStateBlock9Ptr spStateBlock;
  
    D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
             "IDirect3DDevice9::BeginStateBlock()",
             spDevice->BeginStateBlock());

    { ScopeGuard EndStateBlock = MakeObjGuard(*spDevice, &IDirect3DDevice9::EndStateBlock, &IDirect3DStateBlock9Ptr());
             
      if(m_bAlphaBlendEnabled.getStatus() != RenderState<bool>::S_UNSET)
        D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
                 "IDirect3DDevice9::SetRenderState()",
                 spDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, static_cast<bool>(m_bAlphaBlendEnabled)));

      if(m_bLightingEnabled.getStatus() != RenderState<bool>::S_UNSET)
        D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
                 "IDirect3DDevice9::SetRenderState()",
                 spDevice->SetRenderState(D3DRS_LIGHTING, static_cast<bool>(m_bLightingEnabled)));

      if(m_bDepthBufferEnabled.getStatus() != RenderState<bool>::S_UNSET)
        D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
                 "IDirect3DDevice9::SetRenderState()",
                 spDevice->SetRenderState(D3DRS_ZENABLE, static_cast<bool>(m_bDepthBufferEnabled)));

      if(m_eSourceBlendMode.getStatus() != RenderState<BlendMode>::S_UNSET)
        D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
                 "IDirect3DDevice9::SetRenderState()",
                 spDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLENDFromBlendMode(m_eSourceBlendMode)));

      if(m_eDestinationBlendMode.getStatus() != RenderState<BlendMode>::S_UNSET)
        D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
                 "IDirect3DDevice9::SetRenderState()",
                 spDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLENDFromBlendMode(m_eDestinationBlendMode)));

      if(m_eCullMode.getStatus() != RenderState<CullMode>::S_UNSET)
        D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
                 "IDirect3DDevice9::SetRenderState()",
                 spDevice->SetRenderState(D3DRS_CULLMODE, D3DCULLFromCullMode(m_eCullMode)));
                 
      if(m_eFillMode.getStatus() != RenderState<FillMode>::S_UNSET)
        D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
                 "IDirect3DDevice9::SetRenderState()",
                 spDevice->SetRenderState(D3DRS_FILLMODE, D3DFILLMODEFromFillMode(m_eFillMode)));

      for(size_t TextureStage = 0; TextureStage < MaxTextureStages; ++TextureStage) {
        if(m_peColorOperation[TextureStage].getStatus() != RenderState<BlendOperation>::S_UNSET)
          D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
                   "IDirect3DDevice9::SetRenderState()",
                   spDevice->SetTextureStageState(TextureStage, D3DTSS_COLOROP, D3DTEXTUREOPFromBlendOperation(m_peColorOperation[TextureStage])));

        if(m_ppeColorArgument[TextureStage][0].getStatus() != RenderState<BlendArgument>::S_UNSET)
          D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
                   "IDirect3DDevice9::SetRenderState()",
                   spDevice->SetTextureStageState(TextureStage, D3DTSS_COLORARG1, D3DTEXTURESTAGESTATETYPEFromBlendArgument(m_ppeColorArgument[TextureStage][0])));

        if(m_ppeColorArgument[TextureStage][1].getStatus() != RenderState<BlendArgument>::S_UNSET)
          D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
                   "IDirect3DDevice9::SetRenderState()",
                   spDevice->SetTextureStageState(TextureStage, D3DTSS_COLORARG2, D3DTEXTURESTAGESTATETYPEFromBlendArgument(m_ppeColorArgument[TextureStage][1])));

        if(m_peAlphaOperation[TextureStage].getStatus() != RenderState<BlendOperation>::S_UNSET)
          D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
                   "IDirect3DDevice9::SetRenderState()",
                   spDevice->SetTextureStageState(TextureStage, D3DTSS_ALPHAOP, D3DTEXTUREOPFromBlendOperation(m_peAlphaOperation[TextureStage])));

        if(m_ppeAlphaArgument[TextureStage][0].getStatus() != RenderState<BlendArgument>::S_UNSET)
          D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
                   "IDirect3DDevice9::SetRenderState()",
                   spDevice->SetTextureStageState(TextureStage, D3DTSS_ALPHAARG1, D3DTEXTURESTAGESTATETYPEFromBlendArgument(m_ppeAlphaArgument[TextureStage][0])));

        if(m_ppeAlphaArgument[TextureStage][1].getStatus() != RenderState<BlendArgument>::S_UNSET)
          D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
                   "IDirect3DDevice9::SetRenderState()",
                   spDevice->SetTextureStageState(TextureStage, D3DTSS_ALPHAARG2, D3DTEXTURESTAGESTATETYPEFromBlendArgument(m_ppeAlphaArgument[TextureStage][1])));
      }

      EndStateBlock.Dismiss();
      D3DCheck("Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
              "IDirect3DDevice9::EndStateBlock()",
              spDevice->EndStateBlock(&spStateBlock));
    }

    m_spStateBlock = spStateBlock;
  }

  // use 'else' if state block creation sets the states for real at the same time
  if(m_spStateBlock) {
    D3DCheck(
      "Nuclex::Video::Direct3D9RenderStates::activateStateBlock()",
      "IDirect3DStateBlock9::Apply()",
      m_spStateBlock->Apply()
    );
  }
}

// ############################################################################################# //
// # Nuclex::Video::Direct3D9RenderStates::onDeviceReset()                                     # //
// ############################################################################################# //
void Direct3D9RenderStates::onDeviceShutdown(bool) {
  m_spStateBlock = NULL;
  ++m_Changes;
}
