/*
 * JsonZigbeeSession.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_ZIGBEE_JSONZIGBEESESSION_HPP_
#define CONTROLLER_ZIGBEE_JSONZIGBEESESSION_HPP_

#include "typedefs.h"
#include "JsonMessageSession.hpp"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class JsonRecvZigbeeSession : public JsonMessageSession {
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

class JsonSendZigbeeSession : public JsonMessageSession {
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
