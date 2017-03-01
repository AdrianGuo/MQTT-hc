/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Database.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 26 Dec 2016 22:00:35
 *
 ******************************************************************************/

#ifndef DATABASE_HPP_
#define DATABASE_HPP_

#include "Libraries/typedefs.h"
#include "Libraries/LibDb/sqlite3.h"

struct sqlite3;

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class Database {
private:
    sqlite3*    m_pDtbase;
    String      m_strPath;
public:
    Database(const String& strConnectionString = "");
    virtual ~Database();

    sqlite3* Connection() const;
    String ConnectionString() const;

    Database& operator= (const Database& other);
};

typedef Database  Database_t;
typedef Database* Database_p;

#endif /* !DATABASE_HPP_ */
