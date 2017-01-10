/*
 * ZbZdoCmd.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#ifndef ZBZDOCMD_HPP_
#define ZBZDOCMD_HPP_

#include <typedefs.h>
#include <Map.hpp>
#include <string>
#include <RTimer.hpp>
#include <ZbDeviceDb.hpp>
#include <BackupInfo.hpp>

typedef struct {
    String  MAC = "";
    u8_t    byTotalEP       = 0;
    u8_t    byAEReqCount    = 0;
    bool_t  IsAERequested   = FALSE;
    bool_t  IsDone          = FALSE;
    Vector<u8_t>        vEPList;
    Map<u8_t, u16_t>    mapType;
} EPInfo_t;

typedef Map<u16_t, EPInfo_t>   DeviceLogic_t;
typedef Map<u16_t, EPInfo_t>*  DeviceLogic_p;

class ZbZdoCmd {
private:
    RTimer_p m_pTimer;
    TimerFunctor_t m_DAFunctor;
    TimerFunctor_t m_AEFunctor;
    int_t m_iDAHandle;
    int_t m_iAEHandle;

    ZbZdoCmd();
    void_t RestoreBuDevice(u16_t, BackupDev_t);

public:
    static ZbZdoCmd* s_pInstance;
    static ZbZdoCmd* GetInstance();
    static DeviceLogic_t s_mapEPInfo;

    ~ZbZdoCmd();
    void_t ProcRecvMessage(void_p);

    void_t IEEEAddrRequest(u16_t);
    void_t IEEEAddrResponse(u8_p, u32_t);
    void_t DeviceAnnounce(u8_p, u32_t);
    void_t ManualDeviceAnnounce(u16_t, String);
    void_t HandleDeviceAnnounce(void_p);
    void_t ActiveEndpointRequest(u16_t);
    void_t ActiveEndpointResponse(u8_p, u32_t);
    void_t HandleActiveEndpoint(void_p);
    void_t NodeDescRequest(u16_t);
    void_t NodeDescResponse(u8_p, u32_t);
    void_t SimpleDescRequest(Device_t);
    void_t SimpleDescResponse(u8_p, u32_t);
    void_t LeaveRequest(u16_t);
    void_t LeaveResponse(u16_t, u8_p, u32_t);

    DeviceLogic_t GetDeviceLogic();
};

typedef ZbZdoCmd ZbZdoCmd_t;
typedef ZbZdoCmd* ZbZdoCmd_p;

#endif /* ZBZDOCMD_HPP_ */
