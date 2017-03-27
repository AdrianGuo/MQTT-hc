/*
 * JsonZigbeeSession.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_ZIGBEE_JSONZIGBEESESSION_HPP_
#define CONTROLLER_ZIGBEE_JSONZIGBEESESSION_HPP_

#include <JsonMsgSession.hpp>
#include "Typedefs.h"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class JsonRecvZigbeeSession : public JsonMsgSession {
private:
    JsonRecvZigbeeSession() {}
    static JsonRecvZigbeeSession* m_pInstance;
public:
    static JsonRecvZigbeeSession* CreateSession();
    virtual ~JsonRecvZigbeeSession() {}
};

typedef JsonRecvZigbeeSession  JsonRecvZigbeeSession_t;
typedef JsonRecvZigbeeSession* JsonRecvZigbeeSession_p;


/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class JsonSendZigbeeSession : public JsonMsgSession {
private:
    JsonSendZigbeeSession() {}
    static JsonSendZigbeeSession* m_pInstance;
public:
    static JsonSendZigbeeSession* CreateSession();
    virtual ~JsonSendZigbeeSession() {}
};

typedef JsonSendZigbeeSession  JsonSendZigbeeSession_t;
typedef JsonSendZigbeeSession* JsonSendZigbeeSession_p;

#endif /* CONTROLLER_ZIGBEE_JSONZIGBEESESSION_HPP_ */
