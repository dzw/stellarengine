//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Thread.h - Thread                                   //
// ### # #      ###                                                      //
// # ### #      ###  A thread of execution, running in parallel          //
// #  ## #   # ## ## to other threads                                    //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_THREAD_H
#define NUCLEX_SUPPORT_THREAD_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"
#include <memory>

#ifdef NUCLEX_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace Nuclex { namespace Support {

//  //
//  Nuclex::Support::Thread                                              //
//  //
/// Thread
/** Encapsulates a line of code which executes in parallel to the
    other code.
    
    This thread class does neither provide a method for the thread to
    sleep or wait because the intended usage pattern for the thread is:
    @code
    void MyThread::thread() {
      while(moreWork) {
        doSomething();
       
        if(stopRequested())
          break;
      }
    }
    @endcode
    
    A class is derived from Thread, overloading the thread() method with
    the code that should run in parallel to other threads. This code
    should regularly check if stopRequested() returns true, and if it
    does so, cancel processing and return. When the thread's work is
    completed, the thread should also return.
    
    If there may be new tasks that arrive while the thread is processing,
    the thread class should provide a synchronized queue which the thread
    will process until it is empty. If the queue is empty (or stopRequested()
    returns true), the thread simply returns. You should call start()
    each time you enqueue a new task for the thread. If the thread is still
    running, start() will have no effect and the thread processes the
    queued task normally. If the thread was finished in the meantime,
    it will be launched anew by start().
    
    If the thread has continuous work to do, like continually paging
    new data into memory for a terrain engine, it should as well regularly
    check for the stopRequested() signal and continue processing until
    it is set. If there's no more to do (and the thread is designed to
    check for new work itself), the thread is allowed to loop, calling
    yield() each time.
*/
class Thread {
  public:
    class Function {
      public:
        virtual ~Function() {}

      public:        
        virtual void operator()() = 0;
        virtual void requestStop() {}
    };

    NUCLEX_API inline static void interlockedIncrement(long &Value);
    NUCLEX_API inline static void interlockedDecrement(long &Value);
    NUCLEX_API inline static void sleep(long nMilliseconds);

    /// Constructor
    NUCLEX_API Thread(std::auto_ptr<Function> spThreadFunction);
    /// Destructor
    NUCLEX_API ~Thread();

  //
  // Thread implementation
  //
  public:
    /// Stop executing the thread
    NUCLEX_API void requestStop();
    /// Check whether the thread is running
    NUCLEX_API bool isRunning() const;
    /// Join with calling thread
    NUCLEX_API void join();
    
  private:
    struct Impl;
    Impl *m_pImpl;
};

// ####################################################################### //
// # Nuclex::Thread::interlockedIncrement()                              # // 
// ####################################################################### //
inline void Thread::interlockedIncrement(long &Value) {
#ifdef NUCLEX_WIN32
  ::InterlockedIncrement(&Value);
#else
  #error Not implemented yet
#endif
}

// ####################################################################### //
// # Nuclex::Thread::interlockedDecrement()                              # // 
// ####################################################################### //
inline void Thread::interlockedDecrement(long &Value) {
#ifdef NUCLEX_WIN32
  ::InterlockedDecrement(&Value);
#else
  #error Not implemented yet
#endif
}

// ####################################################################### //
// # Nuclex::Thread::sleep()                                             # // 
// ####################################################################### //
inline void Thread::sleep(long nMilliseconds) {
#ifdef NUCLEX_WIN32
  ::Sleep(nMilliseconds);
#else
  #error Not implemented yet
#endif
}

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_THREAD_H
