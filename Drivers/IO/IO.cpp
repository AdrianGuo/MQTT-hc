/*
 * IO.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */
#ifdef MT7688
#include <LogPlus.hpp>
#include <ZbBasicCmd.hpp>
#include <ZbSocketCmd.hpp>
#include <IO.hpp>

#define PERIOD				(100000) //us
#define ALLOW 				(1*1000000/PERIOD)
#define DISALLOW 			(3*1000000/PERIOD)
#define HARDRESET 			(7*1000000/PERIOD)
#define CANCEL 				(10*1000000/PERIOD)

#define HIDDEN 				(3)

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
    m_idwReleasedNo = 1;
    m_byButtonEvent = 0;
    m_ButtonFunctor = makeFunctor((ButtonFunctor_p) NULL, *this, &IO::ButtonEvents);
    m_Button.RecvFunctor(&m_ButtonFunctor);

	m_pLocker   = new Locker();
    m_pRTimer   = RTimer::getTimerInstance();
    m_LEDTimerFunctor   = makeFunctor((TimerFunctor_p) NULL, *this, &IO::HandleLEDTimerWork);
    m_iLEDTimerID = -1;
    m_HiddenTimerFunctor   = makeFunctor((TimerFunctor_p) NULL, *this, &IO::HandleHiddenTimerWork);
    m_iHiddenTimerID = -1;

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
IO::RegisterEvent(
) {
	m_mapEvents[IO::Event::NotStart].ioColor = LED::Color::Pink;
	m_mapEvents[IO::Event::NotStart].ioAction = LED::Action::Latch;

	m_mapEvents[IO::Event::Backup] = m_ioBakState;

	m_mapEvents[IO::Event::Start].ioColor = LED::Color::Pink;
	m_mapEvents[IO::Event::Start].ioAction = LED::Action::Blink;
	m_mapEvents[IO::Event::Start].ioNo = 3;

	m_mapEvents[IO::Event::Reset] = m_mapEvents[IO::Event::Start];

	m_mapEvents[IO::Event::Upgraded] = m_mapEvents[IO::Event::Start];

	m_mapEvents[IO::Event::NotInternet].ioColor = LED::Color::Red;
	m_mapEvents[IO::Event::NotInternet].ioAction = LED::Action::Blink;

	m_mapEvents[IO::Event::NotReach].ioColor = LED::Color::Red;
	m_mapEvents[IO::Event::NotReach].ioAction = LED::Action::Latch;

	m_mapEvents[IO::Event::Reach].ioColor = LED::Color::Blue;
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
	m_mapEvents[IO::Event::Upgrading].prioLevel = IO::PrioLevel::High;

	m_mapEvents[IO::Event::Allowed].ioColor = LED::Color::Blue;
	m_mapEvents[IO::Event::Allowed].ioAction = LED::Action::Blink;

	m_mapEvents[IO::Event::HoldButton].ioColor = LED::Color::Pink;
	m_mapEvents[IO::Event::HoldButton].ioAction = LED::Action::Latch;
	m_mapEvents[IO::Event::HoldButton].prioLevel = IO::PrioLevel::High;

	m_mapEvents[IO::Event::Hidden].ioColor = LED::Color::RedPink;
	m_mapEvents[IO::Event::Hidden].ioAction = LED::Action::Blink;
	m_mapEvents[IO::Event::Hidden].ioNo = 2;
	m_mapEvents[IO::Event::Hidden].prioLevel = IO::PrioLevel::Critical;

	for(Map<Event_t,IOState_t>::const_iterator it = m_mapEvents.begin(); it != m_mapEvents.end(); it++) {
		m_mapEvents[it->first].ioName = it->first;
	}
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
IO::Indicate(
	IOState_t ioState
) {
	LOG_DEBUG("========== Event: %d, Prio: %d ==========", ioState.ioName, ioState.prioLevel);

	if(ioState.boIsBAKed == TRUE) {
		m_pLocker->Lock();
		m_ioBakState = ioState;
		m_pLocker->UnLock();
		m_mapEvents[IO::Event::Backup] = m_ioBakState;
	}

	if(ioState.prioLevel < m_prioCurLevel) {
		LOG_DEBUG("========== Lower priority!!! ==========");
		return;
	} else {

		if(m_iLEDTimerID != -1) {
			LOG_DEBUG("========== CancelTimer ==========");
			m_pRTimer->CancelTimer(m_iLEDTimerID);
			m_pLocker->Lock();
			m_iLEDTimerID = -1;
			m_idwBlinkedNo = 0;
			m_pLocker->UnLock();
		}

		m_pLocker->Lock();
		m_prioCurLevel = ioState.prioLevel;
		m_ioCurState = ioState;
		m_pLocker->UnLock();
	}

	if(m_ioCurState.ioAction == LED::Action::Latch) {
		LOG_DEBUG("========== Start: Latch ==========");;
		m_LED.Set(m_ioCurState.ioColor);
	} else if(m_ioCurState.ioAction == LED::Action::Hold) {
		LOG_DEBUG("========== Start: Hold ==========");
		m_LED.Set(m_ioCurState.ioColor);
		if(ioState.ioTime > 0) {
			m_iLEDTimerID = m_pRTimer->StartTimer(RTimer::Repeat::OneTime, m_ioCurState.ioTime, &m_LEDTimerFunctor, &m_mapEvents[ioState.ioName].ioName);
		}
	} else if(m_ioCurState.ioAction == LED::Action::Blink) {
		LOG_DEBUG("========== Start: Blink ==========");
		m_iLEDTimerID = m_pRTimer->StartTimer(RTimer::Repeat::Forever, 0, &m_LEDTimerFunctor, &m_mapEvents[ioState.ioName].ioName);
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
	if(m_iLEDTimerID == -1 ||
			m_ioCurState.ioName != *((IO::Event*) pbyBuffer)) return;

	if(m_ioCurState.ioAction == LED::Action::Hold) {
		LOG_DEBUG("========== Handle: Hold ==========");
		if(m_iLEDTimerID == -1 ||
				m_ioCurState.ioName != *((IO::Event*) pbyBuffer)) return;
		m_prioCurLevel = PrioLevel::Low;
		Indicate(m_ioBakState);
	} else if (m_ioCurState.ioAction == LED::Action::Blink) {
		LOG_DEBUG("========== Handle: Blink ==========");
		if(m_idwBlinkedNo < m_ioCurState.ioNo || m_ioCurState.ioNo == 0) {
			m_pRTimer->ChangeTimeout(m_iLEDTimerID, m_ioCurState.ioDuty + 1);
			if(m_ioCurState.ioNo != 0) {
				m_pLocker->Lock();
				m_idwBlinkedNo++;
				m_pLocker->UnLock();
			}
			if(m_ioCurState.ioColor < 10) {
				if(m_iLEDTimerID == -1 ||
							m_ioCurState.ioName != *((IO::Event*) pbyBuffer)) return;
				m_LED.Set(m_ioCurState.ioColor);
				sleep(m_ioCurState.ioDuty);
				if(m_iLEDTimerID == -1 ||
							m_ioCurState.ioName != *((IO::Event*) pbyBuffer)) return;
				m_LED.Set(LED::Color::Off);
			} else {
				if(m_iLEDTimerID == -1 ||
							m_ioCurState.ioName != *((IO::Event*) pbyBuffer)) return;
				m_LED.Set((LED::Color) (m_ioCurState.ioColor/10));
				sleep(m_ioCurState.ioDuty);
				if(m_iLEDTimerID == -1 ||
							m_ioCurState.ioName != *((IO::Event*) pbyBuffer)) return;
				m_LED.Set((LED::Color) (m_ioCurState.ioColor%10));
			}
		} else if(m_idwBlinkedNo >= m_ioCurState.ioNo) {
			if(m_iLEDTimerID == -1 ||
						m_ioCurState.ioName != *((IO::Event*) pbyBuffer)) return;
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
IO::ButtonEvents(
    bool_t IsPressed
) {
	if(IsPressed == TRUE) {
		LOG_DEBUG("========== The button was pressed %d! ==========", m_idwReleasedNo);

		if(m_idwReleasedNo == 10) {
			HiddenFunctions();
		}

		if(m_idwReleasedNo >= 0xFFFFFFFE)
			m_idwReleasedNo = 1;

		u32_t tmpNo = m_idwReleasedNo;

		u8_t byCount = 0;
		bool_t byResetCount = TRUE;
		m_byButtonEvent = 0;
		while(1) {
			usleep(PERIOD);
			byCount++;
			if(byCount >= ALLOW) {
				if(byResetCount == TRUE){
					m_idwReleasedNo = 1;
					byResetCount = FALSE;
				}

				if(m_byButtonEvent == 0) {
					if(m_ioCurState.ioColor == LED::Color::Pink) {
						m_LED.Set(LED::Color::Off);
						usleep(200000);
					}
					m_byButtonEvent = 1;
					Inform(IO::Event::HoldButton);
				}

				if(byCount >= DISALLOW && m_byButtonEvent == 1) {
					m_byButtonEvent = 3;
					m_LED.Set(LED::Color::Blue);
				}

				if(byCount >= HARDRESET && m_byButtonEvent == 3) {
					m_byButtonEvent = 7;
					m_LED.Set(LED::Color::Red);
				}

				if(byCount >= CANCEL && m_byButtonEvent == 7) {
					m_byButtonEvent = 10;
					m_LED.Set(LED::Color::Off);
				}

				if(m_idwReleasedNo > 1) {
					m_pLocker->Lock();
					m_idwReleasedNo = 1;
					m_pLocker->UnLock();
					return;
				}
			} else {
				if(m_idwReleasedNo != tmpNo)
					return;
			}
		}
	} else {
		LOG_DEBUG("========== The button was released! ==========");
		m_idwReleasedNo++;

		if(m_byButtonEvent != 0) {
			m_pLocker->Lock();
			m_prioCurLevel = PrioLevel::Low;
			m_pLocker->UnLock();
		}

		if(m_byButtonEvent == 1) {
			/*
			 * Allow to join network.
			 */
			LOG_DEBUG("Allow to join network!");
			m_LED.Set(LED::Color::Off);
			Inform(IO::Event::Allowed);
			ZbSocketCmd::GetInstance()->SendDevAdd(0);
		} else if(m_byButtonEvent == 3) {
			/*
			 * Disallow to join network.
			 */
			LOG_DEBUG("Disallow to join network!");
			m_LED.Set(LED::Color::Off);
			Indicate(m_ioBakState);
			ZbSocketCmd::GetInstance()->SendDevAdd(1);
		} else if(m_byButtonEvent == 7) {
			/*
			 * Factor reset.
			 */
			m_LED.Set(LED::Color::Off);
			Inform(IO::Event::Reset);
			sleep(2);
			LOG_WARN("Factory reset !!!");
			system("jffs2reset -y");
			system("sync");
			system("reboot");
		} else if(m_byButtonEvent == 10) {
			/*
			 * Cancel press button event.
			 */
			m_LED.Set(LED::Color::Off);
			Indicate(m_ioBakState);
			LOG_WARN("Pressed cancel!");
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
IO::HiddenFunctions(
) {
/*
 * For hidden functions for future!!!
 */
	LOG_DEBUG("xxxxxxxxxxxx HiddenFunctions: Actived! xxxxxxxxxxxx");
	Inform(IO::Event::Hidden);
	if(m_iHiddenTimerID != -1) {
		if(m_pRTimer->CancelTimer(m_iHiddenTimerID))
			m_iHiddenTimerID = -1;
	}
	m_iHiddenTimerID = m_pRTimer->StartTimer(RTimer::Repeat::OneTime, HIDDEN, &m_HiddenTimerFunctor, NULL);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
IO::HandleHiddenTimerWork(
	void_p pbyBuffer
) {
	if(m_iHiddenTimerID == -1 || m_idwReleasedNo < 11) return;
	u32_t pressed = m_idwReleasedNo - 11;
	LOG_DEBUG("xxxxxxxxxxxx (%d) xxxxxxxxxxxx", pressed);
	if(pressed == 1) {
		LOG_DEBUG("xxxxxxxxxxxx Switch to DHCP xxxxxxxxxxxx");
		system("uci set  network.lan.proto=dhcp");
		system("uci delete  network.lan.ipaddr");
		system("uci delete  network.lan.netmask");
	} else if(pressed == 3) {
		LOG_DEBUG("xxxxxxxxxxxx Switch to AP xxxxxxxxxxxx");
		system("uci set  wireless.radio0.linkit_mode=sta");
		system("uci set wireless.sta.disabled=0");
	} else if (pressed == 5) {
		LOG_DEBUG("xxxxxxxxxxxx Switch to STA xxxxxxxxxxxx");
		system("uci set  wireless.radio0.linkit_mode=sta");
		system("uci set wireless.sta.disabled=0");
	} else if (pressed == 7) {
		LOG_DEBUG("xxxxxxxxxxxx Reset password xxxxxxxxxxxx");
		system("cp /etc/shadow.bak /etc/shadow");
	}
}

#endif
