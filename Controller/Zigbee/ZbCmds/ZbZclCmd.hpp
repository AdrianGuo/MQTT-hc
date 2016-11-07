/*
 * ZbZclCmd.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#ifndef ZBZCLCMD_HPP_
#define ZBZCLCMD_HPP_

#include <typedefs.h>
#include <ZbMessage.hpp>

class ZbZclCmd {
private:
    ZbZclCmd();

public:
    static ZbZclCmd* s_pInstance;
    static ZbZclCmd* GetInstance();

    ~ZbZclCmd();

    void_t ProcRecvMessage(void_p);

    void_t SetDevice(ZbMessage_p, Device_t, u8_t);
};

typedef ZbZclCmd ZbZclCmd_t;
typedef ZbZclCmd* ZbZclCmd_p;

#endif /* ZBZCLCMD_HPP_ */
