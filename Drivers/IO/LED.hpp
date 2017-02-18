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
#include <Locker.hpp>

class LED {
private:
    mraa::Gpio      m_LED1;
    mraa::Gpio      m_LED2;

    Locker_p        m_pLocker;

public:
    typedef enum {
    	Off = 1, // GPIO18|19: 0|0
        Red,    // GPIO18|19: 0|1
        Blue,	 // GPIO18|19: 1|0
        Pink,	 // GPIO18|19: 1|1
    } Color;
    typedef Color Color_t;

    typedef enum {
    	Latch = 1,
		Hold,
        Blink
    } Action;
    typedef Action Action_t;

    LED(int_t, int_t);
    ~LED();

    void_t Set(Color_t);
};

//#endif
#endif /* DRIVERS_IO_LED_HPP_ */
