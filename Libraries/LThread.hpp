#ifndef LTHREAD_HPP_
#define LTHREAD_HPP_

#include <semaphore.h>
#include "Locker.hpp"
#include "Functor.hpp"
#include "typedefs.h"

typedef Functor1Ret_t(void_p, void_p) threadFunctor_t;
typedef threadFunctor_t* threadFunctor_p;
typedef pthread_t thread_t;
typedef thread_t* thread_p;

class LThread {
private:
    static u8_t m_sbyIndexThread;
    u8_t m_byThreadIndex;
    bool_t m_boIsJoinale;
    thread_t m_threadID;
    locker_p m_pThreadLocker;
    void_p m_pArguments;
    threadFunctor_p m_pThreadFunctor;
    static void_p Execute(void_p pArguments);
public:
    LThread(void_p pArguments = NULL);
    ~LThread();

    bool_t Start();
    bool_t Stop();
    void_t Process(void_p pArguments);
    bool_t RegThreadFunctor(threadFunctor_p pThreadFunctor);

    bool_t SetLocker(locker_p pLocKer);

    bool_t Join();
    bool_t Detach();

    u8_t     GetThreadIndex();
    thread_t GetThreadID();

    bool_t SetTimeOut(u32_t dwTimeout);
    bool_t CancelTimeout();
};

typedef LThread  LThread_t;
typedef LThread* LThread_p;

#endif /* !LTHREAD_HPP_ */
