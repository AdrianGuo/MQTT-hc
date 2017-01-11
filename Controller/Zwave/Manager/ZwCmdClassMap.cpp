/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwCmdClassMap.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 10 Nov 2016 18:04:10
 *
 ******************************************************************************/
#include "AlarmCmdClass.hpp"
#include "AssociationCmdClass.hpp"
#include "BasicCmdClass.hpp"
#include "BinarySwitchCmdClass.hpp"
#include "Crc16EncapCmdClass.hpp"
#include "DeviceResetLocallyCmdClass.hpp"
#include "FirmwareUpdateMDCmdClass.hpp"
#include "ManufacturerSpecificCmdClass.hpp"
#include "MultiChannelCmdClass.hpp"
#include "MultilevelSwitchCmdClass.hpp"
#include "NotificationCmdClass.hpp"
#include "PowerLevelCmdClass.hpp"
#include "SensorBinaryCmdClass.hpp"
#include "SensorMultilevelCmdClass.hpp"
#include "UserCodeCmdClass.hpp"
#include "VersionCmdClass.hpp"
#include "WakeupCmdClass.hpp"
#include "ZwavePlusInfoCmdClass.hpp"

#include "ZwCmdClassMap.hpp"

ZwCmdClassMap_p ZwCmdClassMap::m_pInstance = NULL;

/**
 * @func   ZwCmdClassMap
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClassMap::ZwCmdClassMap() {
    m_mapCmdClass[AlarmCmdClass::GetZwCmdClassId()] =
    AlarmCmdClass::GetZwCmdClassName();
    m_mapCmdClass[AssociationCmdClass::GetZwCmdClassId()] =
    AssociationCmdClass::GetZwCmdClassName();
    m_mapCmdClass[BasicCmdClass::GetZwCmdClassId()] =
    BasicCmdClass::GetZwCmdClassName();
    m_mapCmdClass[BinarySwitchCmdClass::GetZwCmdClassId()] =
    BinarySwitchCmdClass::GetZwCmdClassName();

    m_mapCmdClass[Crc16EncapCmdClass::GetZwCmdClassId()] =
    Crc16EncapCmdClass::GetZwCmdClassName();
    m_mapCmdClass[DeviceResetLocallyCmdClass::GetZwCmdClassId()] =
    DeviceResetLocallyCmdClass::GetZwCmdClassName();
    m_mapCmdClass[FirmwareUpdateMDCmdClass::GetZwCmdClassId()] =
    FirmwareUpdateMDCmdClass::GetZwCmdClassName();
    m_mapCmdClass[ManufacturerSpecificCmdClass::GetZwCmdClassId()] =
    ManufacturerSpecificCmdClass::GetZwCmdClassName();

    m_mapCmdClass[MultiChannelCmdClass::GetZwCmdClassId()] =
    MultiChannelCmdClass::GetZwCmdClassName();
    m_mapCmdClass[MultilevelSwitchCmdClass::GetZwCmdClassId()] =
    MultilevelSwitchCmdClass::GetZwCmdClassName();
    m_mapCmdClass[NotificationCmdClass::GetZwCmdClassId()] =
    NotificationCmdClass::GetZwCmdClassName();
    m_mapCmdClass[PowerLevelCmdClass::GetZwCmdClassId()] =
    PowerLevelCmdClass::GetZwCmdClassName();

    m_mapCmdClass[SensorBinaryCmdClass::GetZwCmdClassId()] =
    SensorBinaryCmdClass::GetZwCmdClassName();
    m_mapCmdClass[SensorBinaryCmdClass::GetZwCmdClassId()] =
    SensorBinaryCmdClass::GetZwCmdClassName();
    m_mapCmdClass[SensorMultilevelCmdClass::GetZwCmdClassId()] =
    SensorMultilevelCmdClass::GetZwCmdClassName();
    m_mapCmdClass[UserCodeCmdClass::GetZwCmdClassId()] =
    UserCodeCmdClass::GetZwCmdClassName();

    m_mapCmdClass[VersionCmdClass::GetZwCmdClassId()] =
    VersionCmdClass::GetZwCmdClassName();
    m_mapCmdClass[WakeupCmdClass::GetZwCmdClassId()] =
    WakeupCmdClass::GetZwCmdClassName();
    m_mapCmdClass[ZwavePlusInfoCmdClass::GetZwCmdClassId()] =
    ZwavePlusInfoCmdClass::GetZwCmdClassName();
}

/**
 * @func   ZwCmdClassMap
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClassMap::ZwCmdClassMap(
    const ZwCmdClassMap& rhs
) : m_mapCmdClass (rhs.m_mapCmdClass) {
}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClassMap&
ZwCmdClassMap::operator= (
    const ZwCmdClassMap& rhs
) {
    m_mapCmdClass = rhs.m_mapCmdClass;
    return *this;
}

/**
 * @func   GetInstance
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClassMap&
ZwCmdClassMap::GetInstance() {
    if (m_pInstance == NULL) {
        m_pInstance = new ZwCmdClassMap();
    }
    return *m_pInstance;
}

/**
 * @func   ~ZwCmdClassMap
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClassMap::~ZwCmdClassMap() {
    m_mapCmdClass.clear();
    delete m_pInstance;
    m_pInstance = NULL;
}

/**
 * @func   operator[]
 * @brief  None
 * @param  None
 * @retval None
 */
String
ZwCmdClassMap::operator[] (
    u8_t bCmdClass
) {
    Map<u8_t, String>::const_iterator it =
            m_mapCmdClass.find(bCmdClass);
    if (it != m_mapCmdClass.end()) {
        return m_mapCmdClass[bCmdClass];
    }
    return String();
}
