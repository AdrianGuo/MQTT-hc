/*
 * ZbDeviceControl.hpp
 *
 *  Created on: Nov 5, 2016
 *      Author: taho
 */

#ifndef ZBDEVICECONTROL_HPP_
#define ZBDEVICECONTROL_HPP_

#include <typedefs.h>
#include <debug.hpp>
#include <ZbSocketCmd.hpp>
#include <ZbModelDb.hpp>
#include <JsonDevRep.hpp>
#include <JsonMessageSession.hpp>
//From Outside


//To Outside
void_t ForwardDeviceInfoToOutside(const ZbDeviceDb_p device) {
    ZbSocketCmd::GetInstance()->SendDevRep(device->DeviceID.GetValue(), device->State);
}

void_t SendDimmersStateToOutside(const ZbDeviceDb_p device) {
    int_t iLevel = device->State;
    if (iLevel <= 165)
        iLevel /= 3;
    else if (iLevel > 165)
        iLevel = 55 + (iLevel - 165) / 2;
    else
        iLevel = 0;

    ZbSocketCmd::GetInstance()->SendDevRep(device->DeviceID.GetValue(), iLevel);
}

void_t SendFansStateToOutside(const ZbDeviceDb_p device) {
    ZbSocketCmd::GetInstance()->SendDevRep(device->DeviceID.GetValue(), (device->State)/3);
}

void_t SendRGBsStateToOutside(const ZbDeviceDb_p device) {
    ZbSocketCmd::GetInstance()->SendRGBRes(device);
}
#endif /* ZBDEVICECONTROL_HPP_ */
