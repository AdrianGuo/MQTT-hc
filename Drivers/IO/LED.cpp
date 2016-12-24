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
LED::LED(int_t GPIONo) : m_LED(GPIONo) {
    m_Result    = m_LED.dir(mraa::DIR_OUT);
    m_pRTimer   = RTimer::getTimerInstance();
    m_pLocker   = new Locker();
    m_Functor   = makeFunctor((timerFunctor_p) NULL, *this, &LED::FuncWrapper);

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
LED::On(u32_t dwTime) {
    if (dwTime == 0) {
        m_LED.write(ON);
    } else {
        u8_t funcIndex = 1;
        m_pRTimer->StartTimer(RTimer::Repeat::OneTime,  dwTime, &m_Functor, &funcIndex);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LED::Off() {
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
    m_LED.write(!m_LED.read());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LED::Blink(u32_t dwDuty) {
//    m_Functor = makeFunctor((timerFunctor_p) NULL, *this, &LED::Toggle);
//    m_pRTimer->StartTimer(RTimer::Repeat::OneTime,  dwTime, &m_Functor, NULL);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LED::FuncWrapper(void_p funcIndex) {
    if (*((u8_p) funcIndex) == 0) {
        LED::On();
    } else if (*((u8_p) funcIndex) == 1) {
        LED::Off();
    } else if (*((u8_p) funcIndex) == 2) {
        LED::Toggle();
    } else
        return;
}

#endif
