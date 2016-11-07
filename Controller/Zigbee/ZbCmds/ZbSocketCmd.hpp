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

    JsonRecvZigbeeSession_p m_pJsonRecvZigbeeSession;

    void_t SendJsonMessage(EvAction, JsonCommand_p);

public:
    static ZbSocketCmd* s_pInstance;
    static ZbSocketCmd* GetInstance();
    ~ZbSocketCmd();

    void_t SendLstAdd(Devices_t);
    void_t SendLstDel(Devices_t);
    void_t SendDevRep(int_t, int_t);
    void_t SendResetRes(u8_t);
    void_t SendRGBRes(ZbDeviceDb_p);
    void_t SendRGBEnableRes(u8_t);
};

typedef ZbSocketCmd     ZbSocketCmd_t;
typedef ZbSocketCmd*    ZbSocketCmd_p;

#endif /* ZBSOCKETCMD_HPP_ */
