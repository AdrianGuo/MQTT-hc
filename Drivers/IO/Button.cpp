/*
 * Button.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */
#ifdef MT7688

#include <Button.hpp>

mraa::Gpio Button::s_Button(18);

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Button::Button() {
    m_Result = s_Button.dir(mraa::DIR_IN);

    if (m_Result != mraa::SUCCESS) {
        mraa::printError(m_Result);
    }

    s_Button.isr(mraa::EDGE_FALLING, &EdgeFallingFunc, NULL);
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
Button::EdgeRisingFunc (
    void_p pBuffer
) {
    s_Button.isrExit();
    Button::PressEvent(FALSE);
    s_Button.isr(mraa::EDGE_FALLING, &Button::EdgeFallingFunc, NULL);
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
    s_Button.isrExit();
    Button::PressEvent(TRUE);
    s_Button.isr(mraa::EDGE_RISING, &Button::EdgeRisingFunc, NULL);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Button::PressEvent (
    bool_t boEvent
) {
	if(boEvent == TRUE) {

	} else {

	}
}

#endif
