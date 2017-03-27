/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: CrC16Encap.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 00:12:46
 *
 ******************************************************************************/
#ifndef CRC16ENCAP_CMDCLASS_HPP_
#define CRC16ENCAP_CMDCLASS_HPP_

#include "Typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class Crc16EncapCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    Crc16EncapCmdClass(u32_t dwHomeId, u8_t byNodeId);

    ValueDevice_p HandleCrc16Encap(u8_p pbCommand, u8_t byLength);
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~Crc16EncapCmdClass();

    virtual u8_t GetMaxVersion() const { return CRC_16_ENCAP_VERSION; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_CRC_16_ENCAP; }
    static const String GetZwCmdClassName() { return "CRC_16_ENCAP"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();
};

typedef Crc16EncapCmdClass  CrC16EncapCmdClass_t;
typedef Crc16EncapCmdClass* CrC16EncapCmdClass_p;

#endif /* !CRC16_ENCAPCMDCLASS_HPP_ */
