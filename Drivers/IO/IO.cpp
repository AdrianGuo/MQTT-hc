/*
 * IO.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */

#include <LogPlus.hpp>
#include <ZbBasicCmd.hpp>
#include <IO.hpp>

#define JOINABLE_HOLDTIME 			(1)
#define FACTORYRESET_HOLDTIME 		(5)

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
    m_idwBlinkedNo = 0;
    m_idwReleasedNo = 0;
    m_boIsBAKed = FALSE;
    m_boIsAllowedReady = FALSE;
    m_boIsFResetReady = FALSE;
    m_boIsDisallowedReady = FALSE;
    m_boIsCanceled = FALSE;
    m_ButtonFunctor = makeFunctor((ButtonFunctor_p) NULL, *this, &IO::ButtonEvents);
    m_Button.RecvFunctor(&m_ButtonFunctor);

	m_pLocker   = new Locker();
    m_pRTimer   = RTimer::getTimerInstance();
    m_LEDTimerFunctor   = makeFunctor((TimerFunctor_p) NULL, *this, &IO::HandleLEDTimerWork);
    m_iLEDTimerID = -1;
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
	IOState_t ioState,
	bool_t boIsBackup
) {
	Indicate(ioState.ioColor, ioState.ioAction,
			ioState.ioTime, ioState.ioDuty, ioState.ioNo, boIsBackup);
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
	bool_t boIsBackup
) {
	m_pLocker->Lock();
	m_boIsBAKed = boIsBackup;
	m_ioCurState.ioColor = ioColor;
	m_ioCurState.ioAction = ioAction;
	m_ioCurState.ioTime = ioTime;
	m_ioCurState.ioDuty = ioDuty;
	m_ioCurState.ioNo = ioNo;
	m_pLocker->UnLock();

	if(boIsBackup == TRUE) {
		LOG_DEBUG("========== Backup State ==========");
		m_pLocker->Lock();
		m_ioBakState = m_ioCurState;
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
IO::HandleLEDTimerWork(
	void_p pbyBuffer
) {
	LOG_DEBUG("========== HandleLEDTimerWork ==========");

	if(m_ioCurState.ioAction == LED::Action::Hold) {
		LOG_DEBUG("========== Handle: Hold ==========");
		if(m_iLEDTimerID != -1) {
			m_pRTimer->CancelTimer(m_iLEDTimerID);
			m_pLocker->Lock();
			m_iLEDTimerID = -1;
			m_pLocker->UnLock();
		}
		Indicate(m_ioBakState, TRUE);
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
			if(m_iLEDTimerID != -1) {
				m_pRTimer->CancelTimer(m_iLEDTimerID);
				m_pLocker->Lock();
				m_iLEDTimerID = -1;
				m_pLocker->UnLock();
			}
			m_pLocker->Lock();
			m_idwBlinkedNo = 0;
			m_pLocker->UnLock();
			Indicate(m_ioBakState, TRUE);
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
IO::Inform(
	Event_t ioEvent
) {
	switch (ioEvent) {
		case NotStart:
			if(m_ioCurState.ioColor == LED::Color::Pink) {
				m_LED.Set(LED::Color::Off);
				usleep(200000);
			}
			Indicate(LED::Color::Pink, LED::Action::Latch);
			break;

		case Start:
		case Reset:
		case Upgraded:
			if(m_ioCurState.ioColor == LED::Color::Pink) {
				m_LED.Set(LED::Color::Off);
				usleep(200000);
			}
			Indicate(LED::Color::Pink, LED::Action::Blink, 0, 1, 3, FALSE);
			break;

		case NotInternet:
			if(m_ioCurState.ioColor == LED::Color::Red) {
				m_LED.Set(LED::Color::Off);
				usleep(200000);
			}
			Indicate(LED::Color::Red, LED::Action::Blink);
			break;

		case NotReach:
			if(m_ioCurState.ioColor == LED::Color::Red) {
				m_LED.Set(LED::Color::Off);
				usleep(200000);
			}
			Indicate(LED::Color::Red, LED::Action::Latch);
			break;

		case Reach:
			if(m_ioCurState.ioColor == LED::Color::Blue) {
				m_LED.Set(LED::Color::Off);
				usleep(200000);
			}
			Indicate(LED::Color::Blue, LED::Action::Latch, 0, 0, 0, TRUE);
			break;

		case AppSig:
			if(m_ioCurState.ioColor == LED::Color::Blue) {
				m_LED.Set(LED::Color::Off);
				usleep(200000);
			}
			Indicate(LED::Color::Blue, LED::Action::Hold, 1, 0, 0, FALSE);
			break;

		case DevSig:
			if(m_ioCurState.ioColor == LED::Color::Red) {
				m_LED.Set(LED::Color::Off);
				usleep(200000);
			}
			Indicate(LED::Color::Red, LED::Action::Hold, 1, 0, 0, FALSE);
			break;

		case Broadcast:
			if(m_ioCurState.ioColor == LED::Color::Blue) {
				m_LED.Set(LED::Color::Off);
				usleep(200000);
			}
			Indicate(LED::Color::Blue, LED::Action::Blink, 0, 0, 10, FALSE);
			break;

		case Upgrading:
			if(m_ioCurState.ioColor == LED::Color::RedBlue) {
				m_LED.Set(LED::Color::Off);
				usleep(200000);
			}
			Indicate(LED::Color::RedBlue, LED::Action::Blink);
			break;

		case Allowed:
			if(m_ioCurState.ioColor == LED::Color::Blue) {
				m_LED.Set(LED::Color::Off);
				usleep(200000);
			}
			Indicate(LED::Color::Blue, LED::Action::Blink);
			break;

		default:
			break;
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
		LOG_DEBUG("========== The button was pressed! ==========");
		usleep(700000);
		if(m_idwReleasedNo == 0) {
			if(m_ioCurState.ioColor == LED::Color::Pink) {
				m_LED.Set(LED::Color::Off);
				usleep(300000);
			}
			Indicate(LED::Color::Pink, LED::Action::Latch);
			m_pLocker->Lock();
			m_boIsAllowedReady = TRUE;
			m_pLocker->UnLock();

			//Wait for disallowing to join event.
			u8_t byCount = 2*1000000/100000;
			while(byCount > 0) {
				if(m_idwReleasedNo > 0) {
					return;
				}
				usleep(100000);
				byCount--;
			}
			Indicate(LED::Color::Blue, LED::Action::Latch);
			m_pLocker->Lock();
			m_boIsDisallowedReady = TRUE;
			m_pLocker->UnLock();

			//Wait for factory reset event.
			byCount = 3*1000000/100000;
			while(byCount > 0) {
				if(m_idwReleasedNo > 0) {
					return;
				}
				usleep(100000);
				byCount--;
			}
			Indicate(LED::Color::Red, LED::Action::Latch);
			m_pLocker->Lock();
			m_boIsFResetReady = TRUE;
			m_pLocker->UnLock();

			//Wait for cacle event.
			byCount = 3*1000000/100000;
			while(byCount > 0) {
				if(m_idwReleasedNo > 0) {
					return;
				}
				usleep(100000);
				byCount--;
			}
			m_LED.Set(LED::Color::Off);
			m_pLocker->Lock();
			m_boIsCanceled = TRUE;
			m_pLocker->UnLock();
		}
	} else {
		LOG_DEBUG("========== The button was released! ==========");
		m_pLocker->Lock();
		m_idwReleasedNo++;
		m_pLocker->UnLock();
		if(m_boIsAllowedReady == TRUE &&
				m_boIsDisallowedReady == FALSE) {
			/*
			 * Allow to join network.
			 */
			m_LED.Set(LED::Color::Off);
			Inform(IO::Event::Allowed);
			ZbBasicCmd::GetInstance()->JoinNwkAllow(0xFF);

			m_pLocker->Lock();
			m_idwReleasedNo = 0;
			m_boIsAllowedReady = FALSE;
			m_pLocker->UnLock();
		}

		if(m_boIsDisallowedReady == TRUE &&
				m_boIsFResetReady == FALSE) {
			LOG_DEBUG("=========================================");
			/*
			 * Disallow to join network.
			 */
//			m_LED.Set(LED::Color::Off);
			Indicate(m_ioBakState, TRUE);
			ZbBasicCmd::GetInstance()->JoinNwkAllow((u8_t) 0x00);

			m_pLocker->Lock();
			m_idwReleasedNo = 0;
			m_boIsAllowedReady = FALSE;
			m_boIsDisallowedReady = FALSE;
			m_pLocker->UnLock();
		}

		if(m_boIsFResetReady == TRUE &&
				m_boIsCanceled == FALSE) {
			/*
			 * Factor reset.
			 */
			m_LED.Set(LED::Color::Off);
			Inform(IO::Event::Reset);
			LOG_WARN("Factory reset !!!");

			m_pLocker->Lock();
			m_idwReleasedNo = 0;
			m_boIsAllowedReady = FALSE;
			m_boIsDisallowedReady = FALSE;
			m_boIsFResetReady = FALSE;
			m_pLocker->UnLock();
		}

		if(m_boIsCanceled == TRUE) {
			/*
			 * Cancel press button event.
			 */
			m_LED.Set(LED::Color::Off);
			Indicate(m_ioBakState, TRUE);
			LOG_WARN("Pressed cancel!");
			ZbBasicCmd::GetInstance()->JoinNwkAllow((u8_t) 0x00);

			m_pLocker->Lock();
			m_idwReleasedNo = 0;
			m_boIsAllowedReady = FALSE;
			m_boIsDisallowedReady = FALSE;
			m_boIsFResetReady = FALSE;
			m_boIsCanceled = FALSE;
			m_pLocker->UnLock();
		}
	}
}
