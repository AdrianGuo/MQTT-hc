/**************************************************************************************************
  Filename:       zcl_ss.hpp
  Revised:        $Date: 2006-04-03 16:27:20 -0700 (Mon, 03 Apr 2006) $
  Revision:       $Revision: 10362 $

  Description:    This file contains the ZCL Security and Safety definitions.


  Copyright 2006-2007 Texas Instruments Incorporated. All rights reserved.

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
  PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
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

#ifndef ZCL_SS_HPP
#define ZCL_SS_HPP
					
/************************************/
/***  IAS Zone Cluster Attributes ***/
/************************************/
  // Zone Information attributes set
#define ATTRID_SS_IAS_ZONE_STATE                                         0x0000
#define ATTRID_SS_IAS_ZONE_TYPE                                          0x0001
#define ATTRID_SS_IAS_ZONE_STATUS                                        0x0002
/*** Zone State Attribute values ***/
#define SS_IAS_ZONE_STATE_NOT_ENROLLED                                   0x00
#define SS_IAS_ZONE_STATE_ENROLLED                                       0x01
/*** Zone Type Attribute values ***/
#define SS_IAS_ZONE_TYPE_STANDARD_CIE                                    0x0000
#define SS_IAS_ZONE_TYPE_MOTION_SENSOR                                   0x000D
#define SS_IAS_ZONE_TYPE_CONTACT_SWITCH                                  0x0015
#define SS_IAS_ZONE_TYPE_FIRE_SENSOR                                     0x0028
#define SS_IAS_ZONE_TYPE_WATER_SENSOR                                    0x002A
#define SS_IAS_ZONE_TYPE_GAS_SENSOR                                      0x002B
#define SS_IAS_ZONE_TYPE_PERSONAL_EMERGENCY_DEVICE                       0x002C
#define SS_IAS_ZONE_TYPE_VIBRATION_MOVEMENT_SENSOR                       0x002D
#define SS_IAS_ZONE_TYPE_REMOTE_CONTROL                                  0x010F
#define SS_IAS_ZONE_TYPE_KEY_FOB                                         0x0115
#define SS_IAS_ZONE_TYPE_KEYPAD                                          0x021D
#define SS_IAS_ZONE_TYPE_STANDARD_WARNING_DEVICE                         0x0225
#define SS_IAS_ZONE_TYPE_INVALID_ZONE_TYPE                               0xFFFF
/*** Zone Status Attribute values ***/
#define SS_IAS_ZONE_STATUS_ALARM1_ALARMED                                0x0001
#define SS_IAS_ZONE_STATUS_ALARM2_ALARMED                                0x0002	
#define SS_IAS_ZONE_STATUS_TAMPERED_YES                                  0x0004
#define SS_IAS_ZONE_STATUS_BATTERY_LOW                                   0x0008
#define SS_IAS_ZONE_STATUS_SUPERVISION_REPORTS_YES                       0x0010
#define SS_IAS_ZONE_STATUS_RESTORE_REPORTS_YES                           0x0020
#define SS_IAS_ZONE_STATUS_TROUBLE_YES                                   0x0040
#define SS_IAS_ZONE_STATUS_AC_MAINS_FAULT                                0x0080

  // Zone Settings attributes set
#define ATTRID_SS_IAS_CIE_ADDRESS                                        0x0010
  // commands:
#define COMMAND_SS_IAS_ZONE_STATUS_CHANGE_NOTIFICATION                   0x00
#define COMMAND_SS_IAS_ZONE_STATUS_ENROLL_REQUEST                        0x01
#define COMMAND_SS_IAS_ZONE_STATUS_ENROLL_RESPONSE                       0x00
   // permitted values for Enroll Response Code field
#define SS_IAS_ZONE_STATUS_ENROLL_RESPONSE_CODE_SUCCESS                  0x00
#define SS_IAS_ZONE_STATUS_ENROLL_RESPONSE_CODE_NOT_SUPPORTED            0x01
#define SS_IAS_ZONE_STATUS_ENROLL_RESPONSE_CODE_NO_ENROLL_PERMIT         0x02
#define SS_IAS_ZONE_STATUS_ENROLL_RESPONSE_CODE_TOO_MANY_ZONES           0x03

/************************************/
/***  IAS ACE Cluster Attributes  ***/
/************************************/
  // cluster has no attributes

  // command IDs received by Server
  //note: EMERGENCY, FIRE,PANIC, GET_ZONE_ID_MAP cmds have no payload
#define COMMAND_SS_IAS_ACE_ARM                                           0x00  
#define COMMAND_SS_IAS_ACE_BYPASS                                        0x01  
#define COMMAND_SS_IAS_ACE_EMERGENCY                                     0x02  
#define COMMAND_SS_IAS_ACE_FIRE                                          0x03  
#define COMMAND_SS_IAS_ACE_PANIC                                         0x04  
#define COMMAND_SS_IAS_ACE_GET_ZONE_ID_MAP                               0x05  
#define COMMAND_SS_IAS_ACE_GET_ZONE_INFORMATION                          0x06  
  // Cmds, generated by Server
#define COMMAND_SS_IAS_ACE_ARM_RESPONSE                                  0x00
#define COMMAND_SS_IAS_ACE_GET_ZONE_ID_MAP_RESPONSE                      0x01 
#define COMMAND_SS_IAS_ACE_GET_ZONE_INFORMATION_RESPONSE                 0x02 
/*** Arm Mode field permitted values ***/
#define SS_IAS_ACE_ARM_DISARM                                            0x00
#define SS_IAS_ACE_ARM_DAY_HOME_ZONES_ONLY                               0x01
#define SS_IAS_ACE_ARM_NIGHT_SLEEP_ZONES_ONLY                            0x02
#define SS_IAS_ACE_ARM_ALL_ZONES                                         0x03
/*** Arm Notification field permitted values ***/
#define SS_IAS_ACE_ARM_NOTIFICATION_ALL_ZONES_DISARMED                   0x00
#define SS_IAS_ACE_ARM_NOTIFICATION_DAY_HOME_ZONES_ONLY                  0x01
#define SS_IAS_ACE_ARM_NOTIFICATION_NIGHT_SLEEP_ZONES_ONLY               0x02
#define SS_IAS_ACE_ARM_NOTIFICATION_ALL_ZONES_ARMED                      0x03

/************************************/
/***  IAS WD Cluster Attributes   ***/
/************************************/
   // Maximum Duration attribute
#define ATTRID_SS_IAS_WD_MAXIMUM_DURATION                                0x0000
  // commands:
#define COMMAND_SS_IAS_WD_START_WARNING                                  0x00
#define COMMAND_SS_IAS_WD_SQUAWK                                         0x01
/***  warningMode field values ***/
#define SS_IAS_START_WARNING_WARNING_MODE_STOP                           0
#define SS_IAS_START_WARNING_WARNING_MODE_BURGLAR                        1
#define SS_IAS_START_WARNING_WARNING_MODE_FIRE                           2
#define SS_IAS_START_WARNING_WARNING_MODE_EMERGENCY                      3
/*** start warning: strobe field values ***/
#define SS_IAS_START_WARNING_STROBE_NO_STROBE_WARNING                    0
#define SS_IAS_START_WARNING_STROBE_USE_STPOBE_IN_PARALLEL_TO_WARNING    1

/*** squawkMode field values ***/
#define SS_IAS_SQUAWK_SQUAWK_MODE_SYSTEM_ALARMED_NOTIFICATION_SOUND      0
#define SS_IAS_SQUAWK_SQUAWK_MODE_SYSTEM_DISARMED_NOTIFICATION_SOUND     1
/*** squawk strobe field values ***/
#define SS_IAS_SQUAWK_STROBE_NO_STROBE_SQUAWK                            0
#define SS_IAS_SQUAWK_STROBE_USE_STROBE_BLINK_IN_PARALLEL_TO_SQUAWK      1
/*** squawk level field values ***/
#define SS_IAS_SQUAWK_SQUAWK_LEVEL_LOW_LEVEL_SOUND                       0
#define SS_IAS_SQUAWK_SQUAWK_LEVEL_MEDIUM_LEVEL_SOUND                    1
#define SS_IAS_SQUAWK_SQUAWK_LEVEL_HIGH_LEVEL_SOUND                      2
#define SS_IAS_SQUAWK_SQUAWK_LEVEL_VERY_HIGH_LEVEL_SOUND                 3

// The maximum number of entries in the Zone table
#define ZCL_SS_MAX_ZONES                                                 256
#define ZCL_SS_MAX_ZONE_ID                                               254

#endif /* ZCL_SS_HPP */
