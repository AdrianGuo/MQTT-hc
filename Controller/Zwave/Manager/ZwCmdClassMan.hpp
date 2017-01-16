#ifndef ZWCMDCLASSMAN_HPP_
#define ZWCMDCLASSMAN_HPP_

#include "typedefs.h"
#include "ZwCmdClass.hpp"

typedef ZwCmdClass_p (*pFunctionCreateCmdClass) (u32_t, u8_t);

class ZwCmdClassMan {
    static ZwCmdClassMan* m_pInstance;
    pFunctionCreateCmdClass m_FuncCreateCmdClass[256];

    ZwCmdClassMan();
    void_t Register(const u8_t byCmdClassId,
                    pFunctionCreateCmdClass pFunctionCreator);
public:
    static ZwCmdClassMan* GetInstance();
    virtual ~ZwCmdClassMan();

    void_t RegisterCmdClass();
    ZwCmdClass_p CreateCmdClass(u8_t byCmdClassId, u32_t dwHomeId, u8_t byNodeId);
};

typedef ZwCmdClassMan  ZwCmdClassMan_t;
typedef ZwCmdClassMan* ZwCmdClassMan_p;

#endif /* !ZWCMDCLASSMAN_HPP_ */
