/*
 * LED.hpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */

#ifndef DRIVERS_IO_LED_HPP_
#define DRIVERS_IO_LED_HPP_
//#ifdef MT7688

#include <typedefs.h>
#include <mraa.hpp>
#include <RTimer.hpp>
#include <Functor.hpp>
#include <Locker.hpp>

class LED {
private:
    mraa::Gpio      m_LED;
    mraa::Result    m_Result;
    RTimer_p        m_pRTimer;
    Locker_p        m_pLocker;
    TimerFunctor_t  m_BlinkFunctor;
    int_t			m_iBlink;

public:
    LED(int_t);
    ~LED();

    void_t On();
    void_t Off();
    void_t Toggle();
    void_t Blink(u32_t dwDuty);

    void_t HandleBlink(void_p);
};

//#endif
#endif /* DRIVERS_IO_LED_HPP_ */
