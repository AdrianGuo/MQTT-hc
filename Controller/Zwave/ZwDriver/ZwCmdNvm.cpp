#include "HelperHc.hpp"
#include "LogPlus.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwPacket.hpp"
#include "ValuePtr.hpp"
#include "ZwCmdNvm.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdNvm::ZwCmdNvm(
) : IZwDriver(),
    m_ValueLstNode (ValueLstNode::GetInstance()),
    m_ValueZwDriver (ValueRef<ValueZwDriver>::GetInstance()) {
    m_pValueZwCmdNvm    = ValuePtr<ValueZwCmdNvm>::GetInstance();
    m_pJsonZwaveSession = JsonSendZwaveSession::CreateSession();
    m_pZwDbModel        = ZwDbModel::CreateModel("zwave.db");
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
ZwCmdNvm::~ZwCmdNvm() { }

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdNvm::HandleMemoryGetIdResponse(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_ValueZwDriver.expectedFuncId == pZwPacket->GetFunctionId()) {
        u32_t dwHomeId = (pZwPacket->GetBuffer()[0] << 24) |
                         (pZwPacket->GetBuffer()[1] << 16) |
                         (pZwPacket->GetBuffer()[2] << 8)  |
                         (pZwPacket->GetBuffer()[3]);
        u8_t byNodeId = pZwPacket->GetBuffer()[4];

        String strHomeId = GetHex(dwHomeId);

        m_pValueZwCmdNvm->homeId = dwHomeId;
        m_pValueZwCmdNvm->nodeId = byNodeId;

        ZwDbController controllerfind = m_pZwDbModel->Find<ZwDbCtrllerItem>();

        if (controllerfind.get() != NULL) {
            ZwDbController controller = m_pZwDbModel->Find<ZwDbCtrllerItem>().
            Where("HomeId = ?").Bind(strHomeId);

            if (controller.get() == NULL) {
                controllerfind.Modify()->HomeId = dwHomeId;
                controllerfind.Modify()->HexHom = strHomeId;
                controllerfind.Modify()->NodeId = byNodeId;
                m_pZwDbModel->Add(controllerfind);
            }
        } else {
            ZwDbController controlleradd =
            m_pZwDbModel->Add(new ZwDbCtrllerItem());
            controlleradd.Modify()->HomeId = dwHomeId;
            controlleradd.Modify()->HexHom = strHomeId;
            controlleradd.Modify()->NodeId = byNodeId;
            m_pZwDbModel->UpdateChanges();
        }

        m_ValueZwDriver.expectedCbakId = 0;
        m_ValueZwDriver.expectedNodeId = 0;
        m_ValueZwDriver.expectedFuncId = 0;
        m_ValueZwDriver.expectedCmdCId = 0;
        m_ValueZwDriver.packetSignal->Set();
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdNvm::RegisterHandlers() {
    // Response
    m_pHandlerResponse->RegisterHandler(FUNC_ID_MEMORY_GET_ID,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdNvm::HandleMemoryGetIdResponse));
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_MEMORY_GET_BYTE,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleReplaceFailedNodeResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_MEMORY_PUT_BYTE,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetRoutingMaxResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_MEMORY_GET_BUFFER,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetSucNodeIdResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_MEMORY_PUT_BUFFER,
//            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdNvm::HandleMemoryGetIdResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_NVM_GET_ID,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleReplaceFailedNodeResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_NVM_EXT_READ_LONG_BYTE,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetRoutingMaxResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_NVM_EXT_WRITE_LONG_BYTE,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetSucNodeIdResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_NVM_EXT_READ_LONG_BUFFER,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetRoutingMaxResponse)); //
//    m_pHandlerResponse->RegisterHandler(FUNC_ID_NVM_EXT_WRITE_LONG_BUFFER,
//        makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdCtrller::HandleSetSucNodeIdResponse)); //

    // Request
    m_pHandlerRequest->RegisterHandler(FUNC_ID_MEMORY_PUT_BUFFER,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwCmdNvm::HandleMemoryPutBufferRequest));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdNvm::HandleMemoryPutBufferRequest(
    ZwPacket_p pZwPacket
) {

}
