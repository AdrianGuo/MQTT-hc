/*
 * ZbSocketCmd.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#ifndef ZBSOCKETCMD_HPP_
#define ZBSOCKETCMD_HPP_

#include <typedefs.h>
#include <Vector.hpp>
#include <ZbMessage.hpp>
#include <IZbDriver.hpp>
#include <ZbDriver.hpp>
#include <ZbModelDb.hpp>
#include <JsonZigbeeSession.hpp>
#include <DeviceInfo.hpp>

class ZbSocketCmd {
private:
    ZbSocketCmd();

    JsonSendZigbeeSession_p m_pJsonSendSession;

    void_t SendJsonMessage(EvAct, JsonCommand_p);

public:
    static ZbSocketCmd* s_pInstance;
    static ZbSocketCmd* GetInstance();
    ~ZbSocketCmd();

    void_t SendLstAdd(Devices_t);
    void_t SendLstDel(Devices_t);
    void_t SendZbStt(Device_t, Json::Value);
    void_t SendResetRes(u8_t);
};

typedef ZbSocketCmd     ZbSocketCmd_t;
typedef ZbSocketCmd*    ZbSocketCmd_p;

#endif /* ZBSOCKETCMD_HPP_ */
