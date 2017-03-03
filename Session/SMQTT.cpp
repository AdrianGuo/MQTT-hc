/*
 * SMQTT.cpp
 *
 *  Created on: Feb 11, 2017
 *      Author: taho
 */

#include <unistd.h>
#include "LogPlus.hpp"
#include "MQTTPacket.h"
#include "iot.h"
#include "custom.pb.h"
#include "pb_decode.h"

#include "SMQTT.hpp"

#define BUFFER_SOCKET_SIZE	(200)
#define KEEPALIVE_INTERVAL	(10)

SMQTT* SMQTT::s_pInstance = NULL;
Transport_p SMQTT::m_spTransport = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SMQTT::SMQTT(
	const_char_p pChostname,
	int_t idwPort,
	String strTenant,
	String strPhoneNo,
	String strHWID,
	String strDev
) {
	m_strTenant = strTenant;
	m_strPhoneWork = strPhoneNo;
	m_strHWID = strHWID;
	m_strDev = strDev;
	m_strOutboundTopic = m_strTenant + "/input/protobuf";
	m_strCmdTopic = m_strTenant + "/commands/" + m_strHWID;
	m_strSysTopic = m_strTenant + "/system/" + m_strHWID;

	m_spTransport = Transport::getInstant(pChostname, idwPort);

    m_pbyBuffer = new u8_t[BUFFER_SOCKET_SIZE];
    memset(m_pbyBuffer, '\0', BUFFER_SOCKET_SIZE);

	m_boIsEstablished = FALSE;
	m_boIsSubscribed = FALSE;
	m_pLock = new Locker();

    m_pNotificationThread = new LThread();
    m_NotificationFunctor = makeFunctor((threadFunctor_p) NULL, *this, &SMQTT::NotifyFunc);
    m_pNotificationThread->RegThreadFunctor(&m_NotificationFunctor);

	m_pTimer = RTimer::getTimerInstance();
	m_KeepAliveFunctor = makeFunctor((TimerFunctor_p) NULL, *this, &SMQTT::HandleKeepAlive);
    m_iKeepAlive = -1;

    m_SMQTTSendFunctor = makeFunctor((SMQTTFunctor_p) NULL, *this, &SMQTT::RecvData);
    SMQTTSendFunctor();

    Phone::getInstant();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SMQTT* SMQTT::GetInstance(
	const_char_p pChostname,
	int_t idwPort,
	String strTenant,
	String strPhoneNo,
	String strHWID,
	String strDev
) {
    if (s_pInstance == NULL) {
        s_pInstance = new SMQTT(pChostname, idwPort, strTenant, strPhoneNo, strHWID, strDev);
    }
    return s_pInstance;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SMQTT::~SMQTT() {
	delete m_pLock;
	delete m_pNotificationThread;
	delete m_spTransport;
	delete m_pbyBuffer;
}

/**
 * @func   Start
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SMQTT::Start() {
    return m_spTransport->Start();
}

/**
 * @func   Connect
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SMQTT::Connect() {
    m_spTransport->Connect();

    if(m_iKeepAlive == -1) {
        m_iKeepAlive = m_pTimer->StartTimer(RTimer::Repeat::Forever, KEEPALIVE_INTERVAL, &m_KeepAliveFunctor, NULL);
    }
    if(m_spTransport->IsConnected() == FALSE) {
    	m_spTransport->Close();
    	return FALSE;
    }
    m_spTransport->SetNonBlocking();
    if(Establish() == TRUE)
    	Subscribe();
    return (m_boIsEstablished && m_boIsSubscribed);
}

/**
 * @func   Close
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SMQTT::Close() {
	if(m_spTransport->IsConnected() == TRUE) {
		m_pLock->Lock();
		int_t idwLen = MQTTSerialize_disconnect(m_pbyBuffer, BUFFER_SOCKET_SIZE);
		int_t idwRet = m_spTransport->DiSend(m_pbyBuffer, idwLen);
		memset(m_pbyBuffer, '\0', BUFFER_SOCKET_SIZE);
		m_pLock->UnLock();
		if (idwRet == idwLen)
			LOG_INFO("Successfully disconnected!");
		else
			LOG_WARN("Disconnect failed!");
	    return m_spTransport->Close();
	}
	return TRUE;
}

/**
 * @func   SClientSendFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::SMQTTSendFunctor() {
    m_spTransport->RecvFunctor(&m_SMQTTSendFunctor);
}

/**
 * @func   Establish
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SMQTT::Establish() {
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	data.clientID.cstring = "me";
	data.keepAliveInterval = 20;
	data.cleansession = 1;
	data.username.cstring = "";
	data.password.cstring = "";
	data.MQTTVersion = 4;

	bool_t boRet = FALSE;
	m_pLock->Lock();
	int_t idwLen = MQTTSerialize_connect(m_pbyBuffer, BUFFER_SOCKET_SIZE, &data);
	int_t idwRet = m_spTransport->DiSend(m_pbyBuffer, idwLen);
	memset(m_pbyBuffer, '\0', BUFFER_SOCKET_SIZE);
	m_pLock->UnLock();
	if (idwRet == idwLen) {
		boRet = TRUE;
		m_pLock->Lock();
		m_boIsEstablished = TRUE;
		m_pLock->UnLock();
		LOG_INFO("MQTT Established!");
	}
	else {
		m_pLock->Lock();
		m_boIsEstablished = FALSE;
		m_pLock->UnLock();
		LOG_WARN("MQTT Unestablished!");
	}
    return boRet;
}

/**
 * @func   IsEstablished
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SMQTT::IsEstablished() {
	return (m_spTransport->IsConnected() && m_boIsEstablished);
}

/**
 * @func   Publish
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::Publish(
	const_char_p pCDevName,
	int_t idwValue,
	bool_t IsBackup
) {
	if(IsBackup == TRUE) {
		m_pLock->Lock();
		m_mapBackupValue[pCDevName] = idwValue;
		m_pLock->UnLock();
	}
	if (IsEstablished() == FALSE) {
		LOG_WARN("Disconnected!");
		return;
	}
	MQTTString outboundTopicString = MQTTString_initializer;
	outboundTopicString.cstring = m_strOutboundTopic.c_str();
	u8_t payload[BUFFER_SOCKET_SIZE];
	u32_t idwPayloadLen = sw_measurement(m_strHWID.c_str(), pCDevName, idwValue, NULL, (u8_p) payload, sizeof(payload), NULL);
	if (idwPayloadLen) {
		m_pLock->Lock();
		int_t idwRet = MQTTSerialize_publish(m_pbyBuffer, BUFFER_SOCKET_SIZE, 0, 0, 0, 0, outboundTopicString, payload, idwPayloadLen);
		m_spTransport->PushBuffer(m_pbyBuffer, idwRet);
		memset(m_pbyBuffer, '\0', BUFFER_SOCKET_SIZE);
		m_pLock->UnLock();
		LOG_INFO("Publish in queue!");
	} else
		LOG_WARN("sw_measurement error!");
}

/**
 * @func   Subscribe
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::Subscribe() {
	if (IsEstablished() == FALSE) {
		LOG_WARN("Disconnected!");
		return;
	}
	MQTTString cmdTopicString = MQTTString_initializer;
	int_t msgid = 1;
	int_t req_qos = 0;
	cmdTopicString.cstring = m_strCmdTopic.c_str();
	m_pLock->Lock();
	int_t idwLen = MQTTSerialize_subscribe(m_pbyBuffer, BUFFER_SOCKET_SIZE, 0, msgid, 1, &cmdTopicString, &req_qos);
	int_t idwRet = m_spTransport->DiSend(m_pbyBuffer, idwLen);
	memset(m_pbyBuffer, '\0', BUFFER_SOCKET_SIZE);
	m_pLock->UnLock();
	if (idwRet == idwLen) {
		m_pLock->Lock();
		m_boIsSubscribed = TRUE;
		m_pLock->UnLock();
		LOG_INFO("Successfully cmdtopic subscribed!");
	}
	else {
		m_pLock->Lock();
		m_boIsSubscribed = FALSE;
		m_pLock->UnLock();
		LOG_WARN("Subscribe failed!");
	}
	MQTTString sysTopicString = MQTTString_initializer;
	sysTopicString.cstring = m_strSysTopic.c_str();
	m_pLock->Lock();
	idwLen = MQTTSerialize_subscribe(m_pbyBuffer, BUFFER_SOCKET_SIZE, 0, msgid, 1, &sysTopicString, &req_qos);
	idwRet = m_spTransport->DiSend(m_pbyBuffer, idwLen);
	memset(m_pbyBuffer, '\0', BUFFER_SOCKET_SIZE);
	m_pLock->UnLock();
	if (idwRet == idwLen) {
		m_pLock->Lock();
		m_boIsSubscribed = TRUE;
		m_pLock->UnLock();
		LOG_INFO("Successfully systopic subscribed!");
	}
	else {
		m_pLock->Lock();
		m_boIsSubscribed = FALSE;
		m_pLock->UnLock();
		LOG_WARN("Subscribe failed!");
	}
}

/**
 * @func   IsSubscribed
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SMQTT::IsSubscribed() {
	return m_boIsSubscribed;
}

/**
 * @func   RecvData
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::RecvData(
) {
    int_t idwRet;
	m_pLock->Lock();
	idwRet = MQTTPacket_read(m_pbyBuffer, BUFFER_SOCKET_SIZE, SMQTT::GetMQTTPacket);
	if (idwRet == PUBLISH)
	{
		u8_t dup;
		int_t qos;
		u8_t retained;
		u16_t msgid;
		int_t payloadlen_in;
		u8_p payload_in;
		int_t rc;
		MQTTString receivedTopic;

		rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
				&payload_in, &payloadlen_in, m_pbyBuffer, BUFFER_SOCKET_SIZE);
		if(rc == 1) {
			LOG_INFO("Message arrived.");
			HandleSpecificCommand(payload_in, payloadlen_in);
		}
//		delete payload_in;
	}
	else {
	    LOG_INFO("MQTTPacket_read idwRet = %d", idwRet);
	}
	memset(m_pbyBuffer, '\0', BUFFER_SOCKET_SIZE);
	m_pLock->UnLock();
}

/**
 * @func   SClientSendFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
SMQTT::GetMQTTPacket(
	u8_p pbyBuffer,
	int_t idwLen
) {
    return m_spTransport->GetBuffer(pbyBuffer, (u32_t) idwLen);
}

/**
 * @func   SClientSendFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::HandleSpecificCommand(
	u8_p pbyBuffer,
	int_t idwLen
) {
	Custom__Header header;
	pb_istream_t stream = pb_istream_from_buffer(pbyBuffer, idwLen);
	if (pb_decode_delimited(&stream, Custom__Header_fields, &header)) {
		LOG_DEBUG("Decoded header for custom command.");
		if (header.command == Custom_Command_PING) {
			LOG_INFO("Handling ping command...");
			Custom_ping ping;
			if (pb_decode_delimited(&stream, Custom_ping_fields, &ping)) {
				PingCommand(ping, header.originator);
			}
		} else if (header.command == Custom_Command_TESTEVENTS) {
			LOG_INFO("Handling testEvents command...");
			Custom_testEvents testEvents;
			if (pb_decode_delimited(&stream, Custom_testEvents_fields,
					&testEvents)) {
				TestEventsCommand(testEvents, header.originator);
			}
		} else if (header.command == Custom_Command_SERIALPRINTLN) {
			LOG_INFO("Handling serialPrintln command...");
			Custom_serialPrintln serialPrintln;
			if (pb_decode_delimited(&stream, Custom_serialPrintln_fields,
					&serialPrintln)) {
				SerialPrintlnCommand(serialPrintln, header.originator);
			}
		}
		//reply ack to server
		AckKnowLedgeCommand(std::string("Call function received"), header.originator);
	}
}

/**
 * @func   PingCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::PingCommand(
	Custom_ping ping,
	char_p originator
) {
	LOG_DEBUG("Handling Ping command...");
}

/**
 * @func   TestEventsCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::TestEventsCommand(
	Custom_testEvents testEvents,
	char_p originator
) {
	LOG_DEBUG("Handling TestEvents command...");
}

/**
 * @func   SerialPrintlnCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::SerialPrintlnCommand(
	Custom_serialPrintln serialPrintln,
	char_p originator
) {
	LOG_DEBUG("Handling SerialPrintln command...");
	LOG_INFO("Message's content: %s", serialPrintln.message);
	if(String(serialPrintln.message) != "")
		m_strPhoneWork = String(serialPrintln.message);
	PushNotification();
}

/**
 * @func   AckKnowLedgeCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::AckKnowLedgeCommand(
    String command,
    char_p originator
) {
    if (originator != NULL)
        LOG_DEBUG("Ack KnowLedge Command");
//    else
//        LOG_DEBUG("Keep alive command");
    u8_t payload[BUFFER_SOCKET_SIZE];
    char_t pCHWID[100];
    strcpy(pCHWID, m_strHWID.c_str());
    char_t pCCommand[100];
    strcpy(pCCommand, command.c_str());

    MQTTString outboundTopicString = MQTTString_initializer;
    outboundTopicString.cstring = m_strOutboundTopic.c_str();

    u32_t idwPayloadLen = sw_acknowledge(pCHWID, pCCommand, payload, sizeof(payload), originator);
    if (idwPayloadLen) {
        int_t idwRet = MQTTSerialize_publish(m_pbyBuffer, BUFFER_SOCKET_SIZE, 0, 0, 0, 0, outboundTopicString, payload, idwPayloadLen);
        m_spTransport->PushBuffer(m_pbyBuffer, idwRet);
        memset(m_pbyBuffer, '\0', BUFFER_SOCKET_SIZE);
//        LOG_INFO("Publish in queue!");
    } else {
        LOG_WARN("sw_measurement error!");
    }
}

/**
 * @func   Notify
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
SMQTT::NotifyFunc(
	void_p byBuffer
) {
    LOG_DEBUG(" %s: \"%s\"", __FUNCTION__, m_strPhoneWork.c_str());
	Phone::getInstant()->AddWork(m_strPhoneWork);

    m_pNotificationThread->Stop();
    return NULL;
}

/**
 * @func   PushNotification
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::PushNotification() {
	if(m_pNotificationThread != NULL)
		m_pNotificationThread->Start();
}

/**
 * @func   HandleKeepAlive
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::HandleKeepAlive(
	void_p pbyBuffer
) {
//    LOG_INFO("Handling KeepAlive...");
	if(m_spTransport->IsConnected() == FALSE) {
		LOG_INFO("Trying to reconnect !!!");
		Close();
		if(Connect() == FALSE) {
			LOG_INFO("Wait for next try!!!");
			Close();
			return;
		} else {
		    Start();
			LOG_INFO("Connected!");
			for(Map<const_char_p, int_t>::const_iterator it = m_mapBackupValue.begin(); it != m_mapBackupValue.end(); it++) {
			    Publish(it->first, it->second, FALSE);
			    m_mapBackupValue.erase(it->first);
			}
		}
	} else {
	    AckKnowLedgeCommand(std::string("Keep alive"), NULL);
	}
}
