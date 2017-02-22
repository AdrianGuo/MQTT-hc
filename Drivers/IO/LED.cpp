/*
 * LED.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */
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
	int_t GPIONo1,
	int_t GPIONo2
) : m_LED1(GPIONo1),
	m_LED2(GPIONo2)
{
	mraa::Result ret1    = m_LED1.dir(mraa::DIR_OUT);
	mraa::Result ret2    = m_LED2.dir(mraa::DIR_OUT);
    if (ret1 != mraa::SUCCESS) {
        mraa::printError(ret1);
    }
    if (ret2 != mraa::SUCCESS) {
        mraa::printError(ret1);
    }
    m_boIsLocked = FALSE;
    m_pLocker   = new Locker();
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
bool_t
LED::Set(
Color_t color
) {
	if(m_boIsLocked == TRUE)
		return FALSE;
	m_pLocker->Lock();
	switch (color) {
		case Color::Off:
			m_LED1.write(OFF);
			m_LED2.write(OFF);
			break;

		case Color::Red:
			m_LED1.write(OFF);
			m_LED2.write(ON);
			break;

		case Color::Blue:
			m_LED1.write(ON);
			m_LED2.write(OFF);
			break;

		case Color::Pink:
			m_LED1.write(ON);
			m_LED2.write(ON);
			break;

		default:
			break;
	}
    m_pLocker->UnLock();
    return TRUE;
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LED::Lock(
) {
	m_pLocker->Lock();
	m_boIsLocked = TRUE;
	m_pLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
LED::UnLock(
) {
	m_pLocker->Lock();
	m_boIsLocked = FALSE;
	m_pLocker->UnLock();
}
