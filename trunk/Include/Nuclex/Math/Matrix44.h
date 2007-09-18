//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Matrix44.h - 4D transformation matrix                                     //
// ### # #      ###                                                                            //
// # ### #      ###  Stores a 4x4 matrix for 4D space transformations and provides             //
// #  ## #   # ## ## a set of common methods to work with this matrix                          //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_MATH_MATRIX44_H
#define NUCLEX_MATH_MATRIX44_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Math.h"
#include "Nuclex/Math/Point3.h"
#include "Nuclex/Support/Exception.h"

namespace Nuclex { namespace Math {

//  //
//  Nuclex::Math::Matrix44                                                                     //
//  //
/// 4D transformation matrix
/** Stores a 4x4 matrix for 4D space transformations and procies a set of common
    methods to work with this matrix.
*/
template<typename VarType>
class Matrix44 {
  public:
    /// Identity matrix
    static const Matrix44 Identity;

    /// Construct orthogonal projection matrix
    static inline Matrix44 orthogonalProjection(
      VarType Left, VarType Right, VarType Top, VarType Bottom,
      VarType Near = -1, VarType Far = 1
    );

    /// Construct perspective projection matrix
    static inline Matrix44 perspectiveProjection(
      VarType FovY, VarType Aspect, VarType Near, VarType Far
    );

    /// Constructor
    inline Matrix44();
    /// Full constructor
    inline Matrix44(
      VarType M11, VarType M12, VarType M13, VarType M14,
      VarType M21, VarType M22, VarType M23, VarType M24,
      VarType M31, VarType M32, VarType M33, VarType M34,
      VarType M41, VarType M42, VarType M43, VarType M44
    );
    /// Direction and translation constructor
    inline Matrix44(
      const Vector3<VarType> &Right, const Vector3<VarType> &Up, const Vector3<VarType> &In,
      const Point3<VarType> &Pos = Point3<VarType>(0, 0, 0)
    );

  //
  // Matrix44 implementation
  //
  public:
    /// Transpose matrix
    inline void transpose();
    /// Normalize matrix
    inline void normalize();
    /// Orthonormalize matrix
    inline void orthoNormalize();
    /// Get determinant of matrix
    inline VarType getDeterminant() const;
    /// Invert matrix
    inline void invert();
    /// Invert matrix (direction only)
    inline void invertQuick();

    /// Translate matrix by offset
    inline void translate(const Vector3<VarType> &Offset);
    /// Rotate around global X axis
    inline void rotateX(VarType Angle);
    /// Rotate around global Y axis
    inline void rotateY(VarType Angle);
    /// Rotate around global Z axis
    inline void rotateZ(VarType Angle);
    /// Rotate around free axis
    inline void rotateAroundPoint(const Vector3<VarType> &Axis, VarType Angle);

    /// Orient matrix towards Point
    inline void lookAt(const Point3<VarType> &To);
    /// Orient matrix othogonal towards Point
    inline void billboardTo(const Point3<VarType> &Pos);

    /// Fast multiply with other matrix (direction only)
    inline void multiplyQuick(const Matrix44 &Matrix);
    /// Apply matrix to Point (transform point)
    inline void applyTo(Point3<VarType> &Point) const; 

    /// Get right vector
    inline Vector3<VarType> getRight() const { return Vector3<VarType>(M[0][0], M[0][1], M[0][2]); }
    /// Get up vector
    inline Vector3<VarType> getUp() const { return Vector3<VarType>(M[1][0], M[1][1], M[1][2]); }
    /// Get in vector
    inline Vector3<VarType> getIn() const { return Vector3<VarType>(M[2][0], M[2][1], M[2][2]); }
    /// Get translation vector
    inline Vector3<VarType> getPos() const { return Vector3<VarType>(M[3][0], M[3][1], M[3][2]); }

    /// Set matrix elements
    inline void set(
      VarType M11, VarType M12, VarType M13, VarType M14,
      VarType M21, VarType M22, VarType M23, VarType M24,
      VarType M31, VarType M32, VarType M33, VarType M34,
      VarType M41, VarType M42, VarType M43, VarType M44
    );
    /// Set to other matrix
    inline void set(const Matrix44 &Matrix);

    /// Scale matrix
    inline Matrix44 &operator *=(VarType V);
    /// Multiply matrix by point
    inline Matrix44 &operator *=(const Vector3<VarType> &Scale);
    /// Multiply matrix by other matrix
    inline Matrix44 &operator *=(const Matrix44 &Matrix);

    /// Return inverted matrix
    inline Matrix44 operator -() const { Matrix44 m(*this); m.invert(); return m; }
    /// Return matrix multiplied by Point
    inline Matrix44 operator *(VarType V) const { return Matrix44(*this) *= V; }
    /// Return matrix multiplied by Point
    inline Matrix44 operator *(const Vector3<VarType> &Point) const {
      return Matrix44(*this) *= Point;
    }
    /// Return matrix multiplied by other matrix
    inline Matrix44 operator *(const Matrix44 &Matrix) const {
      return Matrix44(*this) *= Matrix;
    }

  public:
    /// Matrix elements
    VarType M[4][4];
};

template<typename VarType> const Matrix44<VarType> Matrix44<VarType>::Identity(
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
);

// ############################################################################################# //
// # Nuclex::Math::Matrix44::orthogonalProjection()                                            # //
// ############################################################################################# //
/** Constructs an orthogonal projection matrix

    @param  Left, Right  Horizontal extents of the view area
    @param  Top, Bottom  Vertical extents of the view area
    @param  Near, Far    Clipping planes
    @return An orthogonal projection matrix
*/
template<typename VarType>
inline Matrix44<VarType> Matrix44<VarType>::orthogonalProjection(
  VarType Left, VarType Right, VarType Top, VarType Bottom, VarType Near, VarType Far
) {
  VarType DeltaX = Right - Left;
  VarType DeltaY = Bottom - Top;
  VarType DeltaZ = Far - Near;
  if((DeltaX == 0) || (DeltaY == 0) || (DeltaZ == 0))
    throw InvalidArgumentException(
      "Nuclex::Math::Matrix44::orthogonalProjection()",
      "All of the projection area's dimensions must of greater width than 0"
    );

  return Matrix44(
                             2 / DeltaX,                 0,                        0, 0,
                             0,                          2 / DeltaY,               0, 0,
                             0,                          0,               2 / DeltaZ, 0,
    (-(Right + Left) / DeltaX), (-(Top + Bottom) / DeltaY), (-(Far + Near) / DeltaZ), 1
  );
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::perspectiveProjection()                                           # //
// ############################################################################################# //
/** Constructs a perspective projection matrix

    @param  FovY       Field of view as angle in the view cone
    @param  Aspect     Aspect ratio of the selected video mode
    @param  Near, Far  Clipping planes
    @return A perspective projection matrix
*/
template<typename VarType>
inline Matrix44<VarType> Matrix44<VarType>::perspectiveProjection(
  VarType FovY, VarType Aspect, VarType Near, VarType Far
) {
  VarType DeltaZ = Far - Near;
  VarType Sin = Math::sin(FovY);
  if((DeltaZ == 0) || (Sin == 0) || (Aspect == 0))
    throw InvalidArgumentException("Nuclex::Math::Matrix44::persepctiveProjection()",
                                   "All of the projection area's dimensions must of greater width than 0");

  VarType Cotan = Math::cos(FovY) / Sin;

  return Matrix44(
    Cotan / Aspect,     0,                      0,                          0,
                 0, Cotan,                      0,                          0,
                 0,     0, -(Far + Near) / DeltaZ, -2.f * Near * Far / DeltaZ,
                 0,     0,                     -1,                          1
  );
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::Matrix44()                                            Constructor # //
// ############################################################################################# //
/** Creates a new matrix
*/
template<typename VarType>
inline Matrix44<VarType>::Matrix44() {
  M[0][0] = 1; M[0][1] = 0; M[0][2] = 0; M[0][3] = 0;
  M[1][0] = 0; M[1][1] = 1; M[1][2] = 0; M[1][3] = 0;
  M[2][0] = 0; M[2][1] = 0; M[2][2] = 1; M[2][3] = 0;
  M[3][0] = 0; M[3][1] = 0; M[3][2] = 0; M[3][3] = 1;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::Matrix44()                                            Constructor # //
// ############################################################################################# //
/** Creates a new matrix as copy of an existing matrix

    @param  M11, M12, M13, M14  First row of the matrix
    @param  M21, M22, M23, M24  Second row of the matrix
    @param  M31, M32, M33, M34  Third row of the matrix
    @param  M41, M42, M43, M44  Fourth row of the matrix
*/
template<typename VarType>
inline Matrix44<VarType>::Matrix44(
  VarType M11, VarType M12, VarType M13, VarType M14,
  VarType M21, VarType M22, VarType M23, VarType M24,
  VarType M31, VarType M32, VarType M33, VarType M34,
  VarType M41, VarType M42, VarType M43, VarType M44
) {
  M[0][0] = M11; M[0][1] = M12; M[0][2] = M13; M[0][3] = M14;
  M[1][0] = M21; M[1][1] = M22; M[1][2] = M23; M[1][3] = M24;
  M[2][0] = M31; M[2][1] = M32; M[2][2] = M33; M[2][3] = M34;
  M[3][0] = M41; M[3][1] = M42; M[3][2] = M43; M[3][3] = M44;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::Matrix44()                                            Constructor # //
// ############################################################################################# //
/** Initializes a matrix from the direction vectors

    @param  Right  Vector pointing to the right
    @param  Up     Vector pointing upwards
    @param  In     Vector pointing inwards
    @param  Pos    Initial translation value
*/
template<typename VarType>
inline Matrix44<VarType>::Matrix44(
  const Point3<VarType> &Right, const Point3<VarType> &Up, const Point3<VarType> &In,
  const Point3<VarType> &Pos
) {
  M[0][0] = Right.X; M[0][1] = Right.Y; M[0][2] = Right.Z; M[0][3] = 0;
  M[1][0] = Up.X;    M[1][1] = Up.Y;    M[1][2] = Up.Z;    M[1][3] = 0;
  M[2][0] = In.X;    M[2][1] = In.Y;    M[2][2] = In.Z;    M[2][3] = 0;
  M[3][0] = Pos.X;   M[3][1] = Pos.Y;   M[3][2] = Pos.Z;   M[3][3] = 1;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::transpose()                                                       # //
// ############################################################################################# //
/** Transposes the matrix
*/
template<typename VarType>
inline void Matrix44<VarType>::transpose() {
  VarType Temp;

  Temp = M[0][1]; M[0][1] = M[1][0]; M[1][0] = Temp;
  Temp = M[0][2]; M[0][2] = M[2][0]; M[2][0] = Temp;
  Temp = M[1][2]; M[1][2] = M[2][1]; M[2][1] = Temp;
  Temp = M[0][3]; M[0][3] = M[3][0]; M[3][0] = Temp;
  Temp = M[1][3]; M[1][3] = M[3][1]; M[3][1] = Temp;
  Temp = M[2][3]; M[2][3] = M[3][2]; M[3][2] = Temp;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::normalize()                                                       # //
// ############################################################################################# //
/** Normalizes the matrix
*/
template<typename VarType>
inline void Matrix44<VarType>::normalize() {
  VarType Temp;

  Temp = Math::sqrt(M[0][0] * M[0][0] + M[0][1] * M[0][1] + M[0][2] * M[0][2]);
  M[0][0] /= Temp;
  M[0][1] /= Temp;
  M[0][2] /= Temp;

  Temp = Math::sqrt(M[1][0] * M[1][0] + M[1][1] * M[1][1] + M[1][2] * M[1][2]);
  M[1][0] /= Temp;
  M[1][1] /= Temp;
  M[1][2] /= Temp;

  Temp = Math::sqrt(M[2][0] * M[2][0] + M[2][1] * M[2][1] + M[2][2] * M[2][2]);
  M[2][0] /= Temp;
  M[2][1] /= Temp;
  M[2][2] /= Temp;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::orthoNormalize()                                                  # //
// ############################################################################################# //
/** Orthogonalizes and normalizes the matrix
*/
template<typename VarType>
inline void Matrix44<VarType>::orthoNormalize() {
  VarType Length;
  VarType Dot0, Dot1;

  Length = Math::sqrt(M[0][0] * M[0][0] + M[1][0] * M[1][0] + M[2][0] * M[2][0]);
  M[0][0] /= Length;
  M[1][0] /= Length;
  M[2][0] /= Length;

  Dot0 = M[0][0] * M[0][1] + M[1][0] * M[1][1] + M[2][0] * M[2][1];
  M[0][1] -= Dot0 * M[0][0];
  M[1][1] -= Dot0 * M[1][0];
  M[2][1] -= Dot0 * M[2][0];

  Length = Math::sqrt(M[0][1] * M[0][1] + M[1][1] * M[1][1] + M[2][1] * M[2][1]);
  M[0][1] /= Length;
  M[1][1] /= Length;
  M[2][1] /= Length;

  Dot0 = M[0][0] * M[0][2] + M[1][0] * M[1][2] + M[2][0] * M[2][2];
  Dot1 = M[0][1] * M[0][2] + M[1][1] * M[1][2] + M[2][1] * M[2][2];
  M[0][2] -= Dot0 * M[0][0] + Dot1 * M[0][1];
  M[1][2] -= Dot0 * M[1][0] + Dot1 * M[1][1];
  M[2][2] -= Dot0 * M[2][0] + Dot1 * M[2][1];

  Length = Math::sqrt(M[0][2] * M[0][2] + M[1][2] * M[1][2] + M[2][2] * M[2][2]);
  M[0][2] /= Length;
  M[1][2] /= Length;
  M[2][2] /= Length;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::getDeterminant()                                                  # //
// ############################################################################################# //
/** Calculates the matrix determinant

    @return The matrix determinant
*/
template<typename VarType>
inline VarType Matrix44<VarType>::getDeterminant() const {
  return 
    +(M[0][0] * M[1][1] - M[0][1] * M[1][0]) * (M[2][2] * M[3][3] - M[2][3] * M[3][2])
	-(M[0][0] * M[1][2] - M[0][2] * M[1][0]) * (M[2][1] * M[3][3] - M[2][3] * M[3][1])
	+(M[0][0] * M[1][3] - M[0][3] * M[1][0]) * (M[2][1] * M[3][2] - M[2][2] * M[3][1])
	+(M[0][1] * M[1][2] - M[0][2] * M[1][1]) * (M[2][0] * M[3][3] - M[2][3] * M[3][0])
	-(M[0][1] * M[1][3] - M[0][3] * M[1][1]) * (M[2][0] * M[3][2] - M[2][2] * M[3][0])
	+(M[0][2] * M[1][3] - M[0][3] * M[1][2]) * (M[2][0] * M[3][1] - M[2][1] * M[3][0]);
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::invert()                                                          # //
// ############################################################################################# //
/** Inverts the matrix
*/
template<typename VarType>
inline void Matrix44<VarType>::invert() {
  VarType S = getDeterminant();
  if(S != 0) {
    S = 1 / S;
    set(
      S * (M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) + M[1][2] * (M[2][3] * M[3][1] -
        M[2][1] * M[3][3]) + M[1][3] * (M[2][1] * M[3][2] - M[2][2] * M[3][1])),
      S * (M[2][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) + M[2][2] * (M[0][3] * M[3][1] -
        M[0][1] * M[3][3]) + M[2][3] * (M[0][1] * M[3][2] - M[0][2] * M[3][1])),
      S * (M[3][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2]) + M[3][2] * (M[0][3] * M[1][1] -
        M[0][1] * M[1][3]) + M[3][3] * (M[0][1] * M[1][2] - M[0][2] * M[1][1])),
      S * (M[0][1] * (M[1][3] * M[2][2] - M[1][2] * M[2][3]) + M[0][2] * (M[1][1] * M[2][3] -
        M[1][3] * M[2][1]) + M[0][3] * (M[1][2] * M[2][1] - M[1][1] * M[2][2])),
      S * (M[1][2] * (M[2][0] * M[3][3] - M[2][3] * M[3][0]) + M[1][3] * (M[2][2] * M[3][0] -
        M[2][0] * M[3][2]) + M[1][0] * (M[2][3] * M[3][2] - M[2][2] * M[3][3])),
      S * (M[2][2] * (M[0][0] * M[3][3] - M[0][3] * M[3][0]) + M[2][3] * (M[0][2] * M[3][0] -
        M[0][0] * M[3][2]) + M[2][0] * (M[0][3] * M[3][2] - M[0][2] * M[3][3])),
      S * (M[3][2] * (M[0][0] * M[1][3] - M[0][3] * M[1][0]) + M[3][3] * (M[0][2] * M[1][0] -
        M[0][0] * M[1][2]) + M[3][0] * (M[0][3] * M[1][2] - M[0][2] * M[1][3])),
      S * (M[0][2] * (M[1][3] * M[2][0] - M[1][0] * M[2][3]) + M[0][3] * (M[1][0] * M[2][2] -
        M[1][2] * M[2][0]) + M[0][0] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])),
      S * (M[1][3] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]) + M[1][0] * (M[2][1] * M[3][3] -
        M[2][3] * M[3][1]) + M[1][1] * (M[2][3] * M[3][0] - M[2][0] * M[3][3])),
      S * (M[2][3] * (M[0][0] * M[3][1] - M[0][1] * M[3][0]) + M[2][0] * (M[0][1] * M[3][3] -
        M[0][3] * M[3][1]) + M[2][1] * (M[0][3] * M[3][0] - M[0][0] * M[3][3])),
      S * (M[3][3] * (M[0][0] * M[1][1] - M[0][1] * M[1][0]) + M[3][0] * (M[0][1] * M[1][3] -
        M[0][3] * M[1][1]) + M[3][1] * (M[0][3] * M[1][0] - M[0][0] * M[1][3])),
      S * (M[0][3] * (M[1][1] * M[2][0] - M[1][0] * M[2][1]) + M[0][0] * (M[1][3] * M[2][1] -
        M[1][1] * M[2][3]) + M[0][1] * (M[1][0] * M[2][3] - M[1][3] * M[2][0])),
      S * (M[1][0] * (M[2][2] * M[3][1] - M[2][1] * M[3][2]) + M[1][1] * (M[2][0] * M[3][2] -
        M[2][2] * M[3][0]) + M[1][2] * (M[2][1] * M[3][0] - M[2][0] * M[3][1])),
      S * (M[2][0] * (M[0][2] * M[3][1] - M[0][1] * M[3][2]) + M[2][1] * (M[0][0] * M[3][2] -
        M[0][2] * M[3][0]) + M[2][2] * (M[0][1] * M[3][0] - M[0][0] * M[3][1])),
      S * (M[3][0] * (M[0][2] * M[1][1] - M[0][1] * M[1][2]) + M[3][1] * (M[0][0] * M[1][2] -
        M[0][2] * M[1][0]) + M[3][2] * (M[0][1] * M[1][0] - M[0][0] * M[1][1])),
      S * (M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) + M[0][1] * (M[1][2] * M[2][0] -
        M[1][0] * M[2][2]) + M[0][2] * (M[1][0] * M[2][1] - M[1][1] * M[2][0]))
    );
  }  
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::invertQuick()                                                     # //
// ############################################################################################# //
/** Inverts the matrix
*/
template<typename VarType>
inline void Matrix44<VarType>::invertQuick() {
  VarType S = getDeterminant();
  if(S != 0) {
    S = 1 / S;
    set(
      S * (M[1][1] * M[2][2] - M[1][2] * M[2][1]),
      S * (M[2][1] * M[0][2] - M[2][2] * M[0][1]),
      S * (M[0][1] * M[1][2] - M[0][2] * M[1][1]),
      0,
      S * (M[1][2] * M[2][0] - M[1][0] * M[2][2]),
      S * (M[2][2] * M[0][0] - M[2][0] * M[0][2]),
      S * (M[0][2] * M[1][0] - M[0][0] * M[1][2]),
      0,
      S * (M[1][0] * M[2][1] - M[1][1] * M[2][0]),
      S * (M[2][0] * M[0][1] - M[2][1] * M[0][0]),
      S * (M[0][0] * M[1][1] - M[0][1] * M[1][0]),
      0,
      S * (M[1][0] * (M[2][2] * M[3][1] - M[2][1] * M[3][2]) + M[1][1] * (M[2][0] * M[3][2] -
        M[2][2] * M[3][0]) + M[1][2] * (M[2][1] * M[3][0] - M[2][0] * M[3][1])),
      S * (M[2][0] * (M[0][2] * M[3][1] - M[0][1] * M[3][2]) + M[2][1] * (M[0][0] * M[3][2] -
        M[0][2] * M[3][0]) + M[2][2] * (M[0][1] * M[3][0] - M[0][0] * M[3][1])),
      S * (M[3][0] * (M[0][2] * M[1][1] - M[0][1] * M[1][2]) + M[3][1] * (M[0][0] * M[1][2] -
        M[0][2] * M[1][0]) + M[3][2] * (M[0][1] * M[1][0] - M[0][0] * M[1][1])),
      1
    );
  }  
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::translate()                                                       # //
// ############################################################################################# //
/** Translates the matrix by a Point

    @param  Translation  Translation point
*/
template<typename VarType>
inline void Matrix44<VarType>::translate(const Vector3<VarType> &Translation) {
  M[3][0] += Translation.X;
  M[3][1] += Translation.Y;
  M[3][2] += Translation.Z;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::rotateX()                                                         # //
// ############################################################################################# //
/** Rotates the matrix on the X axis

    @param  Angle  Amount to rotate in RAD
*/
template<typename VarType>
inline void Matrix44<VarType>::rotateX(VarType Angle) {
  VarType Y, Z;
  VarType Sin = Math::sin(Angle);
  VarType Cos = Math::cos(Angle);

  Y = M[0][1]; Z = M[0][2];
  M[0][1] = Y * Cos - Z * Sin;
  M[0][2] = Y * Sin + Z * Cos;

  Y = M[1][1]; Z = M[1][2];
  M[1][1] = Y * Cos - Z * Sin;
  M[1][2] = Y * Sin + Z * Cos;

  Y = M[2][1]; Z = M[2][2];
  M[2][1] = Y * Cos - Z * Sin;
  M[2][2] = Y * Sin + Z * Cos;

  Y = M[3][1]; Z = M[3][2];
  M[3][1] = Y * Cos - Z * Sin;
  M[3][2] = Y * Sin + Z * Cos;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::rotateY()                                                         # //
// ############################################################################################# //
/** Rotates the matrix on the Y axis

    @param  Angle  Amount to rotate in RAD
*/
template<typename VarType>
inline void Matrix44<VarType>::rotateY(VarType Angle) {
  VarType X, Z;
  VarType Sin = Math::sin(Angle);
  VarType Cos = Math::cos(Angle);

  X = M[0][0]; Z = M[0][2];
  M[0][0] = Z * Sin + X * Cos;
  M[0][2] = Z * Cos - X * Sin;

  X = M[1][0]; Z = M[1][2];
  M[1][0] = Z * Sin + X * Cos;
  M[1][2] = Z * Cos - X * Sin;

  X = M[2][0]; Z = M[2][2];
  M[2][0] = Z * Sin + X * Cos;
  M[2][2] = Z * Cos - X * Sin;

  X = M[3][0]; Z = M[3][2];
  M[3][0] = Z * Sin + X * Cos;
  M[3][2] = Z * Cos - X * Sin;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::rotateZ()                                                         # //
// ############################################################################################# //
/** Rotates the matrix on the Z axis

    @param  Angle  Amount to rotate in RAD
*/
template<typename VarType>
inline void Matrix44<VarType>::rotateZ(VarType Angle) {
  VarType X, Y;
  VarType Sin = Math::sin(Angle);
  VarType Cos = Math::cos(Angle);

  X = M[0][0]; Y = M[0][1];
  M[0][0] = X * Cos - Y * Sin;
  M[0][1] = X * Sin + Y * Cos;

  X = M[1][0]; Y = M[1][1];
  M[1][0] = X * Cos - Y * Sin;
  M[1][1] = X * Sin + Y * Cos;

  X = M[2][0]; Y = M[2][1];
  M[2][0] = X * Cos - Y * Sin;
  M[2][1] = X * Sin + Y * Cos;

  X = M[3][0]; Y = M[3][1];
  M[3][0] = X * Cos - Y * Sin;
  M[3][1] = X * Sin + Y * Cos;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::rotateAroundPoint()                                               # //
// ############################################################################################# //
/** Rotates the matrix on an arbitrary axis

    @param  Axis   Point defining the rotation axis
    @param  Angle  Rotation angle in RAD
*/
template<typename VarType>
inline void Matrix44<VarType>::rotateAroundPoint(const Vector3<VarType> &Axis, VarType Angle) {
  Vector3<VarType> NormVec(Axis);
  NormVec.normalize();

  VarType Sin = Math::sin(Angle);
  VarType Cos = Math::cos(Angle);

  Matrix44 Rotation;
  Rotation.M[0][0] = Cos + (1 - Cos) * Axis.X * Axis.X;
  Rotation.M[0][1] = (1 - Cos) * Axis.X * Axis.Y - Sin * Axis.Z;
  Rotation.M[0][2] = (1 - Cos) * Axis.Z * Axis.X + Sin * Axis.Y;
  Rotation.M[1][0] = (1 - Cos) * Axis.X * Axis.Y + Sin * Axis.Z;
  Rotation.M[1][1] = Cos + (1 - Cos) * Axis.Y * Axis.Y;
  Rotation.M[1][2] = (1 - Cos) * Axis.Y * Axis.Z - Sin * Axis.X;
  Rotation.M[2][0] = (1 - Cos) * Axis.Z * Axis.X - Sin * Axis.Y;
  Rotation.M[2][1] = (1 - Cos) * Axis.Y * Axis.Z + Sin * Axis.X;
  Rotation.M[2][2] = Cos + (1 - Cos) * Axis.Z * Axis.Z;
  multiply(Rotation);
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::lookAt()                                                          # //
// ############################################################################################# //
/** Orients the matrix to look at the specified location

    @param  To  Lookat-Point
*/
template<typename VarType>
inline void Matrix44<VarType>::lookAt(const Point3<VarType> &To) {
  Point3<VarType> From(M[3][0], M[3][1], M[3][2]);
  Point3<VarType> Z(From - To);
  Z.normalize();
  
  Vector3<VarType> Y(Point3<VarType>::Up);
  Vector3<VarType> X = Y.crossProduct(Z);
  Y = Z.crossProduct(X);

  X.normalize();
  Y.normalize();

  M[0][0] = X.X; M[0][1] = X.Y; M[0][2] = X.Z; M[0][3] = 0;
  M[1][0] = Y.X; M[1][1] = Y.Y; M[1][2] = Y.Z; M[1][3] = 0;
  M[2][0] = Z.X; M[2][1] = Z.Y; M[2][2] = Z.Z; M[2][3] = 0;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::billboardTo()                                                     # //
// ############################################################################################# //
/** Orients the matrix to look at the specified location

    @param  To  Lookat-Point
*/
template<typename VarType>
inline void Matrix44<VarType>::billboardTo(const Point3<VarType> &To) {
  Point3<VarType> From(M[3][0], M[3][1], M[3][2]);
  Point3<VarType> Z(From - To);
  Z.normalize();
  
  Point3<VarType> Y(Point3<VarType>::Up);
  Point3<VarType> X = Y.crossProduct(Z);
  Y = Z.crossProduct(X);

  X.normalize();
  Y.normalize();
  Z.normalize();

  M[0][0] = X.X; M[0][1] = X.Y; M[0][2] = X.Z; M[0][3] = 0;
  M[1][0] = Y.X; M[1][1] = Y.Y; M[1][2] = Y.Z; M[1][3] = 0;
  M[2][0] = Z.X; M[2][1] = Z.Y; M[2][2] = Z.Z; M[2][3] = 0;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::operator *=()                                                     # //
// ############################################################################################# //
/** Scales the matrix by a uniform value

    @param  V  Scaling value
*/
template<typename VarType>
inline Matrix44<VarType> &Matrix44<VarType>::operator *=(VarType V) {
  M[0][0] *= V; M[0][1] *= V; M[0][2] *= V; M[0][3] *= V;
  M[1][0] *= V; M[1][1] *= V; M[1][2] *= V; M[1][3] *= V;
  M[2][0] *= V; M[2][1] *= V; M[2][2] *= V; M[2][3] *= V;
  M[3][0] *= V; M[3][1] *= V; M[3][2] *= V; M[3][3] *= V;
  
  return *this;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::operator *=()                                                     # //
// ############################################################################################# //
/** Scales the matrix by a Point

    @param  Scale  Scaling Point
*/
template<typename VarType>
inline Matrix44<VarType> &Matrix44<VarType>::operator *=(const Vector3<VarType> &Scale) {
  M[0][0] *= Scale.X; M[0][1] *= Scale.Y; M[0][2] *= Scale.Z;
  M[1][0] *= Scale.X; M[1][1] *= Scale.Y; M[1][2] *= Scale.Z;
  M[2][0] *= Scale.X; M[2][1] *= Scale.Y; M[2][2] *= Scale.Z;
  M[3][0] *= Scale.X; M[3][1] *= Scale.Y; M[3][2] *= Scale.Z;
  
  return *this;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::operator *=()                                                     # //
// ############################################################################################# //
/** Multiply matrix

    @param  Matrix  Matrix to multiply with
*/
template<typename VarType>
inline Matrix44<VarType> &Matrix44<VarType>::operator *=(const Matrix44 &Matrix) {
  VarType X, Y, Z, W;

  X = M[0][0]; Y = M[0][1]; Z = M[0][2]; W = M[0][3];
  M[0][0] = X * Matrix.M[0][0] + Y * Matrix.M[1][0] + Z * Matrix.M[2][0] + W * Matrix.M[3][0];
  M[0][1] = X * Matrix.M[0][1] + Y * Matrix.M[1][1] + Z * Matrix.M[2][1] + W * Matrix.M[3][1];
  M[0][2] = X * Matrix.M[0][2] + Y * Matrix.M[1][2] + Z * Matrix.M[2][2] + W * Matrix.M[3][2];
  M[0][3] = X * Matrix.M[0][3] + Y * Matrix.M[1][3] + Z * Matrix.M[2][3] + W * Matrix.M[3][3];

  X = M[1][0]; Y = M[1][1]; Z = M[1][2]; W = M[1][3];
  M[1][0] = X * Matrix.M[0][0] + Y * Matrix.M[1][0] + Z * Matrix.M[2][0] + W * Matrix.M[3][0];
  M[1][1] = X * Matrix.M[0][1] + Y * Matrix.M[1][1] + Z * Matrix.M[2][1] + W * Matrix.M[3][1];
  M[1][2] = X * Matrix.M[0][2] + Y * Matrix.M[1][2] + Z * Matrix.M[2][2] + W * Matrix.M[3][2];
  M[1][3] = X * Matrix.M[0][3] + Y * Matrix.M[1][3] + Z * Matrix.M[2][3] + W * Matrix.M[3][3];

  X = M[2][0]; Y = M[2][1]; Z = M[2][2]; W = M[2][3];
  M[2][0] = X * Matrix.M[0][0] + Y * Matrix.M[1][0] + Z * Matrix.M[2][0] + W * Matrix.M[3][0];
  M[2][1] = X * Matrix.M[0][1] + Y * Matrix.M[1][1] + Z * Matrix.M[2][1] + W * Matrix.M[3][1];
  M[2][2] = X * Matrix.M[0][2] + Y * Matrix.M[1][2] + Z * Matrix.M[2][2] + W * Matrix.M[3][2];
  M[2][3] = X * Matrix.M[0][3] + Y * Matrix.M[1][3] + Z * Matrix.M[2][3] + W * Matrix.M[3][3];

  X = M[3][0]; Y = M[3][1]; Z = M[3][2]; W = M[3][3];
  M[3][0] = X * Matrix.M[0][0] + Y * Matrix.M[1][0] + Z * Matrix.M[2][0] + W * Matrix.M[3][0];
  M[3][1] = X * Matrix.M[0][1] + Y * Matrix.M[1][1] + Z * Matrix.M[2][1] + W * Matrix.M[3][1];
  M[3][2] = X * Matrix.M[0][2] + Y * Matrix.M[1][2] + Z * Matrix.M[2][2] + W * Matrix.M[3][2];
  M[3][3] = X * Matrix.M[0][3] + Y * Matrix.M[1][3] + Z * Matrix.M[2][3] + W * Matrix.M[3][3];

  return *this;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::multiplyQuick()                                                   # //
// ############################################################################################# //
/** Multiply matrix

    @param  Matrix  Matrix to multiply with
*/
template<typename VarType>
inline void Matrix44<VarType>::multiplyQuick(const Matrix44 &Matrix) {
  VarType X, Y, Z;

  X = M[0][0]; Y = M[0][1]; Z = M[0][2];
  M[0][0] = X * Matrix.M[0][0] + Y * Matrix.M[1][0] + Z * Matrix.M[2][0];
  M[0][1] = X * Matrix.M[0][1] + Y * Matrix.M[1][1] + Z * Matrix.M[2][1];
  M[0][2] = X * Matrix.M[0][2] + Y * Matrix.M[1][2] + Z * Matrix.M[2][2];

  X = M[1][0]; Y = M[1][1]; Z = M[1][2];
  M[1][0] = X * Matrix.M[0][0] + Y * Matrix.M[1][0] + Z * Matrix.M[2][0];
  M[1][1] = X * Matrix.M[0][1] + Y * Matrix.M[1][1] + Z * Matrix.M[2][1];
  M[1][2] = X * Matrix.M[0][2] + Y * Matrix.M[1][2] + Z * Matrix.M[2][2];

  X = M[2][0]; Y = M[2][1]; Z = M[2][2];
  M[2][0] = X * Matrix.M[0][0] + Y * Matrix.M[1][0] + Z * Matrix.M[2][0];
  M[2][1] = X * Matrix.M[0][1] + Y * Matrix.M[1][1] + Z * Matrix.M[2][1];
  M[2][2] = X * Matrix.M[0][2] + Y * Matrix.M[1][2] + Z * Matrix.M[2][2];

  X = M[3][0]; Y = M[3][1]; Z = M[3][2];
  M[3][0] = X * Matrix.M[0][0] + Y * Matrix.M[1][0] + Z * Matrix.M[2][0];
  M[3][1] = X * Matrix.M[0][1] + Y * Matrix.M[1][1] + Z * Matrix.M[2][1];
  M[3][2] = X * Matrix.M[0][2] + Y * Matrix.M[1][2] + Z * Matrix.M[2][2];

  M[3][0] += Matrix.M[3][0];
  M[3][1] += Matrix.M[3][1];
  M[3][2] += Matrix.M[3][2];
  M[0][3]  = 0;
  M[1][3]  = 0;
  M[2][3]  = 0;
  M[3][3]  = 1;
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::applyTo()                                                         # //
// ############################################################################################# //
/** Apply matrix to Point

    @param  Point  Point to apply matrix to
*/
template<typename VarType>
inline void Matrix44<VarType>::applyTo(Point3<VarType> &Point) const {
  Point.set(
    M[0][0] * Point.X + M[1][0] * Point.Y + M[2][0] * Point.Z + M[3][0],
    M[0][1] * Point.X + M[1][1] * Point.Y + M[2][1] * Point.Z + M[3][1],
    M[0][2] * Point.X + M[1][2] * Point.Y + M[2][2] * Point.Z + M[3][2]
  );
}


// ############################################################################################# //
// # Nuclex::Math::Matrix44::set()                                                             # //
// ############################################################################################# //
/** Sets the matrix elements directly

    @param  Matrix  Matrix to set
*/
template<typename VarType>
inline void Matrix44<VarType>::set(const Matrix44 &Matrix) {
  M[0][0] = Matrix.M[0][0]; M[0][1] = Matrix.M[0][1];
  M[0][2] = Matrix.M[0][2]; M[0][3] = Matrix.M[0][3];

  M[1][0] = Matrix.M[1][0]; M[1][1] = Matrix.M[1][1];
  M[1][2] = Matrix.M[1][2]; M[1][3] = Matrix.M[1][3];

  M[2][0] = Matrix.M[2][0]; M[2][1] = Matrix.M[2][1];
  M[2][2] = Matrix.M[2][2]; M[2][3] = Matrix.M[2][3];

  M[3][0] = Matrix.M[3][0]; M[3][1] = Matrix.M[3][1];
  M[3][2] = Matrix.M[3][2]; M[3][3] = Matrix.M[3][3];
}

// ############################################################################################# //
// # Nuclex::Math::Matrix44::set()                                                             # //
// ############################################################################################# //
/** Sets the matrix elements directly

    @param  M11, M12, M13, M14  First row
    @param  M21, M22, M23, M24  Second row
    @param  M31, M32, M33, M34  Third row
    @param  M41, M42, M43, M44  Fourth row
*/
template<typename VarType>
inline void Matrix44<VarType>::set(
  VarType M11, VarType M12, VarType M13, VarType M14,
  VarType M21, VarType M22, VarType M23, VarType M24,
  VarType M31, VarType M32, VarType M33, VarType M34,
  VarType M41, VarType M42, VarType M43, VarType M44
) {
  M[0][0] = M11; M[0][1] = M12; M[0][2] = M13; M[0][3] = M14;
  M[1][0] = M21; M[1][1] = M22; M[1][2] = M23; M[1][3] = M24;
  M[2][0] = M31; M[2][1] = M32; M[2][2] = M33; M[2][3] = M34;
  M[3][0] = M41; M[3][1] = M42; M[3][2] = M43; M[3][3] = M44;
}

}} // namespace Nuclex::Math

#endif // NUCLEX_MATH_MATRIX44_H
