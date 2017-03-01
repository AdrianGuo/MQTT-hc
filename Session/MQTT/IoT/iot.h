/**
 * sitewhere.h - Library for interacting with IoT using Google Protocol Buffers.
 * See https://developers.google.com/protocol-buffers/ for details on protocol buffers.
 * The lightweight C implementation was generated with nanopb (http://code.google.com/p/nanopb/).
 *
 * Copyright 2013-2014 Reveal Technologies LLC.
 */
#ifndef iot_h
#define iot_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Create an encoded registration message for sending to IoT */
unsigned int sw_register(char* hardwareId, char* specificationToken, uint8_t* buffer, size_t length,
		char* originator);
		
/** Create an encoded registration message for sending to IoT */
unsigned int sw_register2(char* hardwareId, char* specificationToken, char* siteToken, uint8_t* buffer, size_t length,
		char* originator);

/** Create an encoded acknowledgement message for sending to IoT */
unsigned int sw_acknowledge(char* hardwareId, char* message, uint8_t* buffer, size_t length, char* originator);

/** Create an encoded measurement message for sending to IoT */
unsigned int sw_measurement(const char* hardwareId, const char* name, float value, int64_t eventDate, uint8_t* buffer, size_t length, char* originator);
		
/** Create an encoded measurement message for sending to IoT */
unsigned int sw_measurement2(char* hardwareId, char* name1, float value1, char* name2, float value2, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator);
		
/** Create an encoded measurement message for sending to IoT */
unsigned int sw_measurement3(char* hardwareId, char* name1, float value1, char* name2, float value2, char* name3, float value3, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator);

/** Create an encoded location message for sending to IoT */
unsigned int sw_location(char* hardwareId, float lat, float lon, float elevation, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator);

/** Create an encoded alert message for sending to IoT */
unsigned int sw_alert(char* hardwareId, char* type, char* message, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
