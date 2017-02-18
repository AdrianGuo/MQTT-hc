/*
 * IO.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: taho
 */

#include <LogPlus.hpp>
#include <IO.hpp>

IO* IO::s_pInstance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
IO::IO(
) : m_LED(18, 19)
{
    m_idwNo = 0;
    m_boIsBAKed = FALSE;

	m_pLocker   = new Locker();
    m_pRTimer   = RTimer::getTimerInstance();
    m_TimerFunctor   = makeFunctor((TimerFunctor_p) NULL, *this, &IO::HandleTimerWork);
    m_iTimerID = -1;
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

	if(m_iTimerID != -1) {
		LOG_DEBUG("========== CancelTimer ==========");
		m_pRTimer->CancelTimer(m_iTimerID);
		m_pLocker->Lock();
		m_iTimerID = -1;
		m_idwNo = 0;
		m_pLocker->UnLock();
	}

	if(ioAction == LED::Action::Latch) {
		LOG_DEBUG("========== Start: Latch ==========");
		m_LED.Set(ioColor);
	} else if(ioAction == LED::Action::Hold) {
		LOG_DEBUG("========== Start: Hold ==========");
		m_LED.Set(ioColor);
		if(ioTime > 0) {
			m_iTimerID = m_pRTimer->StartTimer(RTimer::Repeat::OneTime, ioTime, &m_TimerFunctor, NULL);
		}
	} else if(ioAction == LED::Action::Blink) {
		LOG_DEBUG("========== Start: Blink ==========");
		if(m_ioCurState.ioColor < 10) {
			m_LED.Set(ioColor);
		} else {
			m_LED.Set((LED::Color) (ioColor/10));
		}
		if(m_idwNo == 0) {
			if(ioNo != 0) {
				m_pLocker->Lock();
				m_idwNo++;
				m_pLocker->UnLock();
				LOG_DEBUG("========== %d ==========", m_idwNo);
			}
			m_iTimerID = m_pRTimer->StartTimer(RTimer::Repeat::Forever, (ioDuty + 1), &m_TimerFunctor, NULL);
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
IO::HandleTimerWork(
	void_p pbyBuffer
) {
	LOG_DEBUG("========== HandleTimerWork ==========");

	if(m_ioCurState.ioAction == LED::Action::Hold) {
		LOG_DEBUG("========== Handle: Hold ==========");
		if(m_iTimerID != -1) {
			m_pRTimer->CancelTimer(m_iTimerID);
			m_pLocker->Lock();
			m_iTimerID = -1;
			m_pLocker->UnLock();
		}
		Indicate(m_ioBakState, TRUE);
	} else if (m_ioCurState.ioAction == LED::Action::Blink) {
		LOG_DEBUG("========== Handle: Blink ==========");
		if(m_idwNo < m_ioCurState.ioNo || m_ioCurState.ioNo == 0) {
			if(m_ioCurState.ioNo != 0) {
				m_pLocker->Lock();
				m_idwNo++;
				m_pLocker->UnLock();
				LOG_DEBUG("========== %d ==========", m_idwNo);
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
		} else if(m_idwNo >= m_ioCurState.ioNo) {
			if(m_iTimerID != -1) {
				m_pRTimer->CancelTimer(m_iTimerID);
				m_pLocker->Lock();
				m_iTimerID = -1;
				m_pLocker->UnLock();
			}
			m_pLocker->Lock();
			m_idwNo = 0;
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
			Indicate(LED::Color::Pink, LED::Action::Latch);
			break;

		case Start:
		case Reset:
		case Upgraded:
			Indicate(LED::Color::Pink, LED::Action::Blink, 0, 1, 3, FALSE);
			break;

		case NotInternet:
			Indicate(LED::Color::Red, LED::Action::Blink);
			break;

		case NotReach:
			Indicate(LED::Color::Red, LED::Action::Latch);
			break;

		case Reach:
			Indicate(LED::Color::Blue, LED::Action::Latch, 0, 0, 0, TRUE);
			break;

		case AppSig:
			Indicate(LED::Color::Blue, LED::Action::Hold, 1, 0, 0, FALSE);
			break;

		case DevSig:
			Indicate(LED::Color::Red, LED::Action::Hold, 1, 0, 0, FALSE);
			break;

		case Broadcast:
			Indicate(LED::Color::Blue, LED::Action::Blink);
			break;

		case Upgrading:
			Indicate(LED::Color::RedBlue, LED::Action::Blink);
			break;

		default:
			break;
	}
}
