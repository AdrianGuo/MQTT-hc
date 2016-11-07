#include "debug.hpp"
#include "Database.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Database::Database(
    const String& strConnectionString
) : m_strPath (strConnectionString) {
    int_t rc = sqlite3_open_v2(m_strPath.c_str(), &m_pDatabase,
                               SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                               nullptr);

    if (rc != SQLITE_OK) { DEBUG1("CAN'T CONNECT DATABASE"); }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Database::~Database() {
    m_strPath.clear();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
sqlite3* 
Database::Connection() const {
    return m_pDatabase;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
Database::ConnectionString() const {
    return m_strPath;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Database&
Database::operator= (
    const Database& other
) {
    m_pDatabase = other.Connection();
    m_strPath = other.ConnectionString();
    return *this;
}
