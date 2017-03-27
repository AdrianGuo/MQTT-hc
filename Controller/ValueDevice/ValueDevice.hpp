/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueDevice.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 10 Nov 2016 00:47:10
 *
 ******************************************************************************/
#ifndef VALUE_DEVICE_HPP_
#define VALUE_DEVICE_HPP_

#include "Typedefs.h"
#include "Value.hpp"
#include "json.h"

class ValueDevice : public Value {
private:
public:
    ValueDevice(Value::Type_t typedevice) :
        Value (typedevice) {}
    virtual ~ValueDevice() {}

    virtual bool_t CheckParam() const = 0;
    virtual bool_t CheckJsVal(Json::Value jValue) const = 0;
    virtual bool_t ParseValue(Json::Value jValue) = 0;
    virtual Json::Value CreateJson() = 0;
};

typedef ValueDevice  ValueDevice_t;
typedef ValueDevice* ValueDevice_p;


#endif /* !VALUE_DEVICE_HPP_ */
