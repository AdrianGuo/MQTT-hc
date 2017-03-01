/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Exception.cpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 6 Jan 2017 22:13:19
 *
 ******************************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include "Libraries/LogPlus.hpp"
#include "Libraries/Exception.hpp"

/******************************************************************************/
/*                           CLASS : EXCEPTION                                */
/******************************************************************************/

/**
 * @func   ExceptionBase
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Exception::SetParams(
    const_char_p message,
    va_list args
) {
    char linebuffer [1024] = { 0 };
    vsnprintf(linebuffer, sizeof(linebuffer), message, args);
    m_strWhat.append(linebuffer);
}

/**
 * @func   Exception
 * @brief  None
 * @param  None
 * @retval None
 */
Exception::Exception(
) noexcept {
}

/**
 * @func   ~Exception
 * @brief  None
 * @param  None
 * @retval None
 */
Exception::~Exception(
) noexcept {
    m_strWhat.clear();
}

/**
 * @func   ExceptionBase
 * @brief  None
 * @param  None
 * @retval None
 */
Exception::Exception (
    const_char_p message,
    ...
) noexcept {
    m_strWhat.assign("[exception] ");
    va_list args;
    va_start(args, message);
    SetParams(message, args);
    va_end(args);
}

/**
 * @func   ExceptionBase
 * @brief  None
 * @param  None
 * @retval None
 */
Exception::Exception(
    const Exception& copied
) noexcept : m_strWhat (copied.m_strWhat) {

}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
Exception&
Exception::operator= (
    const Exception& copied
) noexcept {
    m_strWhat = copied.m_strWhat;
    return *this;
}

/**
 * @func   what
 * @brief  None
 * @param  None
 * @retval None
 */
const_char_p
Exception::what(
) const noexcept {
    return m_strWhat.c_str();
}

/******************************************************************************/
/*                     CLASS : EXCEPTION JSON COMMAND                         */
/******************************************************************************/

/**
 * @func   ExceptionJsonCmd
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionInvalidCmd::ExceptionInvalidCmd(
) noexcept {
    m_strWhat.assign("[invalid cmd]");
}

/**
 * @func   ~ExceptionJsonCmd
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionInvalidCmd::~ExceptionInvalidCmd(
) noexcept {

}

/**
 * @func   ExceptionJsonCmd
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionInvalidCmd::ExceptionInvalidCmd (
    const_char_p message,
    ...
) noexcept {
    m_strWhat.assign("[invalid cmd] ");
    va_list args;
    va_start(args, message);
    SetParams(message, args);
    va_end(args);
}

/**
 * @func   ExceptionJsonCmd
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionInvalidCmd::ExceptionInvalidCmd(
    const ExceptionInvalidCmd& copied
) noexcept : Exception (copied) {

}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionInvalidCmd&
ExceptionInvalidCmd::operator= (
    const ExceptionInvalidCmd& copied
) noexcept {
    Exception::operator =(copied);
    return *this;
}

/**
 * @func   what
 * @brief  None
 * @param  None
 * @retval None
 */
const_char_p
ExceptionInvalidCmd::what(
) const noexcept {
    return Exception::what();
}

/******************************************************************************/
/*                        CLASS : EXCEPTION NULLPTR                           */
/******************************************************************************/

/**
 * @func   ExceptionNullPtr
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionNullPtr::ExceptionNullPtr(
) noexcept {
    m_strWhat.assign("[nullptr] ");
}

/**
 * @func   ~ExceptionNullPtr
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionNullPtr::~ExceptionNullPtr(
) noexcept {

}

/**
 * @func   ExceptionNullPtr
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionNullPtr::ExceptionNullPtr (
    const_char_p message,
    ...
) noexcept {
    m_strWhat.assign("[nullptr] ");
    va_list args;
    va_start(args, message);
    SetParams(message, args);
    va_end(args);
}

/**
 * @func   ExceptionNullPtr
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionNullPtr::ExceptionNullPtr(
    const ExceptionNullPtr& copied
) noexcept : Exception (copied) {
}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionNullPtr&
ExceptionNullPtr::operator= (
    const ExceptionNullPtr& copied
) noexcept {
    Exception::operator =(copied);
    return *this;
}

/**
 * @func   what
 * @brief  None
 * @param  None
 * @retval None
 */
const_char_p
ExceptionNullPtr::what(
) const noexcept {
    return Exception::what();
}

/******************************************************************************/
/*                        CLASS : EXCEPTION DATABASE                          */
/******************************************************************************/

/**
 * @func   ExceptionDatabase
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionErrDatabase::ExceptionErrDatabase(
) noexcept {
    m_strWhat.assign("[error db] ");
}

/**
 * @func   ~ExceptionDatabase
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionErrDatabase::~ExceptionErrDatabase(
) noexcept {

}

/**
 * @func   ExceptionDatabase
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionErrDatabase::ExceptionErrDatabase (
    const_char_p message,
    ...
) noexcept {
    m_strWhat.assign("[error db] ");
    va_list args;
    va_start(args, message);
    SetParams(message, args);
    va_end(args);
}

/**
 * @func   ExceptionDatabase
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionErrDatabase::ExceptionErrDatabase(
    const ExceptionErrDatabase& copied
) noexcept : Exception (copied) {

}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionErrDatabase&
ExceptionErrDatabase::operator= (
    const ExceptionErrDatabase& copied
) noexcept {
    Exception::operator =(copied);
    return *this;
}

/**
 * @func   what
 * @brief  None
 * @param  None
 * @retval None
 */
const_char_p
ExceptionErrDatabase::what(
) const noexcept {
    return Exception::what();
}

/******************************************************************************/
/*                           CLASS : WRONG PARAM                              */
/******************************************************************************/

/**
 * @func   ExceptionInvaLidArgs
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionInvalidArg::ExceptionInvalidArg(
) noexcept {

}

/**
 * @func   ~ExceptionInvaLidArgs
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionInvalidArg::~ExceptionInvalidArg(
) noexcept {
    m_strWhat.assign("[invalid arg] ");
}

/**
 * @func   ExceptionInvaLidArgs
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionInvalidArg::ExceptionInvalidArg(
    const ExceptionInvalidArg& copied
) noexcept : Exception (copied) {

}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionInvalidArg&
ExceptionInvalidArg::operator= (
    const ExceptionInvalidArg& copied
) noexcept {
    Exception::operator =(copied);
    return *this;
}

/**
 * @func   ExceptionInvaLidArgs
 * @brief  None
 * @param  None
 * @retval None
 */
ExceptionInvalidArg::ExceptionInvalidArg(
    const_char_p message,
    ...
) noexcept {
    m_strWhat.assign("[invalid arg] ");
    va_list args;
    va_start(args, message);
    SetParams(message, args);
    va_end(args);
}

/**
 * @func   what
 * @brief  None
 * @param  None
 * @retval None
 */
const_char_p
ExceptionInvalidArg::what(
) const noexcept {
    return Exception::what();
}
