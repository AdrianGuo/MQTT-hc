/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DbItem.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-11 11:45:00 (Tue, 11 Oct 2016)
 *
 ******************************************************************************/

#ifndef DB_ITEM_HPP_
#define DB_ITEM_HPP_

#include "../Typedefs.h"
#include "../ValueCore.hpp"
#include "../LogPlus.hpp"

class DbItem : public ValueCore {
private:
    String m_strName;
    DbItem(const DbItem& copied) = delete;
    DbItem& operator= (const DbItem& copied) = delete;
public:
    DbItem(const String strName = String()
            ) : ValueCore (__FUNCTION__),
                m_strName (     strName) {
        #ifdef DB_DBITEM
        LOG_DEBUG("object [%03d] created [TB:%15s]", Index(), m_strName.c_str());
        #endif /* DB_DBITEM */
    }

    DbItem(const_char_p chName = ""
            ) : ValueCore (__FUNCTION__ ),
                m_strName (chName       ) {
        #ifdef DB_DBITEM
        LOG_DEBUG("object [%03d] created [TB:%15s]", Index(), m_strName.c_str());
        #endif /* DB_DBITEM */
    }

    virtual ~DbItem() {
        #ifdef DB_DBITEM
        LOG_DEBUG("object [%03d] deleted [TB:%15s]", Index(), m_strName.c_str());
        #endif /* DB_DBITEM */
        m_strName.clear();
    }

    void_t SetName(String strName) { m_strName = strName; }
    String GetName() const { return m_strName; }
};

typedef DbItem  DbItem_t;
typedef DbItem* DbItem_p;

#endif /* !DB_ITEM_HPP_ */
