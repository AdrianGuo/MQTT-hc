/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: HexFileHelper.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 09:44:17
 *
 ******************************************************************************/
#ifndef HEXFILE_HELPER_HPP_
#define HEXFILE_HELPER_HPP_

#include "Typedefs.h"

class HexFileHelper {
public:
    typedef enum {
        Data = 0x00,
        Eof  = 0x01,
        Extended_Segment_Addr = 0x02,
        Start_Segment_Addr    = 0x03,
        Extended_Linear_Addr  = 0x04,
        Start_Linear_Addr     = 0x05
    } Record;
    static void_t GetBytes();
};

typedef HexFileHelper  HexFileHelper_t;
typedef HexFileHelper* HexFileHelper_p;


#endif /* !HEXFILE_HELPER_HPP_ */
