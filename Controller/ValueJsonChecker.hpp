/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name:
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-30 22:30:00 (Sun, 30 Oct 2016)
 *
 ******************************************************************************/

#ifndef VALUE_JSON_CHECKER_HPP_
#define VALUE_JSON_CHECKER_HPP_

#include "typedefs.h"
#include "Map.hpp"
#include "JsonCommand.hpp"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class ValueJsonChecker {
private:
    typedef Map<String, u32_t> Checker_t;
    Checker_t m_mapCheker;
public:
    ValueJsonChecker() {}
    virtual ~ValueJsonChecker() {}
    void_t Register(const String strJsonMessageName,
                    u32_t dwDestinationFlag = JsonCommand::DesDefault);
    bool_t HasCommand(JsonCommand& jsonCommand) const;
};

typedef ValueJsonChecker  ValueJsonChecker_t;
typedef ValueJsonChecker* ValueJsonChecker_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline void_t
ValueJsonChecker::Register(
    const String strJsonMessageName,
    u32_t dwDestinationFlag
) {
    m_mapCheker[strJsonMessageName] = dwDestinationFlag;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValueJsonChecker::HasCommand (
    JsonCommand& jsonCommand
) const {
    Checker_t::const_iterator_t it = m_mapCheker.find(jsonCommand.GetFullCommand());
    if ((it != m_mapCheker.end()) && (it->second == jsonCommand.GetDesFlag())) {
        return TRUE;
    }
    return FALSE;
}

#endif /* !VALUE_JSON_CHECKER_HPP_ */
