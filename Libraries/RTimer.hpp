#ifndef LTIMER_HPP_
#define LTIMER_HPP_

#include <signal.h>
#include <time.h>
#include "typedefs.h"
#include "Map.hpp"
#include "Functor.hpp"

typedef Functor1_t(void_p)      TimerFunctor_t;
typedef TimerFunctor_t*         TimerFunctor_p;

typedef struct {
    u32_t   start;
    u32_t   timeout;
    bool_t  activeflag;
    i8_t    repeats;
    TimerFunctor_p pTimerFunctor;
    void_p  pBuffer;
} SRTimer_t, *SRTimer_p;

class RTimer {
private:
    const static u32_t MaxTimer = 200;
    u32_t m_dwCount;
    u32_t m_dwTimerIndex;
    SRTimer_t m_Timer[MaxTimer];
    typedef Map<u32_t, RTimer*> MapThreadToTimer;
    static MapThreadToTimer m_mapThreadToTimer;
    static u32_t m_dwNumberOfTimer;

    void_t MemsetRTimer();
    RTimer();
public:
    typedef enum {
        OneTime = 0,
        Forever = 0xFF
    } Repeat;

    virtual ~RTimer();
    static RTimer* getTimerInstance();
    static u32_t NumberOfTimer() { return m_dwNumberOfTimer; }

    void_t Process();
    u32_t getTickCount();

    int_t StartTimer(
            i8_t ibyRepeat,
            u32_t dwTimeout,
            TimerFunctor_p pTimerFunctor,
            void_p pBuffer);
    bool_t RestartTimer(u32_t dwTimerID, i8_t ibyRepeat, u32_t dwTimeout);
    bool_t CancelTimer(u32_t dwTimerID);
    bool_t IsExpired(u32_t dwTimerID);
    u32_t Count() const { return m_dwCount; }
    u32_t Index() const { return m_dwTimerIndex; }
};

typedef RTimer  RTimer_t;
typedef RTimer* RTimer_p;

#endif /* LTIMER_HPP_ */
