#include <stddef.h>
#include "AssociationCmdClass.hpp"
#include "BasicCmdClass.hpp"
#include "BinarySwitchCmdClass.hpp"
#include "MultilevelSwitchCmdClass.hpp"
#include "MultiChannelCmdClass.hpp"
#include "ZwCmdClassMan.hpp"

ZwCmdClassMan_p ZwCmdClassMan::m_pInstance = NULL;

/**
 * @func   ZwCmdClassMan
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClassMan::ZwCmdClassMan() {
    memset(m_FuncCreateCmdClass, 0, sizeof(pFunctionCreateCmdClass) * 256);
    RegisterCmdClass();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClassMan::~ZwCmdClassMan() { }

/**
 * @func   RegisterCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdClassMan::RegisterCmdClass() {
    Register(AssociationCmdClass::GetZwCmdClassID(), AssociationCmdClass::CreateZwCmdClass);
    Register(BasicCmdClass::GetZwCmdClassID(), BasicCmdClass::CreateZwCmdClass);
    Register(BinarySwitchCmdClass::GetZwCmdClassID(), BinarySwitchCmdClass::CreateZwCmdClass);
    Register(MultilevelSwitchCmdClass::GetZwCmdClassID(), MultilevelSwitchCmdClass::CreateZwCmdClass);
    Register(MultiChannelCmdClass::GetZwCmdClassID(), MultiChannelCmdClass::CreateZwCmdClass);
}

/**
 * @func   GetInstance
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClassMan_p
ZwCmdClassMan::GetInstance() {
    if (m_pInstance == NULL) {
        m_pInstance = new ZwCmdClassMan();
    }

    return m_pInstance;
}

/**
 * @func   Register
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdClassMan::Register(
    const u8_t byCmdClassID,
    pFunctionCreateCmdClass pFunctioncreator
) {
    m_FuncCreateCmdClass[byCmdClassID] = pFunctioncreator;
}

/**
 * @func   CreateCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
ZwCmdClassMan::CreateCmdClass(
    u8_t  byCmdClassID,
    u32_t dwHomeID,
    u8_t  byNodeID
) {
    pFunctionCreateCmdClass pFunctioncreator = m_FuncCreateCmdClass[byCmdClassID];

    if (pFunctioncreator == NULL) { return NULL; }

    return pFunctioncreator(dwHomeID, byNodeID);
}
