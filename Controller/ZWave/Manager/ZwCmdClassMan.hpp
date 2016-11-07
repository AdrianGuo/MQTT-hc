#ifndef ZWCMDCLASSMAN_HPP_
#define ZWCMDCLASSMAN_HPP_

#include "typedefs.h"
#include "ZwCmdClass.hpp"

typedef ZwCmdClass_p (*pFunctionCreateCmdClass) (u32_t, u8_t);

class ZwCmdClassMan {
    static ZwCmdClassMan* m_pInstance;
    pFunctionCreateCmdClass m_FuncCreateCmdClass[256];

    ZwCmdClassMan();
public:
    static ZwCmdClassMan* GetInstance();
    virtual ~ZwCmdClassMan();

    void_t RegisterCmdClass();
    void_t Register(const u8_t byCmdClassID, pFunctionCreateCmdClass pFunctionCreator);
    ZwCmdClass_p CreateCmdClass(u8_t byCmdClassID, u32_t dwHomeID, u8_t byNodeID);
};

typedef ZwCmdClassMan  ZwCmdClassMan_t;
typedef ZwCmdClassMan* ZwCmdClassMan_p;

#endif /* !ZWCMDCLASSMAN_HPP_ */
