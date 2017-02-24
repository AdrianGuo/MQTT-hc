/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name:        typedefs.h
 *
 * Description:      Include file for application
 *
 * Author:           TrungTQb
 *
 * Last Changed By:  Author: TrungTQ
 * Revision:         Revision: 1.1
 * Last Changed:     Date: 2016-05-16 11:45:00 (Tue, 16 May 2016)
 *
 ******************************************************************************/
#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <bits/wordsize.h>
#ifdef MT7688
#include <string>
#include <sstream>
#include <cstdlib>
#endif /* MT7688 */
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef unsigned char           byte_t;     // 1 byte
typedef unsigned short          word_t;     // 2 byte
typedef unsigned long           dword_t;    // 8 byte
typedef unsigned int            uint_t;     // 4 byte
typedef unsigned char           bool_t;
typedef char                    char_t;     // 1 byte
typedef void                    void_t;

typedef byte_t*                 byte_p;
typedef word_t*                 word_p;
typedef dword_t*                dword_p;
typedef uint_t*                 uint_p;
typedef char_t*                 char_p;
typedef void_t*                 void_p;

typedef unsigned char           u8_t;       // 1 byte
typedef unsigned short          u16_t;      // 2 byte

#if __WORDSIZE == 64
typedef unsigned int            u32_t;      // 4 byte
typedef unsigned long int       u64_t;
#elif __WORDSIZE == 32
typedef unsigned int            u32_t;      // 4 byte
typedef unsigned long long      u64_t;
#endif

typedef signed char             i8_t;       // 1 byte
typedef signed short            i16_t;      // 2 byte

#if __WORDSIZE == 64
typedef signed int              i32_t;      // 4 byte
typedef signed long int         i64_t;
#elif __WORDSIZE == 32
typedef signed int              i32_t;      // 4 byte
typedef signed long long         i64_t;
#endif

typedef signed int              int_t;
typedef signed int*             int_p;

typedef unsigned char*          u8_p;       // 1 byte
typedef unsigned char**         u8_pp;
typedef unsigned short*         u16_p;      // 2 byte

#if __WORDSIZE == 64
typedef unsigned int*           u32_p;      // 4 byte
typedef unsigned long int*      u64_p;
#elif __WORDSIZE == 32
typedef unsigned int*           u32_p;      // 4 byte
typedef unsigned long long*      u64_p;
#else
typedef unsigned int*           u32_p;      // 4 byte
typedef unsigned long int*      u64_p;
#endif

typedef signed char*            i8_p;       // 1 byte
typedef signed short*           i16_p;      // 2 byte

#if __WORDSIZE == 64
typedef signed int*             i32_p;      // 4 byte
typedef signed long int*        i64_p;
#elif __WORDSIZE == 32
typedef signed int*             i32_p;      // 4 byte
typedef signed long long*       i64_p;
#else
typedef signed int*             i32_p;      // 4 byte
typedef signed long long*       i64_p;
#endif

typedef float                   flo_t;
typedef float*                  flo_p;

typedef double                  dob_t;
typedef double*                 dob_p;

typedef const char              const_char_t;
typedef const char*             const_char_p;

typedef const void              const_void_t;
typedef const void*             const_void_p;

typedef void const              void_const_t;
typedef void const*             void_const_p;

#ifndef TRUE
#define TRUE                    (1)
#endif // TRUE

#ifndef FALSE
#define FALSE                   (0)
#endif // FALSE

#ifndef NULL
#define NULL                    (0)
#endif // NULL

#ifndef BV
#define BV(n)                   (1 << (n))
#endif /* !BV */

#ifndef ST
#define ST(x)                   do { x } while (__LINE__ == -1)
#endif /* !ST */

#ifndef HI_UINT16
#define HI_UINT16(a)            (((a) >> 8) & 0xFF)
#endif /* !HI_UINT16 */

#ifndef LO_UINT16
#define LO_UINT16(a)            ((a) & 0xFF)
#endif /* !LO_UINT16 */

#ifndef MERGE
#define MERGE(h,l)              (((h) << 8) | (l))
#endif /* !MERGE */

#ifdef MT7688
namespace std {
    inline std::string to_string(int i) {
        std::stringstream ss;
        ss << i;
        return ss.str();
    }

    inline int stoi(std::string string) {
        return atoi(string.c_str());
    }
}
#endif /* MT7688 */

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

#endif /* _TYPEDEFS_H_ */
