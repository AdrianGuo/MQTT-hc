/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name:
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-09 16:35:00 (Sun, 09 Oct 2016)
 *
 ******************************************************************************/

#ifndef SET_HPP_
#define SET_HPP_

#include <iostream>
#include <set>
#include "typedefs.h"

template<typename T>
class Set {
public:
    std::set<T> element;
    typedef std::set<T> Set_t;
    typedef typename Set_t::iterator iterator_t;
    typedef typename Set_t::const_iterator const_iterator_t;
    typedef typename Set_t::reverse_iterator reverse_iterator_t;
    typedef typename Set_t::const_reverse_iterator const_reverse_iterator_t;
    typedef typename Set_t::size_type size_type_t;
    typedef typename Set_t::value_type value_type_t;
    typedef typename Set_t::key_compare key_compare_t;
    typedef typename Set_t::value_compare value_compare_t;
    typedef typename Set_t::allocator_type allocator_type_t;

    Set();
    Set(const Set& x);
    ~Set();
    Set& operator= (const Set& x);

    iterator_t begin();
    const_iterator_t begin() const;
    iterator_t end();
    const_iterator_t end() const;
    reverse_iterator_t rbegin();
    const_reverse_iterator_t rbegin() const;
    reverse_iterator_t rend();
    const_reverse_iterator_t rend() const;
    bool_t empty() const;
    size_type_t size() const;
    size_type_t max_size() const;

    std::pair<iterator_t, bool_t> insert (const value_type_t& val);
    std::pair<iterator_t, bool_t> insert (value_type_t&& val);
    iterator_t insert (iterator_t position, const value_type_t& val);

    void_t erase (iterator_t position);
    size_type_t erase (const value_type_t& val);
    void_t erase (iterator_t first, iterator_t last);
    void_t swap (Set& x);
    void_t clear();
    key_compare_t key_comp() const;
    value_compare_t value_comp() const;
    iterator_t find (const value_type_t& val) const;
    size_type_t count (const value_type_t& val) const;
    iterator_t lower_bound (const value_type_t& val) const;
    iterator_t upper_bound (const value_type_t& val) const;
    allocator_type_t get_allocator() const;
};

template<typename T>
inline Set<T>::Set() {}

template<typename T>
inline Set<T>::Set(const Set& x) {
    element = x.element;
}

template<typename T>
inline Set<T>::~Set() {

}

template<typename T>
inline Set<T>&
Set<T>::operator= (const Set& x) {
    element = x.element;
    return *this;
}

template<typename T>
inline typename Set<T>::iterator_t
Set<T>::begin() {
    return element.begin();
}

template<typename T>
inline typename Set<T>::const_iterator_t
Set<T>::begin() const {
    return element.begin();
}

template<typename T>
inline typename Set<T>::iterator_t
Set<T>::end() {
    return element.end();
}

template<typename T>
inline typename Set<T>::const_iterator_t
Set<T>::end() const {
    return element.end();
}

template<typename T>
inline typename Set<T>::reverse_iterator_t
Set<T>::rbegin() {
    return element.rbegin();
}

template<typename T>
inline typename Set<T>::const_reverse_iterator_t
Set<T>::rbegin() const {
    return element.rbegin();
}

template<typename T>
inline typename Set<T>::reverse_iterator_t
Set<T>::rend() {
    return element.rend();
}

template<typename T>
inline typename Set<T>::const_reverse_iterator_t
Set<T>::rend() const {
    return element.rend();
}

template<typename T>
bool_t
Set<T>::empty() const {
    return element.empty();
}

template<typename T>
inline typename Set<T>::size_type_t
Set<T>::size() const {
    return element.size();
}

template<typename T>
inline typename Set<T>::size_type_t
Set<T>::max_size() const {
    return element.max_size();
}

template<typename T>
inline std::pair<typename Set<T>::iterator_t, bool_t>
Set<T>::insert (
    const typename Set<T>::value_type_t& val
) {
    return element.insert(val);
}

template<typename T>
inline std::pair<typename Set<T>::iterator_t, bool_t>
Set<T>::insert (
    typename Set<T>::value_type_t&& val
) {
    return element.insert(val);
}

template<typename T>
inline typename Set<T>::iterator_t
Set<T>::insert (
    typename Set<T>::iterator_t position,
    const typename Set<T>::value_type_t& val
) {
    return element.insert(position, val);
}

template<typename T>
void_t
Set<T>::erase (
    typename Set<T>::iterator_t position
) {
    element.erase(position);
}

template<typename T>
inline typename Set<T>::size_type_t
Set<T>::erase (
    const typename Set<T>::value_type_t& val
) {
    return element.erase(val);
}

template<typename T>
void_t
Set<T>::erase (
    typename Set<T>::iterator_t first,
    typename Set<T>::iterator_t last
) {
    element.erase(first, last);
}

template<typename T>
void_t
Set<T>::swap (
    Set& x
) {
    element.swap(x);
}

template<typename T>
void_t
Set<T>::clear() {
    element.clear();
}

template<typename T>
inline typename Set<T>::key_compare_t
Set<T>::key_comp() const {
    return element.key_comp();
}

template<typename T>
inline typename Set<T>::value_compare_t
Set<T>::value_comp() const {
    return element.value_comp();
}

template<typename T>
inline typename Set<T>::iterator_t
Set<T>::find (
    const typename Set<T>::value_type_t& val
) const {
    return element.find(val);
}

template<typename T>
inline typename Set<T>::size_type_t
Set<T>::count (
    const typename Set<T>::value_type_t& val
) const {
    return element.count(val);
}

template<typename T>
inline typename Set<T>::iterator_t
Set<T>::lower_bound (
    const typename Set<T>::value_type_t& val
) const {
    return element.lower_bound(val);
}

template<typename T>
inline typename Set<T>::iterator_t
Set<T>::upper_bound (
    const typename Set<T>::value_type_t& val
) const {
    return element.upper_bound(val);
}

template<typename T>
inline typename Set<T>::allocator_type_t
Set<T>::get_allocator() const {
    return element.get_allocator();
}

#endif /* !SET_HPP_ */
