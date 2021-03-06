#ifndef CMDCLASS_HPP_
#define CMDCLASS_HPP_

#include "Typedefs.h"
#include "Functor.hpp"
#include "ValueDevice.hpp"
#include "ZwMessage.hpp"

class ZwNode;

#define ZWAVE_PLUS_TX_OPTIONS   (TRANSMIT_OPTION_ACK | TRANSMIT_OPTION_AUTO_ROUTE | TRANSMIT_OPTION_EXPLORE)

class ZwCmdClass {
private:
    u32_t   m_dwHomeId;
    u8_t    m_byNodeId;
    u8_t    m_byVersion;
    bool_t  m_boSecured;
    ZwNode* m_pZwNode;
public:
    ZwCmdClass(
        u32_t dwHomeId = 0,
        u8_t  byNodeId = 0,
        ZwNode* pZwNode = NULL
    ) : m_dwHomeId  (dwHomeId),
        m_byNodeId  (byNodeId),
        m_byVersion (0),
        m_boSecured (FALSE),
        m_pZwNode (pZwNode) {}

    virtual ~ZwCmdClass() {}

    virtual u8_t GetMaxVersion() { return 0; }
    virtual ValueDevice_p HandleMessage(u8_p pBuffer, u8_t byLength) { return NULL; }
    virtual ZwMessage_p GetValue() { return NULL; }
    virtual ZwMessage_p GetVersionValue() { return NULL; }
    virtual ZwMessage_p SetValue(ValueDevice_p pValueDevice) { return NULL; }
    virtual void_t SetValue(ValueDevice_p pValueDevice, u8_p& pBuffer, u8_p pLength);

    void_t  SetNode(ZwNode* pZwNode) { m_pZwNode = pZwNode; }
    ZwNode* GetNode() { return m_pZwNode; }

    u8_t   GetVersion() const { return m_byVersion; }
    void_t SetVersion(u8_t byVersion) { m_byVersion = byVersion; }

    u32_t GetHomeId() const { return m_dwHomeId; }
    u8_t  GetNodeId() const { return m_byNodeId; }
};

typedef ZwCmdClass  ZwCmdClass_t;
typedef ZwCmdClass* ZwCmdClass_p;

/**
 * @func   SetValue
 * @brief  None
 * @param  None
 * @retval None
 */
inline void_t
ZwCmdClass::SetValue(
    ValueDevice_p pValueDevice,
    u8_p& pBuffer,
    u8_p pLength
) {
    pBuffer = NULL;
    pLength = NULL;
}

#endif /* !CMDCLASS_HPP_ */
