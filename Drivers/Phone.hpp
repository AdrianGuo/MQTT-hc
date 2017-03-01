/*
 * Phone.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: hoang
 */

#ifndef DRIVERS_PHONE_HPP_
#define DRIVERS_PHONE_HPP_

#include "typedefs.h"
#include "Queue.hpp"
#include "String.hpp"
#include "Locker.hpp"
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
    String m_strDev;
    Vector<String> m_vecLstDev;

    static Phone* s_pInstance;
    Phone(String strDev = std::string("/dev/ttyUSB0"));

    Queue<PhoneWork_p> m_quePhoneWork;
    Locker_p m_pLocker;

    RTimer_p m_pTimer;
    int_t m_iDoWork;
    TimerFunctor_t m_DoWorkFunctor;
    void_t DoWorkFunc(void_p);

public:
    static Phone* getInstant(String strDev = std::string("/dev/ttyUSB0"));
    virtual ~Phone();

    bool_t SendSms(String, String);
    bool_t MakeCall(String);

    bool_t AddWork(PhoneWork::Type, String, String strText = "");

    void_t UpdateTtyDev();
};

typedef Phone   Phone_t;
typedef Phone*  Phone_p;

#endif /* DRIVERS_PHONE_HPP_ */
