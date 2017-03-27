/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Aggregation.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 3 Mar 2017 05:04:46
 *
 ******************************************************************************/
#ifndef AGGREGATION_HPP_
#define AGGREGATION_HPP_

#include <algorithm>
#include "../Typedefs.h"

// C: DbPtr<>
template<class C>
class Aggregation {
private:
    Vector<C> m_Element;
public:
    void_t Swap(Aggregation<C>& other);
public:
    typedef typename Vector<C>::iterator iterator;
    typedef typename Vector<C>::const_iterator const_iterator;

    Aggregation(Vector<C> vecElement);
    Aggregation(const Aggregation<C>& copied);
    Aggregation& operator= (const Aggregation<C>& copied);

    virtual ~Aggregation();

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    bool_t empty() const;
    int_t  size () const;
    void_t clear();
};

/**
 * @func   Swap
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
Aggregation<C>::Swap(
    Aggregation<C>& other
) {
    std::swap(m_Element, other.m_Element);
}

/**
 * @func   Aggregation
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Aggregation<C>::Aggregation(
    Vector<C> vecElement
) : m_Element (vecElement) {
}

/**
 * @func   Aggregation
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Aggregation<C>::Aggregation(
    const Aggregation<C>& copied
) : m_Element (copied.m_Element) {

}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline Aggregation<C>&
Aggregation<C>::operator= (
    const Aggregation<C>& copied
) {
    Aggregation<C> temp(copied);
    Swap(temp);
    return *this;
}

/**
 * @func   ~Aggregation
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Aggregation<C>::~Aggregation(
) {
}

/**
 * @func   empty
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
Aggregation<C>::empty(
) const {
    return m_Element.empty();
}

/**
 * @func   size
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline int_t
Aggregation<C>::size(
) const {
    return m_Element.size();
}

/**
 * @func   clear
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
Aggregation<C>::clear() {
    m_Element.clear();
}

/**
 * @func   begin
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
typename Aggregation<C>::iterator
Aggregation<C>::begin() {
    return m_Element.begin();
}

/**
 * @func   end
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
typename Aggregation<C>::iterator
Aggregation<C>::end() {
    return m_Element.end();
}

/**
 * @func   begin
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
typename Aggregation<C>::const_iterator
Aggregation<C>::begin(
) const {
    return m_Element.begin();
}

/**
 * @func   end
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
typename Aggregation<C>::const_iterator
Aggregation<C>::end(
) const {
    return m_Element.end();
}

#endif /* !AGGREGATION_HPP_ */
