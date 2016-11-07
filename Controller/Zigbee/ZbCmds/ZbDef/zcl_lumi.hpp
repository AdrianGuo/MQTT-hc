/*
 * zcl_lumi.hpp
 *
 *  Created on: Nov 3, 2016
 *      Author: taho
 */

#ifndef ZCL_LUMI_HPP_
#define ZCL_LUMI_HPP_

//Command ID
#define HC_ERR                                          0x80
#define MC_ERR                                          0x00

#define NWK_INIT                                        0x81
#define NWK_INFO_REQ                                    0x82
#define NWK_INFO_RSP                                    0x01

#define NWK_JOIN                                        0x83
#define NWK_JOIN_INFO_REQ                               0x85
#define NWK_JOIN_INFO_RSP                               0x05

#define ZCL_CMD_REQ                                     0x86
#define ZCL_CMD_RSP                                     0x06

#define ZCL_GLOBAL_CMD_REQ                              0x87
#define ZCL_GLOBAL_CMD_RSP                              0x07

#define ZDO_CMD_REQ                                     0x88
#define ZDO_CMD_RSP                                     0x08

#define DEV_RESET                                       0x8E

#define ZCL_CLUSTER_ID_LUMI_IR                          0xFC15
#define ZCL_LUMI_DEVICEID_IR                            0x0015
#define ATTRID_LUMI_IR                                  0x0000

//Devices
#define LUMI_DEVICE_SWITCH                              (1)
#define LUMI_DEVICE_DIMMER                              (2)
#define LUMI_DEVICE_FAN                                 (3)
#define LUMI_DEVICE_CURTAIN                             (4)
#define LUMI_DEVICE_IR                                  (5)
#define LUMI_DEVICE_PIR                                 (6)
#define LUMI_DEVICE_DOOR                                (7)
#define LUMI_DEVICE_TEMPERATURE                         (8)
#define LUMI_DEVICE_HUMIDITY                            (9)
#define LUMI_DEVICE_ILLUMINANCE                         (10)
#define LUMI_DEVICE_RGB                                 (11)
#define LUMI_DEVICE_INPUT                               (12)
#define LUMI_DEVICE_DAIKIN                              (13)

#endif /* ZCL_LUMI_HPP_ */
