#ifndef EVENT_HPP_
#define EVENT_HPP_

#include "typedefs.h"
#include "Locker.hpp"

#define WAIT_INFINITE           (-1)
#define WAIT_IMMEDIATE          (0)

typedef pthread_cond_t event_t;
typedef event_t* event_p;

class Event : public Locker {
private:
    event_p m_pEvent;
    u8_t m_byWaitingThreads;
    bool_t m_boIsSignaled;
    bool_t m_boManualReset;
public:
    Event();
    virtual ~Event();
    void_t Set();
    void_t Reset();
    bool_t Wait(i32_t idwMsecTimeout);
    bool_t IsSignalled();
};

typedef Event  Event_t;
typedef Event* Event_p;

#endif // !EVENT_HPP_
