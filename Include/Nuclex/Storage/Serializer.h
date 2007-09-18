//  //
// #   #  ###  #   #                           -= Nuclex Library =-                            //
// ##  # #   # ## ## Serializer.h - Object serialization interface                             //
// ### # #      ###                                                                            //
// # ### #      ###  Provides an interface for objects to serialize their data                 //
// #  ## #   # ## ## into custom formats                                                       //
// #   #  ###  #   # R1                              (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_STORAGE_SERIALIZER_H
#define NUCLEX_STORAGE_SERIALIZER_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/Support/Variant.h"

namespace Nuclex { namespace Storage {

//  //
//  Nuclex::Storage::Serializer                                                                //
//  //
/// Structured serializer base class
/** Base class for structured data serializers
*/
class Serializer {
  public:
    class ScopeEnumerator;
    
    /// Destructor
    /** Destroys an instance of Serializer
    */
    NUCLEX_API virtual ~Serializer() {}

  //
  // Serializer implementation
  //
  public:
    /// Retrieve value
    /** Retrieves an attribute from the scope

        @param  sName  Name of the attribute to retrieve
        @return The retrieved value
    */
    template<typename VarType> inline VarType get(const string &sName) {
      return static_cast<VarType>(retrieveValue(sName));
    }

    /// Retrieve value or return default
    /** Retrieves an attribute from the scope or returns the
        specified default value, if the attribute does not exist
        
        @param  sName    Name of the attribute whose value to retrieve
        @param  Default  Default value to return when the attribute doesn't exist
        @return The value of the attribute or the specified default value
    */
    template<typename VarType> inline VarType get(const string &sName, const VarType &Default) {
      return static_cast<VarType>(retrieveValue(sName, Variant(Default)));
    }
    
    /// Store value
    /** Stores an attribute in the scope

        @param  sName  Name of the new attribute
        @param  Value  Value to store
    */
    template<typename VarType> inline void set(const string &sName, const VarType &Value) {
      storeValue(sName, Variant(Value));
    }
    
    /// Enumerate nested scopes of this scope
    /** Creates a new enumerator which enumerates all nested scopes
        of the scope being called on
        
        @param  sName  Name of the scopes to enumerate
        @return A new enumerator
    */
    NUCLEX_API virtual shared_ptr<ScopeEnumerator> enumScopes(const string &sName = "") = 0;
    
    /// Enter nested scope
    /** Enters an existing block of nested data.
        If the method is called in succession with the same name, it
        will enter multiple blocks sharing that name one after another.

        @param  sName  Name of the block to enter
        @param  bOptional  Whether to throw an exception if scope not found
        @return True if the block was entered
    */
    NUCLEX_API virtual shared_ptr<Serializer> openScope(const string &sName, bool bOptional = false) = 0;

    /// Begin nested scope
    /** Begins a new block of nested data. Always creates a new block,
        even if a block with the same name already exists.
        
        @param  sName  Name of the scope to create
    */
    NUCLEX_API virtual shared_ptr<Serializer> createScope(const string &sName) = 0;

  private:
    /// Retrieve value from scope
    virtual Variant retrieveValue(const string &sName) = 0;
    /// Retrieve value from scope or return default
    virtual Variant retrieveValue(const string &sName, const Variant &Default) = 0;
    /// Store value in scope
    virtual void storeValue(const string &sName, const Variant &Value) = 0;
};

//  //
//  Nuclex::Storage::Serializer::ScopeEnumerator                                               //
//  //
/// Enumerator for serializer scopes
/** Enumerates over the nested scopes of a serializer scope
*/
class Serializer::ScopeEnumerator {
  public:
    /// Destructor
    virtual ~ScopeEnumerator() {}
    
  public:
    /// Advance to next entry
    /** Advances to the next Archive. If no more Archives are available,
        false is returned. Also required to be the first method called,
        in order to correctly handle enumerators to empty lists.

        @return True if the next Archive was reached
    */
    NUCLEX_API virtual bool next() = 0;

    /// Get current scope
    /** Returns the current scope being enumerated

        @return The enumerator's current scope
    */
    NUCLEX_API virtual const std::pair<string, shared_ptr<Serializer> > &get() const = 0;
};

}} // namespace Nuclex::Storage

#endif // NUCLEX_STORAGE_SERIALIZER_H
