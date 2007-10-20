//------------------------------------------------------------------------------
//  float4.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "math/d3dx9/d3dx9_float4.h"
#include "math/d3dx9/d3dx9_matrix44.h"

namespace Math
{

//------------------------------------------------------------------------------
/**
*/
float4
float4::transform(const float4& v, const matrix44& m)
{
    float4 res;
    D3DXVec4Transform((D3DXVECTOR4*)&res, (CONST D3DXVECTOR4*)&v, (CONST D3DXMATRIX*)&m);
    return res;
}

} // namespace Math