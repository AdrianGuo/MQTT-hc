/*
 * ZbBasicCmd.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#ifndef ZBBASICCMD_HPP_
#define ZBBASICCMD_HPP_

#include <typedefs.h>
#include <zcl.hpp>
#include <ZbPacket.hpp>
#include <ZbModelDb.hpp>
#include <ZbMessage.hpp>

class ZbBasicCmd {
private:
    ZbBasicCmd();

public:
    static ZbBasicCmd* s_pInstance;
    static ZbBasicCmd* GetInstance();
    ~ZbBasicCmd();

    void_t HCError(u8_t, u8_t);
    void_t MCError(ZbPacket_p);

    void_t NwkInit();
    void_t NwkInfoReq();
    void_t NwkInfoRsp(ZbPacket_p);

    void_t JoinNwkAllow(ZbMessage_p);
    void_t JoinNwkInfoReq();
    bool_t JoinNwkInfoRsp(ZbPacket_p);

    void_t ResetDevice(Device_t, u8_t);
};

typedef ZbBasicCmd ZbCmdBasic_t;
typedef ZbBasicCmd* ZbBasicCmd_p;

#endif /* ZBBASICCMD_HPP_ */
