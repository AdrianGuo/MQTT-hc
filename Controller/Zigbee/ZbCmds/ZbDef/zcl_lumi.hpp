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

#define ATTRID_LUMI_IRCMD                               0x0000
#define ATTRID_LUMI_IRSTATE                             0x0001

#define ZCL_LUMI_DEVICEID_IR                            0x0015
#define ZCL_LUMI_DEVICEID_SECURITY                      0x0016
#define ZCL_LUMI_DEVICEID_POWER                         0x0017

//Devices
#define UNKNOWN_DEVICE                                  (-1)
#define OTHER_BRAND_DEVICE                              (0)
#define LUMI_DEVICE_SWITCH                              (1)
#define LUMI_DEVICE_DIMMER                              (2)
#define LUMI_DEVICE_FAN                                 (3)
#define LUMI_DEVICE_CURTAIN                             (4)
#define LUMI_DEVICE_IR                                  (5)
#define LUMI_DEVICE_POWER                               (127)
#define LUMI_DEVICE_PIR                                 (128)
#define LUMI_DEVICE_DOOR                                (129)
#define LUMI_DEVICE_TEMPERATURE                         (130)
#define LUMI_DEVICE_HUMIDITY                            (131)
#define LUMI_DEVICE_ILLUMINANCE                         (132)
#define LUMI_DEVICE_RGB                                 (6)
#define LUMI_DEVICE_INPUT                               (9)
#define LUMI_DEVICE_DAIKIN                              (8)

#endif /* ZCL_LUMI_HPP_ */
