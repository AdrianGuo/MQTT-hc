/*
 * Phone.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: hoang
 */

#include <stddef.h>
#include <unistd.h>
#include <dirent.h>
#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <fstream>

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

    IsUpdatedConfig = FALSE;
    UpdateGammuConfig();
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
    String strRet;
    String strGammuCmd = String("gammu sendsms TEXT ") + strPhoneNumber + String(" -text \"") + strText + String("\"");
    LOG_INFO("Send gammu command: %s", strGammuCmd.c_str());
//    system(strGammuCmd.c_str());
    strRet = ExecCommand(strGammuCmd.c_str());
    if ((strRet.find("No response in specified timeout") != String::npos) || (strRet.find("Error opening device, it doesn't exist") != String::npos)) {
        LOG_INFO("failed to send sms");
        IsUpdatedConfig = FALSE;
        UpdateGammuConfig();
        //Retry once more time
        strRet = ExecCommand(strGammuCmd.c_str());
        if ((strRet.find("No response in specified timeout") != String::npos) || (strRet.find("Error opening device, it doesn't exist") != String::npos)) {
            LOG_INFO("retry send sms: failed to send sms");
        }
    }

    return TRUE;
}

bool_t
Phone::MakeCall(
    String strPhoneNumber
) {
    String strRet;
    bool_t boRet = TRUE;
    String strGammuCmd = String("gammu dialvoice \"") + strPhoneNumber  + String("\"");
    LOG_INFO("Send gammu command: %s", strGammuCmd.c_str());
//    system(strGammuCmd.c_str());
    strRet = ExecCommand(strGammuCmd.c_str());
    if ((strRet.find("No response in specified timeout") != String::npos) || (strRet.find("Error opening device, it doesn't exist") != String::npos)) {
        LOG_INFO("failed to make call");
        IsUpdatedConfig = FALSE;
        UpdateGammuConfig();
        //Retry once more time
        strRet = ExecCommand(strGammuCmd.c_str());
        if ((strRet.find("No response in specified timeout") != String::npos) || (strRet.find("Error opening device, it doesn't exist") != String::npos)) {
            LOG_INFO("retry call: failed to make call");
            boRet = FALSE;
        }
    }

    if (boRet) {
        sleep(30);
        //Cancel call
        strGammuCmd = String("gammu cancelcall");
        LOG_INFO("Send gammu command: %s", strGammuCmd.c_str());
//        system(strGammuCmd.c_str());
        ExecCommand(strGammuCmd.c_str());
    }

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
    while (TRUE) {
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

String
Phone::ExecCommand(
    const char* cmd
) {
    std::array<char, 128> buffer;
    String result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) LOG_ERROR("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
            result += buffer.data();
    }
    return result;
}

void_t
Phone::UpdateGammuConfig() {
    bool_t boRet = FALSE;
    String strRet;
    DIR *dpdf;
    struct dirent *epdf;
    Vector<String> vecLstDev;
    LOG_INFO("Start update gammu config");

    //Find /dev/ttyUSB add to vecLstDev
    dpdf = opendir("/dev");
    if (dpdf != NULL){
        while ((epdf = readdir(dpdf)) != NULL) {
            if (std::string(epdf->d_name).find("ttyUSB") != std::string::npos) {
                vecLstDev.insert(vecLstDev.begin(), std::string(std::string("/dev/") + epdf->d_name));
            }
        }
        closedir(dpdf);
    }

    for(Vector<String>::iterator it = vecLstDev.begin(); it != vecLstDev.end(); ++it) {
        //Create gammu configuration file
        std::ofstream newFile("/etc/gammurc");
        if(!newFile.is_open()) {
            LOG_WARN("Can not create configure file");
            return;
        }
        newFile << "[gammu]\n\n";
        newFile << "port = " + (*it) + "\n";
        newFile << "model =\n";
        newFile << "connection = at115200\n";
        newFile << "synchronizetime = yes\n";
        newFile << "logfile =\n";
        newFile << "logformat = nothing\n";
        newFile << "use_locking =\n";
        newFile << "gammuloc =\n";
        newFile.close();

        //Check if current configuration file ok
        strRet = ExecCommand("gammu networkinfo");
        if ((strRet.find("No response in specified timeout") != String::npos) || (strRet.find("Error opening device, it doesn't exist") != String::npos)) {
            LOG_INFO("failed to use %s", (*it).c_str());
        } else {
            LOG_INFO("Configure gammu to use %s", (*it).c_str());
            boRet = TRUE;
            break;
        }
    }
    if (!boRet) {
        LOG_WARN("Can not found USB modem");
    }
    //Cleanup
    vecLstDev.erase(vecLstDev.begin(), vecLstDev.end());
    IsUpdatedConfig = TRUE;
}
