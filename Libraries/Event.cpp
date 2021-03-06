#include <sys/time.h>
#include <pthread.h>
#include <errno.h>
#include "LogPlus.hpp"
#include "Event.hpp"

#define EVENT_SUCCESS               (0)

/**
 * @func   Event
 * @brief  None
 * @param  None
 * @retval None
 */
Event::Event() : Locker() {
    m_byWaitingThreads = 0;
    m_boIsSignaled = FALSE;
    m_boManualReset = TRUE;
    m_pEvent = new event_t();
    if (pthread_cond_init(m_pEvent, NULL) != EVENT_SUCCESS) {
        LOG_ERROR("init fail");
    }
}

/**
 * @func   ~Event
 * @brief  None
 * @param  None
 * @retval None
 */
Event::~Event() {
    if (m_pEvent != NULL) {
        pthread_cond_destroy(m_pEvent);
        delete m_pEvent;
        m_pEvent = NULL;
    }
}

/**
 * @func   Set
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Event::Set() {
    int_t idwResult = 0;
    Locker::Lock();
    if (m_boManualReset) {
        m_boIsSignaled = TRUE;
        idwResult = pthread_cond_broadcast(m_pEvent);
        if (idwResult != EVENT_SUCCESS) {
//            debug_event("broadcast fail");
        } else {
//            debug_event("broadcast success");
        }
    } else {
        if (m_byWaitingThreads != 0) {
            idwResult = pthread_cond_signal(m_pEvent);
            if (idwResult != EVENT_SUCCESS) {
//                debug_event("signal fail");
            } else {
//                debug_event("signal success");
            }
        } else {
            m_boIsSignaled = TRUE;
        }
    }
    Locker::UnLock();
}

/**
 * @func   Reset
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Event::Reset() {
    Locker::Lock();
    m_boIsSignaled = FALSE;
    Locker::UnLock();
}

/**
 * @func   Wait
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Event::Wait(
    i32_t idwMsecTimeout
) {
    bool_t boRetVal = TRUE;
    int_t idwResult = 0;
    Locker::Lock();
    if (m_boIsSignaled) {
        if (!m_boManualReset) {
            m_boIsSignaled = FALSE;
        }
    } else {
        m_byWaitingThreads++;
        if (idwMsecTimeout == 0) {
            boRetVal = m_boIsSignaled;
        } else if (idwMsecTimeout > 0) {
            struct timeval now;
            struct timespec abstime;
            gettimeofday(&now, NULL);
            abstime.tv_sec = now.tv_sec + (idwMsecTimeout / 1000);
            now.tv_usec += (idwMsecTimeout % 1000) * 1000;
            while (now.tv_usec >= (1000 * 1000)) {
                now.tv_usec -= (1000 * 1000);
                abstime.tv_sec++;
            }
            abstime.tv_nsec = now.tv_usec * 1000;
            while (!m_boIsSignaled) {
                int_t iOldSate;

                pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &iOldSate);
                idwResult = pthread_cond_timedwait(m_pEvent, Locker::GetLocker(), &abstime);
                pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &iOldSate);

                if (idwResult == ETIMEDOUT) {
                    boRetVal = FALSE;
                    break;
                } else if (idwResult == EVENT_SUCCESS) {
                } else {
                    boRetVal = FALSE;
                    break;
                }
            }
        } else {
            while (!m_boIsSignaled) {
                int_t iOldSate;

                pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &iOldSate);
                idwResult = pthread_cond_wait(m_pEvent, Locker::GetLocker());
                pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &iOldSate);

                if (idwResult == EVENT_SUCCESS) {
                } else {
                    boRetVal = FALSE;
                    break;
                }
            }
        }
        m_byWaitingThreads--;
    }
    Locker::UnLock();
    return boRetVal;
}

/**
 * @func   IsSignalled
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Event::IsSignalled() {
    return m_boIsSignaled;
}
