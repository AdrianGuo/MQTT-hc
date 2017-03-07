/*
 * SMQTT.cpp
 *
 *  Created on: Feb 11, 2017
 *      Author: taho
 */

#include <unistd.h>
#include "LogPlus.hpp"
extern "C" {
#include "iot.h"
#include "iot.pb.h"
#include "custom.pb.h"
#include "pb_decode.h"
}
#include "SMQTT.hpp"

#define BUFFER_SOCKET_SIZE	(2000)
#define KEEPALIVE_INTERVAL	(30)

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
    mqtt_init(&m_MqttBroker, "avengalvon");
    mqtt_init_auth(&m_MqttBroker, "", "");

    m_spTransport->Connect();

    if(m_iKeepAlive == -1) {
        m_iKeepAlive = m_pTimer->StartTimer(RTimer::Repeat::Forever, KEEPALIVE_INTERVAL, &m_KeepAliveFunctor, NULL);
    }
    if(m_spTransport->IsConnected() == FALSE) {
    	m_spTransport->Close();
    	return FALSE;
    }

    // MQTT stuffs
    mqtt_set_alive(&m_MqttBroker, KEEPALIVE_INTERVAL);
    m_MqttBroker.socket_info = (void*)&m_spTransport->m_idwSockfd;
    m_MqttBroker.send = SMQTT::SendMQTTPacket;

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
    mqtt_disconnect(&m_MqttBroker);

	if(m_spTransport->IsConnected() == TRUE) {
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
    int_t idwRet;

    mqtt_connect(&m_MqttBroker);

    idwRet = GetMQTTPacket(m_pbyBuffer, BUFFER_SOCKET_SIZE, TRUE);
    if(idwRet < 0)
    {
        m_pLock->Lock();
        m_boIsEstablished = FALSE;
        m_pLock->UnLock();
        LOG_WARN("Error(%d) on read packet!", idwRet);
        return FALSE;
    }

    if (MQTTParseMessageType(m_pbyBuffer) != MQTT_MSG_CONNACK)
    {
        m_pLock->Lock();
        m_boIsEstablished = FALSE;
        m_pLock->UnLock();
        LOG_WARN("CONNACK expected!");
        return FALSE;
    }

    if (m_pbyBuffer[3] != 0x00)
    {
        m_pLock->Lock();
        m_boIsEstablished = FALSE;
        m_pLock->UnLock();
        LOG_WARN("CONNACK falied!");
        return FALSE;
    }

    LOG_INFO("Successfully cmdtopic established!");
    m_pLock->Lock();
    m_boIsEstablished = TRUE;
    m_pLock->UnLock();
    return TRUE;
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
	String strDevName,
	int_t idwValue,
	bool_t IsBackup
) {
    LOG_INFO("%s: %s - %d - %d", __FUNCTION__, strDevName.c_str(), idwValue, IsBackup);

//	if (IsBackup == TRUE) {
		m_pLock->Lock();
		m_mapBackupValue[strDevName] = idwValue;
		m_pLock->UnLock();
//	}
	if (IsEstablished() == FALSE) {
		LOG_WARN("Disconnected!");
		return;
	}

    char_t pCHWID[100];
    strcpy(pCHWID, m_strHWID.c_str());

    //Import data
    int_t idwNumValue = m_mapBackupValue.size(), i = 0;
    Data data[idwNumValue];
    m_pLock->Lock();
    for(Map<String, int_t>::iterator it = m_mapBackupValue.begin(); it != m_mapBackupValue.end(); it++, i++) {
        strcpy(data[i].name, it->first.c_str());
        data[i].value = (*it).second;
    }
    m_pLock->UnLock();

	u32_t idwPayloadLen = iot_measurement2(pCHWID, idwNumValue, data, NULL, (u8_p) m_pbyBuffer, BUFFER_SOCKET_SIZE, NULL, TRUE);
	mqtt_publish(&m_MqttBroker, m_strOutboundTopic.c_str(), (const char*)m_pbyBuffer, idwPayloadLen, 0);

    //when device left network -> delete device from map
    if (idwValue == -10) {
        Map<String, int_t>::iterator it = m_mapBackupValue.find(strDevName);
        if (it != m_mapBackupValue.end()) {
            LOG_WARN("m_mapBackupValue remove %s", it->first.c_str());
            m_mapBackupValue.erase(it->first);
        }
    }
}

/**
 * @func   Subscribe
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SMQTT::Subscribe() {
    u16_t idwMsgId, idwRecvMsgId;
    int_t idwRet;
    char_t pCcmdTopicString[100];

	if (IsEstablished() == FALSE) {
		LOG_WARN("Disconnected!");
		return FALSE;
	}

    strcpy(pCcmdTopicString, m_strCmdTopic.c_str());
	mqtt_subscribe(&m_MqttBroker, m_strCmdTopic.c_str(), &idwMsgId);

	idwRet = GetMQTTPacket(m_pbyBuffer, BUFFER_SOCKET_SIZE, TRUE);
    if(idwRet < 0)
    {
        m_pLock->Lock();
        m_boIsSubscribed = FALSE;
        m_pLock->UnLock();
        LOG_WARN("Error(%d) on read packet!", idwRet);
        return FALSE;
    }

    if(MQTTParseMessageType(m_pbyBuffer) != MQTT_MSG_SUBACK)
    {
        m_pLock->Lock();
        m_boIsSubscribed = FALSE;
        m_pLock->UnLock();
        LOG_WARN("SUBACK expected!");
        return FALSE;
    }

    idwRecvMsgId = mqtt_parse_msg_id(m_pbyBuffer);
    if(idwMsgId != idwRecvMsgId)
    {
        m_pLock->Lock();
        m_boIsSubscribed = FALSE;
        m_pLock->UnLock();
        LOG_WARN("%d message id was expected, but %d message id was found!\n", idwMsgId, idwRecvMsgId);
        return FALSE;
    }

    LOG_INFO("Successfully cmdtopic subscribed!");
    m_pLock->Lock();
    m_boIsSubscribed = TRUE;
    m_pLock->UnLock();
    return TRUE;
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

	idwRet = GetMQTTPacket(m_pbyBuffer, BUFFER_SOCKET_SIZE);
	if (idwRet == -1) {
	    LOG_WARN("Error(%d) on read packet!", idwRet);
	    return;
	} else if (idwRet > 1) {
        if(MQTTParseMessageType(m_pbyBuffer) == MQTT_MSG_PUBLISH)
        {
            uint8_t topic[255], msg[1000];
            uint16_t len;
            len = mqtt_parse_pub_topic(m_pbyBuffer, topic);
            topic[len] = '\0'; // for printf
            len = mqtt_parse_publish_msg(m_pbyBuffer, msg);
            msg[len] = '\0'; // for printf
            //printf("%s %s\n", topic, msg);
            if (strcmp((const char*)(topic), m_strSysTopic.c_str()) == 0) {
                HandleSystemCommand(msg, len);
            } else if (strcmp((const char*)(topic), m_strCmdTopic.c_str()) == 0) {
                HandleSpecificCommand(msg, len);
            }
        }
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
SMQTT::SendMQTTPacket(
    void_p socket_info,
    void_p pbyBuffer,
    u32_t idwLen
) {
    return m_spTransport->DiSend((u8_p)pbyBuffer, idwLen);
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
	int_t idwLen,
	bool_t boDirect
) {
    if (boDirect) {
        int_t idwTimeout = 1;
        if(idwTimeout > 0)
        {
            fd_set readfds;
            struct timeval tmv;

            // Initialize the file descriptor set
            FD_ZERO (&readfds);
            FD_SET (m_spTransport->m_idwSockfd, &readfds);

            // Initialize the timeout data structure
            tmv.tv_sec = idwTimeout;
            tmv.tv_usec = 0;

            // select returns 0 if timeout, 1 if input available, -1 if error
            if(select(1, &readfds, NULL, NULL, &tmv))
                return -2;
        }
    }

    int idwTotalBytes = 0, idwBytesRcvd, idwPacketLength;
    memset(pbyBuffer, 0, BUFFER_SOCKET_SIZE);

    while (idwTotalBytes < 2) // Reading fixed header
    {
        if (!boDirect) {
            if ((idwBytesRcvd = m_spTransport->GetBuffer(pbyBuffer + idwTotalBytes, (u32_t) BUFFER_SOCKET_SIZE)) <= 0)
                return -1;
        } else {
            if ((idwBytesRcvd = m_spTransport->DiGet(pbyBuffer + idwTotalBytes, (u32_t) BUFFER_SOCKET_SIZE)) <= 0)
                return -1;
        }
        idwTotalBytes += idwBytesRcvd; // Keep tally of total bytes
    }

    idwPacketLength = pbyBuffer[1] + 2; // Remaining length + fixed header length

    while (idwTotalBytes < idwPacketLength) // Reading the packet
    {
        if (!boDirect) {
            if ((idwBytesRcvd = m_spTransport->GetBuffer(pbyBuffer + idwTotalBytes, (u32_t) BUFFER_SOCKET_SIZE)) <= 0)
                return -1;
        } else {
            if ((idwBytesRcvd = m_spTransport->DiGet(pbyBuffer + idwTotalBytes, (u32_t) BUFFER_SOCKET_SIZE)) <= 0)
                return -1;
        }

        idwTotalBytes += idwBytesRcvd; // Keep tally of total bytes
    }

    return idwPacketLength;
}

/**
 * @func   HandleSystemCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::HandleSystemCommand(
    u8_p pbyBuffer,
    int_t idwLen
) {
    Device_Header header;
    pb_istream_t stream = pb_istream_from_buffer(pbyBuffer, idwLen);

    // Read header to find what type of command follows.
    if (pb_decode_delimited(&stream, Device_Header_fields, &header)) {
        // Handle a registration acknowledgement.
        if (header.command == Device_Command_ACK_REGISTRATION) {
            Device_RegistrationAck ack;
            if (pb_decode_delimited(&stream, Device_RegistrationAck_fields, &ack)) {
                if (ack.state == Device_RegistrationAckState_NEW_REGISTRATION) {
                    printf("Registered new device.");
                    //registered = true;
                } else if (ack.state == Device_RegistrationAckState_ALREADY_REGISTERED) {
                    printf("Device was already registered.");
                    //registered = true;
                } else if (ack.state == Device_RegistrationAckState_REGISTRATION_ERROR) {
                    printf("Error registering device.");
                }
            }
        }
    } else {
        printf("Unable to decode system command.");
    }
}

/**
 * @func   HandleSpecificCommand
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
	    if (header.command == Custom_Command_CALL) {
	        Custom_Call call;
	        if (pb_decode_delimited(&stream, Custom_Call_fields, &call)) {
	            //handlePing(ping, header.originator);
	            CallCommand(call, header.originator);

	            //reply ack to server
	            AckKnowLedgeCommand(std::string("Call function received"), header.originator);
	        }
	    } else if (header.command == Custom_Command_SMS) {
	        Custom_Sms sms;
	        if (pb_decode_delimited(&stream, Custom_Sms_fields, &sms)) {
	            SmsCommand(sms, header.originator);

	            //reply ack to server
	            AckKnowLedgeCommand(std::string("Sms function received"), header.originator);
	        }
	    }
	}
}

/**
 * @func   PingCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::CallCommand(
	Custom_Call call,
	char_p originator
) {
	LOG_INFO("Message's content: %s", call.phone_number);
	m_strPhoneWork = String("Call_") + String(call.phone_number);
    PushNotification();
}

/**
 * @func   TestEventsCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SMQTT::SmsCommand(
	Custom_Sms sms,
	char_p originator
) {
	LOG_INFO("Message's content: %s - %s", sms.phone_number, sms.message);
    m_strPhoneWork = String("Sms_") + String(sms.phone_number) + String("_") + String(sms.message);
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
    String strCommand,
    char_p originator
) {
    if (originator != NULL)
        LOG_DEBUG("Ack KnowLedge Command - %s", strCommand.c_str());
    char_t pCHWID[100];
    strcpy(pCHWID, m_strHWID.c_str());
    char_t pCCommand[100];
    strcpy(pCCommand, strCommand.c_str());

    int_t idwLength = iot_acknowledge(pCHWID, pCCommand, m_pbyBuffer, BUFFER_SOCKET_SIZE, originator);
    mqtt_publish(&m_MqttBroker, m_strOutboundTopic.c_str(), (const char*)m_pbyBuffer, idwLength, 0);
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
//	if(m_pNotificationThread != NULL)
//		m_pNotificationThread->Start();

    LOG_DEBUG(" %s: \"%s\"", __FUNCTION__, m_strPhoneWork.c_str());
    Phone::getInstant()->AddWork(m_strPhoneWork);
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
	if ((m_spTransport->IsConnected() == FALSE) || !m_boIsEstablished || !m_boIsSubscribed) {
		LOG_INFO("Trying to reconnect !!!");
		Close();
		if(Connect() == FALSE) {
			LOG_INFO("Wait for next try!!!");
			Close();
			return;
		} else {
		    Start();
		    Subscribe();
			LOG_INFO("Connected!");
			if (m_mapBackupValue.size() > 0) {
                Map<String, int_t>::const_iterator it = m_mapBackupValue.begin();
                if (it != m_mapBackupValue.end()) {
                    String strName = String(it->first);
                    int_t idwValue = it->second;
                    Publish(strName, idwValue);
                }
			}
		}
	} else {
//	    AckKnowLedgeCommand(std::string("Keep alive"), NULL);
	    mqtt_ping(&m_MqttBroker);
	}
}
