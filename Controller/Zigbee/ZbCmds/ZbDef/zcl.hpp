
#ifndef ZCL_HPP
#define ZCL_HPP

// General Clusters
#define ZCL_CLUSTER_ID_GEN_BASIC                             0x0000
#define ZCL_CLUSTER_ID_GEN_POWER_CFG                         0x0001
#define ZCL_CLUSTER_ID_GEN_DEVICE_TEMP_CONFIG                0x0002
#define ZCL_CLUSTER_ID_GEN_IDENTIFY                          0x0003
#define ZCL_CLUSTER_ID_GEN_GROUPS                            0x0004
#define ZCL_CLUSTER_ID_GEN_SCENES                            0x0005
#define ZCL_CLUSTER_ID_GEN_ON_OFF                            0x0006
#define ZCL_CLUSTER_ID_GEN_ON_OFF_SWITCH_CONFIG              0x0007
#define ZCL_CLUSTER_ID_GEN_LEVEL_CONTROL                     0x0008
#define ZCL_CLUSTER_ID_GEN_ALARMS                            0x0009
#define ZCL_CLUSTER_ID_GEN_TIME                              0x000A
#define ZCL_CLUSTER_ID_GEN_LOCATION                          0x000B
#define ZCL_CLUSTER_ID_GEN_ANALOG_INPUT                      0x000C
#define ZCL_CLUSTER_ID_GEN_ANALOG_OUTPUT                     0x000D
#define ZCL_CLUSTER_ID_GEN_ANALOG_VALUE                      0x000E
#define ZCL_CLUSTER_ID_GEN_BINARY_INPUT                      0x000F
#define ZCL_CLUSTER_ID_GEN_BINARY_OUTPUT                     0x0010
#define ZCL_CLUSTER_ID_GEN_BINARY_VALUE                      0x0011
#define ZCL_CLUSTER_ID_GEN_MULTISTATE_INPUT                  0x0012
#define ZCL_CLUSTER_ID_GEN_MULTISTATE_OUTPUT                 0x0013
#define ZCL_CLUSTER_ID_GEN_MULTISTATE_VALUE                  0x0014
#define ZCL_CLUSTER_ID_GEN_COMMISSIONING                     0x0015

#define ZCL_CLUSTER_ID_GEN_KEY_ESTABLISHMENT                 0x0800

// Closures Clusters
#define ZCL_CLUSTER_ID_CLOSURES_SHADE_CONFIG                 0x0100
#define ZCL_CLUSTER_ID_CLOSURES_DOOR_CONFIG                  0x0101

// HVAC Clusters
#define ZCL_CLUSTER_ID_HVAC_PUMP_CONFIG_CONTROL              0x0200
#define ZCL_CLUSTER_ID_HAVC_THERMOSTAT                       0x0201
#define ZCL_CLUSTER_ID_HAVC_FAN_CONTROL                      0x0202
#define ZCL_CLUSTER_ID_HAVC_DIHUMIDIFICATION_CONTROL         0x0203
#define ZCL_CLUSTER_ID_HAVC_USER_INTERFACE_CONFIG            0x0204

// Lighting Clusters
#define ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL                0x0300
#define ZCL_CLUSTER_ID_LIGHTING_BALLAST_CONFIG               0x0301
  
// Measurement and Sensing Clusters
#define ZCL_CLUSTER_ID_MS_ILLUMINANCE_MEASUREMENT            0x0400
#define ZCL_CLUSTER_ID_MS_ILLUMINANCE_LEVEL_SENSING_CONFIG   0x0401
#define ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT            0x0402
#define ZCL_CLUSTER_ID_MS_PRESSURE_MEASUREMENT               0x0403
#define ZCL_CLUSTER_ID_MS_FLOW_MEASUREMENT                   0x0404
#define ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY                  0x0405
#define ZCL_CLUSTER_ID_MS_OCCUPANCY_SENSING                  0x0406

// Security and Safety (SS) Clusters
#define ZCL_CLUSTER_ID_SS_IAS_ZONE                           0x0500
#define ZCL_CLUSTER_ID_SS_IAS_ACE                            0x0501
#define ZCL_CLUSTER_ID_SS_IAS_WD                             0x0502
    
// Protocol Interfaces
#define ZCL_CLUSTER_ID_PI_GENERIC_TUNNEL                     0x0600
#define ZCL_CLUSTER_ID_PI_BACNET_PROTOCOL_TUNNEL             0x0601
#define ZCL_CLUSTER_ID_PI_11073_PROTOCOL_TUNNEL              0x0614

// Advanced Metering Initiative (SE) Clusters 
#define ZCL_CLUSTER_ID_SE_PRICING                            0x0700
#define ZCL_CLUSTER_ID_SE_LOAD_CONTROL                       0x0701
#define ZCL_CLUSTER_ID_SE_SIMPLE_METERING                    0x0702
#define ZCL_CLUSTER_ID_SE_MESSAGE                            0x0703
#define ZCL_CLUSTER_ID_SE_REGISTRATION                       0x0704
#define ZCL_CLUSTER_ID_SE_SE_TUNNELING                       0x0705
#define ZCL_CLUSTER_ID_SE_PRE_PAYMENT                        0x0706  
  
/*** Frame Control bit mask ***/
#define ZCL_FRAME_CONTROL_TYPE                          0x03
#define ZCL_FRAME_CONTROL_MANU_SPECIFIC                 0x04
#define ZCL_FRAME_CONTROL_DIRECTION                     0x08
#define ZCL_FRAME_CONTROL_DISABLE_DEFAULT_RSP           0x10

/*** Frame Types ***/
#define ZCL_FRAME_TYPE_PROFILE_CMD                      0x00
#define ZCL_FRAME_TYPE_SPECIFIC_CMD                     0x01

/*** Frame Client/Server Directions ***/
#define ZCL_FRAME_CLIENT_SERVER_DIR                     0x00
#define ZCL_FRAME_SERVER_CLIENT_DIR                     0x01

/*** Chipcon Manufacturer Code ***/ 
#define CC_MANUFACTURER_CODE                            0x1001

/*** Foundation Command IDs ***/
#define ZCL_CMD_READ                                    0x00
#define ZCL_CMD_READ_RSP                                0x01
#define ZCL_CMD_WRITE                                   0x02
#define ZCL_CMD_WRITE_UNDIVIDED                         0x03
#define ZCL_CMD_WRITE_RSP                               0x04
#define ZCL_CMD_WRITE_NO_RSP                            0x05
#define ZCL_CMD_CONFIG_REPORT                           0x06
#define ZCL_CMD_CONFIG_REPORT_RSP                       0x07
#define ZCL_CMD_READ_REPORT_CFG                         0x08
#define ZCL_CMD_READ_REPORT_CFG_RSP                     0x09
#define ZCL_CMD_REPORT                                  0x0A
#define ZCL_CMD_DEFAULT_RSP                             0x0B
#define ZCL_CMD_DISCOVER                                0x0C
#define ZCL_CMD_DISCOVER_RSP                            0x0D

#define ZCL_CMD_MAX                                     ZCL_CMD_DISCOVER_RSP

/*** Data Types ***/
#define ZCL_DATATYPE_NO_DATA                            0x00
#define ZCL_DATATYPE_DATA8                              0x08
#define ZCL_DATATYPE_DATA16                             0x09
#define ZCL_DATATYPE_DATA24                             0x0A
#define ZCL_DATATYPE_DATA32                             0x0B
#define ZCL_DATATYPE_DATA40                             0x0C
#define ZCL_DATATYPE_DATA48                             0x0D
#define ZCL_DATATYPE_DATA56                             0x0E
#define ZCL_DATATYPE_DATA64                             0x0F
#define ZCL_DATATYPE_BOOLEAN                            0x10
#define ZCL_DATATYPE_BITMAP8                            0x18
#define ZCL_DATATYPE_BITMAP16                           0x19
#define ZCL_DATATYPE_BITMAP24                           0x1A
#define ZCL_DATATYPE_BITMAP32                           0x1B
#define ZCL_DATATYPE_BITMAP40                           0x1C
#define ZCL_DATATYPE_BITMAP48                           0x1D
#define ZCL_DATATYPE_BITMAP56                           0x1E
#define ZCL_DATATYPE_BITMAP64                           0x1F
#define ZCL_DATATYPE_UINT8                              0x20
#define ZCL_DATATYPE_UINT16                             0x21
#define ZCL_DATATYPE_UINT24                             0x22
#define ZCL_DATATYPE_UINT32                             0x23
#define ZCL_DATATYPE_UINT40                             0x24
#define ZCL_DATATYPE_UINT48                             0x25
#define ZCL_DATATYPE_UINT56                             0x26
#define ZCL_DATATYPE_UINT64                             0x27
#define ZCL_DATATYPE_INT8                               0x28
#define ZCL_DATATYPE_INT16                              0x29
#define ZCL_DATATYPE_INT24                              0x2A
#define ZCL_DATATYPE_INT32                              0x2B
#define ZCL_DATATYPE_INT40                              0x2C
#define ZCL_DATATYPE_INT48                              0x2D
#define ZCL_DATATYPE_INT56                              0x2E
#define ZCL_DATATYPE_INT64                              0x2F
#define ZCL_DATATYPE_ENUM8                              0x30
#define ZCL_DATATYPE_ENUM16                             0x31
#define ZCL_DATATYPE_SEMI_PREC                          0x38
#define ZCL_DATATYPE_SINGLE_PREC                        0x39
#define ZCL_DATATYPE_DOUBLE_PREC                        0x3A
#define ZCL_DATATYPE_OCTET_STR                          0x41
#define ZCL_DATATYPE_CHAR_STR                           0x42
#define ZCL_DATATYPE_LONG_OCTET_STR                     0x43
#define ZCL_DATATYPE_LONG_CHAR_STR                      0x44
#define ZCL_DATATYPE_TOD                                0xE0
#define ZCL_DATATYPE_DATE                               0xE1
#define ZCL_DATATYPE_UTC                                0xE2
#define ZCL_DATATYPE_CLUSTER_ID                         0xE8
#define ZCL_DATATYPE_ATTR_ID                            0xE9
#define ZCL_DATATYPE_BAC_OID                            0xEA
#define ZCL_DATATYPE_IEEE_ADDR                          0xF0
#define ZCL_DATATYPE_128_SECURITY_KEY                   0xF1
#define ZCL_DATATYPE_UNKNOWN                            0xFF

/*** Error Status Codes ***/
#define ZCL_STATUS_SUCCESS                              0x00
#define ZCL_STATUS_FAILURE                              0x01
// 0x02-0x7D are reserved.
#define ZCL_STATUS_NOT_AUTHORIZED                       0x7E
#define ZCL_STATUS_MALFORMED_COMMAND                    0x80
#define ZCL_STATUS_UNSUP_CLUSTER_COMMAND                0x81
#define ZCL_STATUS_UNSUP_GENERAL_COMMAND                0x82
#define ZCL_STATUS_UNSUP_MANU_CLUSTER_COMMAND           0x83
#define ZCL_STATUS_UNSUP_MANU_GENERAL_COMMAND           0x84
#define ZCL_STATUS_INVALID_FIELD                        0x85
#define ZCL_STATUS_UNSUPPORTED_ATTRIBUTE                0x86
#define ZCL_STATUS_INVALID_VALUE                        0x87
#define ZCL_STATUS_READ_ONLY                            0x88
#define ZCL_STATUS_INSUFFICIENT_SPACE                   0x89
#define ZCL_STATUS_DUPLICATE_EXISTS                     0x8A
#define ZCL_STATUS_NOT_FOUND                            0x8B
#define ZCL_STATUS_UNREPORTABLE_ATTRIBUTE               0x8C
#define ZCL_STATUS_INVALID_DATA_TYPE                    0x8D
// 0xbd-bf are reserved.
#define ZCL_STATUS_HARDWARE_FAILURE                     0xC0
#define ZCL_STATUS_SOFTWARE_FAILURE                     0xC1
#define ZCL_STATUS_CALIBRATION_ERROR                    0xC2
// 0xc3-0xff are reserved.
#define ZCL_STATUS_CMD_HAS_RSP                          0xFF // Non-standard status (used for Default Rsp)

/*** Attribute Access Control - bit masks ***/
#define ACCESS_CONTROL_READ                             0x01
#define ACCESS_CONTROL_WRITE                            0x02
#define ACCESS_CONTROL_COMMAND                          0x04

#define ZCL_INVALID_CLUSTER_ID                          0xFFFF
#define ZCL_ATTR_ID_MAX                                 0xFFFF

// Used by Configure Reporting Command
#define ZCL_SEND_ATTR_REPORTS                           0x00
#define ZCL_EXPECT_ATTR_REPORTS                         0x01
  
// Predefined Maximum String Length
#define MAX_UTF8_STRING_LEN                             50

#endif /* ZCL_HPP */
