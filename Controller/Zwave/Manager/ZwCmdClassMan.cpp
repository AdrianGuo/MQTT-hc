#include <stddef.h>
#include "AssociationCmdClass.hpp"
#include "BasicCmdClass.hpp"
#include "BinarySwitchCmdClass.hpp"
#include "MultiChannelCmdClass.hpp"
#include "MultilevelSwitchCmdClass.hpp"
#include "ZwavePlusInfoCmdClass.hpp"
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
    Register(AssociationCmdClass::GetZwCmdClassId(), AssociationCmdClass::CreateZwCmdClass);
    Register(BasicCmdClass::GetZwCmdClassId(), BasicCmdClass::CreateZwCmdClass);
    Register(BinarySwitchCmdClass::GetZwCmdClassId(), BinarySwitchCmdClass::CreateZwCmdClass);
    Register(MultilevelSwitchCmdClass::GetZwCmdClassId(), MultilevelSwitchCmdClass::CreateZwCmdClass);
    Register(MultiChannelCmdClass::GetZwCmdClassId(), MultiChannelCmdClass::CreateZwCmdClass);
    Register(ZwavePlusInfoCmdClass::GetZwCmdClassId(), ZwavePlusInfoCmdClass::CreateZwCmdClass);
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
    const u8_t byCmdClassId,
    pFunctionCreateCmdClass pFunctioncreator
) {
    m_FuncCreateCmdClass[byCmdClassId] = pFunctioncreator;
}

/**
 * @func   CreateCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
ZwCmdClassMan::CreateCmdClass(
    u8_t  byCmdClassId,
    u32_t dwHomeId,
    u8_t  byNodeId
) {
    pFunctionCreateCmdClass pFunctioncreator = m_FuncCreateCmdClass[byCmdClassId];

    if (pFunctioncreator == NULL) { return NULL; }

    return pFunctioncreator(dwHomeId, byNodeId);
}
