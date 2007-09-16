//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Variant.cpp - Nuclex dynamically typed variant      //
// ### # #      ###                                                      //
// # ### #      ###  Dynamically typed variant class                     //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Support/Variant.h"
#include "Nuclex/Support/Exception.h"
#include <sstream>

using namespace Nuclex;
using namespace Nuclex::Support;

// ####################################################################### //
// # Nuclex::Variant::Variant()                              Constructor # // 
// ####################################################################### //
/** Creates a new instance of an empty Variant object
*/
Variant::Variant() :
  m_eType(T_NONE) {}

// ####################################################################### //
// # Nuclex::Variant::Variant()                              Constructor # // 
// ####################################################################### //
/** Creates a new instance as copy of an existing instance.

    @param  Value  Instance to copy
*/
Variant::Variant(const Variant &Value) :
  m_eType(T_NONE) {
  operator =(Value);
}

// ####################################################################### //
// # Nuclex::Variant::Variant()                              Constructor # // 
// ####################################################################### //
/** Creates a new instance of Variant initialized with a boolean

    @param  bValue  Initial boolean value
*/
Variant::Variant(bool bValue) :
  m_eType(T_BOOL),
  m_bValue(bValue) {}

// ####################################################################### //
// # Nuclex::Variant::Variant()                              Constructor # // 
// ####################################################################### //
/** Creates a new instance of Variant initialized with an integer

    @param  nValue  Initial integer value
*/
Variant::Variant(int nValue) :
  m_eType(T_INT),
  m_nValue(nValue) {}

// ####################################################################### //
// # Nuclex::Variant::Variant()                              Constructor # // 
// ####################################################################### //
/** Creates a new instance of Variant initialized with an integer

    @param  Value  Initial integer value
*/
Variant::Variant(size_t Value) :
  m_eType(T_SIZE),
  m_Value(Value) {}

// ####################################################################### //
// # Nuclex::Variant::Variant()                              Constructor # // 
// ####################################################################### //
/** Creates a new instance of Variant initialized with a float

    @param  fValue  Initial float value
*/
Variant::Variant(float fValue) :
  m_eType(T_DOUBLE),
  m_dValue(static_cast<double>(fValue)) {}

// ####################################################################### //
// # Nuclex::Variant::Variant()                              Constructor # // 
// ####################################################################### //
/** Creates a new instance of Variant initialized with a double

    @param  dValue  Initial double value
*/
Variant::Variant(double dValue) :
  m_eType(T_DOUBLE),
  m_dValue(dValue) {}

// ####################################################################### //
// # Nuclex::Variant::Variant()                              Constructor # // 
// ####################################################################### //
/** Creates a new instance of Variant initialized with a c string

    @param  pszString  Initial string value
*/
Variant::Variant(const char *pszString) :
  m_eType(T_STRING),
  m_psString(new string(pszString)) {}

// ####################################################################### //
// # Nuclex::Variant::Variant()                              Constructor # // 
// ####################################################################### //
/** Creates a new instance of Variant initialized with a c string

    @param  pszString  Initial string value
*/
Variant::Variant(const wchar_t *pszString) :
  m_eType(T_WSTRING),
  m_psWString(new wstring(pszString)) {}

// ####################################################################### //
// # Nuclex::Variant::Variant()                              Constructor # // 
// ####################################################################### //
/** Creates a new instance of Variant initialized with a c string

    @param  sString  Initial string value
*/
Variant::Variant(const string &sString) :
  m_eType(T_STRING),
  m_psString(new string(sString)) {}

// ####################################################################### //
// # Nuclex::Variant::Variant()                              Constructor # // 
// ####################################################################### //
/** Creates a new instance of Variant initialized with a c string

    @param  sString  Initial string value
*/
Variant::Variant(const wstring &sString) :
  m_eType(T_WSTRING),
  m_psWString(new wstring(sString)) {}

// ####################################################################### //
// # Nuclex::Variant::~Variant()                              Destructor # // 
// ####################################################################### //
/** Destroys an instance of Variant
*/
Variant::~Variant() {
  reset();
}

// ####################################################################### //
// # Nuclex::Variant::operator =()                                       # // 
// ####################################################################### //
/** Assigns a value to the Variant. If the assigned value is of type
    Blob or Object, AddRef() will automatically be called on it.

    @param  Value  Value to assign
    @return The assigned value
*/
Variant &Variant::operator =(const Variant &Value) {
  reset();

  m_eType = Value.m_eType;
  switch(m_eType) {
    case T_NONE: {
      break;
    }
    case T_BOOL: {
      m_bValue = Value.m_bValue;
      break;
    }
    case T_INT: {
      m_nValue = Value.m_nValue;
      break;
    }
    case T_SIZE: {
      m_Value = Value.m_Value;
      break;
    }
    case T_DOUBLE: {
      m_dValue = Value.m_dValue;
      break;
    }
    case T_STRING: {
      m_psString = new string(*Value.m_psString);
      break;
    }
    case T_WSTRING: {
      m_psWString = new wstring(*Value.m_psWString);
      break;
    }
  }

  return *this;
}

// ####################################################################### //
// # Nuclex::Variant::operator =()                                       # // 
// ####################################################################### //
/** Assigns a boolean value to the Variant

    @param  bValue  Boolean value
    @return The assigned value
*/
Variant &Variant::operator =(bool bValue) {
  reset();
  m_eType = T_BOOL;
  m_bValue = bValue;

  return *this;
}

// ####################################################################### //
// # Nuclex::Variant::operator =()                                       # // 
// ####################################################################### //
/** Assigns an integer value to the Variant

    @param  nValue  Integer value
    @return The assigned value
*/
Variant &Variant::operator =(int nValue) {
  reset();
  m_eType = T_INT;
  m_nValue = nValue;

  return *this;
}

// ####################################################################### //
// # Nuclex::Variant::operator =()                                       # // 
// ####################################################################### //
/** Assigns an integer value to the Variant

    @param  Value  Integer value
    @return The assigned value
*/
Variant &Variant::operator =(size_t Value) {
  reset();
  m_eType = T_SIZE;
  m_Value = Value;

  return *this;
}

// ####################################################################### //
// # Nuclex::Variant::operator =()                                       # // 
// ####################################################################### //
/** Assigns a double value to the Variant

    @param  dValue  Double value
    @return The assigned value
*/
Variant &Variant::operator =(double dValue) {
  reset();
  m_eType = T_DOUBLE;
  m_dValue = dValue;

  return *this;
}

// ####################################################################### //
// # Nuclex::Variant::operator =()                                       # // 
// ####################################################################### //
/** Assigns a string value to the Variant

    @param  sString  String value
    @return The assigned value
*/
Variant &Variant::operator =(const string &sString) {
  reset();
  m_eType = T_STRING;
  m_psString = new string(sString);

  return *this;
}

// ####################################################################### //
// # Nuclex::Variant::operator =()                                       # // 
// ####################################################################### //
/** Assigns a string value to the Variant

    @param  sString  String value
    @return The assigned value
*/
Variant &Variant::operator =(const wstring &sString) {
  reset();
  m_eType = T_WSTRING;
  m_psWString = new wstring(sString);

  return *this;
}

// ####################################################################### //
// # Nuclex::Variant::operator bool()                                    # // 
// ####################################################################### //
/** Returns the value of the object as boolean
  
    @return Boolean value of the object
*/
Variant::operator bool() const {
  switch(m_eType) {
    case T_NONE:
      return false;
    case T_BOOL:
      return m_bValue;
    case T_INT:
      return !!m_nValue;
    case T_SIZE:
      return !!m_Value;
    case T_DOUBLE:
      return (m_dValue != 0.0);
    case T_STRING:
      if(*m_psString == "1")
        return true;
      else if(*m_psString == "0")
        return false;
      else
        return m_psString->length() > 0;
    case T_WSTRING:
/*    
      if(*m_psString == L"1")
        return true;
      else if(*m_psString == L"0")
        return false;
      else
*/      
        return m_psWString->length() > 0;
    default:
      return false;
  }
}

// ####################################################################### //
// # Nuclex::Variant::operator long()                                    # // 
// ####################################################################### //
/** Returns the value of the object as integer

    @return Integer value of the object
*/
Variant::operator long() const {
  switch(m_eType) {
    case T_NONE:
      return 0;
    case T_BOOL:
      return static_cast<int>(m_bValue);
    case T_INT:
      return m_nValue;
    case T_SIZE:
      return static_cast<int>(m_Value);
    case T_DOUBLE:
      return static_cast<int>(m_dValue);
    case T_STRING:
      return lexical_cast<int>(*m_psString); //::atoi(m_psString->c_str());
    case T_WSTRING:
      return wlexical_cast<int>(*m_psWString); //::atoi(m_psString->c_str());
    default:
      return 0;
  }
}

// ####################################################################### //
// # Nuclex::Variant::operator size_t()                                  # // 
// ####################################################################### //
/** Returns the value of the object as integer

    @return Integer value of the object
*/
Variant::operator size_t() const {
  switch(m_eType) {
    case T_NONE:
      return 0;
    case T_BOOL:
      return static_cast<size_t>(m_bValue);
    case T_INT:
      return m_nValue;
    case T_SIZE:
      return static_cast<size_t>(m_Value);
    case T_DOUBLE:
      return static_cast<size_t>(m_dValue);
    case T_STRING:
      return static_cast<size_t>(lexical_cast<int>(*m_psString));
    case T_WSTRING:
      return static_cast<size_t>(wlexical_cast<int>(*m_psWString));
    default:
      return 0;
  }
}

// ####################################################################### //
// # Nuclex::Variant::operator double()                                  # // 
// ####################################################################### //
/** Returns the value of the object as double

    @return Double value of the object
*/
Variant::operator double() const {
  switch(m_eType) {
    case T_NONE:
      return 0.0;
    case T_BOOL:
      return static_cast<double>(m_bValue);
    case T_INT:
      return static_cast<double>(m_nValue);
    case T_SIZE:
      return static_cast<double>(m_Value);
    case T_DOUBLE:
      return m_dValue;
    case T_STRING:
      return lexical_cast<double>(*m_psString);
    case T_WSTRING:
      return wlexical_cast<double>(*m_psWString);
    default:
      return 0.0;
  }
}

// ####################################################################### //
// # Nuclex::Variant::operator string()                                  # // 
// ####################################################################### //
/** Returns the value of the object as string

    @return String value of the object
*/
Variant::operator string() const {
  switch(m_eType) {
    case T_NONE:
      return "";
    case T_BOOL:
      return m_bValue ? "1" : "0";
    case T_INT:
      return lexical_cast<string>(m_nValue);
    case T_SIZE:
      return lexical_cast<string>(m_Value);
    case T_DOUBLE:
      return lexical_cast<string>(m_dValue);
    case T_STRING:
      return *m_psString;
    case T_WSTRING:
      return asciiFromUnicode(*m_psWString);
    default:
      return "";
  }
}

// ####################################################################### //
// # Nuclex::Variant::operator wstring()                                 # // 
// ####################################################################### //
/** Returns the value of the object as string

    @return String value of the object
*/
Variant::operator wstring() const {
  switch(m_eType) {
    case T_NONE:
      return L"";
    case T_BOOL:
      return m_bValue ? L"1" : L"0";
    case T_INT:
      return wlexical_cast<wstring>(m_nValue);
    case T_SIZE:
      return wlexical_cast<wstring>(m_Value);
    case T_DOUBLE:
      return wlexical_cast<wstring>(m_dValue);
    case T_STRING:
      return unicodeFromAscii(*m_psString);
    case T_WSTRING:
      return *m_psWString;
    default:
      return L"";
  }
}

// ####################################################################### //
// # Nuclex::Variant::reset()                                            # // 
// ####################################################################### //
/** Clears the contents of the Variant instance. If the Value contains
    an Objects or a Blob, it will be Release()d.
    After the method returns, the Value is in an empty state again.
*/
void Variant::reset() {
  switch(m_eType) {
    case T_STRING: {
      delete m_psString;
      break;
    }
    case T_WSTRING: {
      delete m_psWString;
      break;
    }
  }
  m_eType = T_NONE;
}
