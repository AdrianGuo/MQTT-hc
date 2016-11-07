#ifndef QUEUE_HPP_
#define QUEUE_HPP_

#include <iostream>
#include <queue>
#include "typedefs.h"

template<typename T>
class Queue {
public:
    std::queue<T> element;

    typedef std::queue<T> Queue_t;
    typedef typename Queue_t::size_type size_type_t;
    typedef typename Queue_t::value_type value_type_t;

    Queue();
    virtual ~Queue();
    bool_t empty() const;
    size_type_t size() const;
    value_type_t front();
    value_type_t front() const;
    value_type_t back();
    value_type_t back() const;
    void_t push(const value_type_t& val);
    void_t pop();
};

template<typename T>
inline Queue<T>::Queue() {}

template<typename T>
inline Queue<T>::~Queue() {}

template<typename T>
inline bool_t
Queue<T>::empty() const {
    return element.empty();
}

template<typename T>
inline typename Queue<T>::size_type_t
Queue<T>::size() const {
    return element.size();
}

template<typename T>
inline typename Queue<T>::value_type_t
Queue<T>::front() {
    return element.front();
}

template<typename T>
inline typename Queue<T>::value_type_t
Queue<T>::front() const {
    return element.front();
}

template<typename T>
inline typename Queue<T>::value_type_t
Queue<T>::back() {
    return element.back();
}

template<typename T>
inline typename Queue<T>::value_type_t
Queue<T>::back() const {
    return element.back();
}

template<typename T>
inline void_t
Queue<T>::push(
    const Queue<T>::value_type_t & val) {
    element.push(val);
}

template<typename T>
inline void_t
Queue<T>::pop() {
    element.pop();
}

#endif /* !QUEUE_HPP_ */
