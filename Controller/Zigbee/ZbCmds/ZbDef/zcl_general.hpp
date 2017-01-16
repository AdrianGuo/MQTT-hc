
#ifndef ZCL_GENERAL_HPP
#define ZCL_GENERAL_HPP

/*********************************************************************
 * CONSTANTS
 */
#define ZCL_SCENE_NAME_LEN                                16

/********************************/
/*** Basic Cluster Attributes ***/
/********************************/
  // Basic Device Information
#define ATTRID_BASIC_ZCL_VERSION                          0x0000
#define ATTRID_BASIC_APPL_VERSION                         0x0001
#define ATTRID_BASIC_STACK_VERSION                        0x0002
#define ATTRID_BASIC_HW_VERSION                           0x0003
#define ATTRID_BASIC_MANUFACTURER_NAME                    0x0004
#define ATTRID_BASIC_MODEL_ID                             0x0005
#define ATTRID_BASIC_DATE_CODE                            0x0006
#define ATTRID_BASIC_POWER_SOURCE                         0x0007
  // Basic Device Settings
#define ATTRID_BASIC_LOCATION_DESC                        0x0010
#define ATTRID_BASIC_PHYSICAL_ENV                         0x0011
#define ATTRID_BASIC_DEVICE_ENABLED                       0x0012
#define ATTRID_BASIC_ALARM_MASK                           0x0013

/*** Power Source Attribute values ***/
  // Bits b0-b6 represent the primary power source of the device
#define POWER_SOURCE_UNKNOWN                              0x00
#define POWER_SOURCE_MAINS_1_PHASE                        0x01
#define POWER_SOURCE_MAINS_3_PHASE                        0x02
#define POWER_SOURCE_BATTERY                              0x03
#define POWER_SOURCE_DC                                   0x04
#define POWER_SOURCE_EMERG_MAINS_CONST_PWR                0x05
#define POWER_SOURCE_EMERG_MAINS_XFER_SW                  0x06
  // Bit b7 indicates whether the device has a secondary power source in the
  // form of a battery backup

/*** Power Source Attribute bits  ***/
#define POWER_SOURCE_PRIMARY                              0x7F
#define POWER_SOURCE_SECONDARY                            0x80

/*** Physical Environment Attribute values ***/
#define PHY_UNSPECIFIED_ENV                               0x00
  // Specified per Profile 0x01-0x7F
#define PHY_UNKNOWN_ENV                                   0xFF

/*** Device Enable Attribute values ***/
#define DEVICE_DISABLED                                   0x00
#define DEVICE_ENABLED                                    0x01

/*** Alarm Mask Attribute bits ***/
#define ALARM_MASK_GEN_HW_FAULT                           0x01
#define ALARM_MASK_GEN_SW_FAULT                           0x02

/******************************/
/*** Basic Cluster Commands ***/
/******************************/
#define COMMAND_BASIC_RESET_FACT_DEFAULT                  0x00

/**********************************************/
/*** Power Configuration Cluster Attributes ***/
/**********************************************/
  // Mains Information
#define ATTRID_POWER_CFG_MAINS_VOLTAGE                    0x0000
#define ATTRID_POWER_CFG_MAINS_FREQUENCY                  0x0001
  // Mains Settings
#define ATTRID_POWER_CFG_MAINS_ALARM_MASK                 0x0010
#define ATTRID_POWER_CFG_MAINS_VOLT_MIN_THRES             0x0011
#define ATTRID_POWER_CFG_MAINS_VOLT_MAX_THRES             0x0012
#define ATTRID_POWER_CFG_MAINS_DWELL_TRIP_POINT           0x0013
  // Battery Information
#define ATTRID_POWER_CFG_BATTERY_VOLTAGE                  0x0020
#define ATTRID_POWER_CFG_BATTERY_PERCENTAGE               0x0021
  // Battery Settings
#define ATTRID_POWER_CFG_BAT_MANU                         0x0030
#define ATTRID_POWER_CFG_BAT_SIZE                         0x0031
#define ATTRID_POWER_CFG_BAT_AHR_RATING                   0x0032
#define ATTRID_POWER_CFG_BAT_QUANTITY                     0x0033
#define ATTRID_POWER_CFG_BAT_RATED_VOLTAGE                0x0034
#define ATTRID_POWER_CFG_BAT_ALARM_MASK                   0x0035
#define ATTRID_POWER_CFG_BAT_VOLT_MIN_THRES               0x0036

/*** Mains Alarm Mask Attribute bit ***/
#define MAINS_ALARM_MASK_VOLT_2_LOW                       0x01
#define MAINS_ALARM_MASK_VOLT_2_HI                        0x02

/*** Battery Size Attribute values ***/
#define BAT_SIZE_NO_BATTERY                               0x00
#define BAT_SIZE_BUILT_IN                                 0x01
#define BAT_SIZE_OTHER                                    0x02
#define BAT_SIZE_AA                                       0x03
#define BAT_SIZE_AAA                                      0x04
#define BAT_SIZE_C                                        0x05
#define BAT_SIZE_D                                        0x06
#define BAT_SIZE_UNKNOWN                                  0xFF

/*** Batter Alarm Mask Attribute bit ***/
#define BAT_ALARM_MASK_VOLT_2_LOW                         0x01

/********************************************/
/*** Power Configuration Cluster Commands ***/
/********************************************/
  // No cluster specific commands

/***********************************************************/
/*** Device Temperature Configuration Cluster Attributes ***/
/***********************************************************/
  // Device Temperature Information
#define ATTRID_DEV_TEMP_CURRENT                           0x0000
#define ATTRID_DEV_TEMP_MIN_EXPERIENCED                   0x0001
#define ATTRID_DEV_TEMP_MAX_EXPERIENCED                   0x0002
#define ATTRID_DEV_TEMP_OVER_TOTAL_DWELL                  0x0003
  // Device Temperature Settings
#define ATTRID_DEV_TEMP_ALARM_MASK                        0x0010
#define ATTRID_DEV_TEMP_LOW_THRES                         0x0011
#define ATTRID_DEV_TEMP_HI_THRES                          0x0012
#define ATTRID_DEV_TEMP_LOW_DWELL_TRIP_POINT              0x0013
#define ATTRID_DEV_TEMP_HI_DWELL_TRIP_POINT               0x0014

/*** Device Temp Alarm_Mask Attribute bits ***/
#define DEV_TEMP_ALARM_MASK_2_LOW                         0x01
#define DEV_TEMP_ALARM_MASK_2_HI                          0x02

/**********************************************/
/***     Binary Input Cluster Attributes    ***/
/**********************************************/
#define ATTRID_BINARY_INPUT_PRESENT_VALUE                 0x0055

/*********************************************************/
/*** Device Temperature Configuration Cluster Commands ***/
/*********************************************************/
  // No cluster specific commands

/***********************************/
/*** Identify Cluster Attributes ***/
/***********************************/
#define ATTRID_IDENTIFY_TIME                             0x0000

/*********************************/
/*** Identify Cluster Commands ***/
/*********************************/
#define COMMAND_IDENTIFY                                 0x00
#define COMMAND_IDENTIFY_QUERY                           0x01

#define COMMAND_IDENTIFY_QUERY_RSP                       0x00

/********************************/
/*** Group Cluster Attributes ***/
/********************************/
#define ATTRID_GROUP_NAME_SUPPORT                         0x0000

/******************************/
/*** Group Cluster Commands ***/
/******************************/
#define COMMAND_GROUP_ADD                                 0x00
#define COMMAND_GROUP_VIEW                                0x01
#define COMMAND_GROUP_GET_MEMBERSHIP                      0x02
#define COMMAND_GROUP_REMOVE                              0x03
#define COMMAND_GROUP_REMOVE_ALL                          0x04
#define COMMAND_GROUP_ADD_IF_IDENTIFYING                  0x05

#define COMMAND_GROUP_ADD_RSP                             0x00
#define COMMAND_GROUP_VIEW_RSP                            0x01
#define COMMAND_GROUP_GET_MEMBERSHIP_RSP                  0x02
#define COMMAND_GROUP_REMOVE_RSP                          0x03

/*********************************/
/*** Scenes Cluster Attributes ***/
/*********************************/
  // Scene Management Information
#define ATTRID_SCENES_COUNT                               0x0000
#define ATTRID_SCENES_CURRENT_SCENE                       0x0001
#define ATTRID_SCENES_CURRENT_GROUP                       0x0002
#define ATTRID_SCENES_SCENE_VALID                         0x0003
#define ATTRID_SCENES_NAME_SUPPORT                        0x0004
#define ATTRID_SCENES_LAST_CFG_BY                         0x0005

/*******************************/
/*** Scenes Cluster Commands ***/
/*******************************/
#define COMMAND_SCENE_ADD                                 0x00
#define COMMAND_SCENE_VIEW                                0x01
#define COMMAND_SCENE_REMOVE                              0x02
#define COMMAND_SCENE_REMOVE_ALL                          0x03
#define COMMAND_SCENE_STORE                               0x04
#define COMMAND_SCENE_RECALL                              0x05
#define COMMAND_SCENE_GET_MEMBERSHIP                      0x06

#define COMMAND_SCENE_ADD_RSP                             0x00
#define COMMAND_SCENE_VIEW_RSP                            0x01
#define COMMAND_SCENE_REMOVE_RSP                          0x02
#define COMMAND_SCENE_REMOVE_ALL_RSP                      0x03
#define COMMAND_SCENE_STORE_RSP                           0x04
#define COMMAND_SCENE_GET_MEMBERSHIP_RSP                  0x06

/*********************************/
/*** On/Off Cluster Attributes ***/
/*********************************/
#define ATTRID_ON_OFF                                     0x0000

/*******************************/
/*** On/Off Cluster Commands ***/
/*******************************/
#define COMMAND_OFF                                       0x00
#define COMMAND_ON                                        0x01
#define COMMAND_TOGGLE                                    0x02

/****************************************/
/*** On/Off Switch Cluster Attributes ***/
/****************************************/
  // Switch Information
#define ATTRID_ON_OFF_SWITCH_TYPE                         0x0000
  // Switch Settings
#define ATTRID_ON_OFF_SWITCH_ACTIONS                      0x0010

/*** On Off Switch Type attribute values ***/
#define ON_OFF_SWITCH_TYPE_TOGGLE                         0x00
#define ON_OFF_SWITCH_TYPE_MOMENTARY                      0x01

/*** On Off Switch Actions attribute values ***/
#define ON_OFF_SWITCH_ACTIONS_0                           0x00
#define ON_OFF_SWITCH_ACTIONS_1                           0x01
#define ON_OFF_SWITCH_ACTIONS_2                           0x02

/**************************************/
/*** On/Off Switch Cluster Commands ***/
/**************************************/
  // No cluster specific commands

/****************************************/
/*** Level Control Cluster Attributes ***/
/****************************************/
#define ATTRID_LEVEL_CURRENT_LEVEL                        0x0000
#define ATTRID_LEVEL_REMAINING_TIME                       0x0001
#define ATTRID_LEVEL_ON_OFF_TRANSITION_TIME               0x0010
#define ATTRID_LEVEL_ON_LEVEL                             0x0011

/**************************************/
/*** Level Control Cluster Commands ***/
/**************************************/
#define COMMAND_LEVEL_MOVE_TO_LEVEL                       0x00
#define COMMAND_LEVEL_MOVE                                0x01
#define COMMAND_LEVEL_STEP                                0x02
#define COMMAND_LEVEL_STOP                                0x03
#define COMMAND_LEVEL_MOVE_TO_LEVEL_WITH_ON_OFF           0x04
#define COMMAND_LEVEL_MOVE_WITH_ON_OFF                    0x05
#define COMMAND_LEVEL_STEP_WITH_ON_OFF                    0x06
#define COMMAND_LEVEL_STOP_WITH_ON_OFF                    0x07

/*** Level Control Move (Mode) Command values ***/
#define LEVEL_MOVE_UP                                     0x00
#define LEVEL_MOVE_DOWN                                   0x01

/*** Level Control Step (Mode) Command values ***/
#define LEVEL_STEP_UP                                     0x00
#define LEVEL_STEP_DOWN                                   0x01

/*********************************/
/*** Alarms Cluster Attributes ***/
/*********************************/
  // Alarm Information
#define ATTRID_ALARM_COUNT                                0x0000

/*******************************/
/*** Alarms Cluster Commands ***/
/*******************************/
#define COMMAND_ALARMS_RESET                              0x00
#define COMMAND_ALARMS_RESET_ALL                          0x01
#define COMMAND_ALARMS_GET                                0x02
#define COMMAND_ALARMS_RESET_LOG                          0x03

#define COMMAND_ALARMS_ALARM                              0x00
#define COMMAND_ALARMS_GET_RSP                            0x01

/*******************************/
/*** Time Cluster Attributes ***/
/*******************************/
#define ATTRID_TIME_TIME                                  0x00
#define ATTRID_TIME_STATUS                                0x01

/*** TimeStatus Attribute bits ***/
#define TIME_STATUS_MASTER                                0x01
#define TIME_STATUS_SYNCH                                 0x02

/*****************************/
/*** Time Cluster Commands ***/
/*****************************/
  // No cluster specific commands

/***********************************/
/*** RSSI Location Cluster Attributes ***/
/***********************************/
  // Location Information
#define ATTRID_LOCATION_TYPE                              0x0000
#define ATTRID_LOCATION_METHOD                            0x0001
#define ATTRID_LOCATION_AGE                               0x0002
#define ATTRID_LOCATION_QUALITY_MEASURE                   0x0003
#define ATTRID_LOCATION_NUM_DEVICES                       0x0004

  // Location Settings
#define ATTRID_LOCATION_COORDINATE1                       0x0010
#define ATTRID_LOCATION_COORDINATE2                       0x0011
#define ATTRID_LOCATION_COORDINATE3                       0x0012
#define ATTRID_LOCATION_POWER                             0x0013
#define ATTRID_LOCATION_PATH_LOSS_EXPONENT                0x0014
#define ATTRID_LOCATION_REPORT_PERIOD                     0x0015
#define ATTRID_LOCATION_CALC_PERIOD                       0x0016
#define ATTRID_LOCATION_NUM_RSSI_MEASUREMENTS             0x0017

/*** Location Type attribute bits ***/
#define LOCATION_TYPE_ABSOLUTE                            0x01
#define LOCATION_TYPE_2_D                                 0x02
#define LOCATION_TYPE_COORDINATE_SYSTEM                   0x0C

/*** Location Method attribute values ***/
#define LOCATION_METHOD_LATERATION                        0x00
#define LOCATION_METHOD_SIGNPOSTING                       0x01
#define LOCATION_METHOD_RF_FINGER_PRINT                   0x02
#define LOCATION_METHOD_OUT_OF_BAND                       0x03

/*********************************/
/*** Location Cluster Commands ***/
/*********************************/
#define COMMAND_LOCATION_SET_ABSOLUTE                      0x00
#define COMMAND_LOCATION_SET_DEV_CFG                       0x01
#define COMMAND_LOCATION_GET_DEV_CFG                       0x02
#define COMMAND_LOCATION_GET_DATA                          0x03

#define COMMAND_LOCATION_DEV_CFG_RSP                       0x00
#define COMMAND_LOCATION_DATA_RSP                          0x01
#define COMMAND_LOCATION_DATA_NOTIF                        0x02
#define COMMAND_LOCATION_COMPACT_DATA_NOTIF                0x03
#define COMMAND_LOCATION_RSSI_PING                         0x04

// The maximum number of characters to allow in a scene's name
// remember that the first byte is the length
#define ZCL_GEN_SCENE_NAME_LEN                           16

// The maximum length of the scene extension field:
//   2 + 1 + 1 for On/Off cluster (onOff attibute)
//   2 + 1 + 1 for Level Control cluster (currentLevel attribute)
//   2 + 1 + 4 for Color Control cluster (currentX/currentY attributes)
#define ZCL_GEN_SCENE_EXT_LEN                            15
  
// The maximum number of entries in the Scene table
#define ZCL_GEN_MAX_SCENES                               16

#endif /* ZCL_GENERAL_HPP */
