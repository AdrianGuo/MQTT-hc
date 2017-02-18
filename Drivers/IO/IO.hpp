/*
 * IO.hpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */

#ifndef DRIVERS_IO_HPP_
#define DRIVERS_IO_HPP_
#ifdef MT7688

#include <LED.hpp>
#include <Button.hpp>

class IO {
private:
    LED      m_LED1;
    LED      m_LED2;

    IO();

public:
    ~IO();
    static IO* s_pInstance;
    static  IO* GetInstance();

    void_t Inform(u8_t);
};

#endif

typedef IO 	IO_t;
typedef IO* IO_p;

#endif /* DRIVERS_IO_HPP_ */
