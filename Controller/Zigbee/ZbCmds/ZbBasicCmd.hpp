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
#include <RTimer.hpp>
#include <ZbPacket.hpp>
#include <ZbModelDb.hpp>

class ZbBasicCmd {
private:
    bool_t m_boIsNetAvail;
    RTimer_p m_pTimer;
    TimerFunctor_t m_TimerFunctor;
    int_t m_iTimerHandle;
    u8_t m_byNetReqCount;

    ZbBasicCmd();
    void_t HandleNetworkInfo(void_p);

public:
    static ZbBasicCmd* s_pInstance;
    static ZbBasicCmd* GetInstance();
    ~ZbBasicCmd();

    bool_t IsNetworkAvail();

    void_t HCError(u8_t, u8_t);
    void_t MCError(ZbPacket_p);

    void_t NwkInit();
    void_t NwkInfoReq();
    void_t NwkInfoRsp(ZbPacket_p);

    void_t JoinNwkAllow(ZbPacket_p);
    void_t JoinNwkAllow(u8_t);
    void_t JoinNwkInfoReq();
    bool_t JoinNwkInfoRsp(ZbPacket_p);

    void_t ResetDevice(u16_t, u8_t);
};

typedef ZbBasicCmd ZbCmdBasic_t;
typedef ZbBasicCmd* ZbBasicCmd_p;

#endif /* ZBBASICCMD_HPP_ */
