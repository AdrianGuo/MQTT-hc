/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name:
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-30 16:10:00 (Sun, 30 Oct 2016)
 *
 ******************************************************************************/

#ifndef JSONZWAVE_SESSION_HPP_
#define JSONZWAVE_SESSION_HPP_

#include <JsonMsgSession.hpp>
#include "typedefs.h"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class JsonRecvZwaveSession : public JsonMsgSession {
private:
    JsonRecvZwaveSession() {}
    static JsonRecvZwaveSession* m_pInstance;
public:
    static JsonRecvZwaveSession* CreateSession();
    virtual ~JsonRecvZwaveSession() {}
};

typedef JsonRecvZwaveSession  JsonRecvZwaveSession_t;
typedef JsonRecvZwaveSession* JsonRecvZwaveSession_p;


/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class JsonSendZwaveSession : public JsonMsgSession {
private:
    JsonSendZwaveSession() {}
    static JsonSendZwaveSession* m_pInstance;
public:
    static JsonSendZwaveSession* CreateSession();
    virtual ~JsonSendZwaveSession() {}
};

typedef JsonSendZwaveSession  JsonSendZwaveSession_t;
typedef JsonSendZwaveSession* JsonSendZwaveSession_p;

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class JsonSelfZwaveSession : public JsonMsgSession {
private:
    JsonSelfZwaveSession() {}
    static JsonSelfZwaveSession* m_pInstance;
public:
    static JsonSelfZwaveSession* CreateSession();
    virtual ~JsonSelfZwaveSession() {}
};

typedef JsonSelfZwaveSession  JsonSelfZwaveSession_t;
typedef JsonSelfZwaveSession* JsonSelfZwaveSession_p;


#endif /* !JSONZWAVE_SESSION_HPP_ */
