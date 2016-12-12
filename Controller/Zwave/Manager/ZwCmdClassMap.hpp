/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwCmdClassMap.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 10 Nov 2016 18:03:46
 *
 ******************************************************************************/
#ifndef ZW_CMDCLASS_MAP_HPP_
#define ZW_CMDCLASS_MAP_HPP_

#include "typedefs.h"
#include "Map.hpp"
#include "String.hpp"

class ZwCmdClassMap {
private:
    static ZwCmdClassMap* m_pInstance;
    Map<u8_t, String> m_mapCmdClass;
    ZwCmdClassMap();
public:
    static ZwCmdClassMap* GetInstance();
    virtual ~ZwCmdClassMap();
    String operator[] (u8_t);
};

typedef ZwCmdClassMap  ZwCmdClassMap_t;
typedef ZwCmdClassMap* ZwCmdClassMap_p;

#endif /* !ZW_CMDCLASS_MAP_HPP_ */