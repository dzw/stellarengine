#pragma once
#ifndef MATH_D3DX9_POINT_H
#define MATH_D3DX9_POINT_H
//------------------------------------------------------------------------------
/**
    @class Math::point
    
    A point in homogenous space. A point describes a position in space,
    and has its W component set to 1.0.
    
    (C) 2007 Radon Labs GmbH
*/
#include "math/d3dx9/d3dx9_float4.h"
#include "math/d3dx9/d3dx9_vector.h"

//------------------------------------------------------------------------------
namespace Math
{
class point : public vector
{
public:
    /// default constructor
    point();
    /// construct from components
    point(scalar x, scalar y, scalar z);
    /// construct from float4
    point(const vector& rhs);
    /// copy constructor
    point(const point& rhs);
    /// return a point at the origin (0, 0, 0)
    static point origin();
    /// assignment operator
    void operator=(const point& rhs);
    /// inplace add vector
    void operator+=(const vector& rhs);
    /// inplace subtract vector
    void operator-=(const vector& rhs);
    /// add point and vector
    point operator+(const vector& rhs) const;
    /// subtract vectors from point
    point operator-(const vector& rhs) const;
    /// subtract point from point into a vector
    vector operator-(const point& rhs) const;
    /// equality operator
    bool operator==(const point& rhs) const;
    /// inequality operator
    bool operator!=(const point& rhs) const;
    /// set components
    void set(scalar x, scalar y, scalar z);
};

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point() :
    vector(0.0f, 0.0f, 0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point(scalar x, scalar y, scalar z) :
    vector(x, y, z)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point(const vector& rhs) :
    vector(rhs)
{
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
point::point(const point& rhs) :
    vector(rhs.X, rhs.Y, rhs.Z)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline point
point::origin()
{
    return point(0.0f, 0.0f, 0.0f);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::operator=(const point& rhs)
{
    this->X = rhs.X;
    this->Y = rhs.Y;
    this->Z = rhs.Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::operator+=(const vector& rhs)
{
    this->X += rhs.X;
    this->Y += rhs.Y;
    this->Z += rhs.Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::operator-=(const vector& rhs)
{
    this->X -= rhs.X;
    this->Y -= rhs.Y;
    this->Z -= rhs.Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline point
point::operator+(const vector& rhs) const
{
    return point(this->X + rhs.X, this->Y + rhs.Y, this->Z + rhs.Z);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline point
point::operator-(const vector& rhs) const
{
    return point(this->X - rhs.X, this->Y - rhs.Y, this->Z - rhs.Z);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
point::operator-(const point& rhs) const
{
    return vector(this->X - rhs.X, this->Y - rhs.Y, this->Z - rhs.Z);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
point::operator==(const point& rhs) const
{
    return (this->X == rhs.X) && (this->Y == rhs.Y) && (this->Z == rhs.Z);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
point::operator!=(const point& rhs) const
{
    return (this->X != rhs.X) || (this->Y != rhs.Y) || (this->Z != rhs.Z);
}    

//------------------------------------------------------------------------------
/**
*/
__forceinline void
point::set(scalar x, scalar y, scalar z)
{
    vector::set(x, y, z);
}

} // namespace Math
//------------------------------------------------------------------------------
#endif
