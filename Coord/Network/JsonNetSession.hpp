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
 * Last Changed:     Date: 2016-10-31 11:45:00 (Mon, 31 Oct 2016)
 *
 ******************************************************************************/
#ifndef JSON_NETSESSION_HPP_
#define JSON_NETSESSION_HPP_

#include "typedefs.h"
#include "JsonMessageSession.hpp"

class JsonNetSession : public JsonMessageSession {
private:
    JsonNetSession() {}
    static JsonNetSession* m_instance;
public:
    static JsonNetSession* CreateSession();
    virtual ~JsonNetSession() {}
};

typedef JsonNetSession  JsonNetSession_t;
typedef JsonNetSession* JsonNetSession_p;

#endif /* !JSON_NETSESSION_HPP_ */
