#ifndef ZBDEVICEDB_HPP_
#define ZBDEVICEDB_HPP_

#include "ZbControllerDb.hpp"
#include "DeviceInfo.hpp"
#include "ConfigModel.hpp"
#include "Typedefs.h"
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

    template<ValueDb::Compare C = ValueDb::Compare::Equal>
    bool_t Check(String& value, u32_t dwIndex);

    template<ValueDb::Compare C = ValueDb::Compare::Equal>
    bool_t Check(int_t& value, u32_t dwIndex);

    template<ValueDb::Compare C = ValueDb::Compare::Equal>
    bool_t Check(u8_t& value, u32_t dwIndex);

    template<ValueDb::Compare C = ValueDb::Compare::Equal>
    bool_t Check(u16_t& value, u32_t dwIndex);

private:
    Locker_p m_pLocker;

    bool_t SyncDeviceAction(DeviceInfo, u16_t, u16_t);
    void_t GenerateAttributeDataSize(DeviceInfo);
    void_t GenerateDeviceInfo(String);
};

// V: Value
// C: Compare
template<ValueDb::Compare C>
inline bool_t
ZbDeviceDb::Check(
    int_t&  value,
    u32_t dwIndex
) {
    bool_t boRet = FALSE;
    switch (dwIndex) {
    case 0:
        boRet = DeviceID.Compare(value, C);
        break;
    case 1:
        boRet = Network.Compare(value, C);
        break;
    case 5:
        boRet = Endpoint.Compare(value, C);
        break;
    case 6:
        boRet = Type.Compare(value, C);
        break;
    case 7:
        boRet = ParentID.Compare(value, C);
        break;
    case 8:
        boRet = ControllerID.Compare(value, C);
        break;
    }
    return boRet;
}

// V: Value
// C: Compare
template<ValueDb::Compare C>
inline bool_t
ZbDeviceDb::Check(
    u8_t&  value,
    u32_t dwIndex
) {
    bool_t boRet = FALSE;
    switch (dwIndex) {
    case 0:
        boRet = DeviceID.Compare(value, C);
        break;
    case 1:
        boRet = Network.Compare(value, C);
        break;
    case 5:
        boRet = Endpoint.Compare(value, C);
        break;
    case 6:
        boRet = Type.Compare(value, C);
        break;
    case 7:
        boRet = ParentID.Compare(value, C);
        break;
    case 8:
        boRet = ControllerID.Compare(value, C);
        break;
    }
    return boRet;
}

// V: Value
// C: Compare
template<ValueDb::Compare C>
inline bool_t
ZbDeviceDb::Check(
    u16_t&  value,
    u32_t dwIndex
) {
    bool_t boRet = FALSE;
    switch (dwIndex) {
    case 0:
        boRet = DeviceID.Compare(value, C);
        break;
    case 1:
        boRet = Network.Compare(value, C);
        break;
    case 5:
        boRet = Endpoint.Compare(value, C);
        break;
    case 6:
        boRet = Type.Compare(value, C);
        break;
    case 7:
        boRet = ParentID.Compare(value, C);
        break;
    case 8:
        boRet = ControllerID.Compare(value, C);
        break;
    }
    return boRet;
}

// V: Value
// C: Compare
template<ValueDb::Compare C>
inline bool_t
ZbDeviceDb::Check(
    String&     value,
    u32_t dwIndex
) {
    bool_t boRet = FALSE;
    switch (dwIndex) {
    case 2:
        boRet = MAC.Compare(value, C);
        break;
    case 3:
        boRet = Model.Compare(value, C);
        break;
    case 4:
        boRet = Manufacturer.Compare(value, C);
        break;
    }
    return boRet;
}

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
