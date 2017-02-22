/*
 * IO.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */

#include <LogPlus.hpp>
#include <ZbBasicCmd.hpp>
#include <IO.hpp>

#define PERIOD				(100000) //us
#define ALLOW 				(1*1000000/PERIOD)
#define DISALLOW 			(3*1000000/PERIOD)
#define HARDRESET 			(7*1000000/PERIOD)
#define CANCEL 				(10*1000000/PERIOD)

IO* IO::s_pInstance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
IO::IO(
) : m_LED(18, 19),
	m_Button()
{
    m_prioCurLevel = PrioLevel::Low;
    m_idwBlinkedNo = 0;
    m_idwReleasedNo = 0;
    m_byButtonEvent = 0;
    m_PressedFunctor = makeFunctor((ButtonFunctor_p) NULL, *this, &IO::PressedEvent);
    m_ReleasedFunctor = makeFunctor((ButtonFunctor_p) NULL, *this, &IO::ReleasedEvent);
    m_Button.RecvFunctor(&m_PressedFunctor, &m_ReleasedFunctor);

	m_pLocker   = new Locker();
    m_pRTimer   = RTimer::getTimerInstance();
    m_LEDTimerFunctor   = makeFunctor((TimerFunctor_p) NULL, *this, &IO::HandleLEDTimerWork);
    m_iLEDTimerID = -1;

    RegisterEvent();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
IO::~IO() {
	delete m_pLocker;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
IO*
IO::GetInstance(
) {
    if(s_pInstance == NULL) {
        s_pInstance = new IO();
    }
    return s_pInstance;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
IO::Indicate(
	IOState_t ioState
) {
	Indicate(ioState.ioColor, ioState.ioAction,
			ioState.ioTime, ioState.ioDuty, ioState.ioNo, ioState.boIsBAKed, ioState.prioLevel);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
IO::Indicate(
	LED::Color_t ioColor,
	LED::Action_t ioAction,
	u32_t ioTime,
	u32_t ioDuty,
	u32_t ioNo,
	bool_t boIsBAKed,
	PrioLevel_t prioLevel
) {
	IOState_t ioTemp;
	ioTemp.ioColor  = ioColor;
	ioTemp.ioAction = ioAction;
	ioTemp.ioTime   = ioTime;
	ioTemp.ioDuty   = ioDuty;
	ioTemp.ioNo     = ioNo;

	if(boIsBAKed == TRUE) {
		LOG_DEBUG("========== Backup State ==========");
		m_pLocker->Lock();
		m_ioBakState = ioTemp;
		m_pLocker->UnLock();
	}

	if(prioLevel < m_prioCurLevel) {
		return;
	} else {
		m_pLocker->Lock();
		m_prioCurLevel = prioLevel;
		m_ioCurState = ioTemp;
		m_pLocker->UnLock();
	}

	if(m_iLEDTimerID != -1) {
		LOG_DEBUG("========== CancelTimer ==========");
		m_pRTimer->CancelTimer(m_iLEDTimerID);
		m_pLocker->Lock();
		m_iLEDTimerID = -1;
		m_idwBlinkedNo = 0;
		m_pLocker->UnLock();
	}

	if(ioAction == LED::Action::Latch) {
		LOG_DEBUG("========== Start: Latch ==========");
		m_pLocker->Lock();
		m_prioCurLevel = PrioLevel::Low;
		m_pLocker->UnLock();
		m_LED.Set(ioColor);
	} else if(ioAction == LED::Action::Hold) {
		LOG_DEBUG("========== Start: Hold ==========");
		m_LED.Set(ioColor);
		if(ioTime > 0) {
			m_iLEDTimerID = m_pRTimer->StartTimer(RTimer::Repeat::OneTime, ioTime, &m_LEDTimerFunctor, NULL);
		}
	} else if(ioAction == LED::Action::Blink) {
		LOG_DEBUG("========== Start: Blink ==========");
		if(m_ioCurState.ioColor < 10) {
			m_LED.Set(ioColor);
		} else {
			m_LED.Set((LED::Color) (ioColor/10));
		}
		if(m_idwBlinkedNo == 0) {
			if(ioNo != 0) {
				m_pLocker->Lock();
				m_idwBlinkedNo++;
				m_pLocker->UnLock();
				LOG_DEBUG("========== %d ==========", m_idwBlinkedNo);
			}
			m_iLEDTimerID = m_pRTimer->StartTimer(RTimer::Repeat::Forever, (ioDuty + 1), &m_LEDTimerFunctor, NULL);
		}
		sleep(ioDuty);
		if(m_ioCurState.ioColor < 10) {
			m_LED.Set(LED::Color::Off);
		} else {
			m_LED.Set((LED::Color) (ioColor%10));
		}
	}

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
IO::RegisterEvent(
) {
	m_mapEvents[IO::Event::NotStart].ioColor = LED::Color::Pink;
	m_mapEvents[IO::Event::NotStart].ioAction = LED::Action::Latch;

	m_mapEvents[IO::Event::Start].ioColor = LED::Color::Pink;
	m_mapEvents[IO::Event::Start].ioAction = LED::Action::Blink;
	m_mapEvents[IO::Event::Start].ioNo = 3;

	m_mapEvents[IO::Event::Reset] = m_mapEvents[IO::Event::Start];

	m_mapEvents[IO::Event::Upgraded] = m_mapEvents[IO::Event::Start];

	m_mapEvents[IO::Event::NotInternet].ioColor = LED::Color::Red;
	m_mapEvents[IO::Event::NotInternet].ioAction = LED::Action::Blink;

	m_mapEvents[IO::Event::NotReach].ioColor = LED::Color::Red;
	m_mapEvents[IO::Event::NotReach].ioAction = LED::Action::Latch;

	m_mapEvents[IO::Event::Reach].ioColor = LED::Color::Red;
	m_mapEvents[IO::Event::Reach].ioAction = LED::Action::Latch;
	m_mapEvents[IO::Event::Reach].boIsBAKed = TRUE;

	m_mapEvents[IO::Event::AppSig].ioColor = LED::Color::Blue;
	m_mapEvents[IO::Event::AppSig].ioAction = LED::Action::Hold;
	m_mapEvents[IO::Event::AppSig].ioTime = 1;

	m_mapEvents[IO::Event::DevSig].ioColor = LED::Color::Red;
	m_mapEvents[IO::Event::DevSig].ioAction = LED::Action::Hold;
	m_mapEvents[IO::Event::DevSig].ioTime = 1;

	m_mapEvents[IO::Event::Broadcast].ioColor = LED::Color::Blue;
	m_mapEvents[IO::Event::Broadcast].ioAction = LED::Action::Blink;
	m_mapEvents[IO::Event::Broadcast].ioNo = 10;

	m_mapEvents[IO::Event::Upgrading].ioColor = LED::Color::RedBlue;
	m_mapEvents[IO::Event::Upgrading].ioAction = LED::Action::Blink;

	m_mapEvents[IO::Event::Allowed].ioColor = LED::Color::Blue;
	m_mapEvents[IO::Event::Allowed].ioAction = LED::Action::Blink;

	m_mapEvents[IO::Event::Hold1s].ioColor = LED::Color::Pink;
	m_mapEvents[IO::Event::Hold1s].ioAction = LED::Action::Latch;
	m_mapEvents[IO::Event::Hold1s].prioLevel = IO::PrioLevel::High;

	m_mapEvents[IO::Event::Hold3s].ioColor = LED::Color::Blue;
	m_mapEvents[IO::Event::Hold3s].ioAction = LED::Action::Latch;
	m_mapEvents[IO::Event::Hold3s].prioLevel = IO::PrioLevel::High;

	m_mapEvents[IO::Event::Hold7s].ioColor = LED::Color::Red;
	m_mapEvents[IO::Event::Hold7s].ioAction = LED::Action::Latch;
	m_mapEvents[IO::Event::Hold7s].prioLevel = IO::PrioLevel::High;

	m_mapEvents[IO::Event::Hold10s].ioColor = LED::Color::Off;
	m_mapEvents[IO::Event::Hold10s].ioAction = LED::Action::Latch;
	m_mapEvents[IO::Event::Hold10s].prioLevel = IO::PrioLevel::High;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
IO::Inform(
	Event_t ioEvent
) {
	if(m_mapEvents.find(ioEvent) != m_mapEvents.end()) {
		if(m_ioCurState.ioColor == m_mapEvents[ioEvent].ioColor) {
			m_LED.Set(LED::Color::Off);
			usleep(200000);
		}
		Indicate(m_mapEvents[ioEvent]);
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
IO::HandleLEDTimerWork(
	void_p pbyBuffer
) {
	LOG_DEBUG("========== HandleLEDTimerWork ==========");
	if(m_iLEDTimerID == -1) return;

	if(m_ioCurState.ioAction == LED::Action::Hold) {
		LOG_DEBUG("========== Handle: Hold ==========");
		m_prioCurLevel = PrioLevel::Low;
		Indicate(m_ioBakState);
	} else if (m_ioCurState.ioAction == LED::Action::Blink) {
		LOG_DEBUG("========== Handle: Blink ==========");
		if(m_idwBlinkedNo < m_ioCurState.ioNo || m_ioCurState.ioNo == 0) {
			if(m_ioCurState.ioNo != 0) {
				m_pLocker->Lock();
				m_idwBlinkedNo++;
				m_pLocker->UnLock();
				LOG_DEBUG("========== %d ==========", m_idwBlinkedNo);
			}
			if(m_ioCurState.ioColor < 10) {
				m_LED.Set(m_ioCurState.ioColor);
				sleep(m_ioCurState.ioDuty);
				m_LED.Set(LED::Color::Off);
			} else {
				m_LED.Set((LED::Color) (m_ioCurState.ioColor/10));
				sleep(m_ioCurState.ioDuty);
				m_LED.Set((LED::Color) (m_ioCurState.ioColor%10));
			}
		} else if(m_idwBlinkedNo >= m_ioCurState.ioNo) {
			m_pRTimer->CancelTimer(m_iLEDTimerID);
			m_pLocker->Lock();
			m_iLEDTimerID = -1;
			m_idwBlinkedNo = 0;
			m_pLocker->UnLock();
			m_prioCurLevel = PrioLevel::Low;
			Indicate(m_ioBakState);
		}
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
IO::PressedEvent(
) {
	LOG_DEBUG("========== The button was pressed! ==========");
	u8_t byCount = 0;
	m_pLocker->Lock();
	m_byButtonEvent = 0;
	m_pLocker->UnLock();
	while(1) {
		usleep(PERIOD);
		byCount++;
		if(byCount >= ALLOW && m_byButtonEvent == 0) {
			if(m_ioCurState.ioColor == LED::Color::Pink) {
				m_LED.Set(LED::Color::Off);
				usleep(200000);
			}
			m_pLocker->Lock();
			m_byButtonEvent = 1;
			m_pLocker->UnLock();
			Inform(IO::Event::Hold1s);
		}

		if(byCount >= DISALLOW && m_byButtonEvent == 1) {
			m_pLocker->Lock();
			m_byButtonEvent = 3;
			m_pLocker->UnLock();
			Inform(IO::Event::Hold3s);
		}

		if(byCount >= HARDRESET && m_byButtonEvent == 3) {
			m_pLocker->Lock();
			m_byButtonEvent = 7;
			m_pLocker->UnLock();
			Inform(IO::Event::Hold7s);
		}

		if(byCount >= CANCEL && m_byButtonEvent == 7) {
			m_pLocker->Lock();
			m_byButtonEvent = 10;
			m_pLocker->UnLock();
			Inform(IO::Event::Hold10s);
		}
		LOG_DEBUG("tttttttttttttttttttttttttttttttttttttttttttttt");
		if(m_idwReleasedNo > 0) {
			m_pLocker->Lock();
			m_idwReleasedNo = 0;
			m_byButtonEvent = 0;
			m_pLocker->UnLock();
			LOG_DEBUG("yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy");
			return;
		}
	}
	LOG_DEBUG("+++++++++++++++++++++++++++++++++++++++++++++++++");
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
IO::ReleasedEvent(
) {
	LOG_DEBUG("========== The button was released! ==========");
	m_pLocker->Lock();
	m_idwReleasedNo++;
	m_pLocker->UnLock();

	m_prioCurLevel = PrioLevel::Low;

	if(m_byButtonEvent == 1) {
		/*
		 * Allow to join network.
		 */
		m_LED.Set(LED::Color::Off);
		Inform(IO::Event::Allowed);
		ZbBasicCmd::GetInstance()->JoinNwkAllow(0xFF);
	} else if(m_byButtonEvent == 3) {
		/*
		 * Disallow to join network.
		 */
		m_LED.Set(LED::Color::Off);
		Indicate(m_ioBakState);
		ZbBasicCmd::GetInstance()->JoinNwkAllow((u8_t) 0x00);
	} else if(m_byButtonEvent == 7) {
		/*
		 * Factor reset.
		 */
		m_LED.Set(LED::Color::Off);
		Inform(IO::Event::Reset);
		LOG_WARN("Factory reset !!!");
	} else if(m_byButtonEvent == 10) {
		/*
		 * Cancel press button event.
		 */
		m_LED.Set(LED::Color::Off);
		Indicate(m_ioBakState);
		LOG_WARN("Pressed cancel!");
//		ZbBasicCmd::GetInstance()->JoinNwkAllow((u8_t) 0x00);
	}
	LOG_DEBUG("------------------------------------------");
	while (m_idwReleasedNo > 0) {
		usleep(100000);
		LOG_DEBUG("___________________________________________");
	}
	LOG_DEBUG("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}
