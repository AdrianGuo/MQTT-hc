/*
 * JsonRecvFileSession.hpp
 *
 *  Created on: Feb 23, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_FILE_JSONFILESESSION_HPP_
#define CONTROLLER_FILE_JSONFILESESSION_HPP_

#include <JsonMsgSession.hpp>
#include "typedefs.h"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class JsonRecvFileSession : public JsonMsgSession {
private:
    JsonRecvFileSession() {}
    static JsonRecvFileSession* m_pInstance;
public:
    static JsonRecvFileSession* CreateSession();
    virtual ~JsonRecvFileSession() {}
};

typedef JsonRecvFileSession  JsonRecvFileSession_t;
typedef JsonRecvFileSession* JsonRecvFileSession_p;

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class JsonSendFileSession : public JsonMsgSession {
private:
    JsonSendFileSession() {}
    static JsonSendFileSession* m_pInstance;
public:
    static JsonSendFileSession* CreateSession();
    virtual ~JsonSendFileSession() {}
};

typedef JsonSendFileSession  JsonSendFileSession_t;
typedef JsonSendFileSession* JsonSendFileSession_p;

#endif /* CONTROLLER_FILE_JSONFILESESSION_HPP_ */
