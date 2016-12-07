#include "DbModel.hpp"
#include "DbContextImpl.hpp"

DbModel* DbModel::m_pInstance = NULL;

/**
 * @func   DbModel
 * @brief  None
 * @param  None
 * @retval None
 */
DbModel::DbModel(
    const_char_p cDbName
) : DbContext(cDbName) {
    MapDbTable<DbRoomItem>(DbRoomItem::GetTableName());
    MapDbTable<DbFloorItem>(DbFloorItem::GetTableName());
	MapDbTable<DbDeviceItem>(DbDeviceItem::GetTableName());
	CreateTables();
	Rooms = Find<DbRoomItem>();
	Floors = Find<DbFloorItem>();
	Devices = Find<DbDeviceItem>();
}

/**
 * @func   ~DbModel
 * @brief  None
 * @param  None
 * @retval None
 */
DbModel::~DbModel() {
}

/**
 * @func   CreateModel
 * @brief  None
 * @param  None
 * @retval None
 */
DbModel_p
DbModel::CreateModel(
    const_char_p cDbName
) {
    if (m_pInstance == NULL) {
        m_pInstance = new DbModel(cDbName);
    }
    return m_pInstance;
}
