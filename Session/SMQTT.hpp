/*
 * SMQTT.hpp
 *
 *  Created on: Feb 11, 2017
 *      Author: taho
 */

#ifndef SESSION_SMQTT_HPP_
#define SESSION_SMQTT_HPP_

extern "C" {
#include "libemqtt.h"
}

#include "Phone.hpp"
#include "Transport.hpp"
#include "RTimer.hpp"
#include "LThread.hpp"
#include "custom.pb.h"
#include "Map.hpp"

class SMQTT {
private:
	String m_strHWID;
	String m_strTenant;
	String m_strPhoneWork;
	String m_strDev;
	String m_strCmdTopic;
	String m_strSysTopic;
	String m_strOutboundTopic;
    u8_p m_pbyBuffer;

    static Transport_p m_spTransport;

    mqtt_broker_handle_t m_MqttBroker;

    Map<String, int_t> m_mapBackupValue;

    SMQTTFunctor_t m_SMQTTSendFunctor;

    bool_t m_boIsEstablished;
    bool_t m_boIsSubscribed;
    Locker_p m_pLock;

    LThread_p m_pNotificationThread;
    threadFunctor_t m_NotificationFunctor;

    RTimer_p m_pTimer;
    int_t m_iKeepAlive;
    TimerFunctor_t m_KeepAliveFunctor;

	SMQTT(const_char_p, int_t, String, String, String, String);
	void_t SMQTTSendFunctor();

    static int_t GetMQTTPacket(u8_p, int_t, bool_t boDirect = FALSE);
    static int_t SendMQTTPacket(void_p, void_p, u32_t);
	void_t RecvData();
	void_t HandleSystemCommand(u8_p, int_t);
	void_t HandleSpecificCommand(u8_p, int_t);
	void_t CallCommand(Custom_Call, char_p);
	void_t SmsCommand(Custom_Sms, char_p);
    void_p NotifyFunc(void_p);
    void_t HandleKeepAlive(void_p);
    void_t AckKnowLedgeCommand(String, char_p);

public:
	virtual ~SMQTT();

    static SMQTT* s_pInstance;
    static SMQTT* GetInstance(const_char_p, int_t, String, String, String, String);

	bool_t Connect();
    bool_t Start();
    bool_t Close();
    bool_t Establish();
    bool_t Subscribe();

    bool_t IsEstablished();
    void_t Publish(String strDevName, int_t idwValue, bool_t IsBackup = TRUE);
    bool_t IsSubscribed();

    void_t PushNotification();
};

typedef SMQTT SMQTT_t;
typedef SMQTT* SMQTT_p;

#endif /* SESSION_SMQTT_HPP_ */
