#ifndef MAP_HPP_
#define MAP_HPP_

#include <iostream>
#include <map>
#include "typedefs.h"

template<typename K, typename T>
class Map {
public:
    std::map<K, T> element;
    typedef std::map<K, T> Map_t;
    typedef typename Map_t::iterator iterator_t;
    typedef typename Map_t::reverse_iterator reverse_iterator_t;
    typedef typename Map_t::const_iterator const_iterator_t;
    typedef typename Map_t::const_reverse_iterator const_reverse_iterator_t;
    typedef typename Map_t::size_type size_type_t;
    typedef typename Map_t::mapped_type mapped_type_t;
    typedef typename Map_t::key_type key_type_t;
    typedef typename Map_t::key_compare key_compare_t;
    typedef typename Map_t::value_compare value_compare_t;
    typedef typename Map_t::allocator_type allocator_type_t;
    typedef typename Map_t::value_type value_type_t;
    /**
     * Contruction
     */
    Map();
    virtual ~Map();
    Map<K, T>& operator=(const Map<K, T>& x);
    /**
     * Iterator
     */
    iterator_t begin();
    const_iterator_t begin() const;
    iterator_t end();
    const_iterator_t end() const;
    reverse_iterator_t rbegin();
    const_reverse_iterator_t rbegin() const;
    reverse_iterator_t rend();
    const_reverse_iterator_t rend() const;
    const_iterator_t cbegin();
    const_iterator_t cend();
    const_reverse_iterator_t crbegin();
    const_reverse_iterator_t crend();
    /**
     * Capacity
     */
    bool_t empty();
    size_type_t size();
    size_type_t max_size();
    /**
     * Element Access
     */
    mapped_type_t& operator[] (const key_type_t& k);
    mapped_type_t& at(const key_type_t& k);
    const mapped_type_t& at(const key_type_t& k) const;
    /**
     * Modifiers
     */
    iterator_t insert(iterator_t position, const value_type_t& val);
    void_t erase(iterator_t position);
    size_type_t erase(const key_type_t& k);
    void_t erase(iterator_t first, iterator_t last);
    void_t swap(Map<K, T>& x);
    void_t clear();
    /**
     * Observers
     */
    key_compare_t key_comp() const;
    value_compare_t value_comp() const;
    /**
     * Operations
     */
    iterator_t find(const key_type_t& k);
    const_iterator_t find(const key_type_t& k) const;
    size_type_t count(const key_type_t& k) const;
    iterator_t lower_bound(const key_type_t& k);
    const_iterator_t lower_bound(const key_type_t& k) const;
    iterator_t upper_bound();
    const_iterator_t upper_bound() const;
    /**
     * Allocator
     */
    allocator_type_t  get_allocator() const;
};

/**
 * Contruction
 */
template<typename K, typename T>
inline Map<K, T>::Map() {}

template<typename K, typename T>
inline Map<K, T>::~Map() {}

template<typename K, typename T>
inline Map<K, T>&
Map<K, T>::operator=(
    const Map<K, T> & x
) {
    element = x.element;
    return *this;
}

/**
 * Iterator
 */
template<typename K, typename T>
inline typename Map<K, T>::iterator_t
Map<K, T>::begin() {
    return element.begin();
}

template<typename K, typename T>
inline typename Map<K, T>::const_iterator_t
Map<K, T>::begin() const {
    return element.begin();
}

template<typename K, typename T>
inline typename Map<K, T>::iterator_t
Map<K, T>::end() {
    return element.end();
}

template<typename K, typename T>
inline typename Map<K, T>::const_iterator_t
Map<K, T>::end() const {
    return element.end();
}

template<typename K, typename T>
inline typename Map<K, T>::reverse_iterator_t
Map<K, T>::rbegin() {
    return element.rbegin();
}

template<typename K, typename T>
inline typename Map<K, T>::const_reverse_iterator_t
Map<K, T>::rbegin() const {
    return element.rbegin();
}

template<typename K, typename T>
inline typename Map<K, T>::reverse_iterator_t
Map<K, T>::rend() {
    return element.rend();
}

template<typename K, typename T>
inline typename Map<K, T>::const_reverse_iterator_t
Map<K, T>::rend() const {
    return element.rend();
}

template<typename K, typename T>
inline typename Map<K, T>::const_iterator_t
Map<K, T>::cbegin() {
    return element.cbegin();
}

template<typename K, typename T>
inline typename Map<K, T>::const_iterator_t
Map<K, T>::cend() {
    return element.cend();
}

template<typename K, typename T>
inline typename Map<K, T>::const_reverse_iterator_t
Map<K, T>::crbegin() {
    return element.crbegin();
}

template<typename K, typename T>
inline typename Map<K, T>::const_reverse_iterator_t
Map<K, T>::crend() {
    return element.crend();
}

template<typename K, typename T>
inline bool_t
Map<K, T>::empty() {
    return element.empty();
}

template<typename K, typename T>
inline typename Map<K, T>::size_type_t
Map<K, T>::size() {
    return element.size();
}

template<typename K, typename T>
inline typename Map<K, T>::size_type_t
Map<K, T>::max_size() {
    return element.max_size();
}

template<typename K, typename T>
inline typename Map<K, T>::mapped_type_t &
Map<K, T>::operator[](
    const typename Map<K, T>::key_type_t & k
) {
    return element[k];
}

template<typename K, typename T>
inline typename Map<K, T>::mapped_type_t &
Map<K, T>::at(
    const typename Map<K, T>::key_type_t & k
) {
    return element.at(k);
}

template<typename K, typename T>
inline const typename Map<K, T>::mapped_type_t &
Map<K, T>::at(
    const typename Map<K, T>::key_type_t & k
) const {
    return element.at(k);
}

template<typename K, typename T>
inline typename Map<K, T>::iterator_t
Map<K, T>::insert(
    typename Map<K, T>::iterator_t position,
    const typename Map<K, T>::value_type_t & val
) {
    return element.insert(position, val);
}

template<typename K, typename T>
inline void_t
Map<K, T>::erase(
    typename Map<K, T>::iterator_t position
) {
    element.erase(position);
}

template<typename K, typename T>
inline typename Map<K, T>::size_type_t
Map<K, T>::erase(
    const typename Map<K, T>::key_type_t & k
) {
    return element.erase(k);
}

template<typename K, typename T>
inline void_t
Map<K, T>::erase(
    typename Map<K, T>::iterator_t first,
    typename Map<K, T>::iterator_t last
) {
    element.erase(first, last);
}

template<typename K, typename T>
inline void_t
Map<K, T>::swap(
    Map<K, T> & x
) {
    element.swap(x);
}

template<typename K, typename T>
inline void_t
Map<K, T>::clear() {
    element.clear();
}

/**
 * Observers
 */
template<typename K, typename T>
inline typename Map<K, T>::key_compare_t
Map<K, T>::key_comp() const {
    return element.key_comp();
}

template<typename K, typename T>
inline typename Map<K, T>::value_compare_t
Map<K, T>::value_comp() const {
    return element.value_comp();
}

/**
 * Operations
 */
template<typename K, typename T>
inline typename Map<K, T>::iterator_t
Map<K, T>::find(
    const typename Map<K, T>::key_type_t & k
) {
    return element.find(k);
}

template<typename K, typename T>
inline typename Map<K, T>::const_iterator_t
Map<K, T>::find(
    const typename Map<K, T>::key_type_t & k
) const {
    return element.find(k);
}

template<typename K, typename T>
inline typename Map<K, T>::size_type_t
Map<K, T>::count(
    const typename Map<K, T>::key_type_t & k
) const {
    return element.count(k);
}

template<typename K, typename T>
inline typename Map<K, T>::iterator_t
Map<K, T>::lower_bound(
    const typename Map<K, T>::key_type_t & k
) {
    return element.lower_bound(k);
}

template<typename K, typename T>
inline typename Map<K, T>::const_iterator_t
Map<K, T>::lower_bound(
    const typename Map<K, T>::key_type_t & k
) const {
    return element.lower_bound(k);
}

template<typename K, typename T>
inline typename Map<K, T>::iterator_t
Map<K, T>::upper_bound() {
    return element.upper_bound();
}

template<typename K, typename T>
inline typename Map<K, T>::const_iterator_t
Map<K, T>::upper_bound() const {
    return element.upper_bound();
}

/**
 * Allocator
 */
template<typename K, typename T>
inline typename Map<K, T>::allocator_type_t
Map<K, T>::get_allocator() const {
    return element.get_allocator();
}

#endif /* !MAP_HPP_ */
