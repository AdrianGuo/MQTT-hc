#include "iot.h"
#include "iot.pb.h"
#include "double_conversion.h"

#include "../Protobuf/pb_encode.h"

// Signals end of stream.
uint8_t zero = 0;

unsigned int sw_register(char* hardwareId, char* specificationToken, uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IoT_Header header = { };
	header.command = IoT_Command_SEND_REGISTRATION;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IoT_Header_fields, &header)) {
		return 0;
	}

	IoT_RegisterDevice registerDevice = { };
	strcpy(registerDevice.hardwareId, hardwareId);
	strcpy(registerDevice.specificationToken, specificationToken);
	if (!pb_encode_delimited(&stream, IoT_RegisterDevice_fields, &registerDevice)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int sw_register2(char* hardwareId, char* specificationToken, char* siteToken, uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IoT_Header header = { };
	header.command = IoT_Command_SEND_REGISTRATION;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IoT_Header_fields, &header)) {
		return 0;
	}

	IoT_RegisterDevice registerDevice = { };
	registerDevice.has_siteToken = true;
	strcpy(registerDevice.hardwareId, hardwareId);
	strcpy(registerDevice.specificationToken, specificationToken);
	strcpy(registerDevice.siteToken, siteToken);
	if (!pb_encode_delimited(&stream, IoT_RegisterDevice_fields, &registerDevice)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int sw_acknowledge(char* hardwareId, char* message, uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IoT_Header header = { };
	header.command = IoT_Command_SEND_ACKNOWLEDGEMENT;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IoT_Header_fields, &header)) {
		return 0;
	}

	IoT_Acknowledge ack = { };
	strcpy(ack.hardwareId, hardwareId);
	if (message != NULL) {
		ack.has_message = true;
		strcpy(ack.message, message);
	}
	if (!pb_encode_delimited(&stream, IoT_Acknowledge_fields, &ack)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int sw_measurement(const char* hardwareId, const char* name, float value, int64_t eventDate, uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IoT_Header header = { };
	header.command = IoT_Command_SEND_DEVICE_MEASUREMENTS;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IoT_Header_fields, &header)) {
		return 0;
	}

	Model_DeviceMeasurements measurements = { };
	strcpy(measurements.hardwareId, hardwareId);

	Model_Measurement measurement = { };
	strcpy(measurement.measurementId, name);
	measurement.measurementValue = float_to_double(value);
	measurements.measurement[0] = measurement;
	measurements.measurement_count = 1;

	if (eventDate != NULL) {
		measurements.has_eventDate = true;
		measurements.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, Model_DeviceMeasurements_fields, &measurements)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int sw_measurement2(char* hardwareId, char* name1, float value1, char* name2, float value2, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IoT_Header header = { };
	header.command = IoT_Command_SEND_DEVICE_MEASUREMENTS;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IoT_Header_fields, &header)) {
		return 0;
	}

	Model_DeviceMeasurements measurements = { };
	strcpy(measurements.hardwareId, hardwareId);

	Model_Measurement measurement1 = { };
	strcpy(measurement1.measurementId, name1);
	measurement1.measurementValue = float_to_double(value1);
	measurements.measurement[0] = measurement1;
	
	Model_Measurement measurement2 = { };
	strcpy(measurement2.measurementId, name2);
	measurement2.measurementValue = float_to_double(value2);
	measurements.measurement[1] = measurement2;
	measurements.measurement_count = 2;

	if (eventDate != NULL) {
		measurements.has_eventDate = true;
		measurements.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, Model_DeviceMeasurements_fields, &measurements)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int sw_measurement3(char* hardwareId, char* name1, float value1, char* name2, float value2, char* name3, float value3, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IoT_Header header = { };
	header.command = IoT_Command_SEND_DEVICE_MEASUREMENTS;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IoT_Header_fields, &header)) {
		return 0;
	}

	Model_DeviceMeasurements measurements = { };
	strcpy(measurements.hardwareId, hardwareId);

	Model_Measurement measurement1 = { };
	strcpy(measurement1.measurementId, name1);
	measurement1.measurementValue = float_to_double(value1);
	measurements.measurement[0] = measurement1;
	
	Model_Measurement measurement2 = { };
	strcpy(measurement2.measurementId, name2);
	measurement2.measurementValue = float_to_double(value2);
	measurements.measurement[1] = measurement2;
	
	Model_Measurement measurement3 = { };
	strcpy(measurement3.measurementId, name3);
	measurement3.measurementValue = float_to_double(value3);
	measurements.measurement[2] = measurement3;
	
	measurements.measurement_count = 3;

	if (eventDate != NULL) {
		measurements.has_eventDate = true;
		measurements.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, Model_DeviceMeasurements_fields, &measurements)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int sw_location(char* hardwareId, float lat, float lon, float ele, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IoT_Header header = { };
	header.command = IoT_Command_SEND_DEVICE_LOCATION;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IoT_Header_fields, &header)) {
		return 0;
	}

	Model_DeviceLocation location = { };
	strcpy(location.hardwareId, hardwareId);
	location.latitude = float_to_double(lat);
	location.longitude = float_to_double(lon);
	location.elevation = float_to_double(ele);
	location.has_elevation = true;

	if (eventDate != NULL) {
		location.has_eventDate = true;
		location.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, Model_DeviceLocation_fields, &location)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int sw_alert(char* hardwareId, char* alertType, char* alertMessage, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IoT_Header header = { };
	header.command = IoT_Command_SEND_DEVICE_ALERT;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IoT_Header_fields, &header)) {
		return 0;
	}

	Model_DeviceAlert alert = { };
	strcpy(alert.hardwareId, hardwareId);
	strcpy(alert.alertType, alertType);
	strcpy(alert.alertMessage, alertMessage);
	if (eventDate != NULL) {
		alert.has_eventDate = true;
		alert.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, Model_DeviceAlert_fields, &alert)) {
		return 0;
	}

	return stream.bytes_written;
}
