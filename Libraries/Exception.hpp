/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Exception.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 6 Jan 2017 20:48:25
 *
 ******************************************************************************/
#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include "Libraries/typedefs.h"

/******************************************************************************/
/*                           CLASS : EXCEPTION                                */
/******************************************************************************/
class Exception : public std::exception {
public:
    Exception() noexcept;
    Exception(const Exception& copied) noexcept;
    Exception& operator= (const Exception& copied) noexcept;
    Exception(const_char_p message, ...) noexcept;

    virtual ~Exception() noexcept;

protected:
    String m_strWhat;
    void_t SetParams(const_char_p message, va_list args);

public:
    virtual const_char_p what() const noexcept;
};

typedef Exception  Exception_t;
typedef Exception* Exception_p;

/******************************************************************************/
/*                     CLASS : EXCEPTION JSON COMMAND                         */
/******************************************************************************/

class ExceptionInvalidCmd : public Exception {
public:
    ExceptionInvalidCmd() noexcept;
    ExceptionInvalidCmd(const ExceptionInvalidCmd& copied) noexcept;
    ExceptionInvalidCmd& operator= (const ExceptionInvalidCmd& copied) noexcept;
    ExceptionInvalidCmd(const_char_p message, ...) noexcept;

    virtual ~ExceptionInvalidCmd() noexcept;

public:
    virtual const_char_p what() const noexcept;
};

typedef ExceptionInvalidCmd  ExceptionInvalidCmd_t;
typedef ExceptionInvalidCmd* ExceptionInvalidCmd_p;

/******************************************************************************/
/*                        CLASS : EXCEPTION NULLPTR                           */
/******************************************************************************/

class ExceptionNullPtr : public Exception {
public:
    ExceptionNullPtr() noexcept;
    ExceptionNullPtr(const ExceptionNullPtr& copied) noexcept;
    ExceptionNullPtr& operator= (const ExceptionNullPtr& copied) noexcept;
    ExceptionNullPtr(const_char_p message, ...) noexcept;

    virtual ~ExceptionNullPtr() noexcept;

public:
    virtual const_char_p what() const noexcept;
};

typedef ExceptionNullPtr  ExceptionNullPtr_t;
typedef ExceptionNullPtr* ExceptionNullPtr_p;

/******************************************************************************/
/*                        CLASS : EXCEPTION DATABASE                          */
/******************************************************************************/

class ExceptionErrDatabase : public Exception {
public:
    ExceptionErrDatabase() noexcept;
    ExceptionErrDatabase(const ExceptionErrDatabase& copied) noexcept;
    ExceptionErrDatabase& operator= (const ExceptionErrDatabase& copied) noexcept;
    ExceptionErrDatabase(const_char_p message, ...) noexcept;

    virtual ~ExceptionErrDatabase() noexcept;

public:
    virtual const_char_p what() const noexcept;
};

/******************************************************************************/
/*                           CLASS : WRONG PARAM                              */
/******************************************************************************/

class ExceptionInvalidArg : public Exception {
public:
    ExceptionInvalidArg() noexcept;
    ExceptionInvalidArg(const ExceptionInvalidArg& copied) noexcept;
    ExceptionInvalidArg& operator= (const ExceptionInvalidArg& copied) noexcept;
    ExceptionInvalidArg(const_char_p message, ...) noexcept;

    virtual ~ExceptionInvalidArg() noexcept;

public:
    virtual const_char_p what() const noexcept;
};

typedef ExceptionInvalidArg  ExceptionInvalidArg_t;
typedef ExceptionInvalidArg* ExceptionInvalidArg_p;

#endif /* !EXCEPTION_HPP_ */
