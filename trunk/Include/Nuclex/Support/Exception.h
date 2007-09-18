//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Exception.h - Exception support                     //
// ### # #      ###                                                      //
// # ### #      ###  Base class for exceptions and basic exception       //
// #  ## #   # ## ## classification hierarchy                            //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_EXCEPTION_H
#define NUCLEX_SUPPORT_EXCEPTION_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"
#include <exception>

namespace Nuclex { namespace Support {

//!! SUPER-BUG
//   sWhat could be temporary, the string destroyed, and the string-buffer
//   passed to std::exception may be invalid. std::exception is unlikely to
//   copy the string, because this would be a bad idea in out-of-memory
//   situations.

//  //
//  Nuclex::Support::Exception                                           //
//  //
/// Nuclex exception base class
/** General base class for Nuclex exceptions. Extends the std exception
    with an additional source argument specifying where the error occured.
*/
class Exception :
  public std::exception {
  public:
    /// Constructor
    /** Initializes an instance of CException with a cause and source
        
        @param  sSource  Exception source
                         Recommended form: 'Namespace::Class::Method()'
        @param  sWhat    Cause of the exception
    */
    NUCLEX_API Exception(const string &sSource, const string &sWhat) :
      std::exception(sWhat.c_str()),
      m_sSource(sSource) {}

    /// Exception source
    /** Returns the source of the exception

        @return The exception source
    */
    NUCLEX_API const string &getSource() const { return m_sSource; }

  private:
    string m_sSource;                                 ///< Exception source
};

// ####################################################################### //
// # NUCLEX_DECLAREBASEEXCEPTION                                         # //
// ####################################################################### //
/** Declares an empty exception class derived from std::exception.
    Besides its own type, the exception class also takes a string 
    argument which can be set freely for extended error information
*/
#define NUCLEX_DECLAREBASEEXCEPTION(exception)                              \
  class exception :                                                         \
   public Exception {                                                       \
   public:                                                                  \
     exception(const string &sSource, const string &sWhat) :                \
       Exception(sSource, sWhat) {}                                         \
  };

// ####################################################################### //
// # NUCLEX_DECLAREEXCEPTION                                             # //
// ####################################################################### //
/** Declares an empty exception class derived from std::exception.
    Besides its own type, the exception class also takes a string 
    argument which can be set freely for extended error information
*/
#define NUCLEX_DECLAREEXCEPTION(base, exception)                            \
  class exception :                                                         \
   public base {                                                            \
   public:                                                                  \
     exception(const string &sSource, const string &sWhat) :                \
       base(sSource, sWhat) {}                                              \
  };

// ####################################################################### //
// # Exceptions                                                          # //
// ####################################################################### //
/// Unexpected error
NUCLEX_DECLAREBASEEXCEPTION(UnexpectedException);

/// Invalid argument encountered
NUCLEX_DECLAREBASEEXCEPTION(InvalidArgumentException);
  /// Wrong argument type
  NUCLEX_DECLAREEXCEPTION(InvalidArgumentException, WrongTypeException);
  /// Specified item could not be found
  NUCLEX_DECLAREEXCEPTION(InvalidArgumentException, ItemNotFoundException);

/// Operation failed
NUCLEX_DECLAREBASEEXCEPTION(FailedException);
  /// Operation is not supported
  NUCLEX_DECLAREEXCEPTION(FailedException, NotSupportedException);

/// Memory error
NUCLEX_DECLAREBASEEXCEPTION(MemoryException);
  /// Out of memory
  NUCLEX_DECLAREEXCEPTION(MemoryException, OutOfMemoryException);

/// Resource access error
NUCLEX_DECLAREBASEEXCEPTION(ResourceException);
  /// File cannot be opened
  NUCLEX_DECLAREEXCEPTION(ResourceException, CantOpenResourceException);
  /// Wrong file version
  NUCLEX_DECLAREEXCEPTION(ResourceException, WrongVersionException);
  /// Unsupported file format
  NUCLEX_DECLAREEXCEPTION(ResourceException, UnsupportedFormatException);

/// Exception to be displayed as message to the end user
NUCLEX_DECLAREBASEEXCEPTION(UserLevelException);

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_EXCEPTION_H
