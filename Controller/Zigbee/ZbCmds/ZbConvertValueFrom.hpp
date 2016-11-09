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

/*****************************************************************************/
/***                           From Outside                                ***/
/*****************************************************************************/

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t ConvertOutsideData(
    Device_t device
) {

}


/*****************************************************************************/
/***                            To Outside                                 ***/
/*****************************************************************************/

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardStateToOutside(
    const ZbDeviceDb_p device
) {
    ZbSocketCmd::GetInstance()->SendDevRep(device->DeviceID.GetValue(), device->State);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardDimmerStateToOutside(
    const ZbDeviceDb_p device
) {
    int_t iLevel = device->State;
    if (iLevel <= 165)
        iLevel /= 3;
    else if (iLevel > 165)
        iLevel = 55 + (iLevel - 165) / 2;
    else
        iLevel = 0;

    ZbSocketCmd::GetInstance()->SendDevRep(device->DeviceID.GetValue(), iLevel);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardFanStateToOutside(
    const ZbDeviceDb_p device
) {
    ZbSocketCmd::GetInstance()->SendDevRep(device->DeviceID.GetValue(), (device->State)/3);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardSensorStateToOutside(
    const ZbDeviceDb_p device
) {
    ZbSocketCmd::GetInstance()->SendDevRep(device->DeviceID.GetValue(), (device->State)/3);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardRGBStateToOutside(
    const ZbDeviceDb_p device
) {
    ZbSocketCmd::GetInstance()->SendRGBRes(DbPtr<ZbDeviceDb>(device));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardRGBTimeToOutside(
    const ZbDeviceDb_p device
) {
    ZbSocketCmd::GetInstance()->SendRGBRes(DbPtr<ZbDeviceDb>(device));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardRGBToOutside(
    const ZbDeviceDb_p device
) {
    ZbSocketCmd::GetInstance()->SendRGBRes(DbPtr<ZbDeviceDb>(device));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardDaikinStateToOutside(
    const ZbDeviceDb_p device
) {
    ZbSocketCmd::GetInstance()->SendRGBRes(DbPtr<ZbDeviceDb>(device));
}




#endif /* ZBDEVICECONTROL_HPP_ */

