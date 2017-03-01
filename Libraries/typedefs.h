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
 * Author:           TrungTQ
 *
 * Last Changed By:  Author: TrungTQ (trungkstn@gmail.com)
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
#include <stddef.h>
#include <stdarg.h>
#include <utility>
#include <sys/time.h>
#include <iostream>
#include <exception>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <string.h>


#ifdef MT7688
#include <string>
#include <sstream>
#include <cstdlib>
#endif /* MT7688 */
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

#ifndef __FILENAME__
#define __FILENAME__  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif /* __FILENAME__ */


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
typedef unsigned long long       u64_t;
#endif

typedef signed char             i8_t;       // 1 byte
typedef signed short            i16_t;      // 2 byte

#if __WORDSIZE == 64
typedef signed int              i32_t;      // 4 byte
typedef signed long int         i64_t;
#elif __WORDSIZE == 32
typedef signed int              i32_t;      // 4 byte
typedef signed long long        i64_t;
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
typedef unsigned long long*     u64_p;
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

#ifndef UNUSED
#define UNUSED(x)               (x) = (x)
#endif /* UNUSED */

#ifndef INVALID_SOCKET
#define INVALID_SOCKET          (-1)
#endif /* INVALID_SOCKET */

#ifndef RTIMER_ERROR
#define RTIMER_ERROR            (-1)
#endif /* RTIMER_ERROR */

#ifndef SERIAL_ERROR
#define SERIAL_ERROR            (-1)
#endif /* SERIAL_ERROR */

#ifndef BAUD96
#define BAUD96                  (96)
#endif /* BAUD96 */

#ifndef BAUD192
#define BAUD192                 (192)
#endif /* BAUD192 */

#ifndef BAUD384
#define BAUD384                 (384)
#endif /* BAUD384 */

#ifndef BAUD576
#define BAUD576                 (576)
#endif /* BAUD576 */

#ifndef BAUD1152
#define BAUD1152                (1152)
#endif /* BAUD1152 */

#ifndef WAIT_INFINITE
#define WAIT_INFINITE           (-1)
#endif /* WAIT_INFINITE */

#ifndef WAIT_IMMEDIATE
#define WAIT_IMMEDIATE          (0)
#endif /* WAIT_IMMEDIATE */

#ifndef WAIT_SUCCESS
#define WAIT_SUCCESS            (0)
#endif /* EVENT_SUCCESS */

template<typename T> using Vector = std::vector<T>;
template<typename T> using Queue = std::queue<T>;
template<typename T> using List = std::list<T>;
template<typename T> using Set = std::set<T>;
template<typename K, typename T> using Map = std::map<K, T>;
using String = std::string;

static std::exception_ptr eptr = NULL;

inline void_t
Handle_Eptr(std::exception_ptr eptr) {
    if (eptr != NULL) {
        try {
            std::rethrow_exception(eptr);
        } catch (std::exception &ex) {
            std::cout << "caught exception " << ex.what() << std::endl;
        }
    }
}

#define MACRO_DBUG(x, args ...) \
    ST (\
        struct timeval tv; \
        gettimeofday(&tv, NULL); \
        struct tm* now; \
        now = localtime(&tv.tv_sec); \
        printf("\x1b[36m[DBUG][%04d-%02d-%02d %02d:%02d:%02d.%03d][%010u][%18s:%4d] " x "\n\x1b[39m", \
                now->tm_year + 1990, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, \
                (int_t) tv.tv_usec / 1000, (u32_t) pthread_self(), __FILENAME__, __LINE__, ##args); \
    )

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
