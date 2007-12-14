#pragma once
#ifndef MATH_D3DX9_VECTOR_H
#define MATH_D3DX9_VECTOR_H
//------------------------------------------------------------------------------
/**
    @class Math::vector
    
    A vector in homogenous space. A vector describes a direction and length
    in 3d space and always has a w component of 0.0.
    
    (C) 2007 Radon Labs GmbH
*/
#include "math/d3dx9/d3dx9_float4.h"

namespace Math
{
class vector
{
public:
	/// a comparison result
	typedef char cmpresult;

	/// default constructor
	vector();
	/// construct from components
	vector(scalar x, scalar y, scalar z);
	/// construct from float4
	vector(const float4& rhs);
	/// copy construct
	vector(const vector& rhs);
	
	/// assignment operator
	void operator=(const vector& rhs);
	/// assignment operator
	void operator=(const float4& rhs);
	/// flip sign
	vector operator-() const;
	/// inplace add
	void operator+=(const vector& rhs);
	/// inplace sub
	void operator-=(const vector& rhs);
	/// inplace scalar multiply
	void operator*=(scalar s);
	/// inplace scalar divide
	void operator/=(scalar s);

	/// add 2 vectors
	vector operator+(const vector& rhs) const;
	/// subtract 2 vectors
	vector operator-(const vector& rhs) const;
	/// multiply with scalar
	vector operator*(scalar s) const;
	/// divide with scalar
	vector operator/(scalar s) const;
	/// equality operator
	bool operator==(const vector& rhs) const;
	/// inequality operator
	bool operator!=(const vector& rhs) const;

	/// load content from 16-byte-aligned memory
	void load(const scalar* ptr);
	/// load content from unaligned memory
	void loadu(const scalar* ptr);
	/// write content to 16-byte-aligned memory through the write cache
	void store(scalar* ptr) const;
	/// write content to unaligned memory through the write cache
	void storeu(scalar* ptr) const;
	/// stream content to 16-byte-aligned memory circumventing the write-cache
	void stream(scalar* ptr) const;

	/// set content
	void set(scalar x, scalar y, scalar z);
	/// read/write access to x component
	scalar& x();
	/// read/write access to y component
	scalar& y();
	/// read/write access to z component
	scalar& z();
	/// read-only access to x component
	scalar x() const;
	/// read-only access to y component
	scalar y() const;
	/// read-only access to z component
	scalar z() const;

	/// return length of vector
	scalar length() const;
	/// return squared length of vector
	scalar lengthsq() const;
	/// return compononent-wise absolute
	vector abs() const;

	/// return 3-dimensional cross product
	static vector cross3(const vector& v0, const vector& v1);
	/// return 3d dot product of vectors
	static scalar dot3(const vector& v0, const vector& v1);
	/// return point in barycentric coordinates
	static vector barycentric(const vector& v0, const vector& v1, const vector& v2, scalar f, scalar g);
	/// perform Catmull-Rom interpolation
	static vector catmullrom(const vector& v0, const vector& v1, const vector& v2, const vector& v3, scalar s);
	/// perform Hermite spline interpolation
	static vector hermite(const vector& v1, const vector& t1, const vector& v2, const vector& t2, scalar s);
	/// perform linear interpolation between 2 4d vectors
	static vector lerp(const vector& v0, const vector& v1, scalar s);
	/// return 4d vector made up of largest components of 2 vectors
	static vector maximize(const vector& v0, const vector& v1);
	/// return 4d vector made up of smallest components of 2 vectors
	static vector minimize(const vector& v0, const vector& v1);
	/// return normalized version of 4d vector
	static vector normalize(const vector& v);
	/// transform 4d vector by matrix44
	static float4 transform(const vector& v, const matrix44& m);

	/// perform less-then comparison
	static cmpresult less(const vector& v0, const vector& v1);
	/// perform less-or-equal comparison
	static cmpresult lessequal(const vector& v0, const vector& v1);
	/// perform greater-then comparison
	static cmpresult greater(const vector& v0, const vector& v1);
	/// perform greater-or-equal comparison
	static cmpresult greaterequal(const vector& v0, const vector& v1);
	/// check comparison result for all-condition
	static bool any(cmpresult res);
	/// check comparison result for any-condition
	static bool all(cmpresult res);

protected:
	friend class matrix44;
	friend class point;

	scalar X;
	scalar Y;
	scalar Z;
};

//------------------------------------------------------------------------------
/**
*/
__forceinline
vector::vector()
{
	//  empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline 
vector::vector(scalar x, scalar y, scalar z) :
X(x), Y(y), Z(z)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
vector::vector(const vector& rhs) :
X(rhs.X), Y(rhs.Y), Z(rhs.Z)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline
vector::vector(const float4& rhs) :
X(rhs.x()), Y(rhs.y()), Z(rhs.z())
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::operator=(const vector& rhs)
{
	this->X = rhs.X;
	this->Y = rhs.Y;
	this->Z = rhs.Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::operator=(const float4& rhs)
{
	this->X = rhs.x();
	this->Y = rhs.y();
	this->Z = rhs.z();
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
vector::operator==(const vector& rhs) const
{
	return (this->X == rhs.X) && (this->Y == rhs.Y) && (this->Z == rhs.Z);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
vector::operator!=(const vector& rhs) const
{
	return (this->X != rhs.X) || (this->Y != rhs.Y) || (this->Z != rhs.Z);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::load(const scalar* ptr)
{
	this->X = ptr[0];
	this->Y = ptr[1];
	this->Z = ptr[2];
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::loadu(const scalar* ptr)
{
	this->X = ptr[0];
	this->Y = ptr[1];
	this->Z = ptr[2];
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::store(scalar* ptr) const
{
	ptr[0] = this->X;
	ptr[1] = this->Y;
	ptr[2] = this->Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::storeu(scalar* ptr) const
{
	ptr[0] = this->X;
	ptr[1] = this->Y;
	ptr[2] = this->Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::stream(scalar* ptr) const
{
	ptr[0] = this->X;
	ptr[1] = this->Y;
	ptr[2] = this->Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::operator-() const
{
	return vector(-this->X, -this->Y, -this->Z);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::operator*(scalar t) const
{
	return vector(this->X * t, this->Y * t, this->Z * t);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::operator/(scalar t) const
{
	scalar fInv = 1.0f / t;
	return vector(this->X * fInv, this->Y * fInv, this->Z * fInv);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::operator+=(const vector& rhs)
{
	this->X += rhs.X;
	this->Y += rhs.Y;
	this->Z += rhs.Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::operator-=(const vector& rhs)
{
	this->X -= rhs.X;
	this->Y -= rhs.Y;
	this->Z -= rhs.Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::operator*=(scalar s)
{
	this->X *= s;
	this->Y *= s;
	this->Z *= s;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::operator/=(scalar s)
{
	scalar fInv = 1.0f / s;
	this->X *= fInv;
	this->Y *= fInv;
	this->Z *= fInv;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::operator+(const vector& rhs) const
{
	return vector(this->X + rhs.X, this->Y + rhs.Y, this->Z + rhs.Z);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::operator-(const vector& rhs) const
{
	return vector(this->X - rhs.X, this->Y - rhs.Y, this->Z - rhs.Z);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline void
vector::set(scalar x, scalar y, scalar z)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar&
vector::x()
{
	return this->X;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
vector::x() const
{
	return this->X;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar&
vector::y()
{
	return this->Y;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
vector::y() const
{
	return this->Y;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar&
vector::z()
{
	return this->Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
vector::z() const
{
	return this->Z;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
vector::length() const
{
	return D3DXVec3Length((CONST D3DXVECTOR3*)this);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
vector::lengthsq() const
{
	return D3DXVec3LengthSq((CONST D3DXVECTOR3*)this);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::abs() const
{
	return vector(s_abs(this->X), s_abs(this->Y), s_abs(this->Z));
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::cross3(const vector& v0, const vector& v1)
{
	vector res;
	D3DXVec3Cross((D3DXVECTOR3*)&res, (CONST D3DXVECTOR3*)&v0, (CONST D3DXVECTOR3*) &v1);
	return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline scalar
vector::dot3(const vector& v0, const vector& v1)
{
	return D3DXVec3Dot((CONST D3DXVECTOR3*)&v0, (CONST D3DXVECTOR3*) &v1);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::barycentric(const vector& v0, const vector& v1, const vector& v2, scalar f, scalar g)
{
	vector res;
	D3DXVec3BaryCentric((D3DXVECTOR3*)&res, (CONST D3DXVECTOR3*)&v0, (CONST D3DXVECTOR3*)&v1, (CONST D3DXVECTOR3*)&v2, f, g);
	return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::catmullrom(const vector& v0, const vector& v1, const vector& v2, const vector& v3, scalar s)
{
	vector res;
	D3DXVec3CatmullRom((D3DXVECTOR3*)&res, (CONST D3DXVECTOR3*)&v0, (CONST D3DXVECTOR3*)&v1, (CONST D3DXVECTOR3*)&v2, (CONST D3DXVECTOR3*)&v3, s);
	return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::hermite(const vector& v1, const vector& t1, const vector& v2, const vector& t2, scalar s)
{
	vector res;
	D3DXVec3Hermite((D3DXVECTOR3*)&res, (CONST D3DXVECTOR3*)&v1, (CONST D3DXVECTOR3*)&t1, (CONST D3DXVECTOR3*)&v2, (CONST D3DXVECTOR3*)&t2, s);
	return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::lerp(const vector& v0, const vector& v1, scalar s)
{
	vector res;
	D3DXVec3Lerp((D3DXVECTOR3*)&res, (CONST D3DXVECTOR3*)&v0, (CONST D3DXVECTOR3*)&v1, s);
	return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::maximize(const vector& v0, const vector& v1)
{
	vector res;
	D3DXVec3Maximize((D3DXVECTOR3*)&res, (CONST D3DXVECTOR3*)&v0, (CONST D3DXVECTOR3*)&v1);
	return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::minimize(const vector& v0, const vector& v1)
{
	vector res;
	D3DXVec3Minimize((D3DXVECTOR3*)&res, (CONST D3DXVECTOR3*)&v0, (CONST D3DXVECTOR3*)&v1);
	return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector
vector::normalize(const vector& v)
{
	vector res;
	D3DXVec3Normalize((D3DXVECTOR3*)&res, (CONST D3DXVECTOR3*) &v);
	return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector::cmpresult
vector::less(const vector& v0, const vector& v1)
{
	cmpresult res = 0;
	if (v0.X < v1.X) res |= (1<<0);
	if (v0.Y < v1.Y) res |= (1<<1);
	if (v0.Z < v1.Z) res |= (1<<2);
	return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector::cmpresult
vector::lessequal(const vector& v0, const vector& v1)
{
	cmpresult res = 0;
	if (v0.X <= v1.X) res |= (1<<0);
	if (v0.Y <= v1.Y) res |= (1<<1);
	if (v0.Z <= v1.Z) res |= (1<<2);
	return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector::cmpresult
vector::greater(const vector& v0, const vector& v1)
{
	cmpresult res = 0;
	if (v0.X > v1.X) res |= (1<<0);
	if (v0.Y > v1.Y) res |= (1<<1);
	if (v0.Z > v1.Z) res |= (1<<2);
	return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline vector::cmpresult
vector::greaterequal(const vector& v0, const vector& v1)
{
	cmpresult res = 0;
	if (v0.X >= v1.X) res |= (1<<0);
	if (v0.Y >= v1.Y) res |= (1<<1);
	if (v0.Z >= v1.Z) res |= (1<<2);
	return res;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
vector::any(cmpresult res)
{
	return res != 0;
}

//------------------------------------------------------------------------------
/**
*/
__forceinline bool
vector::all(cmpresult res)
{
	return res == ((1<<0) | (1<<1) | (1<<2));
}
}
////------------------------------------------------------------------------------
//namespace Math
//{
//class vector : public float4
//{
//public:
//    /// default constructor
//    vector();
//    /// construct from components
//    vector(scalar x, scalar y, scalar z);
//    /// construct from float4
//    vector(const float4& rhs);
//    /// copy constructor
//    vector(const vector& rhs);
//    /// return the null vector
//    static vector nullvec();
//    /// return the standard up vector (0, 1, 0)
//    static vector upvec();
//    /// assignment operator
//    void operator=(const vector& rhs);
//    /// add vector inplace
//    void operator+=(const vector& rhs);
//    /// subtract vector inplace
//    void operator-=(const vector& rhs);
//    /// scale vector inplace
//    void operator*=(scalar s);
//    /// add 2 vectors
//    vector operator+(const vector& rhs) const;
//    /// subtract 2 vectors
//    vector operator-(const vector& rhs) const;
//    /// scale vector
//    vector operator*(scalar s) const;
//    /// equality operator
//    bool operator==(const vector& rhs) const;
//    /// inequality operator
//    bool operator!=(const vector& rhs) const;
//    /// set components
//    void set(scalar x, scalar y, scalar z);
//
//    friend class point;
//};
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline
//vector::vector() :
//    float4(0.0f, 0.0f, 0.0f, 0.0f)
//{
//    // empty
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline
//vector::vector(scalar x, scalar y, scalar z) :
//    float4(x, y, z, 0.0f)
//{
//    // empty
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline
//vector::vector(const float4& rhs) :
//    float4(rhs)
//{
//    this->W = 0.0f;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline
//vector::vector(const vector& rhs) :
//    float4(rhs.X, rhs.Y, rhs.Z, 0.0f)
//{
//    // empty
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline vector
//vector::nullvec()
//{
//    return vector(0.0f, 0.0f, 0.0f);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline vector
//vector::upvec()
//{
//    return vector(0.0f, 1.0f, 0.0f);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline void
//vector::operator=(const vector& rhs)
//{
//    this->X = rhs.X;
//    this->Y = rhs.Y;
//    this->Z = rhs.Z;
//    this->W = 0.0;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline void
//vector::operator+=(const vector& rhs)
//{
//    this->X += rhs.X;
//    this->Y += rhs.Y;
//    this->Z += rhs.Z;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline void
//vector::operator-=(const vector& rhs)
//{
//    this->X -= rhs.X;
//    this->Y -= rhs.Y;
//    this->Z -= rhs.Z;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline void
//vector::operator*=(scalar s)
//{
//    this->X *= s;
//    this->Y *= s;
//    this->Z *= s;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline vector
//vector::operator+(const vector& rhs) const
//{
//    return vector(this->X + rhs.X, this->Y + rhs.Y, this->Z + rhs.Z);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline vector
//vector::operator-(const vector& rhs) const
//{
//    return vector(this->X - rhs.X, this->Y - rhs.Y, this->Z - rhs.Z);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline vector
//vector::operator*(scalar s) const
//{
//    return vector(this->X * s, this->Y * s, this->Z * s);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline bool
//vector::operator==(const vector& rhs) const
//{
//    return (this->X == rhs.X) && (this->Y == rhs.Y) && (this->Z == rhs.Z);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline bool
//vector::operator!=(const vector& rhs) const
//{
//    return (this->X != rhs.X) || (this->Y != rhs.Y) || (this->Z != rhs.Z);
//}    
//
////------------------------------------------------------------------------------
///**
//*/
//__forceinline void
//vector::set(scalar x, scalar y, scalar z)
//{
//    float4::set(x, y, z, 0.0f);
//}
//
//} // namespace Math
////------------------------------------------------------------------------------
#endif
