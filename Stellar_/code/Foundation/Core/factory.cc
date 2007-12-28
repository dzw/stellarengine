//------------------------------------------------------------------------------
//  factory.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/factory.h"
#include "core/refcounted.h"
#include "core/rtti.h"

namespace Core
{
Factory* Factory::Singleton = 0;
using namespace Util;

//------------------------------------------------------------------------------
/**
    The factory's constructor is called by the Instance() method, and 
    nobody else.
*/
Factory*
Factory::Instance()
{
    if (0 == Singleton)
    {
        Singleton = s_new(Factory);
        s_assert(0 != Singleton);
    }
    return Singleton;
}

//------------------------------------------------------------------------------
/**
    This static method is used to destroy the factory object and should be
    called right before the main function exits. It will make sure that
    no accidential memory leaks are reported by the debug heap.
*/
void
Factory::Destroy()
{
    if (0 != Singleton)
    {
        s_delete(Singleton);
        Singleton = 0;
    }
}

//------------------------------------------------------------------------------
/**
    The factory's constructor is private so that it may only be called 
    by Factory::Instance().
*/
Factory::Factory() /*:
    nameTable(512)*/
{
    // empty
}

//------------------------------------------------------------------------------
/**
    The factory's destructor is private so that it may only be called
    by Factory::Destroy().
*/
Factory::~Factory()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This registers an Rtti object with the factory and is called from
    an Rtti object's constructor. The function will fail with an error
    message box if a class with an identical class name or fourcc code
    has already been registered.

    NOTE: we cannot use the class name of fourcc from the RTTI object, 
    because it may be that the RTTI object hasn't been initialized yet
    when this method is called (initialization order of global variables
    is undefined).
*/
void
Factory::Register(const Rtti* rtti, const Util::String& className, const FourCC& classFourCC)
{
    s_assert(0 != rtti);

    // check if class name already exists
    if (this->ClassExists(className))
    {
        // NOTE: can't use s_error() here, because Console is not setup yet
        // when this method Register() is called!
        /*String errorMsg;
        errorMsg.Format("Class name '%s' has already been registered!", className.c_str());
        SysFunc::Error(errorMsg.c_str());*/
        s_assert("Class name '%s' has already been registered!");
        return;
    }

    // check if class fourcc already exists
    if (this->ClassExists(classFourCC))
    {
        // NOTE: can't use s_error() here, because Console is not setup yet
        // when this method Register() is called!
        /*String errorMsg;
        errorMsg.Format("Class fourcc '%s' (name: %s) has already been registered!", 
            classFourCC.AsString().c_str(),
            className.c_str());
        SysFunc::Error(errorMsg.c_str());*/
        s_assert("Class fourcc '%s' (name: %s) has already been registered!");
        return;
    }

    // register with lookup tables
    this->nameTable.insert(HashTableMap::value_type(className, rtti));
    this->fourccTable.insert(CCMap::value_type(classFourCC, rtti));
}

//------------------------------------------------------------------------------
/**
    This method checks if a class with the given name has been registered.
*/
bool
Factory::ClassExists(const Util::String& className) const
{
    s_assert(!className.empty());
    return this->nameTable.find(className) != this->nameTable.end();
}

//------------------------------------------------------------------------------
/**
    This method checks if a class with the given fourcc code has been
    registered.
*/
bool
Factory::ClassExists(const FourCC fourCC) const
{
    s_assert(fourCC.IsValid());

    return this->fourccTable.find(fourCC) != this->fourccTable.end();
}

//------------------------------------------------------------------------------
/**
*/
const Rtti*
Factory::GetClassRtti(const String& className) const
{
    s_assert(!className.empty());
    s_assert(this->ClassExists(className));

    return this->nameTable.find(className)->second;
}

//------------------------------------------------------------------------------
/**
*/
const Rtti*
Factory::GetClassRtti(const FourCC& classFourCC) const
{
    s_assert(classFourCC.IsValid());
    s_assert(this->ClassExists(classFourCC));
    
    return this->fourccTable.find(classFourCC)->second;
}

//------------------------------------------------------------------------------
/**
    Create an object by class name.
*/
RefCounted*
Factory::Create(const String& className) const
{
    s_assert(!className.empty());
    
    // check if class exists, otherwise give a meaningful error
    if (!this->ClassExists(className))
    {
        /*String errorMsg;
        errorMsg.Format("Factory::Create('%s'): unknown class name!", className.c_str());
        SysFunc::Error(errorMsg.c_str());*/
        s_assert("Factory::Create('%s'): unknown class name!");
        return 0;
    }

    // lookup RTTI object of class through hash table and create new object
    const Rtti* rtti = this->nameTable.find(className)->second;
    s_assert(0 != rtti);
    RefCounted* newObject = rtti->Create();
    return newObject;
}

//------------------------------------------------------------------------------
/**
    Create an object by FourCC code.
*/
RefCounted*
Factory::Create(const FourCC classFourCC) const
{
    s_assert(classFourCC.IsValid());

    // check if class exists, otherwise give meaningful error
    if (!this->ClassExists(classFourCC))
    {
        /*String errorMsg;
        errorMsg.Format("Factory::Create('%s'): unknown class FourCC code!", classFourCC.AsString().c_str());
        SysFunc::Error(errorMsg.c_str());*/
        s_assert("Factory::Create('%s'): unknown class FourCC code!");
        return 0;
    }

    // lookup RTTI object of class through hash table and create new object
    CCConstIterator fTMapIterator = this->fourccTable.find(classFourCC);
    const Rtti* rtti = fTMapIterator->second;
    s_assert(0 != rtti);
    RefCounted* newObject = rtti->Create();
    return newObject;
}

} // namespace Core