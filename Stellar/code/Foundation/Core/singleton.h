#pragma once
#ifndef CORE_SINGLETON_H
#define CORE_SINGLETON_H
//------------------------------------------------------------------------------
/**
    @file core/singleton.h

    Provides helper macros to implement singleton objects:
    
    - DeclareSingleton      put this into class declaration
    - ImplementSingleton    put this into the implemention file
    - ConstructSingleton    put this into the constructor
    - DestructSingleton     put this into the destructor

    Get a pointer to a singleton object using the static Instance() method:

    Core::Server* coreServer = Core::Server::Instance();
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
#define DeclareSingleton(type) \
public: \
    ThreadLocal static type * Singleton; \
    static type * Instance() { s_assert(0 != Singleton); return Singleton; }; \
    static bool HasInstance() { return 0 != Singleton; }; \
private:

#define DeclareInterfaceSingleton(type) \
public: \
    static type * Singleton; \
    static type * Instance() { s_assert(0 != Singleton); return Singleton; }; \
    static bool HasInstance() { return 0 != Singleton; }; \
private:

#define ImplementSingleton(type) \
    ThreadLocal type * type::Singleton = 0;

#define ImplementInterfaceSingleton(type) \
    type * type::Singleton = 0;

#define ConstructSingleton \
    s_assert(0 == Singleton); Singleton = this;

#define DestructSingleton \
    s_assert(Singleton); Singleton = 0;
//------------------------------------------------------------------------------
#endif

    