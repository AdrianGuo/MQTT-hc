#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <limits.h>
#include <pthread.h>
#include "debug.hpp"
#include "RTimer.hpp"

#define RTIMER_ERROR                (-1)

u32_t RTimer::m_dwNumberOfTimer = 0;
Map<u32_t, RTimer*> RTimer::m_mapThreadToTimer;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
RTimer::MemsetRTimer() {
    for (u32_t i = 0; i < MaxTimer; i++) {
        m_Timer[i].pBuffer = NULL;
        m_Timer[i].pTimerFunctor = NULL;
        m_Timer[i].activeflag = FALSE;
        m_Timer[i].repeats = 0;
        m_Timer[i].start = 0;
        m_Timer[i].timeout = 0;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RTimer::RTimer() : m_dwCount (0) {
    MemsetRTimer();
    m_dwTimerIndex = m_dwNumberOfTimer++;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RTimer::~RTimer() {
    for (u32_t i = 0; i < MaxTimer; i++) {
        if (m_Timer[i].pTimerFunctor != NULL) {
            delete m_Timer[i].pTimerFunctor;
            m_Timer[i].pTimerFunctor = NULL;
        }
    }
    m_dwNumberOfTimer--;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RTimer_p
RTimer::getTimerInstance() {
    u32_t dwthreadfd = (u32_t) pthread_self();
    MapThreadToTimer::const_iterator_t it = m_mapThreadToTimer.find(dwthreadfd);
    if (it == m_mapThreadToTimer.end()) {
        RTimer_p pTimer = new RTimer();
        m_mapThreadToTimer[dwthreadfd] = pTimer;
        return pTimer;
    } else {
        return it->second;
    }

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
RTimer::getTickCount() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec;
}

/**
 * @func   Process
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
RTimer::Process() {
    timerFunctor_t TimerFunctor;
    void_p pParamter;

    for (u32_t i = 0; i < MaxTimer; i++) {
        if (IsExpired(i) && (m_Timer[i].pTimerFunctor != NULL) && m_Timer[i].activeflag) {
            TimerFunctor = *m_Timer[i].pTimerFunctor;
            pParamter = m_Timer[i].pBuffer;

            if ((m_Timer[i].repeats != RTimer::Repeat::Forever) && (m_Timer[i].repeats > 0)) {
                m_Timer[i].repeats--;
            }

            TimerFunctor(pParamter); /* Execute functor */

            if (m_Timer[i].repeats == 0) {
                m_Timer[i].pTimerFunctor = NULL;
                m_Timer[i].pBuffer = NULL;
                m_Timer[i].activeflag = FALSE;
                m_dwCount--;
            }
        }
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
RTimer::StartTimer(
    i8_t ibyRepeat,
    u32_t dwTimeout,
    timerFunctor_p pTimerFunctor,
    void_p pBuffer
) {
    if (pTimerFunctor != NULL) { /* If Functor valid */
        for (u32_t i = 0; i < MaxTimer; i++) {
            if (m_Timer[i].pTimerFunctor == NULL) { /* If Functor invalid */
                m_Timer[i].start = getTickCount();
                m_Timer[i].timeout = dwTimeout;
                m_Timer[i].repeats = ibyRepeat;
                m_Timer[i].pTimerFunctor = pTimerFunctor;
                m_Timer[i].pBuffer = pBuffer;
                m_Timer[i].activeflag = TRUE;
                m_dwCount++;

                return (i);
            }
        }
    }
    return RTIMER_ERROR;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
RTimer::RestartTimer(
    u32_t dwTimerID,
    i8_t ibyRepeat,
    u32_t byTimeout
) {
    if ((dwTimerID > MaxTimer) || (m_Timer[dwTimerID].pTimerFunctor == NULL) || (!m_Timer[dwTimerID].activeflag))
        return FALSE;

    m_Timer[dwTimerID].start = getTickCount();
    m_Timer[dwTimerID].timeout = byTimeout;
    m_Timer[dwTimerID].repeats = ibyRepeat;

    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
RTimer::CancelTimer(
    u32_t dwTimerID
) {
    if ((dwTimerID > MaxTimer) || (m_Timer[dwTimerID].pTimerFunctor == NULL))
        return FALSE;

    m_Timer[dwTimerID].start = 0;
    m_Timer[dwTimerID].timeout = 0;
    m_Timer[dwTimerID].repeats = 0;
    m_Timer[dwTimerID].pTimerFunctor = NULL;
    m_Timer[dwTimerID].pBuffer = NULL;
    m_Timer[dwTimerID].activeflag = FALSE;
    m_dwCount--;

    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
RTimer::IsExpired(
    u32_t dwTimerID
) {
    if ((dwTimerID > MaxTimer) || (m_Timer[dwTimerID].pTimerFunctor == NULL))
        return FALSE;

    bool_t boRetVal = FALSE;
    u32_t dwTickCount = getTickCount();
    u32_t dwDeltaTick = dwTickCount - m_Timer[dwTimerID].start;

    if (dwDeltaTick >= m_Timer[dwTimerID].timeout) {
        boRetVal = TRUE;
        m_Timer[dwTimerID].start = getTickCount();
    }

    return boRetVal;
}
