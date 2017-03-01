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
    LOG_DEBUG("start log");

    if (argc < 7) {
        LOG_ERROR("Usage %s <<number>>", argv[0]);
        return (-1);
    }

    RTimer_p pTimer = RTimer::getTimerInstance();

    SMQTT_p pSMQTT = SMQTT::GetInstance(argv[1], atoi(argv[2]), String(argv[3]),
            String(argv[4]), String(argv[5]), String(argv[6]));
    pSMQTT->Connect();
    pSMQTT->Start();

#ifdef MT7688
    ZbCtrller_p pZbController = new ZbCtrller(argv[7]);

    pZbController->Connect();
    pZbController->Start();
    pZbController->Init();
#endif //MT7688
//
    sleep(1);
//    ZbBasicCmd::GetInstance()->JoinNwkAllow(0xFF);

//    pSMQTT->Publish("start", 123456);
//    pSMQTT->Subscribe();

    IO_Init();

    while(1) {
        pTimer->Process();
//      pSMQTT->Publish("test", -1);
//      sleep(1);
    }
    return 0;
}
