#include <stdlib.h>
#include <pthread.h>
#include "debug.hpp"
#include "LThread.hpp"

#define THREAD_NULL                 (0)
#define THREAD_SUCCESS              (0)
#define THREAD_ERROR                (-1)

u8_t LThread::m_sbyIndexThread = 0;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
LThread::Execute(
    void_p pArguments
) {
    LThread_p pThread = (LThread_p) pArguments;
    DEBUG2("EXECUTE THREAD %d", pThread->GetThreadIndex());
    pThread->Process(pThread->m_pArguments);

    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
LThread::LThread(
    void_p pArguments
) {
    m_threadID = THREAD_NULL;
    m_pThreadLocker = NULL;
    m_pThreadFunctor = NULL;
    m_boIsJoinale = TRUE; /* Default */
    m_pArguments = pArguments;
    m_byThreadIndex = m_sbyIndexThread++;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
LThread::~LThread() {
    if (m_pThreadFunctor != NULL) {
        delete m_pThreadFunctor;
        m_pThreadFunctor = NULL;
    }

    if (m_pThreadLocker != NULL) {
        delete m_pThreadLocker;
        m_pThreadLocker = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
LThread::Start() {
    if (m_pThreadFunctor != NULL) {
        DEBUG2("  START THREAD %d", m_byThreadIndex);
        if (pthread_create(&m_threadID, NULL, LThread::Execute, (void*) this) == THREAD_SUCCESS) {
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
LThread::Stop() {
    DEBUG2("STOP THREAD %d", m_byThreadIndex);
    pthread_exit(NULL);

    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LThread::Process(
    void_p pArguments
) {
    m_pThreadFunctor->operator ()(pArguments);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
LThread::RegThreadFunctor(
    threadFunctor_p pThreadFunctor
) {
    if (pThreadFunctor != NULL) {
        m_pThreadFunctor = pThreadFunctor;
        return TRUE;
    }

    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
LThread::SetLocker(
    locker_p pLocKer
) {
    if (pLocKer != NULL) {
        m_pThreadLocker = pLocKer;
        return TRUE;
    }
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
LThread::Join() {
    int_t idwResult;

    if ((idwResult = pthread_join(m_threadID, NULL)) == THREAD_SUCCESS) {
        DEBUG1("join success");
        m_boIsJoinale = TRUE;
        return TRUE;
    }
    DEBUG1("join fail");

    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
LThread::Detach() {
    int_t idwResult;

    if ((idwResult = pthread_detach(m_threadID)) == THREAD_SUCCESS) {
        DEBUG2(" DETACH THREAD %d", m_byThreadIndex);
        DEBUG1("detach success");
        m_boIsJoinale = FALSE;
        return TRUE;
    }
    DEBUG1("detach fail");
    return FALSE;

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
LThread::GetThreadIndex() {
    return m_byThreadIndex;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
thread_t
LThread::GetThreadID() {
    return m_threadID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
LThread::SetTimeOut(
    u32_t dwTimeout
) {
    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
LThread::CancelTimeout() {
    return TRUE;
}
