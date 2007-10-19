//------------------------------------------------------------------------------
//  refcounted.cc
//  (C) 2006 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "core/refcounted.h"

namespace Core
{
ImplementRootClass(Core::RefCounted, 'REFC');

//#if STELLAR_DEBUG
//using namespace Util;
//
//ThreadLocal bool RefCounted::isInCreate = false;
//RefCountedList RefCounted::list;
//Threading::CriticalSection RefCounted::criticalSection;
//#endif

//------------------------------------------------------------------------------
/**
*/
RefCounted::~RefCounted()
{
    s_assert(0 == this->refCount);
    //#if STELLAR_DEBUG
    //    s_assert(!this->destroyed);
    //    s_assert(0 != this->listIterator);
    //    criticalSection.Enter();
    //    list.Remove(this->listIterator);
    //    criticalSection.Leave();
    //    this->listIterator = 0;
    //    this->destroyed = true;
    //#endif
}

//------------------------------------------------------------------------------
/**
    This method should be called as the very last before an application 
    exits.
*/
void
RefCounted::DumpRefCountingLeaks()
{
    //#if STELLAR_DEBUG
    //list.DumpLeaks();    
    //#endif
}

//------------------------------------------------------------------------------
/**
    Gather per-class stats.
*/
//#if STELLAR_DEBUG
//Dictionary<String,RefCounted::Stats>
//RefCounted::GetOverallStats()
//{
//    Dictionary<String,Stats> result;
//    criticalSection.Enter();
//    // iterate over refcounted list
//    RefCountedList::Iterator iter;
//    for (iter = list.Begin(); iter != list.End(); iter++)
//    {
//        RefCounted* cur = *iter;
//        if (!result.Contains(cur->GetClassName()))
//        {
//            Stats newStats;
//            newStats.className   = cur->GetClassName();
//            newStats.classFourCC = cur->GetClassFourCC();
//            newStats.numObjects  = 1;
//            newStats.overallRefCount = cur->GetRefCount();
//            result.Add(cur->GetClassName(), newStats);
//        }
//        else
//        {
//            Stats& stats = result[cur->GetClassName()];
//            stats.numObjects++;
//            stats.overallRefCount += cur->GetRefCount();
//        }
//    }
//    criticalSection.Leave();
//    return result;
//}
//#endif

} // namespace Foundation