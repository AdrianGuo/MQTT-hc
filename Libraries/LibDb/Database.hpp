#ifndef DATABASE_HPP_
#define DATABASE_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "sqlite3.h"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class Database {
private:
    sqlite3*    m_pDatabase;
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
