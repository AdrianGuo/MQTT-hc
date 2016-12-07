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

#ifndef JSONDB_SESSION_HPP_
#define JSONDB_SESSION_HPP_

#include "typedefs.h"
#include "JsonMessageSession.hpp"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class JsonRecvDbSession : public JsonMessageSession {
private:
    JsonRecvDbSession() {}
    static JsonRecvDbSession* m_pInstance;
public:
    static JsonRecvDbSession* CreateSession();
    virtual ~JsonRecvDbSession() {}
};

typedef JsonRecvDbSession  JsonRecvDbSession_t;
typedef JsonRecvDbSession* JsonRecvDbSession_p;


/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class JsonSendDbSession : public JsonMessageSession {
private:
    JsonSendDbSession() {}
    static JsonSendDbSession* m_pInstance;
public:
    static JsonSendDbSession* CreateSession();
    virtual ~JsonSendDbSession() {}
};

typedef JsonSendDbSession  JsonSendDbSession_t;
typedef JsonSendDbSession* JsonSendDbSession_p;


#endif /* !JSONDB_SESSION_HPP_ */
