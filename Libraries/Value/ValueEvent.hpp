#ifndef VALUE_EVENT_HPP_
#define VALUE_EVENT_HPP_

#include "typedefs.h"
#include "Event.hpp"
#include "Value.hpp"

class ValueEvent : public Value {
private:
    Event_p m_pEvent;
public:
    ValueEvent();
    virtual ~ValueEvent();

    Event_p GetValue();
    void_t SetValue(Event_p pEvent);

    Event& operator* () const { return *m_pEvent; }
    Event* operator->() const { return m_pEvent; }
};

typedef ValueEvent  ValueEvent_t;
typedef ValueEvent* ValueEvent_p;

#endif /* !VALUE_PEVENT_HPP_ */
