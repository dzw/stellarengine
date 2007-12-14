#pragma once
#ifndef MATH_D3DX9_SCALAR_H
#define MATH_D3DX9_SCALAR_H
//------------------------------------------------------------------------------
/**
    @file math/d3dx9/scalar.h
    
    Scalar typedef and math functions for D3DX9 math functions.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/types.h"

namespace Math
{
typedef float scalar;

#ifndef PI
#define PI (3.1415926535897932384626433832795028841971693993751f)
#endif
#define S_PI PI

#ifndef TINY
#define TINY (0.0000001f)
#endif
#define S_TINY TINY

#define s_max(a,b)      (((a) > (b)) ? (a) : (b))
#define s_min(a,b)      (((a) < (b)) ? (a) : (b))
#define s_abs(a)        (((a)<0.0f) ? (-(a)) : (a))
#define s_sgn(a)        (((a)<0.0f) ? (-1) : (1))

#define s_max(a,b)      (((a) > (b)) ? (a) : (b))
#define s_min(a,b)      (((a) < (b)) ? (a) : (b))
#define s_abs(a)        (((a)<0.0f) ? (-(a)) : (a))
#define s_sgn(a)        (((a)<0.0f) ? (-1) : (1))
#define s_deg2rad(d)    (((d)*PI)/180.0f)
#define s_rad2deg(r)    (((r)*180.0f)/PI)

const scalar LN_2 = 0.693147180559945f;

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
s_sin(scalar x)
{
    return sinf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
s_cos(scalar x)
{
    return cosf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
s_asin(scalar x)
{
    return asinf(x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
s_acos(scalar x)
{
    return acosf(x);
}

//------------------------------------------------------------------------------
/**
    log2() function.
*/
__forceinline scalar 
s_log2(scalar f) 
{ 
    return logf(f) / LN_2; 
}

//------------------------------------------------------------------------------
/**
    Integer clamping.
*/
__forceinline int 
s_iclamp(int val, int minVal, int maxVal)
{
    if (val < minVal)      return minVal;
    else if (val > maxVal) return maxVal;
    else return val;
}

//------------------------------------------------------------------------------
/**
    Safe sqrt.
*/
__forceinline scalar 
s_sqrt(scalar x)
{
    return sqrtf(x);
}

//------------------------------------------------------------------------------
/**
    A fuzzy floating point equality check
*/
__forceinline bool 
s_fequal(scalar f0, scalar f1, scalar tol) 
{
    scalar f = f0 - f1;
    return ((f > (-tol)) && (f < tol));
}

//------------------------------------------------------------------------------
/**
    A fuzzy floating point less-then check.
*/
__forceinline bool 
s_fless(scalar f0, scalar f1, scalar tol) 
{
    return ((f0 - f1) < tol);
}

//------------------------------------------------------------------------------
/**
    A fuzzy floating point greater-then check.
*/
__forceinline bool 
s_fgreater(scalar f0, scalar f1, scalar tol) 
{
    return ((f0 - f1) > tol);
}

//------------------------------------------------------------------------------
/**
    Smooth a new value towards an old value using a change value.
*/
__forceinline scalar
s_smooth(scalar newVal, scalar curVal, scalar maxChange)
{
    scalar diff = newVal - curVal;
    if (fabs(diff) > maxChange)
    {
        if (diff > 0.0f)
        {
            curVal += maxChange;
            if (curVal > newVal)
            {
                curVal = newVal;
            }
        }
        else if (diff < 0.0f)
        {
            curVal -= maxChange;
            if (curVal < newVal)
            {
                curVal = newVal;
            }
        }
    }
    else
    {
        curVal = newVal;
    }
    return curVal;
}

//------------------------------------------------------------------------------
/**
    Clamp a value against lower und upper boundary.
*/
__forceinline scalar
s_clamp(scalar val, scalar lower, scalar upper)
{
    if (val < lower)      return lower;
    else if (val > upper) return upper;
    else                  return val;
}

//------------------------------------------------------------------------------
/**
    Saturate a value (clamps between 0.0f and 1.0f)
*/
__forceinline scalar 
s_saturate(scalar val)
{
    if (val < 0.0f)      return 0.0f;
    else if (val > 1.0f) return 1.0f;
    else return val;
}

//------------------------------------------------------------------------------
/**
    Return a pseudo random number between 0 and 1.
*/
__forceinline scalar s_rand()
{
    return scalar(rand()) / scalar(RAND_MAX);
}

//------------------------------------------------------------------------------
/**
    Return a pseudo random number between min and max.
*/
__forceinline scalar 
s_rand(scalar min, scalar max)
{
	scalar unit = scalar(rand()) / RAND_MAX;
	scalar diff = max - min;
	return min + unit * diff;
}

//------------------------------------------------------------------------------
/**
    Chop float to int.
*/
__forceinline int 
s_fchop(scalar f)
{
    /// @todo type cast to int is slow!
    return int(f);
}

//------------------------------------------------------------------------------
/**
    Round float to integer.
*/
__forceinline int 
s_frnd(scalar f)
{
    return s_fchop(floorf(f + 0.5f));
}

//------------------------------------------------------------------------------
/**
    Linearly interpolate between 2 values: ret = x + l * (y - x)
*/
__forceinline float 
s_lerp(scalar x, scalar y, scalar l)
{
    return x + l * (y - x);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
s_fmod(scalar x, scalar y)
{
    return fmodf(x, y);
}

//------------------------------------------------------------------------------
/**
    Normalize an angular value into the range rad(0) to rad(360).
*/
__forceinline scalar s_modangle(scalar a) 
{
    // FIXME: hmm...
    while(a < 0.0f)
    {
        a += s_deg2rad(360.0f);
    }
    if (a >= s_deg2rad(360.0f))
    {
        a = s_fmod(a, s_deg2rad(360.0f));
    }
    return a;
}

} // namespace Math
//------------------------------------------------------------------------------
#endif



    

    