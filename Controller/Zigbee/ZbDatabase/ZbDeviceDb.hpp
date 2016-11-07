#ifndef ZBDEVICEDB_HPP_
#define ZBDEVICEDB_HPP_

#include "ZbControllerDb.hpp"
#include "DeviceInfo.hpp"
#include "ConfigModel.hpp"
#include "typedefs.h"
#include "DbItem.hpp"
#include "DbPtr.hpp"
#include "Config.hpp"
#include "ValueIntDb.hpp"
#include "Locker.hpp"
#include "Map.hpp"

class ZbDeviceDb {
public:
    ZbDeviceDb();
    virtual ~ZbDeviceDb();

    ValueIntDb_t DeviceID;
    ValueIntDb_t Network;
    ValueStrDb_t MAC;
    ValueStrDb_t Model;
    ValueStrDb_t Manufacturer;
    ValueIntDb_t Endpoint;
    ValueIntDb_t Type;
    ValueIntDb_t ParentID;
    ValueIntDb_t ControllerID;

    DbPtr<ZbControllerDb> Controller;
    DbPtr<ZbDeviceDb> Device;
    Collection<DbPtr<ZbDeviceDb>> Devices;

    u8_t RealType;
    Action_t Action;
    int_t& State = Action[DeviceInfo::DI_State].DP_AttributeData;

    void_t ReceiveInforFromDevice(u8_t byAttributeDataType, u8_p const pbyValue);
    void_t GenerateDeviceInfo();
    void_t SyncTypeGroup();
    void_t EnvAttached();
    bool_t IsInterested();

    bool_t SyncDeviceAction();
    bool_t OtherBrandsDevices();
    static u8_t GetAttributeDataSize(u8_t, u8_p*);

    static String GetTableName();
    template<class A> void_t Table(A& action);

private:
    Locker_p m_pLocker;

    bool_t SyncDeviceAction(DeviceInfo, u16_t, u16_t);
    void_t GenerateAttributeDataSize(DeviceInfo);
};

template<class A>
inline void_t
ZbDeviceDb::Table(
    A& action
) {
    Id(action, DeviceID);
    Column(action, Network);
    Column(action, MAC);
    Column(action, Model);
    Column(action, Manufacturer);
    Column(action, Endpoint);
    Column(action, Type);
    Belong(action, Device, ParentID);
    Belong(action, Controller, ControllerID);
    HasMany(action, Devices);
}

typedef ZbDeviceDb  ZbDeviceDb_t;
typedef ZbDeviceDb* ZbDeviceDb_p;

#endif /* !ZBDEVICEDB_HPP_ */
