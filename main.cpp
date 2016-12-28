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

#include "ZwCtrller.hpp"
#include "ZbCtrller.hpp"
#include "HcCtrller.hpp"
#include "LogPlus.hpp"
//#ifdef MT7688
#include "LED.hpp"
//#endif

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                          */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static HcCtrller_p  pHcController       = NULL;
static ZwCtrller_p  pZwController       = NULL;
static ZbCtrller_p  pZbController       = NULL;
static RuleCtrller_p pRuleController    = NULL;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void_t
InitController(
    const_char_p ipname,
    const_char_p macId,
    const_char_p ipport,
    const_char_p zwcom,
    const_char_p zbcom
);

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
int main(int argc, char* argv[]) {
    if (argc < 5) {
        LOG_ERROR("Usage %s <<number>>", argv[0]);
        return (-1);
    }

//#ifdef MT7688
    LED ledNETN(18);
    ledNETN.On();

    LED ledNETE(19);
    ledNETE.Off();
//#endif

    Log::Create("log.txt", TRUE, TRUE, Log::eInfo, Log::eAll);
    LOG_DEBUG("start log");

    String zwcom, zbcom;
    String ipname (argv[1]);
    String ipport (argv[2]);
    String macId  (argv[3]);

    if (argc == 5) {
        String com1 (argv[4]);
        if (com1.find("zw=") != String::npos) {
            zwcom = com1.substr(3, com1.length());
        } else if (com1.find("zb=") != String::npos) {
            zbcom = com1.substr(3, com1.length());
        }
    } else if (argc == 6) {
        zwcom = argv[4];
        zbcom = argv[5];
        zwcom = zwcom.substr(3, zwcom.length());
        zbcom = zbcom.substr(3, zbcom.length());
    }

    InitController(ipname.c_str(),
                   macId.c_str(),
                   ipport.c_str(),
                   zwcom.c_str(),
                   zbcom.c_str());

    if (pHcController != NULL) {
//        pHcController->Debug();
        pHcController->Connect();
//        pHcController->Serve();
    }

    if (pZwController != NULL) {
        pZwController->Connect();
        pZwController->Start();
        pZwController->Init();
    }

    if (pZbController != NULL) {
        pZbController->Connect();
        pZbController->Start();
        pZbController->Init();
    }

    if (pRuleController != NULL) {
        pRuleController->Start();
    }

    while (TRUE) {
        if (pHcController != NULL) {
            pHcController->Process();
        }
    }

    return 0;
}

/**
 * @func   InitController
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
InitController(
    const_char_p ipname,
    const_char_p macId,
    const_char_p ipport,
    const_char_p zwcom,
    const_char_p zbcom
) {
    int_t port = std::stoi(ipport);
    pHcController = new HcCtrller(ipname, port, macId);

    if (strcmp(zwcom, "") != 0) {
        LOG_INFO("init zwave module");
        pZwController = new ZwCtrller(zwcom);
        pHcController->AddZwCtrller(pZwController);
    }

    if (strcmp(zbcom, "") != 0) {
        LOG_INFO("init zigbee module");
        pZbController = new ZbCtrller(zbcom);
        pHcController->AddZbCtrller(pZbController);
    }

    pRuleController = new RuleCtrller();
    pHcController->AddRuleCtrller(pRuleController);
}
