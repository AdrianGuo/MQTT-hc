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
#include "Queue.hpp"

#define IsRequested(x)                      Action[x].DP_IsRequested
#define IsResponsed(x)                      Action[x].DP_IsResponsed
#define PendingReqs(x)                      Action[x].DP_PendingReqs
#define ClusterID(x)                        Action[x].DP_ClusterID
#define AttributeID(x)                      Action[x].DP_AttributeID
#define AttributeDataType(x)                Action[x].AttributeDataType
#define AttributeDataSize(x)                Action[x].AttributeDataSize
#define AttributeData(x)                    Action[x].DP_AttributeData
#define ReserveData(x)                      Action[x].DP_ReserveData
#define PreValue(x)                         Action[x].DP_PreValue
#define DIName(x)                           Action[x].DP_DIName
#define DIStringName(x)                     Action[x].DP_DIStringName

typedef Vector<DeviceProperty>              DeviceProperties;

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
    DbPtr<ZbDeviceDb> ParentDevice;
    Collection<DbPtr<ZbDeviceDb>> IrCmd;

    int_t           RealType;
    String          Name;
    Action_t        Action;
    int_t&          State;
    Queue<u32_t>    OwnersReq;
    bool_t          IsAlive;
    bool_t          PreAlive;
    int_t           idwNumTimesNotReply;

    void_t ReceiveInforFromDevice(DeviceProperties, Vector<u8_p>);
    void_t GenerateDeviceInfo();
    void_t EnvAttached();

    bool_t OtherBrandsDevice();
    static u8_t GetAttributeDataSize(u8_t, u8_p*);

    bool_t IsInterested();
    static bool_t IsInterested(int_t);

    static String GetTableName();
    template<class A> void_t Table(A& action);

private:
    Locker_p m_pLocker;

    bool_t SyncDeviceAction(DeviceInfo, u16_t, u16_t);
    void_t GenerateAttributeDataSize(DeviceInfo);
    void_t GenerateDeviceInfo(String);
};

template<class A>
inline void_t
ZbDeviceDb::Table(
    A& action
) {
    Column(action, DeviceID);
    Column(action, Network);
    Column(action, MAC);
    Column(action, Model);
    Column(action, Manufacturer);
    Column(action, Endpoint);
    Column(action, Type);
    Belong(action, ParentDevice, ParentID);
    Belong(action, Controller, ControllerID);
    HasMany(action, IrCmd);
}

typedef ZbDeviceDb  ZbDeviceDb_t;
typedef ZbDeviceDb* ZbDeviceDb_p;

#endif /* !ZBDEVICEDB_HPP_ */
