/*
 * LED.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */
#ifdef MT7688

#include <IO/LED.hpp>

#define ON      (1)
#define OFF     (0)

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
LED::LED(
	int_t GPIONo
) : m_LED(GPIONo) {
    m_Result    = m_LED.dir(mraa::DIR_OUT);
    m_pRTimer   = RTimer::getTimerInstance();
    m_pLocker   = new Locker();
    m_BlinkFunctor   = makeFunctor((TimerFunctor_p) NULL, *this, &LED::HandleBlink);
    m_iBlink = -1;

    if (m_Result != mraa::SUCCESS) {
        mraa::printError(m_Result);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
LED::~LED(){
    delete m_pLocker;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LED::On() {
	if(m_iBlink != -1) {
		if(m_pRTimer->CancelTimer(m_iBlink))
			m_iBlink = -1;
	}
	m_LED.write(ON);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LED::Off() {
	if(m_iBlink != -1) {
		if(m_pRTimer->CancelTimer(m_iBlink))
			m_iBlink = -1;
	}
    m_LED.write(OFF);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LED::Toggle() {
	if(m_iBlink != -1) {
		if(m_pRTimer->CancelTimer(m_iBlink))
			m_iBlink = -1;
	}
    m_LED.write(!m_LED.read());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LED::Blink(
	u32_t dwDuty
) {
	if(m_iBlink != -1) {
		if(m_pRTimer->CancelTimer(m_iBlink))
			m_iBlink = -1;
	}
	Toggle();
	m_iBlink = m_pRTimer->StartTimer(RTimer::Repeat::Forever, dwDuty, &m_BlinkFunctor, NULL);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LED::HandleBlink(
	void_p pbyBuffer
) {
	m_LED.write(!m_LED.read());
}

#endif
