#ifndef DBCONFIG_MODEL_HPP_
#define DBCONFIG_MODEL_HPP_

#include "ConfigImpl.hpp"

class DbFloorItem;
class DbRoomItem;

template<>
struct ConfigTable<DbFloorItem> : public ConfigDefault {
    typedef ValueIntDb IdType;
	static String InsteadIdField() { return String(); }
};

template<>
struct ConfigTable<DbRoomItem> : public ConfigDefault {
    typedef ValueIntDb IdType;
    static String InsteadIdField() { return String(); }
};

#endif /* !DBCONFIG_MODEL_HPP_ */
