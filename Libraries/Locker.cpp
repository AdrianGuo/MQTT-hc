#include <stddef.h>
#include "LogPlus.hpp"
#include "Locker.hpp"

#define LOCK_SUCCESS                (0)
#define LOCK_ERROR                  (-1)

/**
 * @func   Locker
 * @brief  None
 * @param  None
 * @retval None
 */
Locker::Locker(
) : m_boIsLocked (FALSE),
    m_iRefCount (0),
    m_pLocker (new locker_t()) {
    if (pthread_mutex_init(m_pLocker, NULL) != LOCK_SUCCESS) {
        LOG_ERROR("init fail");
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Locker::~Locker() {
    if (m_pLocker != NULL) {
        Lock();
        if (--m_iRefCount <= 0) {
            UnLock();
            pthread_mutex_destroy(m_pLocker);
            delete m_pLocker;
            m_pLocker = NULL;
        } else {
            UnLock();
        }

    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Locker::Lock() {
    int_t idwResult = 0;
    if (m_pLocker != NULL) {
        if ((idwResult = pthread_mutex_lock(m_pLocker)) == LOCK_SUCCESS) {
            m_iRefCount++;
            m_boIsLocked = TRUE;
            return TRUE;
        }
    }
    LOG_ERROR("lock fail");
    return FALSE;
}

/**
 * @func   UnLock
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Locker::UnLock() {
    int_t idwResult = 0;
    if (m_pLocker != NULL) {
        if((idwResult = pthread_mutex_unlock(m_pLocker)) == LOCK_SUCCESS) {
            m_iRefCount--;
            m_boIsLocked = FALSE;
            return TRUE;
        }
    }
    LOG_ERROR("unlock fail");
    return FALSE;
}

/**
 * @func   TryLock
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Locker::TryLock() {
    int_t idwResult = 0;
    if (m_pLocker != NULL) {
        if ((idwResult = pthread_mutex_trylock(m_pLocker)) == LOCK_SUCCESS) {
            m_iRefCount++;
            m_boIsLocked = TRUE;
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * @func   IsLocked
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Locker::IsLocked() {
    return m_boIsLocked;
}

/**
 * @func   GetLocker
 * @brief  None
 * @param  None
 * @retval None
 */
locker_p
Locker::GetLocker() {
    return m_pLocker;
}
