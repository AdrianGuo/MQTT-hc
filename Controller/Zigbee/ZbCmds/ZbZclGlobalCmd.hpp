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
    ZbZclGlobalCmd();

public:
    static ZbZclGlobalCmd* s_pInstance;
    static ZbZclGlobalCmd* GetInstance();

    ~ZbZclGlobalCmd();

    void_t ProcRecvMessage(void_p);

    void_t ReadAttributeRequest(Device_t, u8_t, ...);
    void_t ReadAttributeResponse(u8_p);

    void_t WriteAttributeRequest(Device_t, DeviceInfo, void_p);
    void_t WriteAttributeResponse(u8_p);
};

typedef ZbZclGlobalCmd ZbZclGlobalCmd_t;
typedef ZbZclGlobalCmd* ZbZclGlobalCmd_p;

#endif /* ZBZCLGLOBCMD_HPP_ */
