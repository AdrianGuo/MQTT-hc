#ifndef LOCKER_HPP_
#define LOCKER_HPP_

#include <pthread.h>
#include "typedefs.h"

typedef pthread_mutex_t locker_t;
typedef locker_t*       locker_p;

class Locker {
protected:
    bool_t m_boIsLocked;
    i32_t  m_iRefCount;
    locker_p m_pLocker;
public:
    Locker();
    ~Locker();

    bool_t   Lock();
    bool_t   UnLock();
    bool_t   TryLock();
    bool_t   IsLocked();
    void_t   DelLocker();
    locker_p GetLocker();
};

typedef Locker  Locker_t;
typedef Locker* Locker_p;

#endif /* !LOCKER_HPP_ */
