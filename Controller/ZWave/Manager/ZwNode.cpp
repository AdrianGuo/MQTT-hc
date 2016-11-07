#include <stddef.h>
#include "ValueByte.hpp"
#include "AssociationCmdClass.hpp"
#include "BasicCmdClass.hpp"
#include "MultilevelSwitchCmdClass.hpp"
#include "MultiChannelCmdClass.hpp"
#include "ZwNode.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwNode::ZwNode(
    u32_t dwHomeID,
    u8_t byNodeID
) {
    m_boInfoReceived = FALSE;
    m_boPlusInfoReceived = FALSE;
    m_boNodeAlive = FALSE;

    m_boAddingNode = FALSE;
    m_boListening = TRUE;
    m_boFreqListening = TRUE;
    m_boSecurity =  FALSE;
    m_boIsController = FALSE;

    m_byCapability = 0;
    m_byNodeID = byNodeID;
    m_byBasic = 4;
    m_byGeneric = 0;
    m_bySpecific = 0;

    m_wVersion = 0;
    m_dwHomeID = dwHomeID;

    m_strManufactureName = "";
    m_strProductName = "";
    m_strNodeName = "";
    m_wManufactureID = 0;
    m_wProductType = 0;
    m_wProductID = 0;
    m_byRoleType = 0;
    m_byNodeType = 0;
    m_byIconType = 0;

    m_boSecured = FALSE;
    m_dwReceivedUnsolicited = 0;

    m_pZwCmdClassManager = ZwCmdClassMan::GetInstance();

    m_pZwEndpoints = NULL;
    m_byNbrOfEndpoints = 0;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwNode::~ZwNode() {
    if (m_pZwEndpoints != NULL) {
        delete[] m_pZwEndpoints;
        m_pZwEndpoints = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwNode::IsSupportMultiChannel() {
    if (GetZwCmdClass(MultiChannelCmdClass::GetZwCmdClassID()) != NULL) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
ZwNode::GetZwCmdClass(
    const u8_t byZWCmdClassID
) {
    Map<u8_t, ZwCmdClass_p>::iterator_t it = m_mZwCmdClass.find(byZWCmdClassID);
    if (it != m_mZwCmdClass.end()) {
        return it->second;
    }
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
ZwNode::AddZwCmdClass(
    const u8_t byZWCmdClassID
) {
    if (GetZwCmdClass(byZWCmdClassID) != NULL) { // If Existed
        return m_mZwCmdClass[byZWCmdClassID];
    }

    ZwCmdClass_p pZwCmdClass = m_pZwCmdClassManager->CreateCmdClass(
                                        byZWCmdClassID, m_dwHomeID, m_byNodeID);

    if (pZwCmdClass != NULL) {
        m_mZwCmdClass[byZWCmdClassID] = pZwCmdClass;
        return pZwCmdClass;
    } else {
        return NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
ZwNode::RmvZwCmdClass(
    const u8_t byZWCmdClassID
) {
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwNode::GetDeviceType() {
    u8_t byRetType = 0;
    if (m_byGeneric == GENERIC_TYPE_SWITCH_BINARY) {
        byRetType = 1;
    } else if (m_byGeneric == GENERIC_TYPE_SWITCH_MULTILEVEL) {
        if (m_bySpecific == SPECIFIC_TYPE_POWER_SWITCH_MULTILEVEL) {
            byRetType = 2;
        } else if (m_bySpecific == SPECIFIC_TYPE_FAN_SWITCH) {
            byRetType = 4;
        }
    }
    return byRetType;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Value_p
ZwNode::ApplicationCommandHandler(
    u8_p pbyInBuffer,
    u8_t byLength
) {
    Value_p pOutValue = NULL;
    u8_t byCmdClassID = pbyInBuffer[0];
    ZwCmdClass_p pZwCmdClass = GetZwCmdClass(byCmdClassID);

    if (pZwCmdClass == NULL) {
        pZwCmdClass = AddZwCmdClass(byCmdClassID);
    }

    if (pZwCmdClass != NULL) {
        pOutValue = pZwCmdClass->HandleMessage(pbyInBuffer, byLength);
    }

    return pOutValue;
}
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwNode::SetLevel(
    const u8_t byLevel
) {
    if ((byLevel < 100) && (byLevel > 0)) {
        ZwCmdClass_p pZwCmdClass = m_pZwCmdClassManager->CreateCmdClass(
                    MultilevelSwitchCmdClass::GetZwCmdClassID(), m_dwHomeID, m_byNodeID);

        if (pZwCmdClass == NULL) { return NULL; }

        ValueByte_t ValueSet = byLevel;
        ZwMessage_p pZwMessage = pZwCmdClass->SetValue(ValueSet);

        delete pZwCmdClass;
        pZwCmdClass = NULL;

        return pZwMessage;
    }

    if (byLevel > 100) { return SetNodeOn(); }

    if (byLevel == 0) { return SetNodeOff(); }

    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwNode::SetNode(bool_t boState) {
    return boState ? SetNodeOn() : SetNodeOff();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwNode::SetNodeOn() {
    ZwCmdClass_p pZwCmdClass = m_pZwCmdClassManager->CreateCmdClass(
            BasicCmdClass::GetZwCmdClassID(), m_dwHomeID, m_byNodeID);

    if (pZwCmdClass == NULL) { return NULL; }

    ValueByte_t ValueSet = 0xFF;
    ZwMessage_p pZwMessage = pZwCmdClass->SetValue(ValueSet);

    delete pZwCmdClass;
    pZwCmdClass = NULL;

    return pZwMessage;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwNode::SetNodeOff() {
    ZwCmdClass_p pZwCmdClass = m_pZwCmdClassManager->CreateCmdClass(
            BasicCmdClass::GetZwCmdClassID(), m_dwHomeID, m_byNodeID);

    if (pZwCmdClass == NULL) { return NULL; }

    ValueByte_t ValueSet = 0;
    ZwMessage_p pZwMessage = pZwCmdClass->SetValue(ValueSet);

    delete pZwCmdClass;
    pZwCmdClass = NULL;

    return pZwMessage;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwNode::SetNbrOfEndpoints(
    u8_t byNbrOfEndpoints
) {
    if (m_pZwEndpoints != NULL) {
        delete[] m_pZwEndpoints;
        m_pZwEndpoints = NULL;
    }
    m_byNbrOfEndpoints = byNbrOfEndpoints;
    m_pZwEndpoints = new ZwEndpoint[byNbrOfEndpoints];
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwNode::GetNbrOfEndpoints() const {
    return m_byNbrOfEndpoints;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwEndpoint_p
ZwNode::GetZwEndPoint(
    u8_t byEndpointID
) {
    if (m_pZwEndpoints == NULL) { return NULL; }

    return &m_pZwEndpoints[byEndpointID - 1];
}
