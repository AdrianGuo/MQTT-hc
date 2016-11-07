#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/signal.h>
#include <sys/select.h>

#include "debug.hpp"
#include "Serial.hpp"

#ifndef DEBUG_SERIAL
#define debug_serial(x)
#else /* DEBUG_SERIAL */
#define debug_serial(x)             DEBUG1(x)
#endif /* DEBUG_SERIAL */

#define BUFFER_SERIAL_SIZE          1024

#define SERIAL_ERROR                (-1)
#define SERIAL_SUCCESS              (0)

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Serial::Serial(
    const_char_p portName,
    u16_t wBaudRate
) {
    m_strNamePort = portName;
    m_wBaudRate = wBaudRate;
    m_idwPortfd = 0;
    m_boIsClosing = FALSE;
    m_boIsConnected = FALSE;
    m_pSerialFunctor = NULL;

    m_pByBuffer = new u8_t[BUFFER_SERIAL_SIZE];
    memset(m_pByBuffer, 0, BUFFER_SERIAL_SIZE);

    m_pSerialLocker = new Locker();
    m_pSerialThread = new LThread();
    m_SerialthreadFunctor = makeFunctor((threadFunctor_p) NULL, *this, &Serial::SerialThreadProc);
    m_pSerialThread->RegThreadFunctor(&m_SerialthreadFunctor);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Serial::~Serial() {
    if (m_pSerialLocker != NULL) {
        delete m_pSerialLocker;
        m_pSerialLocker = NULL;
    }

    if (m_pSerialFunctor != NULL) {
        delete m_pSerialFunctor;
        m_pSerialFunctor = NULL;
    }

    if (m_pByBuffer != NULL) {
        delete[] m_pByBuffer;
        m_pByBuffer = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
Serial::SerialThreadProc(
    void_p pBuffer
) {
    Packet_p pSendPacket = NULL;
    while (TRUE) {
        m_pSerialLocker->Lock();
        if (m_boIsClosing) {
            break;
        }
        m_pSerialLocker->UnLock();

        if (IsConnected() && IsReadable(200)) {
            int_t iLength = read(m_idwPortfd, m_pByBuffer, BUFFER_SERIAL_SIZE);
            if ((m_pSerialFunctor != NULL) && (iLength > 0)) {
//                DEBUG1("RECEIVE PACKET");
                m_pSerialFunctor->operator ()(m_pByBuffer, iLength);
            }
        }

        if (IsConnected() && IsWriteable(0)) {
            m_pSerialLocker->Lock();
            if (!m_queSerialPacket.empty()) {
                pSendPacket = m_queSerialPacket.front();
                m_queSerialPacket.pop();
            }
            m_pSerialLocker->UnLock();

            if (pSendPacket != NULL) {
                int_t iResult = 0;
                if ((iResult = write(m_idwPortfd, pSendPacket->GetBuffer(), pSendPacket->Length())) == SERIAL_ERROR) {
//                    DEBUG1("   SEND PACKET FAIL");
                } else {
                    printf("Sent packet: ");
                    for(int i = 0; i < pSendPacket->Length(); i++) {
                        printf("%02X ", pSendPacket->GetBuffer()[i]);
                    }
                    printf("\n");
//                    DEBUG1("   SEND PACKET");
                }
                delete pSendPacket;
                pSendPacket = NULL;
            }
        }
    }
    m_pSerialLocker->UnLock();

    pthread_exit(NULL);

    m_pSerialLocker->Lock();
    m_boIsStarted = FALSE;
    m_boIsClosing = FALSE;
    m_pSerialLocker->UnLock();

    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Serial::SerialRecvFunctor(
    SerialFunctor_p pRecvFunctor
) {
    if (pRecvFunctor != NULL) {
        m_pSerialFunctor = pRecvFunctor;
        return TRUE;
    }
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
Serial::GetNamePort() {
    return m_strNamePort;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Serial::SetNamePort(
    String strNamePort
) {
    m_strNamePort = strNamePort;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u16_t
Serial::GetBaudRate() {
    return m_wBaudRate;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Serial::SetBaudRate(
   u16_t wBaudRate
) {
    m_wBaudRate = wBaudRate;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Serial::Connect() {
    struct termios newtio;
    bool_t boRetval = TRUE;
    m_pSerialLocker->Lock();
    if (!m_boIsConnected) {
        if ((m_idwPortfd = open(m_strNamePort.data(), O_RDWR | O_NOCTTY | O_NONBLOCK)) == SERIAL_ERROR) {
            debug_serial("connect fail");
            boRetval = FALSE;
        } else {
            m_boIsConnected = TRUE;
            fcntl(m_idwPortfd, F_SETFL, 0);
            tcgetattr(m_idwPortfd, &newtio);

            switch(m_wBaudRate) {
            case BAUD96:
                cfsetspeed(&newtio, B9600);
                break;
            case BAUD192:
                cfsetspeed(&newtio, B19200);
                break;
            case BAUD384:
                cfsetspeed(&newtio, B38400);
                break;
            case BAUD576:
                cfsetspeed(&newtio, B57600);
                break;
            case BAUD1152:
            default:
                cfsetspeed(&newtio, B115200);
            }
            // No parity (8N1), no hard flow control
            newtio.c_cflag |= (CS8 | CLOCAL | CREAD);
            newtio.c_cflag &= ~(PARENB | CSTOPB | CRTSCTS);
            newtio.c_oflag &= ~(ONLCR | OCRNL);
            newtio.c_iflag &= 0;
            newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
            newtio.c_iflag &= ~(INLCR | ICRNL);
            newtio.c_cc[VMIN] = 0; // control characters
            newtio.c_cc[VTIME] = 5; // 0.5 seconds

            tcsetattr(m_idwPortfd, TCSAFLUSH, &newtio);
            debug_serial("connect success");
        }
    } else {
        debug_serial("port's opened");
        boRetval = FALSE;
    }
    m_pSerialLocker->UnLock();
    return boRetval;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Serial::Close() {
    m_pSerialLocker->Lock();
    if (m_idwPortfd > 0) {
        close(m_idwPortfd);
    }
    m_boIsConnected = FALSE;
    m_boIsClosing = TRUE;
    m_pSerialLocker->UnLock();
    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Serial::IsClosing() {
    return m_boIsClosing;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Serial::IsConnected() {
    return m_boIsConnected;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Serial::IsWriteable(
    u32_t dwMsecTimeout
) {
    struct timeval timeout;
    bool_t boRetVal = FALSE;
    fd_set Writefd;
    int_t iResult;

    FD_ZERO(&Writefd);
    FD_SET(m_idwPortfd, &Writefd);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    while (dwMsecTimeout > 1000) {
        dwMsecTimeout -= 1000;
        timeout.tv_sec++;
    }
    timeout.tv_usec = dwMsecTimeout * 1000;

    iResult = select(m_idwPortfd + 1, NULL, &Writefd, NULL, &timeout);

    if (iResult == -1) {
//        debug_serial("select fail");
    } else if (iResult == 0) {
//        debug_serial("timeout");
    } else {
//        debug_serial("readable");
        if (FD_ISSET(m_idwPortfd, &Writefd)) {
            boRetVal = TRUE;
        }
    }

    return boRetVal;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Serial::IsReadable(
    u32_t dwMsecTimeout
) {
    struct timeval timeout;
    bool_t boRetVal = FALSE;
    fd_set readfd;
    int_t iResult;

    FD_ZERO(&readfd);
    FD_SET(m_idwPortfd, &readfd);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    while (dwMsecTimeout > 1000) {
        dwMsecTimeout -= 1000;
        timeout.tv_sec++;
    }
    timeout.tv_usec = dwMsecTimeout * 1000;
    iResult = select(m_idwPortfd + 1, &readfd, NULL, NULL, &timeout);

    if (iResult == -1) {
//        debug_serial("select fail");
    } else if (iResult == 0) {
//        debug_serial("timeout");
    } else {
//        debug_serial("readable");
        if (FD_ISSET(m_idwPortfd, &readfd)) {
            boRetVal = TRUE;
        }
    }

    return boRetVal;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Serial::Start() {
    bool_t boRetVal = FALSE;

    if (!m_boIsStarted && (m_pSerialThread->Start())) {
        m_boIsStarted = TRUE;
        boRetVal = TRUE;
    }

    return boRetVal;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Serial::PushData(
    u8_t byData
) {
    Packet_p pPacket = new Packet(1);
    pPacket->Push(byData);
    m_queSerialPacket.push(pPacket);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Serial::PushBuffer(
    u8_p pByBuffer,
    u32_t dwLength
) {
    m_pSerialLocker->Lock();
    Packet_p pPacket = new Packet(dwLength);
    pPacket->Push(pByBuffer, dwLength);
    m_queSerialPacket.push(pPacket);
    m_pSerialLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Serial::PushPacket(
    Packet_p pPacket
) {
    m_pSerialLocker->Lock();
    m_queSerialPacket.push(pPacket);
    m_pSerialLocker->UnLock();
}
