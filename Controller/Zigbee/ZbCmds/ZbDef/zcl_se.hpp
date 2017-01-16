/**************************************************************************************************
  Filename:       zcl_SE.hpp
  Revised:        $Date: 2006-09-08 15:51:21 -0700 (Fri, 08 Sep 2006) $
  Revision:       $Revision: 11934 $

  Description:    This file contains the ZCL SE (Smart Energy) Profile definitions.


  Copyright 2007-2009 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED �AS IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, 
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com. 
**************************************************************************************************/

/********************************************/
/*** SE Profile Attributes List ************/
/********************************************/

// Simple Metering Attribute Set
#define ATTRID_MASK_SE_READ_INFORMATION_SET          0x0000
#define ATTRID_MASK_SE_TOU_INFORMATION_SET           0x0100
#define ATTRID_MASK_SE_METER_STATUS                  0x0200
#define ATTRID_MASK_SE_FORMATTING                    0x0300
#define ATTRID_MASK_SE_ESP_HISTORICAL_CONSUMPTION    0x0400
#define ATTRID_MASK_SE_LOAD_PROFILE_CONFIGURATION    0x0500
  
// Simple Metering Cluster - Reading Information Set Attributes 0x00
#define ATTRID_SE_CURRENT_SUMMATION_DELIVERED        ( 0x0000 | ATTRID_MASK_SE_READ_INFORMATION_SET )
#define ATTRID_SE_CURRENT_SUMMATION_RECEIVED         ( 0x0001 | ATTRID_MASK_SE_READ_INFORMATION_SET )    
#define ATTRID_SE_CURRENT_MAX_DEMAND_DELIVERED       ( 0x0002 | ATTRID_MASK_SE_READ_INFORMATION_SET )
#define ATTRID_SE_CURRENT_MAX_DEMAND_RECEIVED        ( 0x0003 | ATTRID_MASK_SE_READ_INFORMATION_SET )
#define ATTRID_SE_DFT_SUMMATION                      ( 0x0004 | ATTRID_MASK_SE_READ_INFORMATION_SET )
#define ATTRID_SE_DAILY_FREEZE_TIME                  ( 0x0005 | ATTRID_MASK_SE_READ_INFORMATION_SET )
#define ATTRID_SE_POWER_FACTOR                       ( 0x0006 | ATTRID_MASK_SE_READ_INFORMATION_SET )
#define ATTRID_SE_READING_SNAPSHOT_TIME              ( 0x0007 | ATTRID_MASK_SE_READ_INFORMATION_SET )
#define ATTRID_SE_CURRENT_MAX_DEMAND_DELIVERD_TIME   ( 0x0008 | ATTRID_MASK_SE_READ_INFORMATION_SET )
#define ATTRID_SE_CURRENT_MAX_DEMAND_RECEIVED_TIME   ( 0x0009 | ATTRID_MASK_SE_READ_INFORMATION_SET )  
  
// Simple Metering Cluster - TOU Information Set Attributes 0x01
#define ATTRID_SE_CURRENT_TIER1_SUMMATION_DELIVERED  ( 0x0000 | ATTRID_MASK_SE_TOU_INFORMATION_SET )
#define ATTRID_SE_CURRENT_TIER1_SUMMATION_RECEIVED   ( 0x0001 | ATTRID_MASK_SE_TOU_INFORMATION_SET )
#define ATTRID_SE_CURRENT_TIER2_SUMMATION_DELIVERED  ( 0x0002 | ATTRID_MASK_SE_TOU_INFORMATION_SET )
#define ATTRID_SE_CURRENT_TIER2_SUMMATION_RECEIVED   ( 0x0003 | ATTRID_MASK_SE_TOU_INFORMATION_SET )
#define ATTRID_SE_CURRENT_TIER3_SUMMATION_DELIVERED  ( 0x0004 | ATTRID_MASK_SE_TOU_INFORMATION_SET )
#define ATTRID_SE_CURRENT_TIER3_SUMMATION_RECEIVED   ( 0x0005 | ATTRID_MASK_SE_TOU_INFORMATION_SET )
#define ATTRID_SE_CURRENT_TIER4_SUMMATION_DELIVERED  ( 0x0006 | ATTRID_MASK_SE_TOU_INFORMATION_SET )
#define ATTRID_SE_CURRENT_TIER4_SUMMATION_RECEIVED   ( 0x0007 | ATTRID_MASK_SE_TOU_INFORMATION_SET )
#define ATTRID_SE_CURRENT_TIER5_SUMMATION_DELIVERED  ( 0x0008 | ATTRID_MASK_SE_TOU_INFORMATION_SET )
#define ATTRID_SE_CURRENT_TIER5_SUMMATION_RECEIVED   ( 0x0009 | ATTRID_MASK_SE_TOU_INFORMATION_SET )
#define ATTRID_SE_CURRENT_TIER6_SUMMATION_DELIVERED  ( 0x000A | ATTRID_MASK_SE_TOU_INFORMATION_SET )
#define ATTRID_SE_CURRENT_TIER6_SUMMATION_RECEIVED   ( 0x000B | ATTRID_MASK_SE_TOU_INFORMATION_SET )
  
// Simple Metering Cluslter - Meter Status Attributes 0x02
#define ATTRID_SE_STATUS                             ( 0x0000 | ATTRID_MASK_SE_METER_STATUS )

// Simple Metering Cluster - Formatting Attributes 0x03
#define ATTRID_SE_UNIT_OF_MEASURE                    ( 0x0000 | ATTRID_MASK_SE_FORMATTING )
#define ATTRID_SE_MULTIPLIER                         ( 0x0001 | ATTRID_MASK_SE_FORMATTING )
#define ATTRID_SE_DIVISOR                            ( 0x0002 | ATTRID_MASK_SE_FORMATTING )
#define ATTRID_SE_SUMMATION_FORMATTING               ( 0x0003 | ATTRID_MASK_SE_FORMATTING )
#define ATTRID_SE_DEMAND_FORMATTING                  ( 0x0004 | ATTRID_MASK_SE_FORMATTING )
#define ATTRID_SE_HISTORICAL_CONSUMPTION_FORMATTING  ( 0x0005 | ATTRID_MASK_SE_FORMATTING )
#define ATTRID_SE_METERING_DEVICE_TYPE               ( 0x0006 | ATTRID_MASK_SE_FORMATTING )

// Simple Metering Cluster - ESP Historical Comsumption Attributes 0x04
#define ATTRID_SE_INSTANTANEOUS_DEMAND               ( 0x0000 | ATTRID_MASK_SE_ESP_HISTORICAL_CONSUMPTION )
#define ATTRID_SE_CURRENTDAY_CONSUMPTION_DELIVERED   ( 0x0001 | ATTRID_MASK_SE_ESP_HISTORICAL_CONSUMPTION )
#define ATTRID_SE_CURRENTDAY_CONSUMPTION_RECEIVED    ( 0x0002 | ATTRID_MASK_SE_ESP_HISTORICAL_CONSUMPTION )
#define ATTRID_SE_PREVIOUSDAY_CONSUMPTION_DELIVERED  ( 0x0003 | ATTRID_MASK_SE_ESP_HISTORICAL_CONSUMPTION )
#define ATTRID_SE_PREVIOUSDAY_CONSUMPTION_RECEIVED   ( 0x0004 | ATTRID_MASK_SE_ESP_HISTORICAL_CONSUMPTION )
#define ATTRID_SE_CURRENT_PARTIAL_PROFILE_INTERVAL_START_TIME ( 0x0005 | ATTRID_MASK_SE_ESP_HISTORICAL_CONSUMPTION )
#define ATTRID_SE_CURRENT_PARTIAL_PROFILE_INTERVAL_VALUE      ( 0x0006 | ATTRID_MASK_SE_ESP_HISTORICAL_CONSUMPTION )

// Simple Metering Cluster - Load Profile Configuration Attributes 0x05
#define ATTRID_SE_MAX_NUMBER_OF_PERIODS_DELIVERED    ( 0x0000 | ATTRID_MASK_SE_LOAD_PROFILE_CONFIGURATION )
    
// Demand Response Client Cluster Attributes
#define ATTRID_SE_UTILITY_DEFINED_GROUP               0x0000
#define ATTRID_SE_START_RANDOMIZE_MINUTES             0x0001
#define ATTRID_SE_STOP_RANDOMIZE_MINUTES              0x0002
  
/************************************************************/
/***    SE Profile Command ID                            ***/
/************************************************************/
// Simple Metering Cluster
#define COMMAND_SE_GET_PROFILE_CMD                    0x0000
#define COMMAND_SE_GET_PROFILE_RSP                    0x0000
#define COMMAND_SE_REQ_MIRROR_CMD                     0x0001
#define COMMAND_SE_REQ_MIRROR_RSP                     0x0001
#define COMMAND_SE_MIRROR_REM_CMD                     0x0002
#define COMMAND_SE_MIRROR_REM_RSP                     0x0002
   
// Pricing Cluster
#define COMMAND_SE_GET_CURRENT_PRICE                  0x0000
#define COMMAND_SE_GET_SCHEDULED_PRICE                0x0001
#define COMMAND_SE_PUBLISH_PRICE                      0x0000
  
// Messaging Cluster
#define COMMAND_SE_DISPLAY_MESSAGE                    0x0000
#define COMMAND_SE_CANCEL_MESSAGE                     0x0001
#define COMMAND_SE_GET_LAST_MESSAGE                   0x0000
#define COMMAND_SE_MESSAGE_CONFIRMATION               0x0001
  
// Demand Response and Load Control Cluster
#define COMMAND_SE_LOAD_CONTROL_EVENT                 0x0000
#define COMMAND_SE_CANCEL_LOAD_CONTROL_EVENT          0x0001
#define COMMAND_SE_CANCEL_ALL_LOAD_CONTROL_EVENT      0x0002
#define COMMAND_SE_REPORT_EVENT_STATUS                0x0000
#define COMMAND_SE_GET_SCHEDULED_EVENT                0x0001

/******************************************************************/
/***        Other Constants                                     ***/
/******************************************************************/
  
// Profile Interval Period Time Frame
#define PROFILE_INTERVAL_PERIOD_DAILY                  0
#define PROFILE_INTERVAL_PERIOD_60MIN                  1
#define PROFILE_INTERVAL_PERIOD_30MIN                  2
#define PROFILE_INTERVAL_PERIOD_15MIN                  3
#define PROFILE_INTERVAL_PERIOD_10MIN                  4
#define PROFILE_INTERVAL_PERIOD_450SEC                 5
#define PROFILE_INTERVAL_PERIOD_300SEC                 6
#define PROFILE_INTERVAL_PERIOD_150SEC                 7
  
// Profile Interval Status
#define PROFILE_INTERVAL_STAT_SUCCESS               0x00
#define PROFILE_INTERVAL_STAT_UNDEFINED_CHANNEL     0x01
#define PROFILE_INTERVAL_STAT_CHANNEL_NOT_SUPPORT   0x02
#define PROFILE_INTERVAL_STAT_INVALID_END_TIME      0x03
#define PROFILE_INTERVAL_STAT_TOO_MANY_PERIOD       0x04
#define PROFILE_INTERVAL_STAT_NO_INTERVALS          0x05
// Command Packet Length
#define PACKET_LEN_SE_PUBLISH_PRICE                   34       
#define PACKET_LEN_SE_LOAD_CONTROL_EVENT              23
#define PACKET_LEN_SE_GET_SCHEDULED_EVENT             5
#define PACKET_LEN_SE_CANCEL_LOAD_CONTROL_EVENT       12
#define PACKET_LEN_SE_REPORT_EVENT_STATUS_ONLY        18
#define PACKET_LEN_SE_REPORT_EVENT_STATUS             (PACKET_LEN_SE_REPORT_EVENT_STATUS_ONLY + SE_PROFILE_SIGNATURE_LENGTH)

// Maximum value for Command Fields Range Checking
#define MAX_DURATION_IN_MINUTES_SE_LOAD_CONTROL       1440  // One day 
#define MAX_CRITICAL_LEVEL_SE_LOAD_CONTROL            0x0F
#define MAX_TEMPERATURE_SETPOINT_SE_LOAD_CONTROL      0x7FFF
#define MIN_AVERAGE_LOAD_ADJUSTMENT_PERCENTAGE_SE     -100
#define MAX_AVERAGE_LOAD_ADJUSTMENT_PERCENTAGE_SE     100
#define MAX_DUTY_CYCLE_SE_LOAD_CONTROL                100  
#define MAX_INTERVAL_CHANNEL_SE_SIMPLE_METERING       1  
#define MAX_PROFILE_INTERVAL_PERIOD_SE_SIMPLE_METERING 7
#define MAX_UNIT_OF_MEASURE_SE                        7 
    
// Device Class Field Bitmap
#define SE_DEVICE_CLASS_HVAC_COMPRESSOR_FURNACE       0x0001
#define SE_DEVICE_CLASS_STRIP_HEATER                  0x0002
#define SE_DEVICE_CLASS_WATER_HEATER                  0x0004
#define SE_DEVICE_CLASS_POOL_PUMP                     0x0008
#define SE_DEVICE_CLASS_SMART_APPLIANCES              0x0010
#define SE_DEVICE_CLASS_IRRIGATION_PUMP               0x0020
#define SE_DEVICE_CLASS_MANAGED_CI_LOADS              0x0040
#define SE_DEVICE_CLASS_SIMPLE_MISC_LOADS             0x0080
#define SE_DEVICE_CLASS_EXTERIOR_LIGHTING             0x0100
#define SE_DEVICE_CLASS_INTERIOR_LIGHTING             0x0200
#define SE_DEVICE_CLASS_ELECTRIC_VEHICLE              0x0400
#define SE_DEVICE_CLASS_GENERATION_SYSTEMS            0x0800

// Criticality Levels
#define SE_CRITICALITY_LEVELS_GREEN                   0x01
#define SE_CRITICALITY_LEVELS_1                       0x02
#define SE_CRITICALITY_LEVELS_2                       0x03
#define SE_CRITICALITY_LEVELS_3                       0x04
#define SE_CRITICALITY_LEVELS_4                       0x05
#define SE_CRITICALITY_LEVELS_5                       0x06
#define SE_CRITICALITY_LEVELS_EMERGENCY               0x07
#define SE_CRITICALITY_LEVELS_PLANNED_OUTAGE          0x08
#define SE_CRITICALITY_LEVELS_SERVICE_DISCONNECT      0x09
  
// Event Control Field Bitmap
#define SE_EVENT_CONTROL_FIELD_START_TIME             0x01
#define SE_EVENT_CONTROL_FIELD_END_TIME               0x02
  
// Optional Fields Value
#define SE_OPTIONAL_FIELD_UINT8                       0xFF
#define SE_OPTIONAL_FIELD_UINT16                      0xFFFF
#define SE_OPTIONAL_FIELD_UINT32                      0xFFFFFFFF
#define SE_OPTIONAL_FIELD_TEMPERATURE_SET_POINT       0x8000
#define SE_OPTIONAL_FIELD_INT8                        -128
 
// Event Status Field Values
#define EVENT_STATUS_LOAD_CONTROL_EVENT_RECEIVED         0x01
#define EVENT_STATUS_LOAD_CONTROL_EVENT_STARTED          0x02
#define EVENT_STATUS_LOAD_CONTROL_EVENT_COMPLETED        0x03
#define EVENT_STATUS_LOAD_CONTROL_USER_OPT_OUT           0x04
#define EVENT_STATUS_LOAD_CONTROL_USER_OPT_IN            0x05
#define EVENT_STATUS_LOAD_CONTROL_EVENT_CANCELLED        0x06
#define EVENT_STATUS_LOAD_CONTROL_EVENT_SUPERSEDED       0x07
#define EVENT_STATUS_LOAD_CONTROL_REJECTED_DUPLICATEID   0xFA  
#define EVENT_STATUS_LOAD_CONTROL_REJECTED_EVT_EXPIRED   0xFB 
#define EVENT_STATUS_LOAD_CONTROL_REJECTED_EVT_COMPLETED 0xFC 
#define EVENT_STATUS_LOAD_CONTROL_REJECTED_UNDEFINED_EVT 0xFD 
#define EVENT_STATUS_LOAD_CONTROL_EVENT_REJECTED         0xFE

// Signature type
#define SE_PROFILE_SIGNATURE_TYPE_ECDSA                  0x01              
// Other constants
#define SE_PROFILE_SIGNATURE_LENGTH                   42
#define SE_PROFILE_MSGCTRL_IMPORTANCE                 2
#define SE_PROFILE_MSGCTRL_CONFREQUIRED               7
  
#endif /* ZCL_SE_HPP */
