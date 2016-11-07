#include "ZwDefs.hpp"
#include "ZwEndpoint.hpp"


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwEndpoint::ZwEndpoint(
    u8_t byGeneric,
    u8_t bySpecific
) {
    m_byGeneric = byGeneric;
    m_bySpecific = bySpecific;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwEndpoint::~ZwEndpoint() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwEndpoint::GetTypeEndpoint() {
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
