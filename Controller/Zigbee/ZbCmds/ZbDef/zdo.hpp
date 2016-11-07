/*
 * zdo.hpp
 *
 *  Created on: Nov 3, 2016
 *      Author: taho
 */

#ifndef ZDO_HPP_
#define ZDO_HPP_

#define ZDO_REQUEST_TYPE_SINGLE         0x00
#define ZDO_REQUEST_TYPE_EXTENDED       0x01

//ZDO Mac Capability Flags
#define ZDO_CAPABILITY_ALTERNATE_PAN    0x01
#define ZDO_CAPABILITY_FULL_FUNCTION    0x02
#define ZDO_CAPABILITY_MAINS_POWER      0x04
#define ZDO_CAPABILITY_RX_ON_IDLE       0x08
#define ZDO_CAPABILITY_RESERVED_1       0x10
#define ZDO_CAPABILITY_RESERVED_2       0x20
#define ZDO_CAPABILITY_SECURITY         0x40
#define ZDO_CAPABILITY_ALLOCATE_ADDR    0x80

//ZDO Status Values
#define ZDO_STATUS_SUCCESS              0x00
// 0x01 to 0x7F are reserved
#define ZDO_STATUS_INV_REQUESTTYPE      0x80
#define ZDO_STATUS_DEVICE_NOT_FOUND     0x81
#define ZDO_STATUS_INVALID_EP           0x82
#define ZDO_STATUS_NOT_ACTIVE           0x83
#define ZDO_STATUS_NOT_SUPPORTED        0x84
#define ZDO_STATUS_TIMEOUT              0x85
#define ZDO_STATUS_NO_MATCH             0x86
// 0x87 is reserved
#define ZDO_STATUS_NO_ENTRY             0x88
#define ZDO_STATUS_NO_DESCRIPTOR        0x89
#define ZDO_STATUS_INSUFFICIENT_SPACE   0x8A
#define ZDO_STATUS_NOT_PERMITTED        0x8B
#define ZDO_STATUS_TABLE_FULL           0x8C
#define ZDO_STATUS_NOT_AUTHORIZED       0x8D

//Cluster IDs with the high bit set are responses.
#define ZDO_CLUST_RESPONSE_MASK         0x8000

#define ZDO_CLUST_IS_RESPONSE(c)        (c & ZDO_CLUST_RESPONSE_MASK)

/*********************************************************
                    NWK_addr Descriptor
**********************************************************/

#define ZDO_NET_ADDR_PENDING            0xFFFE
#define ZDO_NET_ADDR_TIMEOUT            0xFFFF
#define ZDO_NET_ADDR_ERROR              0xFFFD

/*********************************************************
                    IEEE_addr Descriptor
**********************************************************/
/// cluster ID for ZDO IEEE_addr request
#define ZDO_IEEE_ADDR_REQ               0x0001
/// cluster ID for ZDO IEEE_addr response
#define ZDO_IEEE_ADDR_RSP               0x8001
/// frame format for ZDO IEEE_addr request
//#define ZDO_IEEE_ADDR_PENDING           WPAN_IEEE_ADDR_UNDEFINED
//#define ZDO_IEEE_ADDR_TIMEOUT           WPAN_IEEE_ADDR_BROADCAST
//#define ZDO_IEEE_ADDR_ERROR             WPAN_IEEE_ADDR_ALL_ZEROS

/*********************************************************
                    Node Descriptor
**********************************************************/
/// cluster ID for ZDO Node_Desc request
#define ZDO_NODE_DESC_REQ               0x0002
/// cluster ID for ZDO Node_Desc response
#define ZDO_NODE_DESC_RSP               0x8002

#define ZDO_NODE_DESC_FLAGS0_TYPE_MASK                      0x07
#define ZDO_NODE_DESC_FLAGS0_TYPE_COORD                     0x00
#define ZDO_NODE_DESC_FLAGS0_TYPE_ROUTER                    0x01
#define ZDO_NODE_DESC_FLAGS0_TYPE_ENDDEV                    0x02

#define ZDO_NODE_DESC_FLAGS0_COMPLEX_DESC_AVAIL             0x08
#define ZDO_NODE_DESC_FLAGS0_USER_DESC_AVAIL                0x10

#define ZDO_NODE_DESC_FLAGS0_RESERVED                       0xE0

#define ZDO_NODE_DESC_FLAGS1_APS_FIELD                      0x07
#define ZDO_NODE_DESC_FLAGS1_FREQUENCY_MASK                 0xF8

#define ZDO_NODE_DESC_FLAGS1_FREQ_868                       (1<<0)
#define ZDO_NODE_DESC_FLAGS1_FREQ_RESERVED_1                (1<<1)
#define ZDO_NODE_DESC_FLAGS1_FREQ_900                       (1<<2)
#define ZDO_NODE_DESC_FLAGS1_FREQ_2400                      (1<<3)
#define ZDO_NODE_DESC_FLAGS1_FREQ_RESERVED_4                (1<<4)

//ZDO Node Descriptor - MAC Capability Flags

#define ZDO_NODE_DESC_MAC_CAPABILITY_ALT_PAN_COORD          (1<<0)
#define ZDO_NODE_DESC_MAC_CAPABILITY_DEVICE_TYPE            (1<<1)
#define ZDO_NODE_DESC_MAC_CAPABILITY_POWER_SOURCE           (1<<2)
#define ZDO_NODE_DESC_MAC_CAPABILITY_RX_ON_IDLE             (1<<3)
#define ZDO_NODE_DESC_MAC_CAPABILITY_RESERVED_4             (1<<4)
#define ZDO_NODE_DESC_MAC_CAPABILITY_RESERVED_5             (1<<5)
#define ZDO_NODE_DESC_MAC_CAPABILITY_SECURITY               (1<<6)
#define ZDO_NODE_DESC_MAC_CAPABILITY_ALLOCATE_ADDR          (1<<7)

//ZDO Node Descriptor - Server Mask Bit Assignments

#define ZDO_NODE_DESC_SERVER_MASK_PRIMARY_TRUST             (1<<0)
#define ZDO_NODE_DESC_SERVER_MASK_BACKUP_TRUST              (1<<1)
#define ZDO_NODE_DESC_SERVER_MASK_PRIMARY_BINDING           (1<<2)
#define ZDO_NODE_DESC_SERVER_MASK_BACKUP_BINDING            (1<<3)
#define ZDO_NODE_DESC_SERVER_MASK_PRIMARY_DISCOVERY         (1<<4)
#define ZDO_NODE_DESC_SERVER_MASK_BACKUP_DISCOVERY          (1<<5)
#define ZDO_NODE_DESC_SERVER_MASK_NETWORK_MANAGER           (1<<6)
// bits 7 through 15 are reserved

/*********************************************************
                    Power Descriptor
**********************************************************/
/// cluster ID for ZDO Power_Desc request
#define ZDO_POWER_DESC_REQ                                  0x0003
/// cluster ID for ZDO Power_Desc response
#define ZDO_POWER_DESC_RSP                                  0x8003

//PowerDescriptor macros

#define ZDO_POWER_SOURCE_MAINS                              0x01
#define ZDO_POWER_SOURCE_RECHARGABLE                        0x02
#define ZDO_POWER_SOURCE_DISPOSABLE                         0x04
#define ZDO_POWER_SOURCE_RESERVED                           0x08

// for .power0, choose one of the following...
#define ZDO_POWER0_SYNC                                     0x00
#define ZDO_POWER0_PERIODIC                                 0x01
#define ZDO_POWER0_STIMULATED                               0x02

// ... and choose one or more of the following
#define ZDO_POWER0_AVAIL_MAINS                              (ZDO_POWER_SOURCE_MAINS << 4)
#define ZDO_POWER0_AVAIL_RECHARGABLE                        (ZDO_POWER_SOURCE_RECHARGABLE << 4)
#define ZDO_POWER0_AVAIL_DISPOSABLE                         (ZDO_POWER_SOURCE_DISPOSABLE << 4)
#define ZDO_POWER0_AVAIL_RESERVED                           (ZDO_POWER_SOURCE_RESERVED << 4)

// for .power 1, choose one of the following...
#define ZDO_POWER1_LEVEL_CRITICAL                           0x00
#define ZDO_POWER1_LEVEL_33                                 0x40
#define ZDO_POWER1_LEVEL_66                                 0x80
#define ZDO_POWER1_LEVEL_FULL                               0xC0

// ... and choose one or more of the following
#define ZDO_POWER1_CURRENT_MAINS                            ZDO_POWER_SOURCE_MAINS
#define ZDO_POWER1_CURRENT_RECHARGABLE                      ZDO_POWER_SOURCE_RECHARGABLE
#define ZDO_POWER1_CURRENT_DISPOSABLE                       ZDO_POWER_SOURCE_DISPOSABLE
#define ZDO_POWER1_CURRENT_RESERVED                         ZDO_POWER_SOURCE_RESERVED

/*********************************************************
                    Simple Descriptor
**********************************************************/
/// cluster ID for ZDO Simple_Desc request
#define ZDO_SIMPLE_DESC_REQ                                 0x0004
/// cluster ID for ZDO Simple_Desc response
#define ZDO_SIMPLE_DESC_RSP                                 0x8004

/*********************************************************
                    Active EP Descriptor
**********************************************************/
/// cluster ID for ZDO Active_EP request
#define ZDO_ACTIVE_EP_REQ                                   0x0005
/// cluster ID for ZDO Active_EP response
#define ZDO_ACTIVE_EP_RSP                                   0x8005

/*********************************************************
                    Match Descriptor
**********************************************************/
/// cluster ID for ZDO Match_Desc request
#define ZDO_MATCH_DESC_REQ                                  0x0006
/// cluster ID for ZDO Match_Desc response
#define ZDO_MATCH_DESC_RSP                                  0x8006

/*********************************************************
                    Complex Descriptor
**********************************************************/
/// cluster ID for ZDO Complex_Desc request
#define ZDO_COMPLEX_DESC_REQ                                0x0007
/// cluster ID for ZDO Complex_Desc response
#define ZDO_COMPLEX_DESC_RSP                                0x8007

/*********************************************************
                    User Descriptor
**********************************************************/
/// cluster ID for ZDO User_Desc request
#define ZDO_USER_DESC_REQ                                   0x0011
/// cluster ID for ZDO User_Desc response
#define ZDO_USER_DESC_RSP                                   0x8011

/*********************************************************
                    Device Announce
**********************************************************/
/// cluster ID for ZDO Device_annce announcement
#define ZDO_DEVICE_ANNCE                                    0x0013

/// Entry for Device Annce handler in cluster table of ZDO endpoint
//#define ZDO_DEVICE_ANNCE_CLUSTER

/*********************************************************
                    Bind/Unbind Request
**********************************************************/
/// cluster ID for ZDO Bind request
#define ZDO_BIND_REQ                                        0x0021
/// cluster ID for ZDO Bind response
#define ZDO_BIND_RSP                                        0x8021
/// cluster ID for ZDO Unbind request
#define ZDO_UNBIND_REQ                                      0x0022
/// cluster ID for ZDO Unbind response
#define ZDO_UNBIND_RSP                                      0x8022
/// zdo_bind_req_t contains 16-bit group address
#define ZDO_BIND_DST_MODE_GROUP                             0x01
/// zdo_bind_req_t contains 64-bit destination address and endpoint
#define ZDO_BIND_DST_MODE_ADDR                              0x03

/*********************************************************
                    Management Leave Request
**********************************************************/
/// cluster ID for ZDO Management Leave Request
#define ZDO_MGMT_LEAVE_REQ          0x0034
/// cluster ID for ZDO Management Leave Response
#define ZDO_MGMT_LEAVE_RSP          0x8034

#define ZDO_MGMT_LEAVE_REQ_FLAG_NONE                        0x00
#define ZDO_MGMT_LEAVE_REQ_FLAG_REJOIN                      0x01
#define ZDO_MGMT_LEAVE_REQ_FLAG_REMOVE_CHILDREN             0x02

#endif /* ZDO_HPP_ */
