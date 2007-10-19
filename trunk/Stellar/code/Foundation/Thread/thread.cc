//------------------------------------------------------------------------------
//  thread.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "thread/thread.h"

namespace Threading
{
#if __WIN32__
ImplementClass(Threading::Thread, 'TRED', Win32::Win32Thread);
#else
#error "Thread class not implemented on this platform!"
#endif
}
