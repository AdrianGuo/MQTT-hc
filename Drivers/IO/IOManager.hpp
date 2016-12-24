/*
 * IOManager.hpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */

#ifndef DRIVERS_IOMMANAGER_HPP_
#define DRIVERS_IOMMANAGER_HPP_
#ifdef MT7688

#include <LED.hpp>
#include <Button.hpp>

class IOManager {
private:
    IOManager(int_t, int_t, int_t);

public:
    LED      LED1;
    LED      LED2;
    LED      LED3;
    Button   Button1;

    ~IOManager();

    static IOManager* s_pInstance;
    IOManager* GetInstance(int_t, int_t, int_t);

    void_t   Inform(u8_t);
};

#endif
#endif /* DRIVERS_IOMMANAGER_HPP_ */
