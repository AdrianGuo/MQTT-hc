/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: JsonDevSession.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 5 Dec 2016 09:33:48
 *
 ******************************************************************************/
#ifndef JSONDEV_SESSION_HPP_
#define JSONDEV_SESSION_HPP_

#include "typedefs.h"
#include "JsonMessageSession.hpp"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class JsonRecvDevSession : public JsonMessageSession {
private:
    JsonRecvDevSession() {}
    static JsonRecvDevSession* m_pInstance;
public:
    static JsonRecvDevSession* CreateSession();
    virtual ~JsonRecvDevSession() {}
};

typedef JsonRecvDevSession  JsonRecvDevSession_t;
typedef JsonRecvDevSession* JsonRecvDevSession_p;


/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class JsonSendDevSession : public JsonMessageSession {
private:
    JsonSendDevSession() {}
    static JsonSendDevSession* m_pInstance;
public:
    static JsonSendDevSession* CreateSession();
    virtual ~JsonSendDevSession() {}
};

typedef JsonSendDevSession  JsonSendDevSession_t;
typedef JsonSendDevSession* JsonSendDevSession_p;

#endif /* !JSONDEV_SESSION_HPP_ */
