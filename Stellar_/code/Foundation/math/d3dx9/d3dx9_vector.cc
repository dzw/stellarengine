//------------------------------------------------------------------------------
//  float4.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "math/d3dx9/d3dx9_vector.h"
#include "math/d3dx9/d3dx9_matrix44.h"
#include "math/d3dx9/d3dx9_float4.h"

namespace Math
{

//------------------------------------------------------------------------------
/**
*/
float4
vector::transform(const vector& v, const matrix44& m)
{
    float4 res;
    D3DXVec3Transform((D3DXVECTOR4*)&res, (CONST D3DXVECTOR3*)&v, (CONST D3DXMATRIX*)&m);
    return res;
}

} // namespace Math