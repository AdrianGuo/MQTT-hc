/*
 * ZbSerialAPI.hpp
 *
 *  Created on: Sep 10, 2016
 *      Author: taho
 */

#ifndef LIBRARIES_ZBDEFS_ZBSERIALAPI_HPP_
#define LIBRARIES_ZBDEFS_ZBSERIALAPI_HPP_

#define SOF1                                            0x4C  /* Start Of Frame */
#define SOF2                                            0x4D  /* Start Of Frame */
#define ACK                                             0x06  /* Acknowledge successfull frame reception */
#define NAK                                             0x15  /* Not Acknowledge successfull frame reception - please retransmit... */
#define CAN                                             0x18  /* Frame received (from host) was dropped - waiting for ACK */

/* Frame sequence */
#define MIN_SEQ                                         0x00
#define MAX_SEQ                                         0xFF


#endif /* LIBRARIES_ZBDEFS_ZBSERIALAPI_HPP_ */
