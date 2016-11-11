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
#include <ZbDeviceDb.hpp>

typedef struct {
    u8_t byTotalEP;
    u8_t byEPCount;
    u8_t byTypeCount;
    Map<u16_t, u16_t> mapType;
} enumEPInfor_t;

typedef Map<u16_t, enumEPInfor_t>   DeviceLogic_t;
typedef Map<u16_t, enumEPInfor_t>*  DeviceLogic_p;

class ZbZdoCmd {
private:
    Map<u16_t,String> m_mapTemps;
    DeviceLogic_t m_mapEPInfor;

    ZbZdoCmd();

public:
    static ZbZdoCmd* s_pInstance;
    static ZbZdoCmd* GetInstance();

    ~ZbZdoCmd();
    void_t ProcRecvMessage(void_p);

    void_t DeviceAnnounce(u8_p, u32_t);
    void_t ActiveEndpointRequest(u16_t);
    void_t ActiveEndpointResponse(u8_p, u32_t);
    void_t NodeDescriptionRequest(Device_t);
    void_t NodeDescriptionResponse(u8_p, u32_t);

    void_t LeaveRequest(u16_t);
    void_t LeaveResponse(u16_t, u8_p, u32_t);

    DeviceLogic_t GetDeviceLogic();
};

typedef ZbZdoCmd ZbZdoCmd_t;
typedef ZbZdoCmd* ZbZdoCmd_p;

#endif /* ZBZDOCMD_HPP_ */
