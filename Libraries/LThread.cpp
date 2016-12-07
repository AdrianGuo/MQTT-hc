#include <stdlib.h>
#include <pthread.h>
#include "debug.hpp"
#include "LogPlus.hpp"
#include "LThread.hpp"

#define THREAD_NULL                 (0)
#define THREAD_SUCCESS              (0)
#define THREAD_ERROR                (-1)

u8_t LThread::m_sbyIndexThread = 0;

/**
 * @func   Execute
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
LThread::Execute(
    void_p pArguments
) {
    LThread_p pThread = (LThread_p) pArguments;
//    DEBUG2("execute thread %d", pThread->GetThreadIndex());
    LOG_INFO("execute thread %d", pThread->GetThreadIndex());
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
    m_threadId = THREAD_NULL;
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
//        DEBUG2("start thread %d", m_byThreadIndex);
        LOG_INFO("start thread %d", m_byThreadIndex);
        if (pthread_create(&m_threadId, NULL, LThread::Execute, (void*) this) == THREAD_SUCCESS) {
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * @func   Stop
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
LThread::Stop() {
//    DEBUG2("stop thread %d", m_byThreadIndex);
    LOG_INFO("stop thread %d", m_byThreadIndex);
    pthread_exit(NULL);

    return TRUE;
}

/**
 * @func   Process
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LThread::Process(
    void_p pArguments
) {
    (*m_pThreadFunctor)(pArguments);
}

/**
 * @func   RegThreadFunctor
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
 * @func   SetLocker
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
 * @func   Join
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
LThread::Join() {
    int_t idwResult;

    if ((idwResult = pthread_join(m_threadId, NULL)) == THREAD_SUCCESS) {
        LOG_INFO("join success");
        m_boIsJoinale = TRUE;
        return TRUE;
    }
    LOG_INFO("join fail");
    return FALSE;
}

/**
 * @func   Detach
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
LThread::Detach() {
    int_t idwResult;

    if ((idwResult = pthread_detach(m_threadId)) == THREAD_SUCCESS) {
        LOG_INFO("detach thread %d success", m_byThreadIndex);
        m_boIsJoinale = FALSE;
        return TRUE;
    }
    LOG_INFO("detach fail");
    return FALSE;

}

/**
 * @func   GetThreadIndex
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
LThread::GetThreadIndex() {
    return m_byThreadIndex;
}

/**
 * @func   GetThreadId
 * @brief  None
 * @param  None
 * @retval None
 */
thread_t
LThread::GetThreadId() {
    return m_threadId;
}

/**
 * @func   SetTimeOut
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
 * @func   CancelTimeout
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
LThread::CancelTimeout() {
    return TRUE;
}
