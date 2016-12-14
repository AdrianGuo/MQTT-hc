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

#include "typedefs.h"
#include "Map.hpp"
#include "String.hpp"

#include "JsonDevGet.hpp"
#include "JsonDevSet.hpp"
#include "JsonDevStt.hpp"
#include "JsonDevOff.hpp"
#include "JsonDevAdd.hpp"
#include "JsonDevDel.hpp"
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

#include "DbManager.hpp"

#define SPACE               (' ')
#define AT                  ('@')
/**
 * @func  DbManager
 * @brief  None
 * @param  None
 * @retval None
 */
DbManager::DbManager() {
    m_pCtrllerFunctor = NULL;
    m_pDbModelDb = DbModel::CreateModel("newhc.db");
    m_pJsonRecvDbSession = JsonRecvDbSession::CreateSession();
    m_pJsonSendDbSession = JsonSendDbSession::CreateSession();
//    LoadDatabase();
    RegisterDbSession();
}

/**
 * @func   ~DbManager
 * @brief  None
 * @param  None
 * @retval None
 */
DbManager::~DbManager() {
    if (m_pJsonRecvDbSession != NULL) {
        delete m_pJsonRecvDbSession;
        m_pJsonRecvDbSession = NULL;
    }
    if (m_pJsonSendDbSession != NULL) {
        delete m_pJsonSendDbSession;
        m_pJsonSendDbSession = NULL;
    }

    m_Rooms.clear();
    m_Floors.clear();
}

/**
 * @func   SetFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
DbManager::SetFunctor(
    CtrllerFunctor_p pRecvFunctor
) {
    if (pRecvFunctor != NULL) {
        m_pCtrllerFunctor = pRecvFunctor;
        return TRUE;
    }
    return FALSE;
}

/**
 * @func   PushJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::PushJsonCommand(
    JsonCommand_p pJsonCommand
) {
    if ((m_pCtrllerFunctor != NULL) && (pJsonCommand != NULL)) {
        (*m_pCtrllerFunctor)(pJsonCommand);
    }
}


/**
 * @func   Debug
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::Debug() {
    if (m_pDbModelDb != NULL) {
        DbRoom room1 = m_pDbModelDb->Find<DbRoomItem>().Where("roomid = ?").Bind(1);
        room1.Modify()->FloorId = 1;
        m_pDbModelDb->Add(room1);
        m_pDbModelDb->UpdateChanges();
    }
}

/**
 * @func   RegisterDbSession
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::RegisterDbSession() {
    // Reciver
    // Room
    m_pJsonRecvDbSession->MapJsonMessage<JsonRoomAdd>(JsonRoomAdd::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonRoomDel>(JsonRoomDel::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonRoomEdit>(JsonRoomEdit::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonRoomLinkDev>(JsonRoomLinkDev::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonRoomUlinkDev>(JsonRoomUlinkDev::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonRoomLstRes>(JsonRoomLstRes::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonRoomLstDevRes>(JsonRoomLstDevRes::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonRoomSync>(JsonRoomSync::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonRoomSyncDev>(JsonRoomSyncDev::GetStrCmd());
    // Floor
    m_pJsonRecvDbSession->MapJsonMessage<JsonFloorAdd>(JsonFloorAdd::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonFloorDel>(JsonFloorDel::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonFloorEdit>(JsonFloorEdit::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonFloorLinkRoom>(JsonFloorLinkRoom::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonFloorUlinkRoom>(JsonFloorUlinkRoom::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonFloorLstRes>(JsonFloorLstRes::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonFloorLstRoomRes>(JsonFloorLstRoomRes::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonFloorSync>(JsonFloorSync::GetStrCmd());
    m_pJsonRecvDbSession->MapJsonMessage<JsonFloorSyncRoom>(JsonFloorSyncRoom::GetStrCmd());

    // Send
    // Room
    m_pJsonSendDbSession->MapJsonMessage<JsonRoomAddRes>(JsonRoomAddRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonRoomDelRes>(JsonRoomDelRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonRoomEditRes>(JsonRoomEditRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonRoomLinkDevRes>(JsonRoomLinkDevRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonRoomUlinkDevRes>(JsonRoomUlinkDevRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonRoomLstRes>(JsonRoomLstRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonRoomLstDevRes>(JsonRoomLstDevRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonRoomSyncRes>(JsonRoomSyncRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonRoomSyncDevRes>(JsonRoomSyncDevRes::GetStrCmd());
    // Floor
    m_pJsonSendDbSession->MapJsonMessage<JsonFloorAddRes>(JsonRoomAddRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonFloorDelRes>(JsonRoomDelRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonFloorEditRes>(JsonRoomEditRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonFloorLinkRoomRes>(JsonRoomLinkDevRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonFloorUlinkRoomRes>(JsonRoomUlinkDevRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonFloorLstRes>(JsonRoomLstRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonFloorLstRoomRes>(JsonRoomLstDevRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonFloorSyncRes>(JsonRoomSyncRes::GetStrCmd());
    m_pJsonSendDbSession->MapJsonMessage<JsonFloorSyncRoomRes>(JsonRoomSyncDevRes::GetStrCmd());
}

/**
 * @func   LoadDatabase
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::LoadDatabase() {
    for (DbFloors::const_iterator it = m_pDbModelDb->Floors.begin();
            it != m_pDbModelDb->Floors.end(); it++) {
        m_Floors.push_back(*it);
    }

    for (DbRooms::const_iterator it = m_pDbModelDb->Rooms.begin();
            it != m_pDbModelDb->Rooms.end(); it++) {
        m_Rooms.push_back(*it);
    }

    for (DbDevices::const_iterator it = m_pDbModelDb->Devices.begin();
            it != m_pDbModelDb->Devices.end(); it++) {
    }
}

/**
 * @func   HandlerFloorCmdAdd
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerFloorCmdAdd(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonFloorAdd> jsonFloorAdd =
    m_pJsonRecvDbSession->GetJsonMapping<JsonFloorAdd>();

    if (!jsonFloorAdd->ParseJsonCommand(pJsonCommand)) { return; }

    Vector<JsonFloorAdd::Floor_t> lstAddFloor = jsonFloorAdd->ListFloor();
    Vector<JsonFloorAddRes::Floor_t> lstAddFloorRes;

    for (u32_t i = 0; i < lstAddFloor.size(); i++) {
        JsonFloorAddRes::Floor_t floorRes;
        floorRes.floorid = lstAddFloor[i].floorid;
        floorRes.name = lstAddFloor[i].name;
        std::replace(floorRes.name.begin(), floorRes.name.end(), SPACE, AT);
        DbFloor floorfind =
        m_pDbModelDb->Find<DbFloorItem>().Where("FloorId = ?").Bind(lstAddFloor[i].floorid);

        if (floorfind.get() == NULL) {
            DbFloor flooradd = m_pDbModelDb->Add(new DbFloorItem());
            flooradd.Modify()->FloorId = lstAddFloor[i].floorid;
            flooradd.Modify()->Name = lstAddFloor[i].name;
            floorRes.ret = 0;
        } else {
            if (floorfind->Name == lstAddFloor[i].name) {
                floorRes.ret = 0;
            } else {
                floorRes.ret = 1;
            }
        }
        lstAddFloorRes.push_back(floorRes);
    }

    m_pDbModelDb->UpdateChanges();

    JsonMessagePtr<JsonFloorAddRes> jsonFloorAddRes =
    m_pJsonSendDbSession->GetJsonMapping<JsonFloorAddRes>();

    JsonCommand_p pJsonCommandRes =
    jsonFloorAddRes->CreateJsonCommand(lstAddFloorRes);

    pJsonCommandRes->SetSrcFlag(JsonCommand::Flag::Coord);
    pJsonCommandRes->SetDesFlag(JsonCommand::Flag::NetWork);

    PushJsonCommand(pJsonCommandRes);
}

/**
 * @func   HandlerFloorCmdEdit
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerFloorCmdEdit(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonFloorEdit> jsonFloorEdit =
    m_pJsonRecvDbSession->GetJsonMapping<JsonFloorEdit>();

    if (!jsonFloorEdit->ParseJsonCommand(pJsonCommand)) { return; }

    Vector<JsonFloorEdit::Floor_t> lstEditFloor = jsonFloorEdit->ListFloor();
    Vector<JsonFloorEditRes::Floor_t> lstEditFloorRes;

    for (u32_t i = 0; i < lstEditFloor.size(); i++) {
        JsonFloorEditRes::Floor_t floorRes;
        floorRes.floorid = lstEditFloor[i].floorid;
        floorRes.name    = lstEditFloor[i].name;
        std::replace(floorRes.name.begin(), floorRes.name.end(), SPACE, AT);
        DbFloor floorfind = m_pDbModelDb->Find<DbFloorItem>().
        Where("FloorId = ?").Bind(lstEditFloor[i].floorid);
        if (floorfind.get() != NULL) {
            floorfind.Modify()->Name = lstEditFloor[i].name;
            m_pDbModelDb->Add(floorfind);
            floorRes.ret = 0;
        } else {
            floorRes.ret = 1;
        }
        lstEditFloorRes.push_back(floorRes);
    }
    m_pDbModelDb->UpdateChanges();

    JsonMessagePtr<JsonFloorEditRes> jsonFloorEditRes =
    m_pJsonSendDbSession->GetJsonMapping<JsonFloorEditRes>();

    JsonCommand_p pJsonCommandRes =
    jsonFloorEditRes->CreateJsonCommand(lstEditFloorRes);

    pJsonCommandRes->SetSrcFlag(JsonCommand::Flag::Coord);
    pJsonCommandRes->SetDesFlag(JsonCommand::Flag::NetWork);

    PushJsonCommand(pJsonCommandRes);
}

/**
 * @func   HandlerFloorCmdDel
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerFloorCmdDel(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonFloorDel> jsonFloorDel =
    m_pJsonRecvDbSession->GetJsonMapping<JsonFloorDel>();

    if (!jsonFloorDel->ParseJsonCommand(pJsonCommand)) { return; }

    Vector<JsonFloorDel::Floor_t> lstDelFloor = jsonFloorDel->ListFloor();
    Vector<JsonFloorDelRes::Floor_t> lstDelFloorRes;

    for (u32_t i = 0; i < lstDelFloor.size(); i++) {
        JsonFloorDelRes::Floor_t floorRes;
        floorRes.floorid = lstDelFloor[i].floorid;

        DbFloor floorfind = m_pDbModelDb->Find<DbFloorItem>().
        Where("FloorId = ?").Bind(lstDelFloor[i].floorid);

        if (floorfind.get() != NULL) {
            floorfind.Remove();
        }
        floorRes.ret = 0;
        lstDelFloorRes.push_back(floorRes);
    }
    m_pDbModelDb->UpdateChanges();
    JsonMessagePtr<JsonFloorDelRes> jsonFloorDelRes =
    m_pJsonSendDbSession->GetJsonMapping<JsonFloorDelRes>();

    JsonCommand_p pJsonCommandRes =
    jsonFloorDelRes->CreateJsonCommand(lstDelFloorRes);

    pJsonCommandRes->SetSrcFlag(JsonCommand::Flag::Coord);
    pJsonCommandRes->SetDesFlag(JsonCommand::Flag::NetWork);

    PushJsonCommand(pJsonCommandRes);
}

/**
 * @func   HandlerFloorCmdLinkRoom
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerFloorCmdLinkRoom(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonFloorLinkRoom> jsonFloorLinkRoom =
    m_pJsonRecvDbSession->GetJsonMapping<JsonFloorLinkRoom>();

    if (!jsonFloorLinkRoom->ParseJsonCommand(pJsonCommand)) { return; }

    Vector<JsonFloorLinkRoomRes::Room_t> lstLinkRoomRes;
    Vector<JsonFloorLinkRoom::Room_t> lstLinkRoom = jsonFloorLinkRoom->LstRoom();
    i32_t idwFloorId = jsonFloorLinkRoom->FloorId();

    for (u32_t i = 0; i < lstLinkRoom.size(); i++) {
        JsonFloorLinkRoomRes::Room_t roomRes;
        roomRes.roomid = lstLinkRoom[i].roomid;

        DbRoom roomfind = m_pDbModelDb->Find<DbRoomItem>().
        Where("roomid = ?").Bind(lstLinkRoom[i].roomid);

        if (roomfind.get() != NULL) {
            roomfind.Modify()->FloorId = idwFloorId;
            m_pDbModelDb->Add(roomfind);
            roomRes.ret = 0;
        } else {
            roomRes.ret = 1;
        }
        lstLinkRoomRes.push_back(roomRes);
    }
    m_pDbModelDb->UpdateChanges();

    JsonMessagePtr<JsonFloorLinkRoomRes> jsonFloorLinkRoomRes =
    m_pJsonSendDbSession->GetJsonMapping<JsonFloorLinkRoomRes>();

    JsonCommand_p pJsonCommandRes =
    jsonFloorLinkRoomRes->CreateJsonCommand(idwFloorId, lstLinkRoomRes);

    pJsonCommandRes->SetSrcFlag(JsonCommand::Flag::Coord);
    pJsonCommandRes->SetDesFlag(JsonCommand::Flag::NetWork);

    PushJsonCommand(pJsonCommandRes);
}

/**
 * @func   HandlerFloorCmdUlinkRoom
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerFloorCmdUlinkRoom(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonFloorUlinkRoom> jsonFloorUlinkRoom =
    m_pJsonRecvDbSession->GetJsonMapping<JsonFloorUlinkRoom>();

    if (!jsonFloorUlinkRoom->ParseJsonCommand(pJsonCommand)) { return; }

    Vector<JsonFloorUlinkRoomRes::Room_t> lstUlinkRoomRes;
    Vector<JsonFloorUlinkRoom::Room_t> lstLinkRoom = jsonFloorUlinkRoom->LstRoom();
    i32_t idwFloorId = jsonFloorUlinkRoom->FloorId();

    for (u32_t i = 0; i < lstLinkRoom.size(); i++) {
        JsonFloorUlinkRoomRes::Room_t roomRes;
        roomRes.roomid = lstLinkRoom[i].roomid;

        DbRoom roomfind = m_pDbModelDb->Find<DbRoomItem>().
        Where("roomid = ?").Bind(lstLinkRoom[i].roomid);

        if (roomfind.get() != NULL) {
            if (idwFloorId == roomfind->FloorId) {
                roomfind.Modify()->Floor = DbFloor(); // Set Null
                m_pDbModelDb->Add(roomfind);
            }
            roomRes.ret = 0;
        } else {
            roomRes.ret = 1;
        }
        lstUlinkRoomRes.push_back(roomRes);
    }
    m_pDbModelDb->UpdateChanges();

    JsonMessagePtr<JsonFloorUlinkRoomRes> jsonFloorUlinkRoomRes =
    m_pJsonSendDbSession->GetJsonMapping<JsonFloorUlinkRoomRes>();

    JsonCommand_p pJsonCommandRes =
    jsonFloorUlinkRoomRes->CreateJsonCommand(idwFloorId, lstUlinkRoomRes);

    pJsonCommandRes->SetSrcFlag(JsonCommand::Flag::Coord);
    pJsonCommandRes->SetDesFlag(JsonCommand::Flag::NetWork);
    PushJsonCommand(pJsonCommandRes);
}

/**
 * @func   HandlerFloorCmdLstRes
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerFloorCmdLstRes(
    JsonCommand_p pJsonCommand
) {
}

/**
 * @func   HandlerFloorCmdLstRoomRes
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerFloorCmdLstRoomRes(
    JsonCommand_p pJsonCommand
) {
}

/**
 * @func   HandlerFloorCmdSync
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerFloorCmdSync(
    JsonCommand_p pJsonCommand
) {
}

/**
 * @func   HandlerFloorCmdSyncRoom
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerFloorCmdSyncRoom(
    JsonCommand_p pJsonCommand
) {
}

/**
 * @func   HandlerRoomCmdAdd
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerRoomCmdAdd(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonRoomAdd> jsonRoomAdd =
    m_pJsonRecvDbSession->GetJsonMapping<JsonRoomAdd>();

    if (!jsonRoomAdd->ParseJsonCommand(pJsonCommand)) { return; }

    Vector<JsonRoomAdd::Room_t> lstAddRoom = jsonRoomAdd->ListRoom();
    Vector<JsonRoomAddRes::Room_t> lstAddRoomRes;
    for (u32_t i = 0; i < lstAddRoom.size(); i++) {
        JsonRoomAddRes::Room_t roomRes;
        roomRes.roomid = lstAddRoom[i].roomid;
        roomRes.name = lstAddRoom[i].name;
        std::replace(roomRes.name.begin(), roomRes.name.end(), SPACE, AT);

        DbRoom roomfind = m_pDbModelDb->Find<DbRoomItem>().
        Where("RoomId = ?").Bind(lstAddRoom[i].roomid);

        if (roomfind.get() == NULL) {
            DbRoom roomadd = m_pDbModelDb->Add(new DbRoomItem());
            roomadd.Modify()->RoomId = lstAddRoom[i].roomid;
            roomadd.Modify()->Name   = lstAddRoom[i].name;
            roomRes.ret = 0;
        } else {
            if (roomfind->Name == lstAddRoom[i].name) {
                roomRes.ret = 0; // if exist
            } else {
                roomRes.ret = 1;
            }
        }
        lstAddRoomRes.push_back(roomRes);
    }
    m_pDbModelDb->UpdateChanges();

    JsonMessagePtr<JsonRoomAddRes> jsonRoomAddRes =
    m_pJsonSendDbSession->GetJsonMapping<JsonRoomAddRes>();

    JsonCommand_p pJsonCommandRes =
    jsonRoomAddRes->CreateJsonCommand(lstAddRoomRes);

    pJsonCommandRes->SetSrcFlag(JsonCommand::Flag::Coord);
    pJsonCommandRes->SetDesFlag(JsonCommand::Flag::NetWork);

    PushJsonCommand(pJsonCommandRes);
}

/**
 * @func   HandlerRoomCmdEdit
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerRoomCmdEdit(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonRoomEdit> jsonRoomEdit =
    m_pJsonRecvDbSession->GetJsonMapping<JsonRoomEdit>();

    if (!jsonRoomEdit->ParseJsonCommand(pJsonCommand)) { return; }

    Vector<JsonRoomEdit::Room_t> lstEditRoom = jsonRoomEdit->ListRoom();
    Vector<JsonRoomEditRes::Room_t> lstEditRoomRes;
    for (u32_t i = 0; i < lstEditRoom.size(); i++) {
        JsonRoomEditRes::Room_t roomRes;
        roomRes.roomid = lstEditRoom[i].roomid;
        roomRes.name   = lstEditRoom[i].name;
        std::replace(roomRes.name.begin(), roomRes.name.end(), SPACE, AT);

        DbRoom roomfind = m_pDbModelDb->Find<DbRoomItem>().
        Where("RoomId = ?").Bind(lstEditRoom[i].roomid);

        if (roomfind.get() != NULL) {
            roomfind.Modify()->Name = lstEditRoom[i].name;
            m_pDbModelDb->Add(roomfind);
            roomRes.ret = 0;
        } else {
            roomRes.ret = 1;
        }
        lstEditRoomRes.push_back(roomRes);
    }
    m_pDbModelDb->UpdateChanges();

    JsonMessagePtr<JsonRoomEditRes> jsonRoomEditRes =
    m_pJsonSendDbSession->GetJsonMapping<JsonRoomEditRes>();

    JsonCommand_p pJsonCommandRes =
    jsonRoomEditRes->CreateJsonCommand(lstEditRoomRes);

    pJsonCommandRes->SetSrcFlag(JsonCommand::Flag::Coord);
    pJsonCommandRes->SetDesFlag(JsonCommand::Flag::NetWork);

    PushJsonCommand(pJsonCommandRes);
}

/**
 * @func   HandlerRoomCmdDel
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerRoomCmdDel(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonRoomDel> jsonRoomDel =
    m_pJsonRecvDbSession->GetJsonMapping<JsonRoomDel>();

    if (!jsonRoomDel->ParseJsonCommand(pJsonCommand)) { return; }

    Vector<JsonRoomDelRes::Room_t> lstDelRoomRes;
    Vector<JsonRoomDel::Room_t> lstDelRoom = jsonRoomDel->ListRoom();

    for (u32_t i = 0; i < lstDelRoom.size(); i++) {
        JsonRoomDelRes::Room_t roomRes;
        roomRes.roomid = lstDelRoom[i].roomid;

        DbRoom roomDel = m_pDbModelDb->Find<DbRoomItem>().
        Where("RoomId = ?").Bind(lstDelRoom[i].roomid);

        if (roomDel.get() != NULL) {
            roomDel.Remove();
        }
        roomRes.ret = 0;
        lstDelRoomRes.push_back(roomRes);
    }
    m_pDbModelDb->UpdateChanges();

    JsonMessagePtr<JsonRoomDelRes> jsonRoomDelRes =
    m_pJsonSendDbSession->GetJsonMapping<JsonRoomDelRes>();

    JsonCommand_p pJsonCommandRes =
    jsonRoomDelRes->CreateJsonCommand(lstDelRoomRes);

    pJsonCommandRes->SetSrcFlag(JsonCommand::Flag::Coord);
    pJsonCommandRes->SetDesFlag(JsonCommand::Flag::NetWork);

    PushJsonCommand(pJsonCommandRes);
}

/**
 * @func   HandlerRoomCmdLinkDev
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerRoomCmdLinkDev(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonRoomLinkDev> jsonRoomLinkDev =
    m_pJsonRecvDbSession->GetJsonMapping<JsonRoomLinkDev>();

    if (!jsonRoomLinkDev->ParseJsonCommand(pJsonCommand)) { return; }

    i32_t idwRoomId = jsonRoomLinkDev->RoomId();
    Vector<JsonRoomLinkDevRes::Device_t> lstLinkDeviceRes;
    Vector<JsonRoomLinkDev::Device_t> lstLinkDevice = jsonRoomLinkDev->LstDev();

    for (u32_t i = 0; i < lstLinkDevice.size(); i++) {
        JsonRoomLinkDevRes::Device_t deviceRes;
        deviceRes.devid = lstLinkDevice[i].devid;
        deviceRes.order = lstLinkDevice[i].order;
        deviceRes.netwk = lstLinkDevice[i].netwk;

        DbDevice device = m_pDbModelDb->Find<DbDeviceItem>().
        Where("DeviceId = ?").Bind(deviceRes.devid).
        Where("Position = ?").Bind(deviceRes.order).
        Where("TypeNet = ?").Bind(deviceRes.netwk);

        if (device.get() != NULL) {
            device.Modify()->RoomId = idwRoomId;
            m_pDbModelDb->Add(device);
            deviceRes.ret = 0;
        } else {
            deviceRes.ret = 1;
        }
        lstLinkDeviceRes.push_back(deviceRes);
    }
    m_pDbModelDb->UpdateChanges();

    JsonMessagePtr<JsonRoomLinkDevRes> jsonRoomLinkDevRes =
    m_pJsonSendDbSession->GetJsonMapping<JsonRoomLinkDevRes>();

    JsonCommand_p pJsonCommandRes =
    jsonRoomLinkDevRes->CreateJsonCommand(idwRoomId, lstLinkDeviceRes);

    pJsonCommandRes->SetSrcFlag(JsonCommand::Flag::Coord);
    pJsonCommandRes->SetDesFlag(JsonCommand::Flag::NetWork);

    PushJsonCommand(pJsonCommandRes);
}

/**
 * @func   HandlerRoomCmdUlinkDev
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerRoomCmdUlinkDev(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonRoomUlinkDev> jsonRoomUlinkDev =
    m_pJsonRecvDbSession->GetJsonMapping<JsonRoomUlinkDev>();

    if (!jsonRoomUlinkDev->ParseJsonCommand(pJsonCommand)) { return; }

    i32_t idwRoomId = jsonRoomUlinkDev->RoomId();
    Vector<JsonRoomUlinkDevRes::Device_t> lstUlinkDeviceRes;
    Vector<JsonRoomUlinkDev::Device_t> lstLinkDevice = jsonRoomUlinkDev->LstDev();

    for (u32_t i = 0; i < lstLinkDevice.size(); i++) {
        JsonRoomUlinkDevRes::Device_t deviceRes;
        deviceRes.devid = lstLinkDevice[i].devid;
        deviceRes.order = lstLinkDevice[i].order;
        deviceRes.netwk = lstLinkDevice[i].netwk;

        DbDevice device = m_pDbModelDb->Find<DbDeviceItem>().
        Where("DeviceId = ?").Bind(deviceRes.devid).
        Where("Position = ?").Bind(deviceRes.order).
        Where("TypeNet = ?").Bind(deviceRes.netwk);

        if (device.get() != NULL) {
            if (idwRoomId == device->RoomId) {
                device.Modify()->Room = DbRoom(); // Set Null
                m_pDbModelDb->Add(device);
            }
            deviceRes.ret = 0;
        } else {
            deviceRes.ret = 1;
        }
        lstUlinkDeviceRes.push_back(deviceRes);
    }
    m_pDbModelDb->UpdateChanges();

    JsonMessagePtr<JsonRoomUlinkDevRes> jsonRoomUlinkDevRes =
    m_pJsonSendDbSession->GetJsonMapping<JsonRoomUlinkDevRes>();

    JsonCommand_p pJsonCommandRes =
    jsonRoomUlinkDevRes->CreateJsonCommand(idwRoomId, lstUlinkDeviceRes);

    pJsonCommandRes->SetSrcFlag(JsonCommand::Flag::Coord);
    pJsonCommandRes->SetDesFlag(JsonCommand::Flag::NetWork);

    PushJsonCommand(pJsonCommandRes);
}

/**
 * @func   HandlerRoomCmdLstRes
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerRoomCmdLstRes(
    JsonCommand_p pJsonCommand
) {
}

/**
 * @func   HandlerRoomCmdLstDevRes
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerRoomCmdLstDevRes(
    JsonCommand_p pJsonCommand
) {
}

/**
 * @func   HandlerRoomCmdSync
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerRoomCmdSync(
    JsonCommand_p pJsonCommand
) {
}

/**
 * @func   HandlerRoomCmdSyncDev
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbManager::HandlerRoomCmdSyncDev(
    JsonCommand_p pJsonCommand
) {
}
