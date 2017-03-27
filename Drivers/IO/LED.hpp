/*
 * LED.hpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */

#ifndef DRIVERS_IO_LED_HPP_
#define DRIVERS_IO_LED_HPP_

#ifdef MT7688
#include <Typedefs.h>
#include <mraa.hpp>
#include <Locker.hpp>

class LED {
private:
    mraa::Gpio      m_LED1;
    mraa::Gpio      m_LED2;
    bool_t			m_boIsLocked;

    Locker_p        m_pLocker;

public:
    typedef enum {
    	Off = 1, // GPIO18|19: 0|0
        Red,    // GPIO18|19: 0|1
        Blue,	 // GPIO18|19: 1|0
        Pink,	 // GPIO18|19: 1|1
		RedBlue = (10*Red + Blue),
		RedPink = (10*Red + Pink),
		BluePink = (10*Red + Blue)
    } Color;
    typedef Color Color_t;

    typedef enum Action {
    	Latch = 1,
		Hold,
        Blink
    } Action_t;

    LED(int_t, int_t);
    ~LED();

    bool_t Set(Color_t);
    void_t Lock();
    void_t UnLock();
};
#endif

#endif /* DRIVERS_IO_LED_HPP_ */
