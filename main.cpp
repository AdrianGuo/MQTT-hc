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
#include <RTimer.hpp>
#include <unistd.h>
#include <string.h>

#include "debug.hpp"

#include "ZbCtrller.hpp"
#include "NetCtrller.hpp"
#include "HCCtrller.hpp"

#include "Functor.hpp"

#include "ClientSock.hpp"
#include "SClient.hpp"

#include "Serial.hpp"
#include "SZbSerial.hpp"

#include "ZbDriver.hpp"
#include "ZbBasicCmd.hpp"
#include "ZbZclGlobalCmd.hpp"

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                          */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static HCCtrller_p      pHcController       = NULL;
static ZbCtrller_p      pZbController       = NULL;
static NetCtrller_p     pNetController      = NULL;

static ClientSock_p     pClientSocket       = NULL;
static SClient_p        pSessionClient      = NULL;

static Serial_p         pZbSerial           = NULL;
static SZbSerial_p      pSessionZbSerial    = NULL;
static ZbDriver_p       pZbDriver           = NULL;
static ZbBasicCmd_p     pZbBasicCmd           = NULL;

static RTimer_p         pSystemTimer        = NULL;

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
void_t InitZbDriver(const_char_p portname);
void_t InitNetwrk(const_char_p ipname, int_t ipport);
void_t InitController(const_char_p macID);
void_t InitSysTimer();
void_t DesTroyObjects();

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
int main(int argc, char* argv[]) {
    DEBUG1("main");

    if (argc < 5) {
        DEBUG2("Usage %s <<number>>", argv[0]);
        return (-1);
    }

    String ipname(argv[1]);
    String portname(argv[3]);
    String macID(argv[4]);
    int_t ipport = atoi(argv[2]);

    InitZbDriver(portname.c_str());
    InitNetwrk(ipname.c_str(), ipport);
    InitController(macID.c_str());
    InitSysTimer();

    if (pZbSerial->Connect()) {
        DEBUG1("zb-com connected");
    } else {
        DEBUG1("zb-com failed");
    }

    pZbController->Start();
    pZbController->IniZbCtrller();

    pNetController->InitProcess();
    pSessionClient->Connect();
    pSessionClient->Start();
    pNetController->Start();

    pZbBasicCmd->NwkInfoReq();

    while (TRUE) {
        if (pHcController != NULL) { pHcController->Process(); }
//        if (pSystemTimer != NULL) { pSystemTimer->Process(); }
    }

    DesTroyObjects();

    return 0;
}


/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DesTroyObjects() {
    DEBUG1("DESTROY OBJECTS");

    if (pHcController != NULL) {
        delete pHcController;
        pHcController = NULL;
    }

    if (pSystemTimer != NULL) {
        delete pSystemTimer;
        pSystemTimer = NULL;
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t InitZbDriver(const_char_p portname) {
    pZbSerial = new Serial(portname, BAUD192);
    pSessionZbSerial = new SZbSerial(pZbSerial);
    pZbDriver = ZbDriver::GetInstance(pSessionZbSerial);
    pZbBasicCmd = ZbBasicCmd::GetInstance();
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t InitNetwrk(const_char_p ipname, int_t ipport) {
    pClientSocket = new ClientSock(ipname, ipport);
    pSessionClient = new SClient(pClientSocket);
    pClientSocket->SetNonBlocking();
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t InitController(const_char_p macID) {
    pHcController = new HCCtrller(pSessionClient);
    pZbController = new ZbCtrller(pZbDriver);
    pNetController = new NetCtrller(pSessionClient, macID);

    pHcController->AddCtrller(pZbController);
    pHcController->AddCtrller(pNetController);
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t InitSysTimer() {
    pSystemTimer = RTimer::getTimerInstance();
}
