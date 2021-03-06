/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name:        main.cpp
 *
 * Description:      Include file for application
 *
 * Author:           TrungTQ (trungkstn@gmail.com)
 *
 * Last Changed By:  TrungTQ
 * Revision:         Revision: 1.0
 * Last Changed:     Date: 2016-08-02 12:20:00 (Tue, 02 Aug 2016)
 *
 * Note:
 *
 ******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <unistd.h>
#include <string.h>


#include "Controller/File/Version.hpp"
#include "ZbCtrller.hpp"
#include "HcCtrller.hpp"
#include "LogPlus.hpp"
#include "SMQTT.hpp"
#include "RTimer.hpp"
#include "IO.hpp"
#include "ZbBasicCmd.hpp"

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
int main(int argc, char* argv[]) {
    Log::Create("log.txt", TRUE, TRUE, Log::eInfo, Log::eAll);
    Log::Start();
    LOG_DEBUG("start log");
    LOG_INFO("App version %s", VERSION.c_str());

    if (argc < 6) {
        LOG_ERROR("Usage %s <<number>>", argv[0]);
        return (-1);
    }

    RTimer_p pTimer = RTimer::getTimerInstance();

    SMQTT_p pSMQTT = SMQTT::GetInstance(argv[1], atoi(argv[2]), String(argv[3]),
            String(argv[4]), String(argv[5]));
    if (pSMQTT->Connect()) {
        pSMQTT->Start();
    } else {
        LOG_WARN("SMQTT connect failed");
    }

    ZbCtrller_p pZbController = new ZbCtrller(argv[6]);

    pZbController->Connect();
    pZbController->Start();
    pZbController->Init();

    sleep(1);
//    LOG_WARN("Allow to join network!");
//    ZbBasicCmd::GetInstance()->JoinNwkAllow((u8_t) 0xFF);
    LOG_WARN("Disallow to join network!");
    ZbBasicCmd::GetInstance()->JoinNwkAllow((u8_t) 0x00);

    IO_Init();

    while(1) {
        pTimer->Process();
        usleep(50000);
    }
    return 0;
}
