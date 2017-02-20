/*
 * Button.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */

#include <Button.hpp>

mraa::Gpio Button::m_sButton = mraa::Gpio(38);
ButtonFunctor_p Button::m_spButtonFunctor = NULL;
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Button::Button()
{
	mraa::Result result = m_sButton.dir(mraa::DIR_IN);
    if (result != mraa::SUCCESS) {
        mraa::printError(result);
    }
    m_sButton.isr(mraa::EDGE_FALLING, &EdgeFallingFunc, NULL);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Button::~Button(){
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Button::EdgeFallingFunc (
    void_p pBuffer
) {
    m_sButton.isrExit();
    m_sButton.isr(mraa::EDGE_RISING, &Button::EdgeRisingFunc, NULL);
    (*m_spButtonFunctor)(TRUE);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Button::EdgeRisingFunc (
    void_p pBuffer
) {
    m_sButton.isrExit();
    m_sButton.isr(mraa::EDGE_FALLING, &Button::EdgeFallingFunc, NULL);
    (*m_spButtonFunctor)(FALSE);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Button::RecvFunctor(
	ButtonFunctor_p pRecvFunctor
) {
    if (pRecvFunctor != NULL) {
        m_spButtonFunctor = pRecvFunctor;
        return TRUE;
    }
    return FALSE;
}

