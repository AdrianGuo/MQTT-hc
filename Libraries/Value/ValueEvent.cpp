#include "ValueEvent.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueEvent::ValueEvent() {
    m_pEvent = new Event();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueEvent::~ValueEvent() {
    if (m_pEvent != NULL) {
        delete m_pEvent;
        m_pEvent = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Event_p
ValueEvent::GetValue() {
    return m_pEvent;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueEvent::SetValue(
    Event_p pEvent
) {
    m_pEvent = pEvent;
    SetChange();
}
