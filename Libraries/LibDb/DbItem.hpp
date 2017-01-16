/*
 * ValueDb.hpp
 *
 *  Created on: 22 Sep 2016
 *      Author: TrungTQ
 */

#ifndef DB_ITEM_HPP_
#define DB_ITEM_HPP_

#include "Value.hpp"
#include "Map.hpp"
#include "Vector.hpp"
#include "String.hpp"

class DbItem {
private:
    String m_strName;
public:
    DbItem(const String strName = "") { m_strName = strName; }
    DbItem(const_char_p cName) { m_strName = cName; }
    virtual ~DbItem() {}

    void_t SetName(String strName) { m_strName = strName; }
    String GetName() const { return m_strName; }
};

typedef DbItem  DbItem_t;
typedef DbItem* DbItem_p;

#endif /* !DB_ITEM_HPP_ */
