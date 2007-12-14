#pragma once
#ifndef CORE_FACTORY_H
#define CORE_FACTORY_H
//------------------------------------------------------------------------------
/**
    @class Core::Factory

    Provides the central object factory mechanism for Nebula3. Classes
    which are derived from RefCounted register themselves automatically
    to the central Factory object through the DeclareClass and
    ImplementClass macros.

    Ctuo
    Copyright (c) 2007
*/
#include "utility/string.h"
#include "utility/fourcc.h"
#include "utility/hashtable.h"
#include "core/ptr.h"
#include <map>

//------------------------------------------------------------------------------
namespace Core 
{
class RefCounted;
class Rtti;

//------------------------------------------------------------------------------
class Factory
{
public:
    /// get pointer to singleton instance (cannot use singleton.h!)
    static Factory* Instance();
    /// static instance destruction method
    static void Destroy();

    /// register a RTTI object with the factory
    void Register(const Rtti* rtti, const Util::String& className, const Util::FourCC& classFourCC);
    /// check if a class exists by class name
    bool ClassExists(const Util::String& className) const;
    /// check if a class exists by FourCC code
    bool ClassExists(const Util::FourCC classFourCC) const;
    /// get class rtti object by name
    const Rtti* GetClassRtti(const Util::String& className) const;
    /// get class rtti object by fourcc code
    const Rtti* GetClassRtti(const Util::FourCC& classFourCC) const;
    /// create an object by class name
    RefCounted* Create(const Util::String& className) const;
    /// create an object by FourCC code
    RefCounted* Create(const Util::FourCC classFourCC) const;

private:
    /// constructor is private
    Factory();
    /// destructor is private
    ~Factory();

    typedef std::map<Util::FourCC, const Rtti*> CCMap;
    typedef CCMap::const_iterator CCConstIterator;
    typedef Util::HashTable<const Util::String, const Rtti*> HashTableMap;

    static Factory* Singleton;
    HashTableMap nameTable;// for fast lookup by class name
    CCMap fourccTable;  // for fast lookup by fourcc code
};

}; // namespace Foundation
//------------------------------------------------------------------------------
#endif