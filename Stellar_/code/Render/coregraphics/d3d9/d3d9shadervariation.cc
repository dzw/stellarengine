//------------------------------------------------------------------------------
//  d3d9shadervariation.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/d3d9/d3d9shadervariation.h"
#include "coregraphics/shaderserver.h"

namespace Direct3D9
{
ImplementClass(Direct3D9::D3D9ShaderVariation, 'D9SV', Base::ShaderVariationBase);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
D3D9ShaderVariation::D3D9ShaderVariation() :
    d3d9Effect(0),
    hTechnique(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
D3D9ShaderVariation::~D3D9ShaderVariation()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
D3D9ShaderVariation::Setup(ID3DXEffect* effect, D3DXHANDLE handle)
{
    s_assert(0 != effect);
    s_assert(0 != handle);
    s_assert(0 == this->d3d9Effect);
    s_assert(0 == this->hTechnique);
    s_assert(0 == this->featureMask);
    s_assert(0 == this->numPasses);

    this->d3d9Effect = effect;
    this->hTechnique = handle;

    // setup name and feature mask
    D3DXTECHNIQUE_DESC desc;
    HRESULT hr = this->d3d9Effect->GetTechniqueDesc(this->hTechnique, &desc);
    s_assert(SUCCEEDED(hr));
    this->SetName(Name(desc.Name));
    this->SetNumPasses(desc.Passes);

    // generate the feature mask from the "Mask" annotation
    D3DXHANDLE hFeatureAnnotation = this->d3d9Effect->GetAnnotationByName(this->hTechnique, "Mask");
    if (NULL != hFeatureAnnotation)
    {
        LPCSTR strPtr = 0;
        hr = this->d3d9Effect->GetString(hFeatureAnnotation, &strPtr);
        s_assert(SUCCEEDED(hr));
        ShaderFeature::Mask mask = ShaderServer::Instance()->FeatureStringToMask(strPtr);
        this->SetFeatureMask(mask);
    }
    else
    {
        // issue a warning
        s_warning("No Features annotation in technique '%s'!\n", this->GetName().Value().c_str());
    }
}

} // namespace Direct3D9