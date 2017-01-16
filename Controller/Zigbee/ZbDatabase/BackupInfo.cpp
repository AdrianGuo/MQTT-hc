/*
 * BackupInfo.cpp
 *
 *  Created on: Dec 27, 2016
 *      Author: taho
 */

#include <BackupInfo.hpp>

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
BackupInfoDb::BackupInfoDb() :
    MAC ("", "MAC"),
    Model ("", "Model"),
    Manufacturer ("", "Manufacturer"),
    EndpointNo (0, "EndpointNo"),
    No1 (0, "No1"),
    No2 (0, "No2"),
    No3 (0, "No3"),
    No4 (0, "No4"),
    No5 (0, "No5"),
    No6 (0, "No6"),
    No7 (0, "No7"),
    No8 (0, "No8"),
    No9 (0, "No9"),
    No10 (0, "No10"),
    No11 (0, "No11"),
    No12 (0, "No12"),
    No13 (0, "No13"),
    No14 (0, "No14"),
    No15 (0, "No15"),
    No16 (0, "No16"),
    No17 (0, "No17"),
    No18 (0, "No18"),
    No19 (0, "No19"),
    No20 (0, "No20") {
    EpOrd[1] = &No1;
    EpOrd[2] = &No2;
    EpOrd[3] = &No3;
    EpOrd[4] = &No4;
    EpOrd[5] = &No5;
    EpOrd[6] = &No6;
    EpOrd[7] = &No7;
    EpOrd[8] = &No8;
    EpOrd[9] = &No9;
    EpOrd[10] = &No10;
    EpOrd[11] = &No11;
    EpOrd[12] = &No12;
    EpOrd[13] = &No13;
    EpOrd[14] = &No14;
    EpOrd[15] = &No15;
    EpOrd[16] = &No16;
    EpOrd[17] = &No17;
    EpOrd[18] = &No18;
    EpOrd[19] = &No19;
    EpOrd[20] = &No20;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
BackupInfoDb::~BackupInfoDb() { }

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
BackupInfoDb::GetTableName() {
    return "ZBBACKUP";
}
