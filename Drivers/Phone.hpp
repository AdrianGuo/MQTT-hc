/*
 * Phone.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: hoang
 */

#ifndef DRIVERS_PHONE_HPP_
#define DRIVERS_PHONE_HPP_

#include "Typedefs.h"
#include "Queue.hpp"
#include "String.hpp"
#include "Locker.hpp"
#include "LThread.hpp"
#include "RTimer.hpp"
#include "Vector.hpp"

class PhoneWork {
public:
    typedef enum {
        Sms,
        Call,
    } Type;

    String GetPhoneNumber();
    String GetText();
    Type GetType();

    PhoneWork(Type, String, String strText = "");
    virtual ~PhoneWork();

private:
    String m_strPhoneNumber;
    String m_strText;
    Type m_eType;
};

typedef PhoneWork   PhoneWork_t;
typedef PhoneWork*  PhoneWork_p;

class Phone {
private:
    static Phone* s_pInstance;
    Phone();

    Queue<PhoneWork_p> m_quePhoneWork;
    Locker_p m_pLocker;

    bool_t SendSms(String, String);
    bool_t MakeCall(String);

    void_p DoWorkFunc(void_p);
    LThread_p m_pDoWorkThread;
    threadFunctor_t m_DoWorkFunctor;

public:
    static Phone* getInstant();
    virtual ~Phone();

    bool_t AddWork(String);
};

typedef Phone   Phone_t;
typedef Phone*  Phone_p;

#endif /* DRIVERS_PHONE_HPP_ */
