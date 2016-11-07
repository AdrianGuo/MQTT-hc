#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <iostream>
#include <vector>
#include "typedefs.h"

template<typename T>
class Vector {
public:
    std::vector<T> element;
    typedef std::vector<T> Vector_t;
    typedef typename Vector_t::iterator iterator_t;
    typedef typename Vector_t::const_iterator const_iterator_t;
    typedef typename Vector_t::reverse_iterator reverse_iterator_t;
    typedef typename Vector_t::const_reverse_iterator const_reverse_iterator_t;
    typedef typename Vector_t::size_type size_type_t;
    typedef typename Vector_t::reference reference_t;
    typedef typename Vector_t::const_reference const_reference_t;
    typedef typename Vector_t::value_type value_type_t;
    typedef typename Vector_t::allocator_type allocator_type_t;
    /**
    * Contruction
    */
    Vector();
    ~Vector();
    Vector& operator=(const Vector& x);
    /**
    * Iterators
    */
    iterator_t begin();
    const_iterator_t begin() const;

    iterator_t end();
    const_iterator_t end() const;

    reverse_iterator_t rbegin();
    const_reverse_iterator_t rbegin() const;

    reverse_iterator_t rend();
    const_reverse_iterator_t rend() const;

    /**
    * Capacity
    */
    size_type_t size() const;
    size_type_t max_size() const;
    void_t resize(size_type_t n);
    size_type_t capacity() const;
    bool_t empty() const;
    void_t reserve(size_type_t n);
    /**
    * Element access
    */
    reference_t operator[](size_type_t n);
    const_reference_t operator[](size_type_t n) const;
    reference_t at(size_type_t n);
    const_reference_t at(size_type_t n) const;
    reference_t front();
    const_reference_t front() const;
    reference_t back();
    const_reference_t back() const;
    /**
    * Modifiers
    */
    void_t push_back(const value_type_t& val);
    void_t pop_back();

    iterator_t insert(iterator_t position, const value_type_t& val);
    void_t insert(iterator_t position, size_type_t n, const value_type_t& val);
    template<class InputIterator>
    void_t insert(iterator_t position, InputIterator first, InputIterator last);

    iterator_t erase(iterator_t position);
    iterator_t erase(iterator_t first, iterator_t last);
    void_t swap(Vector& x);
    void_t clear();

    allocator_type_t get_allocator() const;
};

template<typename T>
inline Vector<T>::Vector() {}

template<typename T>
inline Vector<T>::~Vector() {}

template<typename T>
inline Vector<T>&
Vector<T>::operator=(
    const Vector<T>& x
) {
    element = x.element;
    return *this;
}

template<typename T>
inline typename Vector<T>::iterator_t
Vector<T>::begin() {
    return element.begin();
}

template<typename T>
inline typename Vector<T>::const_iterator_t
Vector<T>::begin() const {
    return element.begin();
}

template<typename T>
inline typename Vector<T>::iterator_t
Vector<T>::end() {
    return element.end();
}

template<typename T>
inline typename Vector<T>::const_iterator_t
Vector<T>::end() const {
    return element.end();
}

template<typename T>
inline typename Vector<T>::reverse_iterator_t
Vector<T>::rbegin() {
    return element.rbegin();
}

template<typename T>
inline typename Vector<T>::const_reverse_iterator_t
Vector<T>::rbegin() const {
    return element.rbegin();
}

template<typename T>
inline typename Vector<T>::reverse_iterator_t
Vector<T>::rend() {
    return element.rend();
}

template<typename T>
inline typename Vector<T>::const_reverse_iterator_t
Vector<T>::rend() const {
    return element.rend();
}

template<typename T>
inline typename Vector<T>::size_type_t
Vector<T>::size() const {
    return element.size();
}

template<typename T>
inline typename Vector<T>::size_type_t
Vector<T>::max_size() const {
    return element.max_size();
}

template<typename T>
inline void_t
Vector<T>::resize(
    typename Vector<T>::size_type_t n) {
    return element.resize(n);
}

template<typename T>
inline typename Vector<T>::size_type_t
Vector<T>::capacity() const {
    return element.capacity();
}

template<typename T>
inline bool_t
Vector<T>::empty() const {
    return element.empty();
}

template<typename T>
inline void_t
Vector<T>::reserve(
    typename Vector<T>::size_type_t n
) {
    return element.reserve(n);
}

template<typename T>
inline typename Vector<T>::reference_t
Vector<T>::operator[](
    typename Vector<T>::size_type_t n
) {
    return element[n];
}

template<typename T>
inline typename Vector<T>::const_reference_t
Vector<T>::operator[](
    typename Vector<T>::size_type_t n
) const {
    return element[n];
}

template<typename T>
inline typename Vector<T>::reference_t
Vector<T>::at(
    typename Vector<T>::size_type_t n
) {
    return element.at(n);
}

template<typename T>
inline typename Vector<T>::const_reference_t
Vector<T>::at(
    typename Vector<T>::size_type_t n
) const {
    return element.at(n);
}

template<typename T>
inline typename Vector<T>::reference_t
Vector<T>::front() {
    return element.front();
}

template<typename T>
inline typename Vector<T>::const_reference_t
Vector<T>::front() const {
    return element.front();
}

template<typename T>
inline typename Vector<T>::reference_t
Vector<T>::back() {
    return element.back();
}

template<typename T>
inline typename Vector<T>::const_reference_t
Vector<T>::back() const {
    return element.back();
}

template<typename T>
inline void_t
Vector<T>::push_back(
    const typename Vector<T>::value_type_t & val
) {
    return element.push_back(val);
}

template<typename T>
inline void_t
Vector<T>::pop_back() {
    return element.pop_back();
}

template<typename T>
inline typename Vector<T>::iterator_t
Vector<T>::insert(
    typename Vector<T>::iterator_t position,
    const typename Vector<T>::value_type_t & val
) {
    return element.insert(position, val);
}

template<typename T>
inline void_t
Vector<T>::insert(
    typename Vector<T>::iterator_t position,
    typename Vector<T>::size_type_t n,
    const typename Vector<T>::value_type_t & val
) {
    return element.insert(position, n, val);
}

template<typename T>
template<class InputIterator>
inline void_t
Vector<T>::insert(
    typename Vector<T>::iterator_t position,
    InputIterator first,
    InputIterator last
) {
    element.insert(position, first, last);
}

template<typename T>
inline typename Vector<T>::iterator_t
Vector<T>::erase(
    typename Vector<T>::iterator_t position
) {
    return element.erase(position);
}

template<typename T>
inline typename Vector<T>::iterator_t
Vector<T>::erase(
    typename Vector<T>::iterator_t first,
    typename Vector<T>::iterator_t last
) {
    return element.erase(first, last);
}

template<typename T>
inline void_t
Vector<T>::swap(
    Vector<T>& x
) {
    element.swap(x.element);
}

template<typename T>
inline void_t
Vector<T>::clear() {
    return element.clear();
}

template<typename T>
inline typename Vector<T>::allocator_type_t
Vector<T>::get_allocator() const {
    return element.get_allocator();
}

#endif /* !VECTOR_HPP_ */
