#include <stddef.h>
#include "LogPlus.hpp"

#include "ZwSerialAPI.hpp"

#include "ValueByte.hpp"
#include "ValueDevice.hpp"
#include "ValueSwitch.hpp"
#include "ValueDimmer.hpp"
#include "ValuePirSensor.hpp"

#include "AssociationCmdClass.hpp"
#include "BasicCmdClass.hpp"
#include "BinarySwitchCmdClass.hpp"
#include "MultiChannelCmdClass.hpp"
#include "MultilevelSwitchCmdClass.hpp"
#include "ZwavePlusInfoCmdClass.hpp"
#include "ZwNode.hpp"

Map<Value::Type_t, u8_t> ZwNode::m_mapValueToCmdClass;

/**
 * @func   MapValueToCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwNode::MapValueToCmdClass() {
    m_mapValueToCmdClass[ValueSwitch::ValueType()] =
    BinarySwitchCmdClass::GetZwCmdClassId();
    m_mapValueToCmdClass[ValueDimmer::ValueType()] =
    MultilevelSwitchCmdClass::GetZwCmdClassId();
    m_mapValueToCmdClass[ValuePirSensor::ValueType()] =
    MultilevelSwitchCmdClass::GetZwCmdClassId();
}

/**
 * @func   ZwNode
 * @brief  None
 * @param  None
 * @retval None
 */
ZwNode::ZwNode(
    u32_t dwHomeId,
    u8_t  byNodeId
) : m_boInfoReceived (FALSE),
    m_boPlusInfoReceived (FALSE),
    m_boNodeAlive (FALSE),
    m_boAddingNode (FALSE),
    m_boListening (TRUE),
    m_boFreqListening (TRUE),
    m_boSecurity (FALSE),
    m_boIsController (FALSE),
    m_byNodeId (byNodeId),
    m_byCapability (0),
    m_byBasic (4),
    m_byGeneric (0),
    m_bySpecific (0),
    m_byDeviceType (0),
    m_byVersion (0),
    m_dwHomeId (dwHomeId),
    m_strManufactureName (String()),
    m_strProductName (String()),
    m_strNodeName (String()),
    m_wManufactureId (0),
    m_wProductType (0),
    m_wProductId (0),
    m_byRoleType (0),
    m_byNodeType (0),
    m_wIconType (0),
    m_boSecured (FALSE),
    m_dwReceivedUnsolicited (0),
    m_byNbrOfEndpoints (0) {
    m_pZwCmdClassManager = ZwCmdClassMan::GetInstance();
    m_pControllerFunctor = NULL;

}

/**
 * @func   ~ZwNode
 * @brief  None
 * @param  None
 * @retval None
 */
ZwNode::~ZwNode() {
    for (ZwEnpoint::iterator_t it = m_ZwEndpoints.begin();
            it != m_ZwEndpoints.end(); it++) {
        delete it->second;
    }
    m_ZwEndpoints.clear();
}

/**
 * @func   SetNodeValue
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwNode::SetNodeValue(
    ValueDevice_p pValueDevice
) {
    Map<Value::Type_t, u8_t>::const_iterator_t it =
    m_mapValueToCmdClass.find(pValueDevice->GetType());
    if (it != m_mapValueToCmdClass.end()) {
        ZwCmdClass_p pZwCmdClass =
        m_pZwCmdClassManager->CreateCmdClass(it->second, m_dwHomeId, m_byNodeId);
        ZwMessage_p pZwMessage = pZwCmdClass->SetValue(pValueDevice);

        delete pZwCmdClass;
        pZwCmdClass = NULL;

        return pZwMessage;
    }
    return NULL;
}

/**
 * @func   IsMultiChannel
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwNode::IsMultiChannel() {
    if (GetZwCmdClass(MultiChannelCmdClass::GetZwCmdClassId()) != NULL) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @func   GetZwCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
ZwNode::GetZwCmdClass(
    const u8_t byZwCmdClassId
) {
    MapCmdClass::iterator_t it = m_mZwCmdClass.find(byZwCmdClassId);
    if (it != m_mZwCmdClass.end()) {
        return it->second;
    }
    return NULL;
}

/**
 * @func   AddZwCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
ZwNode::AddZwCmdClass(
    const u8_t byZwCmdClassId
) {
    if (GetZwCmdClass(byZwCmdClassId) != NULL) {
        return m_mZwCmdClass[byZwCmdClassId];
    }

    ZwCmdClass_p pZwCmdClass =
    m_pZwCmdClassManager->CreateCmdClass(byZwCmdClassId, m_dwHomeId, m_byNodeId);

    if (pZwCmdClass != NULL) {
        m_mZwCmdClass[byZwCmdClassId] = pZwCmdClass;
        pZwCmdClass->SetNode(this);
        return pZwCmdClass;
    } else {
        return NULL;
    }
}

/**
 * @func   RmvZwCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwNode::RmvZwCmdClass(
    const u8_t byZwCmdClassId
) {
    MapCmdClass::iterator_t it = m_mZwCmdClass.find(byZwCmdClassId);
    if (it == m_mZwCmdClass.end()) {
        return ;
    }
    delete it->second;
    m_mZwCmdClass.erase(it);
}

/**
 * @func   SetDeviceType
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwNode::SetDeviceType () {
    if (m_byGeneric == GENERIC_TYPE_SWITCH_BINARY) {
        m_byDeviceType = 1;
    } else if (m_byGeneric == GENERIC_TYPE_SWITCH_MULTILEVEL) {
        if (m_bySpecific == SPECIFIC_TYPE_POWER_SWITCH_MULTILEVEL) {
            m_byDeviceType = 2;
        } else if (m_bySpecific == SPECIFIC_TYPE_FAN_SWITCH) {
            m_byDeviceType = 4;
        }
    }
}

/**
 * @func   ApplicationCommandHandler
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
ZwNode::ApplicationCommandHandler(
    u8_p pbyCommand,
    u8_t byLength
) {
    u8_t byCmdClass = pbyCommand[0];
    ValueDevice_p pValueDevice = NULL;
    ZwCmdClass_p  pZwCmdClass  = AddZwCmdClass(byCmdClass);

    if (pZwCmdClass != NULL) {
        pValueDevice = pZwCmdClass->HandleMessage(pbyCommand, byLength);
    }

    return pValueDevice;
}

/**
 * @func   SetLevel
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwNode::SetLevel(
    const u8_t byLevel
) {
    ZwCmdClass_p pZwCmdClass = m_pZwCmdClassManager->CreateCmdClass(
    MultilevelSwitchCmdClass::GetZwCmdClassId(), m_dwHomeId, m_byNodeId);

    if (pZwCmdClass == NULL) { return NULL; }

    ValueByte_t ValueSet = byLevel;

    ZwMessage_p pZwMessage =
    ((MultilevelSwitchCmdClass_p)pZwCmdClass)->SetValue((byLevel > 99) ? 99 : byLevel);

    delete pZwCmdClass;
    pZwCmdClass = NULL;

    return pZwMessage;
}

/**
 * @func   SetNodeOn
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwNode::SetNodeOn() {
    ZwCmdClass_p pZwCmdClass = m_pZwCmdClassManager->CreateCmdClass(
    BasicCmdClass::GetZwCmdClassId(), m_dwHomeId, m_byNodeId);

    if (pZwCmdClass == NULL) { return NULL; }

    ZwMessage_p pZwMessage = ((BasicCmdClass_p)pZwCmdClass)->SetValue(0xFF);

    delete pZwCmdClass;
    pZwCmdClass = NULL;

    return pZwMessage;
}

/**
 * @func   SetNodeOff
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwNode::SetNodeOff() {
    ZwCmdClass_p pZwCmdClass = m_pZwCmdClassManager->CreateCmdClass(
    BasicCmdClass::GetZwCmdClassId(), m_dwHomeId, m_byNodeId);

    if (pZwCmdClass == NULL) { return NULL; }

    u8_t byValue = 0;
    ZwMessage_p pZwMessage = ((BasicCmdClass_p)pZwCmdClass)->SetValue(byValue);

    delete pZwCmdClass;
    pZwCmdClass = NULL;

    return pZwMessage;
}

/**
 * @func   GetEndpoint
 * @brief  None
 * @param  None
 * @retval None
 */
ZwNode_p
ZwNode::GetEndpoint(
    u8_t byEndpointId
) {
    ZwEnpoint::iterator_t it = m_ZwEndpoints.find(byEndpointId);
    if (it != m_ZwEndpoints.end()) {
        return it->second;
    }
    return NULL;
}

/**
 * @func   AddEndpoint
 * @brief  None
 * @param  None
 * @retval None
 */
ZwNode_p
ZwNode::AddEndpoint(
    u8_t byEndpointId
) {
    if (GetEndpoint(byEndpointId) != NULL) {
        return m_ZwEndpoints[byEndpointId];
    }

    ZwNode_p pZwaveNode = new ZwNode();
    m_ZwEndpoints[byEndpointId] = pZwaveNode;
    return pZwaveNode;
}

/**
 * @func   RmvEndpoint
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwNode::RmvEndpoint(
    u8_t byEndpointId
) {
    ZwEnpoint::iterator_t it = m_ZwEndpoints.find(byEndpointId);
    if (it == m_ZwEndpoints.end()) {
        return ;
    }
    delete it->second;
    m_ZwEndpoints.erase(it);
}

/**
 * @func   operator[]
 * @brief  None
 * @param  None
 * @retval None
 */
ZwNode_p
ZwNode::operator[] (
    u8_t byEndpointId
) {
    if (byEndpointId == 0) {
        return this;
    }
    return GetEndpoint(byEndpointId);
}

/**
 * @func   SetEnpointValue
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwNode::SetEnpointValue(
   u8_t byEndpointId,
   ValueDevice_p pValueDevice
) {
    ZwNode_p pZwaveEndP = GetEndpoint(byEndpointId);
    if (pZwaveEndP != NULL) {
        pZwaveEndP->SetNodeValue(pValueDevice);
    }
    return NULL;
}

/**
 * @func   SetCallbackFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwNode::SetCallbackFunctor(
    ZwCtrllerFunctor_p pCtrllerFunctor
) {
    m_pControllerFunctor = pCtrllerFunctor;
}

/**
 * @func   ProcessFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwNode::ProcessFunctor(
    EvAction event,
    void_p pBuffer
) {
    if (m_pControllerFunctor != NULL) {
        (*m_pControllerFunctor) (event, pBuffer);
    }
}

/**
 * @func   GetNodeProtocolInfo
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwNode::GetNodeProtocolInfo() {
    LOG_INFO("node protocol info %d", m_byNodeId);
    ZwMessage_p pZwMessage = new ZwMessage(m_byNodeId, REQUEST,
    FUNC_ID_ZW_GET_NODE_PROTOCOL_INFO, TRUE);
    pZwMessage->ResetPacket(1);
    pZwMessage->Push(m_byNodeId);
    return pZwMessage;
}

/**
 * @func   RequestNodeInfo
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwNode::RequestNodeInfo() {
    LOG_INFO("node info %d", m_byNodeId);
    ZwMessage_p pZwMessage = new ZwMessage(m_byNodeId, REQUEST,
    FUNC_ID_ZW_REQUEST_NODE_INFO, TRUE);
    pZwMessage->ResetPacket(1);
    pZwMessage->Push(m_byNodeId);
    return pZwMessage;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwNode::GetZwPlusInfo() {
    LOG_INFO("plus info %d", m_byNodeId);
    ZwCmdClass_p pZwCmdClass = m_pZwCmdClassManager->CreateCmdClass(
    ZwavePlusInfoCmdClass::GetZwCmdClassId(), m_dwHomeId, m_byNodeId);
    ZwMessage_p pZwMessage = pZwCmdClass->GetValue();
    delete pZwCmdClass;
    pZwCmdClass = NULL;
    return pZwMessage;
}
