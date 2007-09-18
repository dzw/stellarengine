//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Variant.h - Nuclex dynamically typed variant        //
// ### # #      ###                                                      //
// # ### #      ###  Dynamically typed variant class                     //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_VARIANT_H
#define NUCLEX_SUPPORT_VARIANT_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"

namespace Nuclex { namespace Support {

//  //
//  Nuclex::Support::Variant                                             //
//  //
/// Dynamically typed value class
/** Variant implements a dynamically typed value, meaning its type can be 
    changed at runtime. It provides implicit conversions to and from
    all basic Nuclex data types (including String, CBlob and CObject)
    and is mainly used for scripting purpouses.
    The Variant class can also persist itself and the value it
    contains in exclusion of CObject (because it won't be able to
    recreate it at loading time).
*/
class Variant {
  public:
    enum Type {
      T_NONE = 0,                                     ///< Empty
      T_BOOL,                                         ///< Boolean
      T_INT,                                          ///< Integer
      T_SIZE,                                         ///< Size
      T_DOUBLE,                                       ///< Double
      T_STRING,                                       ///< String
      T_WSTRING                                       ///< WString
    };

    /// Constructor
    NUCLEX_API Variant();
    /// Copy constructor
    NUCLEX_API Variant(const Variant &Value);
    /// Boolean constructor
    NUCLEX_API Variant(bool bValue);
    /// Int constructor
    NUCLEX_API Variant(int nValue);
    /// Size constructor
    NUCLEX_API Variant(size_t Value);
    /// Double constructor
    NUCLEX_API Variant(float fValue);
    /// Double constructor
    NUCLEX_API Variant(double dValue);
    /// String constructor
    NUCLEX_API Variant(const char *pszString);
    /// String constructor
    NUCLEX_API Variant(const wchar_t *pszString);
    /// String constructor
    NUCLEX_API Variant(const string &sString);
    /// String constructor
    NUCLEX_API Variant(const wstring &sString);
    /// Destructor
    NUCLEX_API ~Variant();

  //
  // Variant implementation
  //
  public:
    /// Reset contents
    NUCLEX_API void reset();

    /// Retrieve type of value
    NUCLEX_API inline Type getType() const { return m_eType; }

    /// Convert to specified type
    template<typename VarType> VarType to() const {
      return static_cast<VarType>(*this);
    }

    /// Assign copy
    NUCLEX_API Variant &operator =(const Variant &Value);
    /// Assign boolean
    NUCLEX_API Variant &operator =(bool bValue);
    /// Assign integer
    NUCLEX_API Variant &operator =(int nValue);
    /// Assign size
    NUCLEX_API Variant &operator =(size_t Value);
    /// Assign double
    NUCLEX_API Variant &operator =(float fValue) { return operator =(static_cast<double>(fValue)); }
    /// Assign double
    NUCLEX_API Variant &operator =(double dValue);
    /// Assign string
    NUCLEX_API Variant &operator =(const char *pszString) { return operator =(string(pszString)); }
    /// Assign string
    NUCLEX_API Variant &operator =(const wchar_t *pszString) { return operator =(wstring(pszString)); }
    /// Assign string
    NUCLEX_API Variant &operator =(const string &sString);
    /// Assign string
    NUCLEX_API Variant &operator =(const wstring &sString);

    /// Convert to boolean
    NUCLEX_API inline operator bool() const;
    /// Convert to integer
    NUCLEX_API inline operator int() const { return static_cast<int>(operator long()); }
    /// Convert to integer
    NUCLEX_API inline operator long() const;
    /// Convert to size_t
    NUCLEX_API inline operator size_t() const;
    /// Convert to double
    NUCLEX_API operator float() const { return static_cast<float>(operator double()); }
    /// Convert to double
    NUCLEX_API operator double() const;
    /// Convert to string
    NUCLEX_API operator string() const;
    /// Convert to string
    NUCLEX_API operator wstring() const;

  private:
    Type m_eType;                                     ///< Value type
    union {
      bool     m_bValue;                              ///< Boolean value
      int      m_nValue;                              ///< Int value
      size_t   m_Value;                               ///< Size value
      double   m_dValue;                              ///< Double value
      string  *m_psString;                            ///< String value
      wstring *m_psWString;                           ///< WString value
    };
};

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_VARIANT_H
