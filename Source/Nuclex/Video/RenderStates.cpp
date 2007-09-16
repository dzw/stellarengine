//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## RenderStates.cpp - Rendering parameters                                   //
// ### # #      ###                                                                            //
// # ### #      ###  Additional parameters for adjusting rendering behavior                    //
// #  ## #   # ## ##                                                                           //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Video/RenderStates.h"

using namespace Nuclex;
using namespace Nuclex::Video;

// The entire design is totally fucked up
//
// Slightly unconforming operator =()
// Cumbersome writing required at plugin side
//   (eg comparing the current setting with the new one)
// 

// ############################################################################################# //
// # Nuclex::Video::RenderStates::clear()                                                      # //
// ############################################################################################# //
void RenderStates::clear() {
  m_bAlphaBlendEnabled.setStatus(RenderState<bool>::S_UNSET);
  m_bLightingEnabled.setStatus(RenderState<bool>::S_UNSET);
  m_bDepthBufferEnabled.setStatus(RenderState<bool>::S_UNSET);
  m_eSourceBlendMode.setStatus(RenderState<BlendMode>::S_UNSET);
  m_eDestinationBlendMode.setStatus(RenderState<BlendMode>::S_UNSET);
  m_eCullMode.setStatus(RenderState<CullMode>::S_UNSET);
  m_eFillMode.setStatus(RenderState<FillMode>::S_UNSET);
  
  for(size_t TextureStage = 0; TextureStage < MaxTextureStages; ++TextureStage) {
    m_peColorOperation[TextureStage].setStatus(RenderState<BlendOperation>::S_UNSET);
    m_ppeColorArgument[TextureStage][0].setStatus(RenderState<BlendArgument>::S_UNSET);
    m_ppeColorArgument[TextureStage][1].setStatus(RenderState<BlendArgument>::S_UNSET);
    m_peAlphaOperation[TextureStage].setStatus(RenderState<BlendOperation>::S_UNSET);
    m_ppeAlphaArgument[TextureStage][0].setStatus(RenderState<BlendArgument>::S_UNSET);
    m_ppeAlphaArgument[TextureStage][1].setStatus(RenderState<BlendArgument>::S_UNSET);

    m_peMinificationFilter[TextureStage].setStatus(RenderState<TextureFilter>::S_UNSET);
    m_peMagnificationFilter[TextureStage].setStatus(RenderState<TextureFilter>::S_UNSET);
    m_ppeTextureAddressMode[TextureStage][0].setStatus(RenderState<TextureAddressMode>::S_UNSET);
    m_ppeTextureAddressMode[TextureStage][1].setStatus(RenderState<TextureAddressMode>::S_UNSET);
    m_ppeTextureAddressMode[TextureStage][2].setStatus(RenderState<TextureAddressMode>::S_UNSET);
  }
  
  m_Changes = 0;
}

// ############################################################################################# //
// # Nuclex::Video::RenderStates::operator +=()                                                # //
// ############################################################################################# //
RenderStates &RenderStates::operator +=(const RenderStates &Other) {
  if(Other.m_bAlphaBlendEnabled.getStatus() != RenderState<bool>::S_UNSET)
    enableAlphaBlend(Other.m_bAlphaBlendEnabled);

  if(Other.m_bLightingEnabled.getStatus() != RenderState<bool>::S_UNSET)
    enableLighting(Other.m_bLightingEnabled);

  if(Other.m_bDepthBufferEnabled.getStatus() != RenderState<bool>::S_UNSET)
    enableDepthBuffer(Other.m_bDepthBufferEnabled);

  if(Other.m_eSourceBlendMode.getStatus() != RenderState<BlendMode>::S_UNSET)
    setSourceBlendMode(Other.m_eSourceBlendMode);

  if(Other.m_eDestinationBlendMode.getStatus() != RenderState<BlendMode>::S_UNSET)
    setDestinationBlendMode(Other.m_eDestinationBlendMode);

  if(Other.m_eCullMode.getStatus() != RenderState<CullMode>::S_UNSET)
    setCullMode(Other.m_eCullMode);

  if(Other.m_eFillMode.getStatus() != RenderState<FillMode>::S_UNSET)
    setFillMode(Other.m_eFillMode);

  for(size_t TextureStage = 0; TextureStage < MaxTextureStages; ++TextureStage) {
    if(Other.m_peColorOperation[TextureStage].getStatus() != RenderState<BlendOperation>::S_UNSET)
      setColorOperation(TextureStage, Other.m_peColorOperation[TextureStage]);

    if(Other.m_ppeColorArgument[TextureStage][0].getStatus() != RenderState<BlendArgument>::S_UNSET)
      setColorArgument(TextureStage, 0, Other.m_ppeColorArgument[TextureStage][0]);

    if(Other.m_ppeColorArgument[TextureStage][1].getStatus() != RenderState<BlendArgument>::S_UNSET)
      setColorArgument(TextureStage, 1, Other.m_ppeColorArgument[TextureStage][1]);
      
    if(Other.m_peAlphaOperation[TextureStage].getStatus() != RenderState<BlendOperation>::S_UNSET)
      setAlphaOperation(TextureStage, Other.m_peAlphaOperation[TextureStage]);

    if(Other.m_ppeAlphaArgument[TextureStage][0].getStatus() != RenderState<BlendArgument>::S_UNSET)
      setAlphaArgument(TextureStage, 0, Other.m_ppeAlphaArgument[TextureStage][0]);

    if(Other.m_ppeAlphaArgument[TextureStage][1].getStatus() != RenderState<BlendArgument>::S_UNSET)
      setAlphaArgument(TextureStage, 1, Other.m_ppeAlphaArgument[TextureStage][1]);

    if(Other.m_peMinificationFilter[TextureStage].getStatus() != RenderState<TextureFilter>::S_UNSET)
      setMinificationFilter(TextureStage, Other.m_peMinificationFilter[TextureStage]);

    if(Other.m_peMagnificationFilter[TextureStage].getStatus() != RenderState<TextureFilter>::S_UNSET)
      setMagnificationFilter(TextureStage, Other.m_peMagnificationFilter[TextureStage]);

    if(Other.m_ppeTextureAddressMode[TextureStage][0].getStatus() != RenderState<TextureAddressMode>::S_UNSET)
      setTextureAddressMode(TextureStage, D_U, Other.m_ppeTextureAddressMode[TextureStage][0]);
      
    if(Other.m_ppeTextureAddressMode[TextureStage][1].getStatus() != RenderState<TextureAddressMode>::S_UNSET)
      setTextureAddressMode(TextureStage, D_V, Other.m_ppeTextureAddressMode[TextureStage][1]);

    if(Other.m_ppeTextureAddressMode[TextureStage][2].getStatus() != RenderState<TextureAddressMode>::S_UNSET)
      setTextureAddressMode(TextureStage, D_W, Other.m_ppeTextureAddressMode[TextureStage][2]);
  }

  return *this;
}
