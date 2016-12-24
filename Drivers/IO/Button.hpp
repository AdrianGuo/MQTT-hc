/*
 * Button.hpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */

#ifndef DRIVERS_IO_BUTTON_HPP_
#define DRIVERS_IO_BUTTON_HPP_
#ifdef MT7688

#include <typedefs.h>
#include <mraa.hpp>

class Button {
private:
    mraa::Result    m_Result;

    static void_t PressEvent(bool_t);
    static void_t EdgeRisingFunc (void_p);
    static void_t EdgeFallingFunc (void_p);

public:
    Button();
    ~Button();

    static mraa::Gpio s_Button;
};

#endif
#endif /* DRIVERS_IO_BUTTON_HPP_ */
