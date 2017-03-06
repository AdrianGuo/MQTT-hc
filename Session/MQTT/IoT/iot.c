#include "iot.h"
#include "iot.pb.h"
#include "../Protobuf/pb_encode.h"
#include "double_conversion.h"

// Signals end of stream.
uint8_t zero = 0;

unsigned int iot_register(char* hardwareId, char* specificationToken, uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IotEvent_Header header = { };
	header.command = IotEvent_Command_SEND_REGISTRATION;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IotEvent_Header_fields, &header)) {
		return 0;
	}

	IotEvent_RegisterDevice registerDevice = { };
	strcpy(registerDevice.hardwareId, hardwareId);
	strcpy(registerDevice.specificationToken, specificationToken);
	if (!pb_encode_delimited(&stream, IotEvent_RegisterDevice_fields, &registerDevice)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int iot_acknowledge(char* hardwareId, char* message, uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IotEvent_Header header = { };
	header.command = IotEvent_Command_SEND_ACKNOWLEDGEMENT;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IotEvent_Header_fields, &header)) {
		return 0;
	}

	IotEvent_Acknowledge ack = { };
	strcpy(ack.hardwareId, hardwareId);
	if (message != NULL) {
		ack.has_message = true;
		strcpy(ack.message, message);
	}
	if (!pb_encode_delimited(&stream, IotEvent_Acknowledge_fields, &ack)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int iot_measurement(char* hardwareId, char* name, float value, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator, bool updateState) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IotEvent_Header header = { };
	header.command = IotEvent_Command_SEND_DEVICE_MEASUREMENTS;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IotEvent_Header_fields, &header)) {
		return 0;
	}

	Model_DeviceMeasurements measurements = { };
	strcpy(measurements.hardwareId, hardwareId);

	Model_Measurement measurement = { };
	strcpy(measurement.measurementId, name);
	measurement.measurementValue = float_to_double(value);
	measurements.measurement[0] = measurement;
	measurements.measurement_count = 1;
	measurements.has_updateState = true;
	measurements.updateState = updateState;

	if (eventDate != NULL) {
		measurements.has_eventDate = true;
		measurements.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, Model_DeviceMeasurements_fields, &measurements)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int iot_measurement2(char* hardwareId, uint8_t count, Data* data, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator, bool updateState) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IotEvent_Header header = { };
	header.command = IotEvent_Command_SEND_DEVICE_MEASUREMENTS;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IotEvent_Header_fields, &header)) {
		return 0;
	}
	Model_DeviceMeasurements measurements = { };
	strcpy(measurements.hardwareId, hardwareId);
	int i;
	for(i=0;i<count;i++) {
		Model_Measurement measurement = { };
		strcpy(measurement.measurementId, data->name);
		measurement.measurementValue = data->value;
		measurements.measurement[i] = measurement;
		data++;
	}
	measurements.measurement_count = count;
	measurements.has_updateState = true;
	measurements.updateState = updateState;

	if (eventDate != NULL) {
		measurements.has_eventDate = true;
		measurements.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, Model_DeviceMeasurements_fields, &measurements)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int iot_location(char* hardwareId, float lat, float lon, float ele, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator, bool updateState) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IotEvent_Header header = { };
	header.command = IotEvent_Command_SEND_DEVICE_LOCATION;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IotEvent_Header_fields, &header)) {
		return 0;
	}

	Model_DeviceLocation location = { };
	strcpy(location.hardwareId, hardwareId);
	location.latitude = float_to_double(lat);
	location.longitude = float_to_double(lon);
	location.elevation = float_to_double(ele);
	location.has_elevation = true;
	location.has_updateState = true;
	location.updateState = updateState;

	if (eventDate != NULL) {
		location.has_eventDate = true;
		location.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, Model_DeviceLocation_fields, &location)) {
		return 0;
	}

	return stream.bytes_written;
}

unsigned int iot_alert(char* hardwareId, char* alertType, char* alertMessage, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator, bool updateState) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	IotEvent_Header header = { };
	header.command = IotEvent_Command_SEND_DEVICE_ALERT;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, IotEvent_Header_fields, &header)) {
		return 0;
	}

	Model_DeviceAlert alert = { };
	strcpy(alert.hardwareId, hardwareId);
	strcpy(alert.alertType, alertType);
	strcpy(alert.alertMessage, alertMessage);
	alert.has_updateState = true;
	alert.updateState = updateState;
	if (eventDate != NULL) {
		alert.has_eventDate = true;
		alert.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, Model_DeviceAlert_fields, &alert)) {
		return 0;
	}

	return stream.bytes_written;
}
