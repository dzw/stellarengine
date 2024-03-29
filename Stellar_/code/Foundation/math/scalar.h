#pragma once
#ifndef MATH_SCALAR_H
#define MATH_SCALAR_H
//------------------------------------------------------------------------------
/**
    @file math/scalar.h
    
    Nebula's scalar datatype.
    
    (C) 2007 Radon Labs GmbH
*/
#if __WIN32__
#include "math/d3dx9/d3dx9_scalar.h"
#else
#error "scalar class not implemented!"
#endif
//------------------------------------------------------------------------------
#endif
    