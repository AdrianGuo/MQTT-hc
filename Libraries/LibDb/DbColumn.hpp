/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DbColum.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 4 Mar 2017 08:22:32
 *
 ******************************************************************************/
#ifndef DB_COLUMN_HPP_
#define DB_COLUMN_HPP_

template<class C>
class DbColum {
private:
    Map<String, ValueDb> ObjVal_t;
    ObjVal_t m_mapObjVal;
public:
    DbColum();
};

#endif /* !DB_COLUMN_HPP_ */
