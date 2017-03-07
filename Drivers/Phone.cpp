/*
 * Phone.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: hoang
 */

#include <stddef.h>
#include <unistd.h>
#include <dirent.h>

#include "LogPlus.hpp"
#include "Phone.hpp"

#define DO_WORK_INTERVAL    (2)
const String DEFAULT_SMS    = "Canh bao Data center";

Phone* Phone::s_pInstance = NULL;

PhoneWork::PhoneWork(
    PhoneWork::Type ePhoneWorkType,
    String strPhoneNumber,
    String strText
) {
    m_eType = ePhoneWorkType;
    m_strPhoneNumber = strPhoneNumber;
    m_strText = strText;
}

PhoneWork::~PhoneWork() {
}

PhoneWork::Type
PhoneWork::GetType() {
    return m_eType;
}

String
PhoneWork::GetPhoneNumber() {
    return m_strPhoneNumber;
}

String
PhoneWork::GetText() {
    return m_strText;
}


Phone*
Phone::getInstant() {
    if (s_pInstance == NULL) {
        s_pInstance = new Phone();
    }
    return s_pInstance;
}

Phone::Phone() {
    m_pLocker = new Locker();

    m_pDoWorkThread = new LThread();
    m_DoWorkFunctor = makeFunctor((threadFunctor_p)NULL, *this, &Phone::DoWorkFunc);
    m_pDoWorkThread->RegThreadFunctor(&m_DoWorkFunctor);
    m_pDoWorkThread->Start();
}

Phone::~Phone() {
    if (m_pLocker != NULL) {
        delete(m_pLocker);
        m_pLocker = NULL;
    }
}

bool_t
Phone::SendSms(
    String strPhoneNumber,
    String strText
) {
    String strGammuCmd = String("gammu sendsms TEXT ") + strPhoneNumber + String(" -text ") + strText;
    LOG_INFO("Send gammu command: %s", strGammuCmd.c_str());
    system(strGammuCmd.c_str());

    return TRUE;
}

bool_t
Phone::MakeCall(
    String strPhoneNumber
) {
    String strGammuCmd = String("gammu dialvoice ") + strPhoneNumber;
    LOG_INFO("Send gammu command: %s", strGammuCmd.c_str());
    system(strGammuCmd.c_str());
    sleep(30);

    //Cancel call
    strGammuCmd = String("gammu cancelcall");
    LOG_INFO("Send gammu command: %s", strGammuCmd.c_str());
    system(strGammuCmd.c_str());

    return TRUE;
}

bool_t
Phone::AddWork(
    String strWork
) {
    PhoneWork::Type eType;
    String strPhoneNumber, strText;
    size_t index, index2;
    PhoneWork_p pPhonework;

    // If not have phone number -> wrong command
    index = strWork.find("_");
    if ((index == String::npos) || (index == strWork.length() - 1 ))
        return FALSE;

    if (strWork.find("Sms") == 0) {
        eType = PhoneWork::Type::Sms;
        index2 = strWork.find("_", index + 1);
        if (index2 != String::npos) {
            strPhoneNumber = strWork.substr(index + 1, index2 - index - 1);
            strText = (index2 < strWork.length() - 1) ? strWork.substr(index2 + 1) : DEFAULT_SMS;
        } else {
            strPhoneNumber = strWork.substr(index + 1);
            strText = DEFAULT_SMS;
        }
    } else if (strWork.find("Call") == 0) {
        eType = PhoneWork::Type::Call;
        strPhoneNumber = strWork.substr(index + 1);
    } else {
        return FALSE;
    }

    m_pLocker->Lock();
    pPhonework = new PhoneWork(eType, strPhoneNumber, strText);
    m_quePhoneWork.push(pPhonework);
    m_pLocker->UnLock();
    return TRUE;
}

void_p
Phone::DoWorkFunc(
    void_p pbyBuffer
) {
    while (1) {
        m_pLocker->Lock();
        PhoneWork_p pPhoneWork = NULL;
        if (!m_quePhoneWork.empty()) {
            pPhoneWork = m_quePhoneWork.front();
            m_pLocker->UnLock();
            if (pPhoneWork->GetType() == PhoneWork::Type::Sms) {
                SendSms(pPhoneWork->GetPhoneNumber(), pPhoneWork->GetText());
            } else {
                MakeCall(pPhoneWork->GetPhoneNumber());
            }
            m_pLocker->UnLock();
            m_quePhoneWork.pop();
            if (pPhoneWork != NULL) {
                delete(pPhoneWork);
                pPhoneWork = NULL;
            }
        }
        m_pLocker->UnLock();
        usleep(50000);
    }
    m_pLocker->UnLock();

    LOG_INFO("thread exit");
    pthread_exit(NULL);
    return NULL;
}
