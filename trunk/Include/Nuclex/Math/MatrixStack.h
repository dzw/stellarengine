//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## MatrixStack.h - Nuclex matrix stack                 //
// ### # #      ###                                                      //
// # ### #      ###  Stack of matrices                                   //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_MATH_MATRIXSTACK_H
#define NUCLEX_MATH_MATRIXSTACK_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Math/Math.h"
#include "Nuclex/Math/Matrix44.h"
#include <stack>
#include <vector>

namespace Nuclex { namespace Math {

//  //
//  Nuclex::Math::MatrixStack                                            //
//  //
/// Matrix stack
/** A stack of matrices. Matrices that get pushed are concatenated, so
    the bottommost matrix will always perform all operation of a matrices
    pushed so far.
*/
template<typename VarType>
class MatrixStack {
  public:
    typedef Matrix44<VarType> MatrixType;
  
    /// Constructor
    NUCLEX_API inline MatrixStack();

  //
  // MatrixStack implementation
  //
  public:
    /// Get current stack depth
    NUCLEX_API inline size_t getSize() const { return m_Stack.size(); }

    /// Push matrix onto the stack
    NUCLEX_API inline void push(const MatrixType &Matrix);
    /// Pop matrix from the stack
    NUCLEX_API inline void pop();
    /// Get concatenated matrix
    NUCLEX_API inline const MatrixType &top() const { return m_Matrices.top(); }

  private:
    std::stack<MatrixType> m_Matrices;                ///< Stacked matrices
};

// ####################################################################### //
// # Nuclex::MatrixStack::MatrixStack()                      Constructor # // 
// ####################################################################### //
/** Initializes a new matrix stack instance
*/
template<typename VarType>
inline MatrixStack<VarType>::MatrixStack() {
  m_Matrices.push(MatrixType::Identity);
}

// ####################################################################### //
// # Nuclex::MatrixStack::push()                                         # // 
// ####################################################################### //
/** Pushes a matrix onto the stack

    @param  Matrix  Matrix to push
*/
template<typename VarType>
inline void MatrixStack<VarType>::push(const MatrixType &Matrix) {
  m_Matrices.push(Matrix * m_Matrices.top());
}

// ####################################################################### //
// # Nuclex::MatrixStack::pop()                                          # // 
// ####################################################################### //
/** Pops a matrix from the stack
*/
template<typename VarType>
inline void MatrixStack<VarType>::pop() {
  if(m_Matrices.size() <= 1)
    throw FailedException("Nuclex::MatrixStack::pop()",
                          "The stack is empty");

  m_Matrices.pop();
}

}} // namespace Nuclex::Math

#endif // NUCLEX_MATH_MATRIXSTACK_H
