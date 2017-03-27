#ifndef DBMANAGER_HPP_
#define DBMANAGER_HPP_

#include "Typedefs.h"
#include "String.hpp"
#include "Map.hpp"
#include "Functor.hpp"
#include "ICtrller.hpp"
#include "DbModel.hpp"
#include "JsonCommand.hpp"
#include "JsonDbSession.hpp"

typedef Functor1_t(JsonCommand_p)   HandlerDbCmdFunctor_t;
typedef HandlerDbCmdFunctor_t*      HandlerDbCmdFunctor_p;

class DbManager {
public:
	DbManager();
	~DbManager();

	DbModel_p m_pDbModelDb;

	bool_t SetFunctor(CtrllerFunctor_p pRecvFunctor);
	void_t PushJsonCommand(JsonCommand_p pJsonCommand);

	void_t Debug();
private:
	CtrllerFunctor_p m_pCtrllerFunctor;

	JsonRecvDbSession_p m_pJsonRecvDbSession;
	JsonSendDbSession_p m_pJsonSendDbSession;

	Vector<DbFloor> m_Floors;
	Vector<DbRoom>  m_Rooms;

	void_t LoadDatabase();
	void_t RegisterDbSession();

public:
	void_t HandlerFloorCmdAdd(JsonCommand_p pJsonCommand);
	void_t HandlerFloorCmdEdit(JsonCommand_p pJsonCommand);
	void_t HandlerFloorCmdDel(JsonCommand_p pJsonCommand);
	void_t HandlerFloorCmdLinkRoom(JsonCommand_p pJsonCommand);
	void_t HandlerFloorCmdUlinkRoom(JsonCommand_p pJsonCommand);
	void_t HandlerFloorCmdLstRes(JsonCommand_p pJsonCommand);
	void_t HandlerFloorCmdLstRoomRes(JsonCommand_p pJsonCommand);
	void_t HandlerFloorCmdSync(JsonCommand_p pJsonCommand);
	void_t HandlerFloorCmdSyncRoom(JsonCommand_p pJsonCommand);

	void_t HandlerRoomCmdAdd(JsonCommand_p pJsonCommand);
	void_t HandlerRoomCmdEdit(JsonCommand_p pJsonCommand);
	void_t HandlerRoomCmdDel(JsonCommand_p pJsonCommand);
	void_t HandlerRoomCmdLinkDev(JsonCommand_p pJsonCommand);
	void_t HandlerRoomCmdUlinkDev(JsonCommand_p pJsonCommand);
	void_t HandlerRoomCmdLstRes(JsonCommand_p pJsonCommand);
    void_t HandlerRoomCmdLstDevRes(JsonCommand_p pJsonCommand);
    void_t HandlerRoomCmdSync(JsonCommand_p pJsonCommand);
    void_t HandlerRoomCmdSyncDev(JsonCommand_p pJsonCommand);
};

typedef DbManager  DbManager_t;
typedef DbManager* DbManager_p;

#endif /* DBMANAGER_HPP_ */
