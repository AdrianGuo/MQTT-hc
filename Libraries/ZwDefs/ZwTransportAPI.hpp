#ifndef ZWTRANSPORTAPI_HPP_
#define ZWTRANSPORTAPI_HPP_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/


/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/* Max number of nodes in a Z-wave system */
#define ZW_MAX_NODES                                    232

/************************************************************/
/* ZwNode Information frame*/
/************************************************************/
#define NODEPARM_MAX                                    35 /* max. number of parameters */

/* Transmit frame option flags */
#define TRANSMIT_OPTION_ACK                             0x01 /* request acknowledge from destination node */
#define TRANSMIT_OPTION_LOW_POWER                       0x02 /* transmit AtPosition low output power level (1/3 of normal RF range)*/

#ifdef ZW_SLAVE
#define TRANSMIT_OPTION_RETURN_ROUTE                    0x04 /* request transmission via return route */
#endif /* ZW_SLAVE */

#define TRANSMIT_OPTION_AUTO_ROUTE                      0x04 /* request retransmission via repeater nodes */
/* do not use response route - Even if available */
#define TRANSMIT_OPTION_NO_ROUTE                        0x10
/* Use explore frame if needed */
#define TRANSMIT_OPTION_EXPLORE                         0x20

/* Transmit frame option flag which are valid when sending explore frames  */
#define TRANSMIT_EXPLORE_OPTION_ACK                     TRANSMIT_OPTION_ACK
#define TRANSMIT_EXPLORE_OPTION_LOW_POWER               TRANSMIT_OPTION_LOW_POWER

/* Received frame status flags */
/**
 * \defgroup RECEIVE_STATUS Status codes for receiving frames.
 * \addtogroup RECEIVE_STATUS
 * @{
 */

/**
 * A response route is locked by the application
 */
#define RECEIVE_STATUS_ROUTED_BUSY                      0x01

/**
 * Received AtPosition low output power level, this must
 * have the same value as TRANSMIT_OPTION_LOW_POWER
 */
#define RECEIVE_STATUS_LOW_POWER                        0x02
/**
 * Mask for masking out the received frametype bits
 */
#define RECEIVE_STATUS_TYPE_MASK                        0x0C
/**
 * Received frame is singlecast frame (rxOptions == xxxx00xx)
 */
#define RECEIVE_STATUS_TYPE_SINGLE                      0x00
/**
 * Received frame is broadcast frame  (rxOptions == xxxx01xx)
 */
#define RECEIVE_STATUS_TYPE_BROAD                       0x04
/**
 * Received frame is multicast frame (rxOptions == xxxx10xx)
 * Only TYPE_BROAD can be active AtPosition the same time as TYPE_EXPLORE
 */
#define RECEIVE_STATUS_TYPE_MULTI                       0x08
/**
 * Received frame is an explore frame (rxOptions == xxx1xxxx)
 * Only TYPE_BROAD can be active AtPosition the same time as TYPE_EXPLORE
 */
#define RECEIVE_STATUS_TYPE_EXPLORE                     0x10
/** 
 * Received frame is not send to me (useful only in promiscuous mode)
 */
#define RECEIVE_STATUS_FOREIGN_FRAME                    0x40

/* Predefined ZwNode ID's */
#define NODE_BROADCAST                                  0xFF /* broadcast */
#define ZW_TEST_NOT_A_NODEID                            0x00

/* Transmit complete codes */
#define TRANSMIT_COMPLETE_OK                            0x00
#define TRANSMIT_COMPLETE_NO_ACK                        0x01 /* retransmission error */
#define TRANSMIT_COMPLETE_FAIL                          0x02 /* transmit error */
#define TRANSMIT_ROUTING_NOT_IDLE                       0x03 /* transmit error */

#ifdef ZW_CONTROLLER
/* Assign route transmit complete but no routes was found */
#define TRANSMIT_COMPLETE_NOROUTE                       0x04  /* no route found in assignroute */
/* therefore nothing was transmitted */
#endif /* ZW_CONTROLLER */

/* ZW_REDISCOVERY_NEEDED callback values. */
/* Note that they are different from ZW_REQUEST_NETWORK_UPDATE callbacks */
#define ZW_ROUTE_LOST_FAILED                            0x04 /* ZwNode Asked wont help us */
#define ZW_ROUTE_LOST_ACCEPT                            0x05 /* Accepted to help */

#define ZW_MAX_CACHED_RETURN_ROUTE_DESTINATIONS         5

#define ZW_RF_TEST_SIGNAL_CARRIER                       0x00
#define ZW_RF_TEST_SIGNAL_CARRIER_MODULATED             0x01

/* Max hops in route */
#define MAX_REPEATERS                                   4

/* TX_STATUS_TYPE Last Used Route array size definitions */
#define LAST_USED_ROUTE_CONF_SIZE                       1
#define LAST_USED_ROUTE_SIZE                            (MAX_REPEATERS + LAST_USED_ROUTE_CONF_SIZE)

/* TX_STATUS_TYPE Last Used Route array index definitions */
#define LAST_USED_ROUTE_REPEATER_0_INDEX                0
#define LAST_USED_ROUTE_REPEATER_1_INDEX                1
#define LAST_USED_ROUTE_REPEATER_2_INDEX                2
#define LAST_USED_ROUTE_REPEATER_3_INDEX                3
#define LAST_USED_ROUTE_CONF_INDEX                      4

/* RSSI feedback constants */
/**
 * This is a signed 8-bit value. Note that values from RSSI_RESERVED_START to
 * 124 are reserved. All values below RSSI_RESERVED_START are received power
 * in dBms. Other values are defined below.
 */
#define RSSI_NOT_AVAILABLE                              127 /* RSSI measurement not available */
#define RSSI_MAX_POWER_SATURATED                        126 /* Receiver saturated. RSSI too high to measure precisely. */
#define RSSI_BELOW_SENSITIVITY                          125 /* No signal detected. The RSSI is too low to measure precisely. */
#define RSSI_RESERVED_START                             11 /* All values above and including RSSI_RESERVED_START are reserved, except those defined above. */

#define ZWAVE_PLUS_TX_OPTIONS                           (TRANSMIT_OPTION_ACK | TRANSMIT_OPTION_AUTO_ROUTE | TRANSMIT_OPTION_EXPLORE)

#endif /* !ZWTRANSPORTAPI_HPP_ */
