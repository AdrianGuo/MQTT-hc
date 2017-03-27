/*
 * BackupInfo.hpp
 *
 *  Created on: Dec 27, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_ZIGBEE_ZBDATABASE_BACKUPINFO_HPP_
#define CONTROLLER_ZIGBEE_ZBDATABASE_BACKUPINFO_HPP_

#include "Typedefs.h"
#include "Map.hpp"
#include "ValueIntDb.hpp"
#include "ValueStrDb.hpp"
#include "ConfigModel.hpp"
#include "Collection.hpp"
#include "ValueStrDb.hpp"

class BackupInfoDb {
public:
    BackupInfoDb();
    virtual ~BackupInfoDb();

    ValueStrDb_t MAC;
    ValueStrDb_t Model;
    ValueStrDb_t Manufacturer;
    ValueIntDb_t EndpointNo;

    ValueIntDb_t No1;
    ValueIntDb_t No2;
    ValueIntDb_t No3;
    ValueIntDb_t No4;
    ValueIntDb_t No5;
    ValueIntDb_t No6;
    ValueIntDb_t No7;
    ValueIntDb_t No8;
    ValueIntDb_t No9;
    ValueIntDb_t No10;
    ValueIntDb_t No11;
    ValueIntDb_t No12;
    ValueIntDb_t No13;
    ValueIntDb_t No14;
    ValueIntDb_t No15;
    ValueIntDb_t No16;
    ValueIntDb_t No17;
    ValueIntDb_t No18;
    ValueIntDb_t No19;
    ValueIntDb_t No20;

    Map<u8_t,ValueIntDb_p> EpOrd;

    static String GetTableName();
    template<class A> void_t Table(A& action);
};

template<class A>
inline void_t
BackupInfoDb::Table(
    A& action
) {
    Column(action, MAC);
    Column(action, Model);
    Column(action, Manufacturer);
    Column(action, EndpointNo);

    Column(action, No1);
    Column(action, No2);
    Column(action, No3);
    Column(action, No4);
    Column(action, No5);
    Column(action, No6);
    Column(action, No7);
    Column(action, No8);
    Column(action, No9);
    Column(action, No10);
    Column(action, No11);
    Column(action, No12);
    Column(action, No13);
    Column(action, No14);
    Column(action, No15);
    Column(action, No16);
    Column(action, No17);
    Column(action, No18);
    Column(action, No19);
    Column(action, No20);
}

typedef BackupInfoDb  BackupInfoDb_t;
typedef BackupInfoDb* BackupInfoDb_p;

#endif /* CONTROLLER_ZIGBEE_ZBDATABASE_BACKUPINFO_HPP_ */
