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

#ifdef MT7688
#define IO_Init()		{IO::GetInstance();}
#define Notify(x)		{IO::GetInstance()->Inform(IO::Event::x);}
#define DrtSetLED(x)	{IO::GetInstance()->DrtSet(LED::Color::x);}
#define RestoreLED()	{IO::GetInstance()->Restore();}
#else
#define IO_Init()		{}
#define Notify(x)		{}
#define DrtSet(x)		{}
#define Restore()		{}
#endif

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
#ifdef MT7688
class IO {
public:
    ~IO();
    static IO* s_pInstance;
    static  IO* GetInstance();

    typedef enum Event {
    	Unidentified = 0,	/* 0:  */
		Backup, 			/* 1:  */
    	NotStart, 			/* 2:  latch-pink */
        Start, 				/* 3:  blink-pink-3 */
		Reset, 				/* 4:  blink-pink-3 */
		Upgraded, 			/* 5:  blink-pink-3 */
        NotInternet, 		/* 6:  blink-red-0 */
        NotReach, 			/* 7:  latch-red */
		Reach, 				/* 8:  latch-blue */
		AppSig, 			/* 9:  blink-blue-1 */
		DevSig, 			/* 10: blink-red-1 */
		Broadcast, 			/* 11: blink-blue-10 */
		Upgrading, 			/* 12: blink-blue/red-0*/
		Allowed, 			/* 13: blink-blue-0*/
		HoldButton,			/* 14: */
		Hidden				/* 15: */
    } Event_t;

    typedef enum PrioLevel {
    	Low = 0,
    	Normal,
		High,
		Critical
    } PrioLevel_t;

    typedef struct IOState {
    	IO::Event		ioName = Unidentified;
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
        	ioName		= rhs.ioName;
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
        	if(ioName != rhs.ioName) return true;
            if(ioColor != rhs.ioColor) return true;
            if(ioAction != rhs.ioAction) return true;
            if(ioTime != rhs.ioTime) return true;
            if(ioDuty != rhs.ioDuty) return true;
            if(ioNo != rhs.ioNo) return true;
            return false;
        }
    } IOState_t;

	void_t Inform(Event_t);
	void_t Restore();
	void_t DrtSet(LED::Color_t);

private:
    LED       	m_LED;
    Button		m_Button;
    IOState_t 	m_ioBakState;
    IOState_t 	m_ioCurState;
    PrioLevel_t	m_prioCurLevel;

    Map<Event_t,IOState_t>	m_mapEvents;

    u32_t 		m_idwBlinkedNo;
    u32_t		m_idwReleasedNo;
    ButtonFunctor_t m_ButtonFunctor;

    u8_t		m_byButtonEvent;

    Locker_p        m_pLocker;
    RTimer_p        m_pRTimer;
    TimerFunctor_t  m_LEDTimerFunctor;
    int_t			m_iLEDTimerID;
    TimerFunctor_t  m_HiddenTimerFunctor;
    int_t			m_iHiddenTimerID;

    IO();

    void_t RegisterEvent();
    void_t Indicate(IOState_t);

	void_t HandleLEDTimerWork(void_p);
	void_t ButtonEvents(bool_t);
	void_t HiddenFunctions();
	void_t HandleHiddenTimerWork(void_p);
};

typedef IO 	IO_t;
typedef IO* IO_p;
#endif

#endif /* DRIVERS_IO_HPP_ */
