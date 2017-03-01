/*
 * Phone.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: hoang
 */

#include <stddef.h>
#include <dirent.h>

#include "LogPlus.hpp"
#include "Phone.hpp"

#define DO_WORK_INTERVAL    (2)

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
Phone::getInstant(
    String strDev
) {
    if (s_pInstance == NULL) {
        s_pInstance = new Phone(strDev);
    }
    return s_pInstance;
}

Phone::Phone(
    String strDev
) {
    LOG_DEBUG(" %s: \"%s\"", __FUNCTION__, strDev.c_str());
    m_strDev = strDev;

    m_pLocker = new Locker();

    m_pTimer = RTimer::getTimerInstance();
    m_DoWorkFunctor = makeFunctor((TimerFunctor_p)NULL, *this, &Phone::DoWorkFunc);
    m_iDoWork = m_pTimer->StartTimer(RTimer::Forever, DO_WORK_INTERVAL, &m_DoWorkFunctor, NULL);
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
    LOG_DEBUG(" %s: \"%s\", \"%s\"", __FUNCTION__, strPhoneNumber.c_str(), strText.c_str());
    return TRUE;
}

bool_t
Phone::MakeCall(
    String strPhoneNumber
) {
    String strDev;
    for(Vector<String>::iterator it = m_vecLstDev.begin(); it != m_vecLstDev.end(); ++it) {
        strDev = *it;
        String strATCall = R"(echo -e "ATD)" + strPhoneNumber + R"(;" > )" + strDev;
        LOG_DEBUG("Send AT command: %s", strATCall.c_str());
        system(strATCall.c_str());
    }

    return TRUE;
}

bool_t
Phone::AddWork(
    PhoneWork::Type eType,
    String strPhoneNumber,
    String strText
) {
    m_pLocker->Lock();
    PhoneWork_p pPhonework = new PhoneWork(eType, strPhoneNumber, strText);
    m_quePhoneWork.push(pPhonework);
    m_pLocker->UnLock();
    return TRUE;
}

void_t
Phone::DoWorkFunc(
    void_p pbyBuffer
) {
    m_pLocker->Lock();
    PhoneWork_p pPhoneWork = NULL;
    if (!m_quePhoneWork.empty()) {
        UpdateTtyDev();
        pPhoneWork = m_quePhoneWork.front();
        if (pPhoneWork->GetType() == PhoneWork::Type::Sms) {
            SendSms(pPhoneWork->GetPhoneNumber(), pPhoneWork->GetText());
        } else {
            MakeCall(pPhoneWork->GetPhoneNumber());
        }
        m_quePhoneWork.pop();
        if (pPhoneWork != NULL) {
            delete(pPhoneWork);
            pPhoneWork = NULL;
        }
    }
    m_pLocker->UnLock();
}

void_t
Phone::UpdateTtyDev() {
    DIR *dpdf;
    struct dirent *epdf;

    m_vecLstDev.erase(m_vecLstDev.begin(), m_vecLstDev.end());
    dpdf = opendir("/dev");
    if (dpdf != NULL){
        while (epdf = readdir(dpdf)){
            if (std::string(epdf->d_name).find("ttyUSB") != std::string::npos) {
//                LOG_DEBUG("Find ttyUSB : %s",epdf->d_name);
                m_vecLstDev.push_back(std::string(std::string("/dev/") + epdf->d_name));
            }
        }
        closedir(dpdf);
    }
}
