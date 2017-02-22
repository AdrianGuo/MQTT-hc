/*
 * IO.hpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */

#ifndef DRIVERS_IO_HPP_
#define DRIVERS_IO_HPP_

#include <Map.hpp>
#include <RTimer.hpp>
#include <Locker.hpp>
#include <LED.hpp>
#include <Button.hpp>

#define Notify(x)		{IO::GetInstance()->Inform(IO::Event::x);}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class IO {
public:
    ~IO();
    static IO* s_pInstance;
    static  IO* GetInstance();


    typedef enum PrioLevel {
    	Low = 0,
    	Normal,
		High,
		Critical
    } PrioLevel_t;

    typedef struct IOState {
    	LED::Color_t 	ioColor = LED::Color::Off;
    	LED::Action_t 	ioAction = LED::Action::Latch;
    	u32_t 			ioTime = 0;
    	u32_t 			ioDuty = 1;
    	u32_t 			ioNo = 0;
    	bool_t			boIsBAKed = FALSE;
    	PrioLevel_t 	prioLevel = PrioLevel::Low;
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
		Allowed, // blink-blue-0
		Hold1s,
		Hold3s,
		Hold7s,
		Hold10s
    } Event_t;

    void_t Indicate(IOState_t);
	void_t Indicate(LED::Color_t ioColor, LED::Action_t ioAction, u32_t idwTime,
			u32_t idwDuty, u32_t idwNo, bool_t boIsBAKed,
			PrioLevel_t prioCurLevel);
	void_t HandleLEDTimerWork(void_p);

	void_t Inform(Event_t);
	void_t PressedEvent();
	void_t ReleasedEvent();

private:
    LED       	m_LED;
    Button		m_Button;
    IOState_t 	m_ioBakState;
    IOState_t 	m_ioCurState;
    PrioLevel_t	m_prioCurLevel;

    Map<Event_t,IOState_t>	m_mapEvents;

    u32_t 		m_idwBlinkedNo;
    u32_t		m_idwReleasedNo;
    ButtonFunctor_t m_PressedFunctor;
    ButtonFunctor_t m_ReleasedFunctor;

    u8_t		m_byButtonEvent;

    Locker_p        m_pLocker;
    RTimer_p        m_pRTimer;
    TimerFunctor_t  m_LEDTimerFunctor;
    int_t			m_iLEDTimerID;

    IO();
    void_t RegisterEvent();
};

typedef IO 	IO_t;
typedef IO* IO_p;

#endif /* DRIVERS_IO_HPP_ */
