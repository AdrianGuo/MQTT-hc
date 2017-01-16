/*
 * IOManager.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */
#ifdef MT7688

#include <IOManager.hpp>

IOManager* IOManager::s_pInstance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
IOManager::IOManager(
    int_t LED1Pin,
    int_t LED2Pin
) : LED1(LED1Pin),
    LED2(LED2Pin)
{

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
IOManager::~IOManager() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
IOManager*
IOManager::GetInstance(
    int_t LED1Pin,
    int_t LED2Pin
) {
    if(s_pInstance == NULL) {
        s_pInstance = new IOManager(LED1Pin, LED2Pin);
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
IOManager::Inform(u8_t event) {

}

#endif
