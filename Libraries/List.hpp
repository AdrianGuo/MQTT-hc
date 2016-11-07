#ifndef LIST_HPP_
#define LIST_HPP_

#include <iostream>
#include <list>
#include "typedefs.h"

template<typename T>
class List {
public:
    std::list<T> element;
    typedef std::list<T> List_t;
    typedef typename List_t::iterator iterator_t;
    typedef typename List_t::const_iterator const_iterator_t;
    typedef typename List_t::reverse_iterator reverse_iterator_t;
    typedef typename List_t::const_reverse_iterator const_reverse_iterator_t;
    typedef typename List_t::reference reference_t;
    typedef typename List_t::const_reference const_reference_t;
    typedef typename List_t::value_type value_type_t;
    typedef typename List_t::size_type size_type_t;

    List& operator=(const List& x);
    iterator_t end();
    const_iterator_t end() const;
    iterator_t begin();
    const_iterator_t begin() const;
    reverse_iterator_t rbegin();
    const_reverse_iterator_t rbegin() const;
    reverse_iterator_t rend();
    const_reverse_iterator_t rend() const;
    bool_t empty() const;
    size_type_t size() const;
    size_type_t max_size() const;
    reference_t front();
    const_reference_t front() const;
    reference_t back();
    const_reference_t back() const;
    void_t assign (size_type_t n, const value_type_t& val);
    void_t push_front (const value_type_t& val);
    void_t pop_front();
    void_t push_back (const value_type_t& val);
    void_t pop_back();
    iterator_t insert (iterator_t position, const value_type_t& val);
    void_t insert (iterator_t position, size_type_t n, const value_type_t& val);
    iterator_t erase (iterator_t position);
    iterator_t erase (iterator_t first, iterator_t last);
    void_t swap (List& x);
    void_t resize (size_type_t n);
    void_t clear();

    void_t splice (iterator_t position, List& x);
    void_t splice (iterator_t position, List& x, iterator_t i);
    void_t splice (iterator_t position, List& x, iterator_t first, iterator_t last);
    void_t remove(const value_type_t& val);
    void_t reverse();

    bool_t operator== (const List& rhs);
    bool_t operator!= (const List& rhs);
    bool_t operator<  (const List& rhs);
    bool_t operator<= (const List& rhs);
    bool_t operator>  (const List& rhs);
    bool_t operator>= (const List& rhs);

};

template<typename T>
inline List<T>&
List<T>::operator=(
    const List<T>& x
) {
    element = x.element;
    return *this;
}

template<typename T>
inline typename List<T>::iterator_t
List<T>::end() {
    return element.end();
}

template<typename T>
inline typename List<T>::const_iterator_t
List<T>::end() const {
    return element.end();
}

template<typename T>
inline typename List<T>::iterator_t
List<T>::begin() {
    return element.begin();
}

template<typename T>
inline typename List<T>::const_iterator_t
List<T>::begin() const {
    return element.begin();
}

template<typename T>
inline typename List<T>::reverse_iterator_t
List<T>::rbegin() {
    return element.rbegin();
}

template<typename T>
inline typename List<T>::const_reverse_iterator_t
List<T>::rbegin() const {
    return element.rbegin();
}

template<typename T>
inline typename List<T>::reverse_iterator_t
List<T>::rend() {
    return element.rend();
}

template<typename T>
inline typename List<T>::const_reverse_iterator_t
List<T>::rend() const {
    return element.rend();
}

template<typename T>
bool_t
List<T>::empty() const {
    return element.empty();
}

template<typename T>
inline typename List<T>::size_type_t
List<T>::size() const {
    return element.size();
}

template<typename T>
inline typename List<T>::size_type_t
List<T>::max_size() const {
    return element.max_size();
}

template<typename T>
inline typename List<T>::reference_t
List<T>::front() {
    return element.front();
}

template<typename T>
inline typename List<T>::const_reference_t
List<T>::front() const {
    return element.front();
}

template<typename T>
inline typename List<T>::reference_t
List<T>::back() {
    return element.back();
}

template<typename T>
inline typename List<T>::const_reference_t
List<T>::back() const {
    return element.back();
}

template<typename T>
inline void_t
List<T>::assign (
    typename List<T>::size_type_t n,
    const typename List<T>::value_type_t& val
) {
    element.assign(n, val);
}

template<typename T>
inline void_t
List<T>::push_front (
    const typename List<T>::value_type_t& val
) {
    element.push_front(val);
}

template<typename T>
inline void_t
List<T>::pop_front() {
    element.pop_front();
}

template<typename T>
inline void_t
List<T>::push_back (
    const typename List<T>::value_type_t& val
) {
    element.push_back(val);
}

template<typename T>
inline void_t
List<T>::pop_back() {
    element.pop_back();
}

template<typename T>
inline typename List<T>::iterator_t
List<T>::insert (
    typename List<T>::iterator_t position,
    const typename List<T>::value_type_t& val
) {
    return element.insert(position, val);
}

template<typename T>
void_t
List<T>::insert (
    typename List<T>::iterator_t position,
    typename List<T>::size_type_t n,
    const typename List<T>::value_type_t& val
) {
    element.insert(position, n, val);
}

template<typename T>
inline typename List<T>::iterator_t
List<T>::erase (
    typename List<T>::iterator_t position
) {
    return element.erase(position);
}

template<typename T>
inline typename List<T>::iterator_t
List<T>::erase (
    iterator_t first,
    iterator_t last
) {
    return element.erase(first, last);
}

template<typename T>
inline  void_t
List<T>::swap (List<T>& x) {
    element.swap(x);
}

template<typename T>
inline void_t
List<T>::resize (
    typename List<T>::size_type_t n
) {
    element.resize(n);
}

template<typename T>
inline void_t
List<T>::clear() {
    element.clear();
}

template<typename T>
inline void_t
List<T>::splice (
    typename List<T>::iterator_t position,
    List<T>& x
) {
    element.splice(position, x);
}

template<typename T>
inline void_t
List<T>::splice (
    typename List<T>::iterator_t position,
    List<T>& x,
    typename List<T>::iterator_t i
) {
    element.splice(position, x, i);
}

template<typename T>
inline void_t
List<T>::splice (
    typename List<T>::iterator_t position,
    List<T>& x,
    typename List<T>::iterator_t first,
    typename List<T>::iterator_t last
) {
    element.splice(position, x, first, last);
}

template<typename T>
inline void_t
List<T>::remove(
    const typename List<T>::value_type_t & val
) {
    element.remove(val);
}

template<typename T>
inline void_t
List<T>::reverse() {
    element.reverse();
}

template<typename T>
inline bool_t
List<T>::operator== (
    const List<T>& rhs
) {
    return element == rhs.element;
}

template<typename T>
inline bool_t
List<T>::operator!= (
    const List<T>& rhs
) {
    return element != rhs.element;
}

template<typename T>
inline bool_t
List<T>::operator< (
    const List<T>& rhs
) {
    return element < rhs.element;
}

template<typename T>
inline bool_t
List<T>::operator<= (
    const List<T>& rhs
) {
    return element <= rhs.element;
}

template<typename T>
inline bool_t
List<T>::operator> (
    const List<T>& rhs
) {
    return element > rhs.element;
}

template<typename T>
inline bool_t
List<T>::operator>= (
    const List<T>& rhs
) {
    return element >= rhs.element;
}

#endif /* !LIST_HPP_ */
