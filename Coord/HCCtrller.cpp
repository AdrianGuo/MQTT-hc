/*******************************************************************************
*
* Copyright (c) 2016
* Lumi, JSC.
* All Rights Reserved
*
*
* Description:      Include file for application
*
* Author:
*
* Last Changed By:  TrungTQ
* Revision:         Revision: 1.0
* Last Changed:     Date: 2016-08-08 10:00:00 (Mon, 08 Aug 2016)
*
* Note:             
*
*******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stddef.h>
#include <unistd.h>
#include "HelperHc.hpp"
#include "LogPlus.hpp"
#include "JsonAuthReq.hpp"
#include "JsonAuthRes.hpp"
#include "JsonKaliveReq.hpp"
#include "JsonKaliveRes.hpp"

#include "JsonDevGet.hpp"
#include "JsonDevSet.hpp"
#include "JsonDevStt.hpp"
#include "JsonDevOff.hpp"
#include "JsonDevAdd.hpp"
#include "JsonDevDel.hpp"
#include "JsonDevRestart.hpp"
#include "JsonDevReset.hpp"
#include "JsonDevResetRes.hpp"
#include "JsonDevInfo.hpp"
#include "JsonDevInfoRes.hpp"
#include "JsonDevLst.hpp"
#include "JsonDevLstRes.hpp"
#include "JsonDevLstAdd.hpp"
#include "JsonDevLstAddRes.hpp"
#include "JsonDevLstDel.hpp"
#include "JsonDevLstDelRes.hpp"
#include "JsonDevSync.hpp"
#include "JsonDevSyncRes.hpp"

#include "JsonFloorAdd.hpp"
#include "JsonFloorAddRes.hpp"
#include "JsonFloorDel.hpp"
#include "JsonFloorDelRes.hpp"
#include "JsonFloorEdit.hpp"
#include "JsonFloorEditRes.hpp"
#include "JsonFloorLst.hpp"
#include "JsonFloorLstRes.hpp"
#include "JsonFloorLinkRoom.hpp"
#include "JsonFloorLinkRoomRes.hpp"
#include "JsonFloorUlinkRoom.hpp"
#include "JsonFloorUlinkRoomRes.hpp"
#include "JsonFloorSync.hpp"
#include "JsonFloorSyncRes.hpp"
#include "JsonFloorSyncRoom.hpp"
#include "JsonFloorSyncRoomRes.hpp"
#include "JsonFloorLstRoom.hpp"
#include "JsonFloorLstRoomRes.hpp"

#include "JsonRoomAdd.hpp"
#include "JsonRoomAddRes.hpp"
#include "JsonRoomDel.hpp"
#include "JsonRoomDelRes.hpp"
#include "JsonRoomEdit.hpp"
#include "JsonRoomEditRes.hpp"
#include "JsonRoomLst.hpp"
#include "JsonRoomLstRes.hpp"
#include "JsonRoomLinkDev.hpp"
#include "JsonRoomLinkDevRes.hpp"
#include "JsonRoomUlinkDev.hpp"
#include "JsonRoomUlinkDevRes.hpp"
#include "JsonRoomSync.hpp"
#include "JsonRoomSyncRes.hpp"
#include "JsonRoomSyncDev.hpp"
#include "JsonRoomSyncDevRes.hpp"
#include "JsonRoomLstDev.hpp"
#include "JsonRoomLstDevRes.hpp"

#include "HCCtrller.hpp"

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                          */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                             EXPORTED METHODS                               */
/******************************************************************************/
/**
 * @func   HCCtrller
 * @brief  None
 * @param  None
 * @retval None
 */
HCCtrller::HCCtrller(
    const_char_p ipname,
    int_t ipport,
    const_char_p cMacID
) : m_DbManager(),
    m_DevManager(),
    m_NetManager(cMacID),
    m_SessionClient (ipname, ipport),
    m_pZwCtrller (NULL),
    m_pZbCtrller (NULL),
    m_pHCCtrllerLocker (new Locker()),
    m_boIsDebug (FALSE) {
    m_pDbModelDb = DbModel::CreateModel("newhc.db");
    m_HCCtrllerFunctor =
    makeFunctor((HCCtrllerFunctor_p) NULL, *this, &HCCtrller::RecvCommandFromSession);
    m_CtrllerFunctor =
    makeFunctor((CtrllerFunctor_p) NULL, *this, &HCCtrller::RecvCommandFromModules);

    m_DbManager.SetFunctor (&m_CtrllerFunctor);
    m_NetManager.SetFunctor(&m_CtrllerFunctor);
    m_DevManager.SetFunctor(&m_CtrllerFunctor);

    m_NetManager.SetSessionClient(&m_SessionClient);
    m_SessionClient.SClientRecvFunctor(&m_HCCtrllerFunctor);
    RegisterHandler();
}

/**
 * @func   ~HCCtrller
 * @brief  None
 * @param  None
 * @retval None
 */
HCCtrller::~HCCtrller() {
    if (m_pHCCtrllerLocker != NULL) {
        delete m_pHCCtrllerLocker;
        m_pHCCtrllerLocker = NULL;
    }

    if (m_pDbModelDb != NULL) {
        delete m_pDbModelDb;
        m_pDbModelDb = NULL;
    }
}

/**
 * @func   RegisterHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HCCtrller::RegisterHandler(
    String strJsonCommand,
    HandlerJsonCmdFunctor_t funcTor
) {
    m_mapHandlerFunctor[strJsonCommand] = funcTor;
}

/**
 * @func   RegisterHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HCCtrller::RegisterHandler() {
    /* Database */
    // Floor
    RegisterHandler(JsonFloorAdd::GetStrCmd(),
    makeFunctor((HandlerDbCmdFunctor_p) NULL, m_DbManager, &DbManager::HandlerFloorCmdAdd));
    RegisterHandler(JsonFloorEdit::GetStrCmd(),
    makeFunctor((HandlerDbCmdFunctor_p) NULL, m_DbManager, &DbManager::HandlerFloorCmdEdit));
    RegisterHandler(JsonFloorDel::GetStrCmd(),
    makeFunctor((HandlerDbCmdFunctor_p) NULL, m_DbManager, &DbManager::HandlerFloorCmdDel));
    RegisterHandler(JsonFloorLinkRoom::GetStrCmd(),
    makeFunctor((HandlerDbCmdFunctor_p) NULL, m_DbManager, &DbManager::HandlerFloorCmdLinkRoom));
    RegisterHandler(JsonFloorUlinkRoom::GetStrCmd(),
    makeFunctor((HandlerDbCmdFunctor_p) NULL, m_DbManager, &DbManager::HandlerFloorCmdUlinkRoom));

    // Room
    RegisterHandler(JsonRoomAdd::GetStrCmd(),
    makeFunctor((HandlerDbCmdFunctor_p) NULL, m_DbManager, &DbManager::HandlerRoomCmdAdd));
    RegisterHandler(JsonRoomEdit::GetStrCmd(),
    makeFunctor((HandlerDbCmdFunctor_p) NULL, m_DbManager, &DbManager::HandlerRoomCmdEdit));
    RegisterHandler(JsonRoomDel::GetStrCmd(),
    makeFunctor((HandlerDbCmdFunctor_p) NULL, m_DbManager, &DbManager::HandlerRoomCmdDel));
    RegisterHandler(JsonRoomLinkDev::GetStrCmd(),
    makeFunctor((HandlerDbCmdFunctor_p) NULL, m_DbManager, &DbManager::HandlerRoomCmdLinkDev));
    RegisterHandler(JsonRoomUlinkDev::GetStrCmd(),
    makeFunctor((HandlerDbCmdFunctor_p) NULL, m_DbManager, &DbManager::HandlerRoomCmdLinkDev));

    /* Dev */
    RegisterHandler(JsonDevSet::GetStrCmd(),
    makeFunctor((HandlerDevCmdFunctor_p) NULL, m_DevManager, &DevManager::HandlerDevCmdSet));
    RegisterHandler(JsonDevGet::GetStrCmd(),
    makeFunctor((HandlerDevCmdFunctor_p) NULL, m_DevManager, &DevManager::HandlerDevCmdGet));
    RegisterHandler(JsonDevDel::GetStrCmd(),
    makeFunctor((HandlerDevCmdFunctor_p) NULL, m_DevManager, &DevManager::HandlerDevCmdDel));
    RegisterHandler(JsonDevAdd::GetStrCmd(),
    makeFunctor((HandlerDevCmdFunctor_p) NULL, m_DevManager, &DevManager::HandlerDevCmdAdd));
    RegisterHandler(JsonDevLstDel::GetStrCmd(),
    makeFunctor((HandlerDevCmdFunctor_p) NULL, m_DevManager, &DevManager::HandlerDevCmdLstDel));
    RegisterHandler(JsonDevLstAdd::GetStrCmd(),
    makeFunctor((HandlerDevCmdFunctor_p) NULL, m_DevManager, &DevManager::HandlerDevCmdLstAdd));
    RegisterHandler(JsonDevStt::GetStrCmd(),
    makeFunctor((HandlerDevCmdFunctor_p) NULL, m_DevManager, &DevManager::HandlerDevCmdStt));
    RegisterHandler(JsonDevReset::GetStrCmd(),
    makeFunctor((HandlerDevCmdFunctor_p) NULL, m_DevManager, &DevManager::HandlerDevCmdReset));
    RegisterHandler(JsonDevInfo::GetStrCmd(),
    makeFunctor((HandlerDevCmdFunctor_p) NULL, m_DevManager, &DevManager::HandlerDevCmdInfo));
    RegisterHandler(JsonDevLstRes::GetStrCmd(),
    makeFunctor((HandlerDevCmdFunctor_p) NULL, m_DevManager, &DevManager::HandlerDevCmdLstRes));
    RegisterHandler(JsonDevLstAddRes::GetStrCmd(),
    makeFunctor((HandlerDevCmdFunctor_p) NULL, m_DevManager, &DevManager::HandlerDevCmdLstAddRes));
    RegisterHandler(JsonDevLstDelRes::GetStrCmd(),
    makeFunctor((HandlerDevCmdFunctor_p) NULL, m_DevManager, &DevManager::HandlerDevCmdLstDelRes));
    RegisterHandler(JsonDevSyncRes::GetStrCmd(),
    makeFunctor((HandlerDevCmdFunctor_p) NULL, m_DevManager, &DevManager::HandlerDevCmdSyncRes));

    /* Network */
    RegisterHandler(JsonAuthRes::GetStrCmd(),
    makeFunctor((HandlerNetCmdFunctor_p) NULL, m_NetManager, &NetManager::HandlerNetCmdAuthRes));
    RegisterHandler(JsonKaliveRes::GetStrCmd(),
    makeFunctor((HandlerNetCmdFunctor_p) NULL, m_NetManager, &NetManager::HandlerNetCmdKaliveRes));

}

/**
 * @func   ProcessHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HCCtrller::ProcessHandler(
    JsonCommand_p pJsonCommand
) {
    String strJsonCommandName = pJsonCommand->GetFullCommand();
    MapHandlerFunctor::const_iterator_t it =
            m_mapHandlerFunctor.find(strJsonCommandName);
    if (it != m_mapHandlerFunctor.end()) {
        m_mapHandlerFunctor[strJsonCommandName](pJsonCommand);
    }
}

/**
 * @func   Debug
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HCCtrller::Debug() {
    m_boIsDebug = TRUE;
    m_SessionClient.Connect();
    m_SessionClient.Start();
}

/**
 * @func   Connect
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HCCtrller::Connect() {
    m_boIsDebug = FALSE;
    m_NetManager.Connect();
}

/**
 * @func   AddZwCtrller
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HCCtrller::AddZwCtrller(
    ZwCtrller_p pZwCtrller
) {
    m_pZwCtrller = pZwCtrller;
    m_pZwCtrller->CtrllerRecvFunctor(&m_CtrllerFunctor);
}

/**
 * @func   AddZbCtrller
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HCCtrller::AddZbCtrller(
    ZbCtrller_p pZbCtrller
) {
    m_pZbCtrller = pZbCtrller;
    m_pZbCtrller->CtrllerRecvFunctor(&m_CtrllerFunctor);
}

/**
 * @func   RecvCommandFromSession
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
HCCtrller::RecvCommandFromSession(
    JsonCommand_p pJsonCommand
) {
    m_pHCCtrllerLocker->Lock();
    if (pJsonCommand->IsJsonAvailable()) {
        LOGCOMMAND(Log::Level::eDebug, pJsonCommand);
        m_queJsonCommand.push(pJsonCommand);
        m_pHCCtrllerLocker->UnLock();
        return TRUE;
    }
    m_pHCCtrllerLocker->UnLock();
    return FALSE;
}

/**
 * @func   RecvCommandFromModules
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
HCCtrller::RecvCommandFromModules(
    JsonCommand_p pJsonCommand
) {
    m_pHCCtrllerLocker->Lock();
    if (pJsonCommand->IsJsonAvailable()) {
        LOGCOMMAND(Log::Level::eDebug, pJsonCommand);
        m_queJsonCommand.push(pJsonCommand);
        m_pHCCtrllerLocker->UnLock();
        return TRUE;
    }
    m_pHCCtrllerLocker->UnLock();
    return FALSE;
}

/**
 * @func   Process
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HCCtrller::Process() {
    JsonCommand_p pJsonCommand = NULL;

    m_pHCCtrllerLocker->Lock();
    if (!m_queJsonCommand.empty()) {
        pJsonCommand = m_queJsonCommand.front();
        m_queJsonCommand.pop();
    }
    m_pHCCtrllerLocker->UnLock();

    if (pJsonCommand != NULL) {
        if (pJsonCommand->GetDesFlag() == JsonCommand::Flag::NetWork) {
            if (m_NetManager.IsAuthenticated() && !m_boIsDebug) {
                m_SessionClient.JsCommandToPacket(pJsonCommand);
            }
            if (m_boIsDebug) {
                m_SessionClient.JsCommandToPacket(pJsonCommand);
            }
        } else if (pJsonCommand->GetDesFlag() == JsonCommand::Flag::Zwave) {
            if (m_pZwCtrller != NULL) {
                m_pZwCtrller->ProcessHandler(pJsonCommand);
            } else {
                delete pJsonCommand;
                pJsonCommand = NULL;
            }
        } else if (pJsonCommand->GetDesFlag() == JsonCommand::Flag::Zigbee) {
            if (m_pZbCtrller != NULL) {
                m_pZbCtrller->ProcessHandler(pJsonCommand);
            } else {
                delete pJsonCommand;
                pJsonCommand = NULL;
            }
        } else {
            ProcessHandler(pJsonCommand);
        }
    }

    m_NetManager.Process();
    usleep(50);
}

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
