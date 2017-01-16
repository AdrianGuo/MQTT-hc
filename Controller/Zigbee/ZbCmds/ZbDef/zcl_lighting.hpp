#ifndef ZCL_LIGHTING_HPP
#define ZCL_LIGHTING_HPP
					
/*****************************************/
/***  Color Control Cluster Attributes ***/
/*****************************************/
  // Color Information attributes set
#define ATTRID_LIGHTING_COLOR_CONTROL_CURRENT_HUE                        0x0000
#define ATTRID_LIGHTING_COLOR_CONTROL_CURRENT_SATURATION                 0x0001
#define ATTRID_LIGHTING_COLOR_CONTROL_REMAINING_TIME                     0x0002
#define ATTRID_LIGHTING_COLOR_CONTROL_CURRENT_X                          0x0003
#define ATTRID_LIGHTING_COLOR_CONTROL_CURRENT_Y                          0x0004
#define ATTRID_LIGHTING_COLOR_CONTROL_DRIFT_COMPENSATION                 0x0005
#define ATTRID_LIGHTING_COLOR_CONTROL_COMPENSATION_TEXT                  0x0006
#define ATTRID_LIGHTING_COLOR_CONTROL_COLOR_TEMPERATURE                  0x0007
#define ATTRID_LIGHTING_COLOR_CONTROL_COLOR_MODE                         0x0008
  
  // Defined Primaries Inofrmation attribute Set
#define ATTRID_LIGHTING_COLOR_CONTROL_NUM_PRIMARIES                      0x0010
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_1_X                        0x0011
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_1_Y                        0x0012
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_1_INTENSITY                0x0013
  // 0x0014 is reserved
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_2_X                        0x0015
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_2_Y                        0x0016
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_2_INTENSITY                0x0017
  // 0x0018 is reserved
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_3_X                        0x0019
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_3_Y                        0x001a
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_3_INTENSITY                0x001b

  // Additional Defined Primaries Information attribute set
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_4_X                        0x0020
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_4_Y                        0x0021
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_4_INTENSITY                0x0022
  // 0x0023 is reserved
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_5_X                        0x0024
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_5_Y                        0x0025
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_5_INTENSITY                0x0026
  // 0x0027 is reserved
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_6_X                        0x0028
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_6_Y                        0x0029
#define ATTRID_LIGHTING_COLOR_CONTROL_PRIMARY_6_INTENSITY                0x002a

  // Defined Color Points Settings attribute set
#define ATTRID_LIGHTING_COLOR_CONTROL_WHITE_POINT_X                      0x0030
#define ATTRID_LIGHTING_COLOR_CONTROL_WHITE_POINT_Y                      0x0031
#define ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_R_X                    0x0032
#define ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_R_Y                    0x0033
#define ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_R_INTENSITY            0x0034
  // 0x0035 is reserved
#define ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_G_X                    0x0036
#define ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_G_Y                    0x0037
#define ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_G_INTENSITY            0x0038
  // 0x0039 is reserved
#define ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_B_X                    0x003a
#define ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_B_Y                    0x003b
#define ATTRID_LIGHTING_COLOR_CONTROL_COLOR_POINT_B_INTENSITY            0x003c
  // 0x003d is reserved
  
  /*** Drift Compensation Attribute values ***/
#define DRIFT_COMP_NONE                                                  0x00
#define DRIFT_COMP_OTHER_UNKNOWN                                         0x01
#define DRIFT_COMP_TEMPERATURE_MONITOR                                   0x02
#define DRIFT_COMP_OPTICAL_LUMINANCE_MONITOR_FEEDBACK                    0x03
#define DRIFT_COMP_OPTICAL_COLOR_MONITOR_FEEDBACK                        0x04
  
  /*** Color Mode Attribute values ***/
#define COLOR_MODE_CURRENT_HUE_SATURATION                                0x00
#define COLOR_MODE_CURRENT_X_Y                                           0x01
#define COLOR_MODE_COLOR_TEMPERATURE                                     0x02
  
/*****************************************/
/***  Color Control Cluster Commands   ***/
/*****************************************/
#define COMMAND_LIGHTING_MOVE_TO_HUE                                     0x00
#define COMMAND_LIGHTING_MOVE_HUE                                        0x01
#define COMMAND_LIGHTING_STEP_HUE                                        0x02
#define COMMAND_LIGHTING_MOVE_TO_SATURATION                              0x03
#define COMMAND_LIGHTING_MOVE_SATURATION                                 0x04
#define COMMAND_LIGHTING_STEP_SATURATION                                 0x05
#define COMMAND_LIGHTING_MOVE_TO_HUE_AND_SATURATION                      0x06
#define COMMAND_LIGHTING_MOVE_TO_COLOR                                   0x07
#define COMMAND_LIGHTING_MOVE_COLOR                                      0x08
#define COMMAND_LIGHTING_STEP_COLOR                                      0x09
#define COMMAND_LIGHTING_MOVE_TO_COLOR_TEMPERATURE                       0x0a
  
  /***  Move To Hue Cmd payload: direction field values  ***/
#define LIGHTING_MOVE_TO_HUE_DIRECTION_SHORTEST_DISTANCE                 0x00
#define LIGHTING_MOVE_TO_HUE_DIRECTION_LONGEST_DISTANCE                  0x01
#define LIGHTING_MOVE_TO_HUE_DIRECTION_UP                                0x02
#define LIGHTING_MOVE_TO_HUE_DIRECTION_DOWN                              0x03
  /***  Move Hue Cmd payload: moveMode field values   ***/
#define LIGHTING_MOVE_HUE_STOP                                           0x00
#define LIGHTING_MOVE_HUE_UP                                             0x01
#define LIGHTING_MOVE_HUE_DOWN                                           0x03
  /***  Step Hue Cmd payload: stepMode field values ***/
#define LIGHTING_STEP_HUE_UP                                             0x01
#define LIGHTING_STEP_HUE_DOWN                                           0x03
  /***  Move Saturation Cmd payload: moveMode field values ***/
#define LIGHTING_MOVE_SATURATION_STOP                                    0x00
#define LIGHTING_MOVE_SATURATION_UP                                      0x01
#define LIGHTING_MOVE_SATURATION_DOWN                                    0x03
  /***  Step Saturation Cmd payload: stepMode field values ***/
#define LIGHTING_STEP_SATURATION_UP                                      0x01
#define LIGHTING_STEP_SATURATION_DOWN                                    0x03

/*****************************************************************************/
/***          Ballast Configuration Cluster Attributes                     ***/
/*****************************************************************************/
  // Ballast Information attribute set
#define ATTRID_LIGHTING_BALLAST_CONFIG_PHYSICAL_MIN_LEVEL                0x0000
#define ATTRID_LIGHTING_BALLAST_CONFIG_PHYSICAL_MAX_LEVEL                0x0001
#define ATTRID_LIGHTING_BALLAST_BALLAST_STATUS                           0x0002
/*** Ballast Status Attribute values (by bit number) ***/
#define LIGHTING_BALLAST_STATUS_NON_OPERATIONAL                          1 // bit 0 is set
#define LIGHTING_BALLAST_STATUS_LAMP_IS_NOT_IN_SOCKET                    2 // bit 1 is set
  // Ballast Settings attributes set
#define ATTRID_LIGHTING_BALLAST_MIN_LEVEL                                0x0010
#define ATTRID_LIGHTING_BALLAST_MAX_LEVEL                                0x0011
#define ATTRID_LIGHTING_BALLAST_POWER_ON_LEVEL                           0x0012
#define ATTRID_LIGHTING_BALLAST_POWER_ON_FADE_TIME                       0x0013
#define ATTRID_LIGHTING_BALLAST_INTRISTIC_BALLAST_FACTOR                 0x0014
#define ATTRID_LIGHTING_BALLAST_BALLAST_FACTOR_ADJUSTMENT                0x0015
  // Lamp Information attributes set
#define ATTRID_LIGHTING_BALLAST_LAMP_QUANTITY                            0x0020
  // Lamp Settings attributes set
#define ATTRID_LIGHTING_BALLAST_LAMP_TYPE                                0x0030
#define ATTRID_LIGHTING_BALLAST_LAMP_MANUFACTURER                        0x0031
#define ATTRID_LIGHTING_BALLAST_LAMP_RATED_HOURS                         0x0032
#define ATTRID_LIGHTING_BALLAST_LAMP_BURN_HOURS                          0x0033
#define ATTRID_LIGHTING_BALLAST_LAMP_ALARM_MODE                          0x0034
#define ATTRID_LIGHTING_BALLAST_LAMP_BURN_HOURS_TRIP_POINT               0x0035
/*** Lamp Alarm Mode attribute values  ***/
#define LIGHTING_BALLAST_LAMP_ALARM_MODE_BIT_0_NO_ALARM                  0
#define LIGHTING_BALLAST_LAMP_ALARM_MODE_BIT_0_ALARM                     1

#endif /* ZCL_LIGHTING_HPP */
