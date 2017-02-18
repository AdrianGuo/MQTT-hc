/*
 * IO.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */
#ifdef MT7688

#include <IO.hpp>

IO* IO::s_pInstance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
IO::IO(
) : m_LED1(18),
    m_LED2(19)
{

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
IO::~IO() {
	delete s_pInstance;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
IO*
IO::GetInstance(
) {
    if(s_pInstance == NULL) {
        s_pInstance = new IO();
    }
    return s_pInstance;
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
IO::Inform(u8_t event) {
	switch (event) {
		case 1:
			m_LED1.On();
			m_LED2.Off();
			break;

		case 2:
			m_LED1.Off();
			m_LED2.On();
			break;

		case 3:
			m_LED1.On();
			m_LED2.On();
			break;

		case 4:
			m_LED1.Off();
			m_LED2.Off();
			break;

		default:
			break;
	}}

#endif
