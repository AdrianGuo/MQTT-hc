/*
 * Button.hpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */

#ifndef DRIVERS_IO_BUTTON_HPP_
#define DRIVERS_IO_BUTTON_HPP_

#ifdef MT7688
#include <Typedefs.h>
#include <mraa.hpp>
#include <Functor.hpp>

typedef Functor1_t(bool_t)             	   ButtonFunctor_t;
typedef ButtonFunctor_t*                   ButtonFunctor_p;

class Button {
private:
	static ButtonFunctor_p m_spButtonFunctor;

    static void_t EdgeRisingFunc (void_p);
    static void_t EdgeFallingFunc (void_p);

public:
    Button();
    ~Button();

    static mraa::Gpio m_sButton;
    bool_t RecvFunctor(ButtonFunctor_p pRecvFunctor);
};
#endif
#endif /* DRIVERS_IO_BUTTON_HPP_ */
