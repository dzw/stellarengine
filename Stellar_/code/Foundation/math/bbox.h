#pragma once
#ifndef MATH_BBOX_H
#define MATH_BBOX_H
//------------------------------------------------------------------------------
/**
    @class Math::bbox

    Nebula's bounding box class.

    @todo: UNTESTED!

    (C) 2004 RadonLabs GmbH
*/
#include "math/point.h"
#include "math/vector.h"
#include "math/matrix44.h"
#include "math/line.h"
#include "math/plane.h"
#include "math/clipstatus.h"
#include "utility/array.h"

//------------------------------------------------------------------------------
namespace Math
{
class bbox
{
public:
    /// clip codes
    enum 
    {
        ClipLeft   = (1<<0),
        ClipRight  = (1<<1),
        ClipBottom = (1<<2),
        ClipTop    = (1<<3),
        ClipNear   = (1<<4),
        ClipFar    = (1<<5),
    };

    /// constructor 1
    bbox();
    /// constructor 3
    bbox(const point& center, const vector& extents);
    /// construct bounding box from matrix44
    bbox(const matrix44& m);
    /// get center of box
    point center() const;
    /// get extents of box
    point extents() const;
    /// get size of box
    vector size() const;
    /// get diagonal size of box
    scalar diagonal_size() const;
    /// set from matrix44
    void set(const matrix44& m);
    /// set from center point and extents
    void set(const point& center, const vector& extents);
    /// begin extending the box
    void begin_extend();
    /// extend the box
    void extend(const point& p);
    /// extend the box
    void extend(const bbox& box);
    /// this resets the bounding box size to zero if no extend() method was called after begin_extend()
    void end_extend();
    /// transform bounding box
    void transform(const matrix44& m);
    /// check for intersection with axis aligned bounding box
    bool intersects(const bbox& box) const;
    /// check if this box completely contains the parameter box
    bool contains(const bbox& box) const;
    /// return true if this box contains the position
    bool contains(const point& p) const;
    /// check for intersection with other bounding box
    ClipStatus::Type clipstatus(const bbox& other) const;
    /// check for intersection with projection volume
    ClipStatus::Type clipstatus(const matrix44& viewProjection) const;
    /// create a matrix which transforms a unit cube to this bounding box
    matrix44 to_matrix44() const;
    /// return one of the 8 corner points
    point corner_point(int index) const;
    /// return side planes in clip space
	void get_clipplanes(const matrix44& viewProjection, Util::Array<plane>& outPlanes) const;

    point pmin;
    point pmax;
};

//------------------------------------------------------------------------------
/**
*/
inline
bbox::bbox() :
    pmin(-0.5f, -0.5f, -0.5f),
    pmax(+0.5f, +0.5f, +0.5f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
bbox::bbox(const point& center, const vector& extents)
{
    this->pmin = center - extents;
    this->pmax = center + extents;
}

//------------------------------------------------------------------------------
/**
    Construct a bounding box around a 4x4 matrix. The translational part
    defines the center point, and the x,y,z vectors of the matrix
    define the extents.
*/
inline void
bbox::set(const matrix44& m)
{
    // get extents
    vector extents = float4::maximize(float4::maximize(m.row0().abs(), m.row1().abs()), m.row2().abs()) * 0.5f;
    point center = m.row3();
    this->pmin = center - extents;
    this->pmax = center + extents;
}

//------------------------------------------------------------------------------
/**
*/
inline
bbox::bbox(const matrix44& m)
{
    this->set(m);
}

//------------------------------------------------------------------------------
/**
*/
inline point
bbox::center() const
{
    return this->pmin + ((this->pmax - this->pmin) * 0.5f);
}

//------------------------------------------------------------------------------
/**
*/
inline point
bbox::extents() const
{
    vector vec = (this->pmax - this->pmin) * 0.5f;
	return point(vec.x(), vec.y(), vec.z());
}

//------------------------------------------------------------------------------
/**
*/
inline vector
bbox::size() const
{
    vector vec = this->pmax - this->pmin;
	return point(vec.x(), vec.y(), vec.z());
}

//------------------------------------------------------------------------------
/**
*/
inline void
bbox::set(const point& center, const vector& extents)
{
    this->pmin = center - extents;
    this->pmax = center + extents;
}

//------------------------------------------------------------------------------
/**
*/
inline void
bbox::begin_extend()
{
    this->pmin.set(+1000000.0f, +1000000.0f, +1000000.0f);
    this->pmax.set(-1000000.0f, -1000000.0f, -1000000.0f);
}

//------------------------------------------------------------------------------
/**
    This just checks whether the extend() method has actually been called after
    begin_extend() and just sets vmin and vmax to the null vector if it hasn't.
*/
inline
void
bbox::end_extend()
{
    if ((this->pmin == point(+1000000.0f, +1000000.0f, +1000000.0f)) &&
        (this->pmax == point(-1000000.0f, -1000000.0f, -1000000.0f)))
    {
        this->pmin.set(0.0f, 0.0f, 0.0f);
        this->pmax.set(0.0f, 0.0f, 0.0f);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void
bbox::extend(const point& p)
{
    this->pmin = vector::minimize(this->pmin, p);
    this->pmax = vector::maximize(this->pmax, p);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
bbox::extend(const bbox& box)
{
    this->pmin = vector::minimize(this->pmin, box.pmin);
    this->pmax = vector::maximize(this->pmax, box.pmax);
}

//------------------------------------------------------------------------------
/**
    Transforms this axis aligned bounding by the 4x4 matrix. This bounding
    box must be axis aligned with the matrix, the resulting bounding
    will be axis aligned in the matrix' "destination" space.

    E.g. if you have a bounding box in model space 'modelBox', and a
    'modelView' matrix, the operation
    
    modelBox.transform(modelView)

    would transform the bounding box into view space.
*/
inline void
bbox::transform(const matrix44& m)
{
    this->set(matrix44::multiply(this->to_matrix44(), m));
}

//------------------------------------------------------------------------------
/**
    Check for intersection of 2 axis aligned bounding boxes. The 
    bounding boxes must live in the same coordinate space.
*/
inline bool
bbox::intersects(const bbox& box) const
{
    bool lt = vector::any(vector::less(this->pmax, box.pmin));
    bool gt = vector::any(vector::greater(this->pmin, box.pmax));
    return !(lt || gt);
}

//------------------------------------------------------------------------------
/**
    Check if the parameter bounding box is completely contained in this
    bounding box.
*/
inline bool
bbox::contains(const bbox& box) const
{
    bool lt = vector::all(vector::less(this->pmin, box.pmin));
    bool ge = vector::all(vector::greaterequal(this->pmax, box.pmax));
    return lt && ge;
}

//------------------------------------------------------------------------------
/**
    Check if position is inside bounding box.
*/
inline bool
bbox::contains(const point& p) const
{
    bool lt = vector::all(vector::less(this->pmin, p));
    bool ge = vector::all(vector::greaterequal(this->pmax, p));
    return lt && ge;
}

//------------------------------------------------------------------------------
/**
    Create a transform matrix which would transform a unit cube to this
    bounding box.
*/
inline matrix44
bbox::to_matrix44() const
{
	vector vec = this->size();
    matrix44 m = matrix44::scaling(float4(vec.x(), vec.y(), vec.z(), 1.0));
    vec = this->center();
	m.row3() = float4(vec.x(), vec.y(), vec.z(), 1.0);
    return m;
}

//------------------------------------------------------------------------------
/**
*/
inline scalar
bbox::diagonal_size() const
{
    return (this->pmax - this->pmin).length();
}

}; // namespace Math
//------------------------------------------------------------------------------
#endif
