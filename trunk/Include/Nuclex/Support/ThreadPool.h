//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## ThreadPool.h - Thread pool                          //
// ### # #      ###                                                      //
// # ### #      ###  A pool of worker threads for parallel processing    //
// #  ## #   # ## ##                                                     //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#ifndef NUCLEX_SUPPORT_THREADPOOL_H
#define NUCLEX_SUPPORT_THREADPOOL_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/String.h"
#include "Nuclex/Support/Thread.h"
#include "Nuclex/Support/Synchronization.h"
#include "Nuclex/Support/Invocation.h"
#include <vector>
#include <deque>

namespace Nuclex { namespace Support {

//  //
//  Nuclex::Support::ThreadPool                                          //
//  //
/// Thread pool
class ThreadPool {
  public:
    NUCLEX_API ThreadPool(size_t NumThreads);
    NUCLEX_API ~ThreadPool();
    
  //
  // ThreadPool implementation
  //
  public:
    /// Enqueue a task to be executed
    NUCLEX_API void enqueue(std::auto_ptr<Thread::Function> spTask);
    
  private:
    /// A worker thread
    struct WorkerThread :
      public Thread::Function {
      /// Constructor
      WorkerThread(ThreadPool &Owner) :
        m_Owner(Owner),
        m_Signal(false),
        m_bStopRequested(false),
        m_bWorking(false) {}
      
      /// The threaded method
      void operator()();
      /// Request the thread to stop
      void requestStop() { m_bStopRequested = true; }
      /// Access signal for suspending idle threads
      Signal &getSignal() { return m_Signal; }
      /// Check whether the thread is currently working
      bool isWorking() const { return m_bWorking; }
      
      private:
        ThreadPool &m_Owner;                          ///< The worker thread's owner
        Signal      m_Signal;                         ///< Signal to suspend thread
        bool        m_bStopRequested;                 ///< Whether a stop was requested
        bool        m_bWorking;                       ///< Whether the thread is working
    };

    typedef std::vector<std::pair<shared_ptr<Thread>, WorkerThread *> > WorkerThreadVector;
    typedef std::deque<shared_ptr<Thread::Function> > TaskDeque;
    
    WorkerThreadVector m_WorkerThreads;
    Mutex              m_TasksMutex;
    TaskDeque          m_Tasks;
};

}} // namespace Nuclex::Support

#endif // NUCLEX_SUPPORT_THREADPOOL_H
