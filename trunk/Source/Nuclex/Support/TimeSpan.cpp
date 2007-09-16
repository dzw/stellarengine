//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## TimeSpan.h - Accurate time span                     //
// ### # #      ###                                                      //
// # ### #      ###  Stores a time span with up to microsecond           //
// #  ## #   # ## ## accuracy using integers                             //
// #   #  ###  #   # R1        (C)2002-2004 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Support/TimeSpan.h"

using namespace Nuclex;
using namespace Nuclex::Support;

#ifdef NUCLEX_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#endif

/* Time wrap solutions


   class TimeSpan {
     class OverflowingTimeSpan {
       operator -= OverflowingTimeSpan
     };
     public:
       static OverflowingTimeSpan getRunningTime();
*/

namespace {

//  //
//  SystemTimer                                                          //
//  //
/// System timer
/** Represents the system timer using platform-dependent code.
*/
class SystemTimer {
  public:
    /// Constructor
    SystemTimer() {
#ifdef NUCLEX_WIN32
      LARGE_INTEGER Frequency;

      // Try to use the PerformanceCounter, if one exists
      if(::QueryPerformanceFrequency(&Frequency))
        m_Frequency = Frequency;
      else
        m_Frequency.QuadPart = 0;

      if(m_Frequency.QuadPart)
        ::QueryPerformanceCounter(&m_LaunchPC);
      else
        m_LaunchTGT = ::timeGetTime();
#endif // NUCLEX_WIN32
    }

    /// Get time since application start
    TimeSpan getRunningTime() {
#ifdef NUCLEX_WIN32
      if(m_Frequency.QuadPart) {
        LARGE_INTEGER Counter;
        ::QueryPerformanceCounter(&Counter);

        return TimeSpan(static_cast<size_t>(
          (Counter.QuadPart - m_LaunchPC.QuadPart) * 1000000 / m_Frequency.QuadPart
        ));
      } else {
        return TimeSpan(timeGetTime());
      }
#else
#error Not implemented yet
#endif
    }

  private:
#ifdef NUCLEX_WIN32
    LARGE_INTEGER m_Frequency;                        ///< Peformance Counter frequency
    union {
      LARGE_INTEGER m_LaunchPC;                       ///< Launch time for QueryPerformanceCounter()
      DWORD         m_LaunchTGT;                      ///< Launch time for timeGetTime()
    };
#endif
} g_SystemTimer;

} // namespace

// ####################################################################### //
// # Nuclex::TimeSpan::getRunningTime()                                  # // 
// ####################################################################### //
/** Returns the total uptime of the application. This value can be
    used for timing purposes, like calculating the time spent to draw
    a single frame.
    
    This time span will wrap after the range of a size_t is exceeded.
    Handling the wrap around properly shouldn't require any special
    precautions because the class automatically assumes a wrap when
    a longer time span is subtracted from a shorter time span.
    
    @return The application's uptime
*/
TimeSpan TimeSpan::getRunningTime() {
  return g_SystemTimer.getRunningTime();
}
