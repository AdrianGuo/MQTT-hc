#include "ValueBase.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueBase::ValueBase() {}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueBase::~ValueBase() {
    for (u32_t i = 0; i < m_vecElements.size(); i++) {
        delete m_vecElements[i];
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueBase::Add(
    Value_p pAttrib,
    String strName
) {
    ValueBaseElement_p pElement = new ValueBaseElement();
    pElement->pAttrib = pAttrib;
    pElement->strName = strName;
    m_vecElements.push_back(pElement);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueBase::SetChange() {
    for (u32_t i = 0; i < m_vecElements.size(); i++) {
        m_vecElements[i]->pAttrib->SetChange();
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueBase::IsChanged() {
    if (Value::IsChanged()) {
        return TRUE;
    } else {
        for (u32_t i = 0; i < m_vecElements.size(); i++) {
            if (m_vecElements[i]->pAttrib->IsChanged()) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueBase::Process() {
    for (u32_t i = 0; i < m_vecElements.size(); i++) {
        m_vecElements[i]->pAttrib->Process();
    }
}
