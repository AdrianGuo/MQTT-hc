/*
 * ZbZclGlobalCmd.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#ifndef ZBZCLGLOBCMD_HPP_
#define ZBZCLGLOBCMD_HPP_

#include <typedefs.h>
#include <Map.hpp>
#include <ZbDeviceDb.hpp>


class ZbZclGlobalCmd {
private:
    Locker_p m_pLock;

    ZbZclGlobalCmd();
    void_t ProcessException(u16_t, u8_p);
    void_t SaveDevicesInfo(u16_t);
    void_t AddConfigDevice(u16_t);

public:
    static ZbZclGlobalCmd* s_pInstance;
    static ZbZclGlobalCmd* GetInstance();
    static void_t RequestDevicesState(Device_t);

    ~ZbZclGlobalCmd();

    void_t ProcRecvMessage(void_p);

    void_t ReadAttributeRequest(Device_t, Vector<DeviceInfo>);
    void_t ReadAttributeRequest(Device_t, DeviceInfo);
    void_t ReadAttributeResponse(u8_p);

    void_t WriteAttributeRequest(Device_t, DeviceProperties);
    void_t WriteAttributeRequest(Device_t, DeviceProperty);
    void_t WriteAttributeResponse(u8_p);

    void_t Broadcast();
};

typedef ZbZclGlobalCmd ZbZclGlobalCmd_t;
typedef ZbZclGlobalCmd* ZbZclGlobalCmd_p;

#endif /* ZBZCLGLOBCMD_HPP_ */
