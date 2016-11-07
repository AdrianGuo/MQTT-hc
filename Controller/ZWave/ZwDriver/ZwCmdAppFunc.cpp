#include "debug.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwCtrllerAPI.hpp"
#include "ZwPacket.hpp"
#include "String.hpp"
#include "ZwNode.hpp"
#include "JsonCommand.hpp"
#include "JsonMessagePtr.hpp"
#include "JsonDevRep.hpp"
#include "JsonZwLstAdd.hpp"
#include "JsonZwLstDel.hpp"
#include "ValueShare.hpp"
#include "ZwCmdAppFunc.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdAppFunc::ProcResetLocally(
    u8_t byNodeID
) {
    if (m_ppValueZwNode[byNodeID - 1] != NULL) {
        delete m_ppValueZwNode[byNodeID - 1];
        m_ppValueZwNode[byNodeID - 1] = NULL;
    }

    Vector<u8_t> vectorId;
    vectorId.push_back(byNodeID);
    JsonMessagePtr<JsonZwLstDel> jsonZwLstDel =
                            m_pJsonZwaveSession->GetJsonMapping<JsonZwLstDel>();
    JsonCommand_p pJsonCommand = jsonZwLstDel->CreateJsonCommand(vectorId);
    pJsonCommand->SetSrcFlag(JsonCommand::Flag::Zwave);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);

    if (m_pZwCtrllerCallbackFunctor != NULL) {
        m_pZwCtrllerCallbackFunctor->operator ()(EvAction::None, pJsonCommand);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdAppFunc::ProcDeviceReport(
    u8_t byNodeId,
    u8_p pbyInBuffer,
    u8_t byLength
) {
    if ((m_pZwCtrllerCallbackFunctor == NULL) || (m_ppValueZwNode[byNodeId - 1] == NULL)) { return; }

    ZwNode_p pZwNode = m_ppValueZwNode[byNodeId - 1]->GetValue();
    Value_p pOutValue = pZwNode->ApplicationCommandHandler(pbyInBuffer, byLength);

    if (pOutValue != NULL) {
        u8_t byValue;
        if (pOutValue->GetType() == ValueType_Bool) {
            byValue = ((ValueBool_p) pOutValue)->GetValue();
        } else if (pOutValue->GetType() == ValueType_Byte){
            byValue = ((ValueByte_p) pOutValue)->GetValue();
        }

        JsonMessagePtr<JsonDevRep> jsonDevRep = m_pJsonZwaveSession->GetJsonMapping<JsonDevRep>();
        JsonCommand_p pJsonCommand = jsonDevRep->CreateJsonCommand(byNodeId, byValue);
        pJsonCommand->SetSrcFlag(JsonCommand::Flag::Zwave);
        pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);

        delete pOutValue;
        pOutValue = NULL;

        m_pZwCtrllerCallbackFunctor->operator ()(EvAction::None, pJsonCommand);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t 
ZwCmdAppFunc::HandleApplicationCommandHandlerRequest(
    ZwPacket_p pZwPacket
) {
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    u8_t byNodeID   = pZwPacket->GetBuffer()[1];
    u8_t byLength   = pZwPacket->GetBuffer()[2];
    u8_t byCmdClass = pZwPacket->GetBuffer()[3];

    ZwNode_p pZwNode = NULL;

    if (m_ppValueZwNode[byNodeID - 1] != NULL) {
        pZwNode = m_ppValueZwNode[byNodeID - 1]->GetValue();
    }

    if (pZwNode == NULL) { return; }

    if ((FUNC_ID_APPLICATION_COMMAND_HANDLER == m_pValueZwDriver->ByExpectedFunctionID) &&
        (m_pValueZwDriver->ByExpectedNodeID == byNodeID)) { // From Get Command
        m_pValueZwDriver->ByExpectedCallbackID = 0;
        m_pValueZwDriver->ByExpectedNodeID = 0;
        m_pValueZwDriver->ByExpectedFunctionID = 0;
        m_pValueZwDriver->ByExpectedCmdClassID = 0;
        m_pValueZwDriver->EvPacketSignal->Set();
    } else {
        pZwNode->IncrementUnsolicitedCount();
    }

    if (!pZwNode->IsNodeAlive()) {
        pZwNode->SetNodeAlive(TRUE);
    }

    if (byCmdClass == COMMAND_CLASS_DEVICE_RESET_LOCALLY) {
        ProcResetLocally(byNodeID);
        return;
    }

    ProcDeviceReport(byNodeID, &pZwPacket->GetBuffer()[3], byLength);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t 
ZwCmdAppFunc::HandleApplicationNodeInformationRequest(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdAppFunc::HandleApplicationSlaveUpdateRequest(
    ZwPacket_p pZwPacket
) {
    if ((m_pValueZwDriver == NULL) || (pZwPacket == NULL)) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    u8_p pBuffer = pZwPacket->GetBuffer();
    u8_t byNodeID   = pBuffer[1]; // bSource
    u8_t byLength   = pBuffer[2];
    u8_t byBasic    = pBuffer[3];
    u8_t byGeneric  = pBuffer[4];
    u8_t bySpecific = pBuffer[5];

    if ((byNodeID != 0) && (byNodeID != 0xFF) && (m_ppValueZwNode != NULL)) {
        DEBUG2("node info %d", byNodeID);

        if (m_ppValueZwNode[byNodeID - 1] != NULL) {
            m_ppValueZwNode[byNodeID - 1]->GetValue()->SetNodeID(byNodeID);
            m_ppValueZwNode[byNodeID - 1]->GetValue()->SetBasic(byBasic);
            m_ppValueZwNode[byNodeID - 1]->GetValue()->SetGeneric(byGeneric);
            m_ppValueZwNode[byNodeID - 1]->GetValue()->SetSpecific(bySpecific);

            for (u8_t i = 0; i < byLength - 2; i++) {
                m_ppValueZwNode[byNodeID - 1]->GetValue()->AddZwCmdClass(pBuffer[6 + i]);
            }
        }
    }
    m_pValueZwDriver->EvPacketSignal->Set();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdAppFunc::HandleApplicationControllerUpdateRequest(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdAppFunc::ZwCmdAppFunc() {
    m_pValueZwDriver    = ValueShare<ValueZwDriver>::GetInstance();
    m_pValueZwAppFunc   = ValueShare<ValueZwAppFunc>::GetInstance();
    m_ppValueZwNode     = NULL;

    m_pZwCtrllerCallbackFunctor = NULL;
    m_pJsonZwaveSession = JsonSendZwaveSession::CreateSession();

    m_pHandlerRequest   = HandlerRequest::GetInstance();
    m_pHandlerResponse  = HandlerResponse::GetInstance();
    RegisterHandlers();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdAppFunc::~ZwCmdAppFunc() { }

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdAppFunc::RegisterHandlers() {
    m_pHandlerRequest->RegisterHandler(FUNC_ID_APPLICATION_COMMAND_HANDLER,
            makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdAppFunc::HandleApplicationCommandHandlerRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_SERIAL_API_APPL_NODE_INFORMATION,
            makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdAppFunc::HandleApplicationNodeInformationRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_APPLICATION_UPDATE,
            makeFunctor((HandlerRequestFunctor_p) NULL, *this, &ZwCmdAppFunc::HandleApplicationSlaveUpdateRequest));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdAppFunc::ShareNodeObject(
    void_p pObject
) {
    m_ppValueZwNode = (ValueZwNode_pp) pObject;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdAppFunc::ShareCallbackFunctor(
    void_p pCallbackFunctor
) {
    m_pZwCtrllerCallbackFunctor = (ZwCtrllerFunctor_p) pCallbackFunctor;
}
