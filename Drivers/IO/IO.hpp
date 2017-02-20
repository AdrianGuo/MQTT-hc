/*
 * IO.hpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */

#ifndef DRIVERS_IO_HPP_
#define DRIVERS_IO_HPP_

#include <RTimer.hpp>
#include <Locker.hpp>
#include <LED.hpp>
#include <Button.hpp>

#define Notify(x)		{IO::GetInstance()->Inform(IO::Event::x);}

typedef struct IOState {
	LED::Color_t 	ioColor = LED::Color::Off;
	LED::Action_t 	ioAction = LED::Action::Latch;
	u32_t 			ioTime = 0;
	u32_t 			ioDuty = 0;
	u32_t 			ioNo = 0;

    /*
     * Overloaded operators
     */
    inline IOState&
    operator=(
        const IOState& rhs
    ){
    	ioColor		= rhs.ioColor;
    	ioAction	= rhs.ioAction;
    	ioTime		= rhs.ioTime;
    	ioDuty		= rhs.ioDuty;
    	ioNo		= rhs.ioNo;
        return *this;
    }

    inline bool
    operator!=(
        const IOState& rhs
    ){
        if(ioColor != rhs.ioColor) return true;
        if(ioAction != rhs.ioAction) return true;
        if(ioTime != rhs.ioTime) return true;
        if(ioDuty != rhs.ioDuty) return true;
        if(ioNo != rhs.ioNo) return true;
        return false;
    }
} IOState_t;


/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class IO {
private:
    LED       	m_LED;
    Button		m_Button;
    IOState_t 	m_ioBakState;
    IOState_t 	m_ioCurState;

    u32_t 		m_idwBlinkedNo;
    bool_t		m_boIsBAKed;
    u32_t		m_idwReleasedNo;
    ButtonFunctor_t m_ButtonFunctor;

    //Variables for coincident events preventing
    bool_t		m_boIsJoinableReady;
    bool_t		m_boIsFResetReady;

    Locker_p        m_pLocker;
    RTimer_p        m_pRTimer;
    TimerFunctor_t  m_LEDTimerFunctor;
    int_t			m_iLEDTimerID;

    IO();

public:
    ~IO();
    static IO* s_pInstance;
    static  IO* GetInstance();

    typedef enum Event {
    	NotStart = 0, // latch-pink
        Start, // blink-pink-3
		Reset, // blink-pink-3
		Upgraded, // blink-pink-3
        NotInternet, // blink-red-0
        NotReach, // latch-red
		Reach, // latch-blue
		AppSig, // blink-blue-1
		DevSig, // blink-red-1
		Broadcast, // blink-blue-10
		Upgrading, // blink-blue/red-0
		Joinable // blink-blue-0
    } Event_t;

    void_t Indicate(IOState_t, bool_t boIsBAKed = FALSE);
	void_t Indicate(LED::Color_t ioColor, LED::Action_t ioAction =
			LED::Action::Hold, u32_t idwTime = 0, u32_t idwDuty = 1,
			u32_t idwNo = 0, bool_t boIsBAKed = FALSE);
	void_t HandleLEDTimerWork(void_p);
	void_t Inform(Event_t);

	void_t ButtonEvents(bool_t);
};

typedef IO 	IO_t;
typedef IO* IO_p;

#endif /* DRIVERS_IO_HPP_ */
