
#ifndef ZCL_CLOSURES_HPP
#define ZCL_CLOSURES_HPP

/**********************************************/
/*** Shade Configuration Cluster Attributes ***/
/**********************************************/
  // Shade information attributes set
#define ATTRID_CLOSURES_PHYSICAL_CLOSED_LIMIT                          0x0000
#define ATTRID_CLOSURES_MOTOR_STEP_SIZE                                0x0001
#define ATTRID_CLOSURES_STATUS                                         0x0002
/*** Status attribute bit values ***/
#define CLOSURES_STATUS_SHADE_IS_OPERATIONAL                           0x01
#define CLOSURES_STATUS_SHADE_IS_ADJUSTING                             0x02
#define CLOSURES_STATUS_SHADE_DIRECTION                                0x04
#define CLOSURES_STATUS_SHADE_MOTOR_FORWARD_DIRECTION                  0x08
  // Shade settings attributes set
#define ATTRID_CLOSURES_CLOSED_LIMIT                                   0x0010
#define ATTRID_CLOSURES_MODE                                           0x0012
/*** Mode attribute values ***/
#define CLOSURES_MODE_NORMAL_MODE                                      0x00
#define CLOSURES_MODE_CONFIGURE_MODE                                   0x01
// cluster has no specific commands

/**********************************************/
/*** Logical Cluster ID - for mapping only ***/
/***  These are not to be used over-the-air ***/
/**********************************************/
#define ZCL_CLOSURES_LOGICAL_CLUSTER_ID_SHADE_CONFIG                   0x0010

/**********************************************/
/***            Door Attributes             ***/
/**********************************************/
#define ATTRID_CLOSURES_LOCK_STATE                                    0x0000
#define ATTRID_CLOSURES_LOCK_TYPE                                     0x0001
#define ATTRID_CLOSURES_ACTUATOR_ENABLED                              0x0002
#define ATTRID_CLOSURES_DOOR_STATE                                    0x0003
#define ATTRID_CLOSURES_DOOR_OPEN_EVENTS                              0x0004
#define ATTRID_CLOSURES_DOOR_CLOSED_EVENTS                            0x0005
#define ATTRID_CLOSURES_OPEN_PERIOD                                   0x0006

#endif /* ZCL_CLOSURES_HPP */
