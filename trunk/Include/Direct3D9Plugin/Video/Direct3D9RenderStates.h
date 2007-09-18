//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Direct3D9RenderStateSet.h - Render state set        //
// ### # #      ###                                                      //
// # ### #      ###  A set of render states                              //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_DIRECT3D9RENDERSTATES_H
#define NUCLEX_DIRECT3D9RENDERSTATES_H

#include "Direct3D9Plugin/Direct3D9Plugin.h"
#include "Nuclex/Video/RenderStates.h"
#include <comdef.h>
#include "DirectX/D3D9.h"
#include "SigC++/SigC++.h"

namespace Nuclex { namespace Video {

class Direct3D9VideoDevice;

//  //
//  Nuclex::Video::Direct3D9RenderStates                                 //
//  //
/// Render states
/** Stores a set of render states
*/
class Direct3D9RenderStates :
  public RenderStates,
  public sigc::trackable {
  public:
    /// Constructor
    NUCLEXDIRECT3D9_API Direct3D9RenderStates(Direct3D9VideoDevice &VideoDevice);
    /// Destructor
    NUCLEXDIRECT3D9_API virtual ~Direct3D9RenderStates() {}

  //
  // Direct3D9RenderStateSet implementation
  //
  public:
    NUCLEXDIRECT3D9_API const IDirect3DStateBlock9Ptr &getStateBlock() const {
      return m_spStateBlock;
    }
    
    NUCLEXDIRECT3D9_API void activateStateBlock(const IDirect3DDevice9Ptr &spDevice) const;

  //
  // RenderStateSet implementation
  //
  public:
    NUCLEXDIRECT3D9_API void clear();

  private:
    void onDeviceShutdown(bool);
  
    mutable IDirect3DStateBlock9Ptr m_spStateBlock;
};

// ####################################################################### //
// # Nuclex::Video::D3DBLENDFromBlendMode()                              # //
// ####################################################################### //
/** Converts a nuclex blend mode into its direct3d equivalent

    @param  eMode  BlendMode to convert
    @return The equivalent direct3d blend mode
*/
inline D3DBLEND D3DBLENDFromBlendMode(RenderStates::BlendMode eMode) {
  switch(eMode) {
    case RenderStates::BM_ZERO: return D3DBLEND_ZERO;
    case RenderStates::BM_ONE: return D3DBLEND_ONE;
    case RenderStates::BM_SRCCOLOR: return D3DBLEND_SRCCOLOR;
    case RenderStates::BM_INVSRCCOLOR: return D3DBLEND_INVSRCCOLOR;
    case RenderStates::BM_SRCALPHA: return D3DBLEND_SRCALPHA;;
    case RenderStates::BM_INVSRCALPHA: return D3DBLEND_INVSRCALPHA;
    case RenderStates::BM_DESTALPHA: return D3DBLEND_DESTALPHA;
    case RenderStates::BM_INVDESTALPHA: return D3DBLEND_INVDESTALPHA;
    case RenderStates::BM_DESTCOLOR: return D3DBLEND_DESTCOLOR;
    case RenderStates::BM_INVDESTCOLOR: return D3DBLEND_INVDESTCOLOR;
    case RenderStates::BM_SRCALPHASAT: return D3DBLEND_SRCALPHASAT;
    case RenderStates::BM_BOTHSRCALPHA: return D3DBLEND_BOTHSRCALPHA;
    case RenderStates::BM_BOTHINVSRCALPHA: return D3DBLEND_BOTHINVSRCALPHA;
    default: throw UnexpectedException("Nuclex::Video::D3DBLENDFromBlendMode()", "Unknown blend mode");
  }
}

// ####################################################################### //
// # Nuclex::Video::D3DCULLFromCullMode()                                # //
// ####################################################################### //
/** Converts a nuclex cull mode into its direct3d equivalent

    @param  eMode  CullMode to convert
    @return The equivalent direct3d cull mode
*/
inline D3DCULL D3DCULLFromCullMode(RenderStates::CullMode eMode) {
  switch(eMode) {
    case RenderStates::CM_OFF: return D3DCULL_NONE;
    case RenderStates::CM_CLOCKWISE: return D3DCULL_CW;
    case RenderStates::CM_COUNTERCLOCKWISE: return D3DCULL_CCW;
    default: throw UnexpectedException("Nuclex::Video::D3DCULLFromCullMode()", "Unknown culling mode");
  }
}

// ####################################################################### //
// # Nuclex::Video::D3DFILLFromFillMode()                                # //
// ####################################################################### //
/** Converts a nuclex fill mode into its direct3d equivalent

    @param  eMode  CullMode to convert
    @return The equivalent direct3d fill mode
*/
inline D3DFILLMODE D3DFILLMODEFromFillMode(RenderStates::FillMode eMode) {
  switch(eMode) {
    case RenderStates::FM_POINT: return D3DFILL_POINT;
    case RenderStates::FM_WIREFRAME: return D3DFILL_WIREFRAME;
    case RenderStates::FM_SOLID: return D3DFILL_SOLID;
    default: throw UnexpectedException("Nuclex::Video::D3DFILLMODEFromFillMode()", "Unknown fill mode");
  }
}

// ####################################################################### //
// # Nuclex::Video::D3DTEXTUREOPFromBlendOperation()                     # //
// ####################################################################### //
/** Converts a nuclex blend operation into its direct3d equivalent

    @param  eOperation  Blend operation to convert
    @return The equivalent direct3d texture operation
*/
inline D3DTEXTUREOP D3DTEXTUREOPFromBlendOperation(RenderStates::BlendOperation eOperation) {
  switch(eOperation) {
    case RenderStates::BO_NONE: return D3DTOP_DISABLE;
    case RenderStates::BO_ARGUMENT1: return D3DTOP_SELECTARG1;
    case RenderStates::BO_ARGUMENT2: return D3DTOP_SELECTARG2;
    case RenderStates::BO_MODULATE: return D3DTOP_MODULATE;
    case RenderStates::BO_MODULATE2X: return D3DTOP_MODULATE2X;
    case RenderStates::BO_MODULATE4X: return D3DTOP_MODULATE4X;
    default: throw UnexpectedException("Nuclex::Video::D3DTEXTUREOPFromBlendOperation()", "Unknown blend operation");
  }
}

// ####################################################################### //
// # Nuclex::Video::D3DTEXTURESTAGESTATETYPEFromBlendArgument()          # //
// ####################################################################### //
/** Converts a nuclex blend argument into its direct3d equivalent

    @param  eArgument  Blend argument to convert
    @return The equivalent direct3d texture stage state
*/
inline D3DTEXTURESTAGESTATETYPE D3DTEXTURESTAGESTATETYPEFromBlendArgument(RenderStates::BlendArgument eArgument) {
  switch(eArgument) {
    case RenderStates::BA_COLOROPERATION: return D3DTSS_COLOROP;
    case RenderStates::BA_COLORARGUMENT1: return D3DTSS_COLORARG1;
    case RenderStates::BA_COLORARGUMENT2: return D3DTSS_COLORARG2;
    case RenderStates::BA_ALPHAOPERATION: return D3DTSS_ALPHAOP;
    case RenderStates::BA_ALPHAARGUMENT1: return D3DTSS_ALPHAARG1;
    case RenderStates::BA_ALPHAARGUMENT2: return D3DTSS_ALPHAARG2;
    default: throw UnexpectedException("Nuclex::Video::D3DTEXTURESTAGESTATETYPEFromBlendArgument()", "Unknown blend argument");
  }
}

// ####################################################################### //
// # Nuclex::Video::D3DTEXTUREFILTERTYPEFromTextureFilter()              # //
// ####################################################################### //
/** Converts a nuclex texture filter into its direct3d equivalent

    @param  eFilter  Texture filter to convert
    @return The equivalent direct3d texture filter type
*/
inline D3DTEXTUREFILTERTYPE D3DTEXTUREFILTERTYPEFromTextureFilter(RenderStates::TextureFilter eFilter) {
  switch(eFilter) {
    case RenderStates::TF_NONE: return D3DTEXF_NONE;
    case RenderStates::TF_NEAREST: return D3DTEXF_POINT;
    case RenderStates::TF_LINEAR: return D3DTEXF_LINEAR;
    case RenderStates::TF_ANISOTROPIC: return D3DTEXF_ANISOTROPIC;
    default: throw UnexpectedException("Nuclex::Video::D3DTEXTUREFILTERTYPEFromTextureFilter()", "Unknown texture filter");
  }
}

// ####################################################################### //
// # Nuclex::Video::D3DTEXTUREADDRESSFromTextureAddressMode()            # //
// ####################################################################### //
/** Converts a nuclex texture addressing mode into its direct3d equivalent

    @param  eMode  Texture addressing mode to convert
    @return The equivalent direct3d texture addressing mode
*/
inline D3DTEXTUREADDRESS D3DTEXTUREADDRESSFromTextureAddressMode(RenderStates::TextureAddressMode eMode) {
  switch(eMode) {
    case RenderStates::TAM_WRAP: return D3DTADDRESS_WRAP;
    case RenderStates::TAM_MIRROR: return D3DTADDRESS_MIRROR;
    case RenderStates::TAM_CLAMP: return D3DTADDRESS_CLAMP;
    case RenderStates::TAM_BORDER: return D3DTADDRESS_BORDER;
    case RenderStates::TAM_MIRRORONCE: return D3DTADDRESS_MIRRORONCE;
    default: throw UnexpectedException("Nuclex::Video::D3DTEXTUREADDRESSFromTextureAddressMode()", "Unknown texture addressing mode");
  }
}

// ####################################################################### //
// # Nuclex::Video::D3DSAMPLERSTATETYPEFromDimension()                   # //
// ####################################################################### //
/** Converts a nuclex dimension into a direct3d texture addressing dimension

    @param  eDimension  Dimension to convert
    @return The matching direct3d texture addressign dimension
*/
inline D3DSAMPLERSTATETYPE D3DSAMPLERSTATETYPEFromDimension(RenderStates::Dimension eDimension) {
  switch(eDimension) {
    case RenderStates::D_U: return D3DSAMP_ADDRESSU;
    case RenderStates::D_V: return D3DSAMP_ADDRESSV;
    case RenderStates::D_W: return D3DSAMP_ADDRESSW;
    default: throw UnexpectedException("Nuclex::Video::D3DSAMPLERSTATETYPEFromDimension()", "Unknown dimension");
  }
}

}} // namespace Nuclex::Video

#endif // NUCLEX_VIDEO_DIRECT3D9RENDERSTATES_H
