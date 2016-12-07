#ifndef STRING_HPP_
#define STRING_HPP_

#include <iostream>
#include <string>
#include <algorithm>
#include "typedefs.h"

class String {
public:
    static const size_t npos = -1;
    std::string element;
    typedef std::string String_t;
    typedef typename String_t::iterator iterator_t;
    typedef typename String_t::const_iterator const_iterator_t;
    typedef typename String_t::reverse_iterator reverse_iterator_t;
    typedef typename String_t::const_reverse_iterator const_reverse_iterator_t;
    typedef typename String_t::allocator_type allocator_type_t;
    typedef typename String_t::value_type value_type_t;

    String();
    String(const String& str);
    String(const String& str, size_t pos, size_t len = npos);
    String(const_char_p s);
    String(const_char_p s, size_t n);
    String(size_t n, char c);
    ~String() {}

    String& operator= (const String& str);
    String& operator= (const_char_p s);
    String& operator= (char c);

    iterator_t begin();
    const_iterator_t begin() const;

    iterator_t end();
    const_iterator_t end() const;

    reverse_iterator_t rbegin();
    const_reverse_iterator_t rbegin() const;

    reverse_iterator_t rend();
    const_reverse_iterator_t rend() const;

    size_t size() const;
    size_t length() const;
    size_t max_size() const;

    void_t resize(size_t n);
    void_t resize(size_t n, char c);
    size_t capacity() const;
    void_t reserve(size_t n = 0);
    void_t clear();
    bool_t empty() const;

    char& operator[] (size_t pos);
    const char& operator[] (size_t pos) const;
    char& at(size_t pos);
    const char& at(size_t pos) const;

    String& operator+= (const String& str);
    String& operator+= (const_char_p s);
    String& operator+= (char c);

    String& append(const String& str);
    String& append(const_char_p s);
    String& append(const_char_p s, size_t n);
    String& append(size_t n, char c);

    void_t push_back(char c);

    String& assign(const String& str);
    String& assign(const String& str, size_t subpos, size_t sublen);
    String& assign(const_char_p s);
    String& assign(const_char_p s, size_t n);
    String& assign(size_t n, char c);

    String& insert(size_t pos, String& str);
    String& insert(size_t pos, String& str, size_t subpos, size_t sublen);
    String& insert(size_t pos, const_char_p s);
    String& insert(size_t pos, const_char_p s, size_t n);
    String& insert(size_t pos, size_t n, char c);
    void_t insert(iterator_t p, size_t n, char c);
    iterator_t insert(iterator_t p, char c);

    String& erase(size_t pos = 0, size_t len = npos);
    iterator_t erase(iterator_t p);
    iterator_t erase(iterator_t first, iterator_t last);

    String& replace(size_t pos, size_t len, const String& str);
    String& replace(iterator_t i1, iterator_t i2, const String& str);
    String& replace(size_t pos, size_t len, const String& str, size_t subpos, size_t sublen);
    String& replace(size_t pos, size_t len, const_char_p s);
    String& replace(iterator_t i1, iterator_t i2, const_char_p s);
    String& replace(size_t pos, size_t len, const_char_p s, size_t n);
    String& replace(iterator_t i1, iterator_t i2, const_char_p s, size_t n);
    String& replace(size_t pos, size_t len, size_t n, char c);
    String& replace(iterator_t i1, iterator_t i2, size_t n, char c);

    void_t swap(String& str);

    const char* c_str() const;
    const char* data() const;

    allocator_type_t get_allocator() const;

    size_t copy(char_p s, size_t len, size_t pos = 0) const;

    size_t find(const String& str, size_t pos = 0) const;
    size_t find(const_char_p s, size_t pos = 0) const;
    size_t find(const_char_p s, size_t pos, size_t n) const;
    size_t find(char c, size_t pos = 0) const;

    size_t rfind(const String& str, size_t pos = npos) const;
    size_t rfind(const_char_p s, size_t pos = npos) const;
    size_t rfind(const_char_p s, size_t pos, size_t n) const;
    size_t rfind(char c, size_t pos = npos) const;

    size_t find_first_of(const String& str, size_t pos = 0) const;
    size_t find_first_of(const_char_p s, size_t pos = 0) const;
    size_t find_first_of(const_char_p s, size_t pos, size_t n) const;
    size_t find_first_of(char c, size_t pos = 0) const;

    size_t find_last_of(const String& str, size_t pos = npos) const;
    size_t find_last_of(const_char_p s, size_t pos = npos) const;
    size_t find_last_of(const_char_p s, size_t pos, size_t n) const;
    size_t find_last_of(char c, size_t pos = npos) const;

    size_t find_first_not_of(const String& str, size_t pos = 0) const;
    size_t find_first_not_of(const_char_p s, size_t pos = 0) const;
    size_t find_first_not_of(const_char_p s, size_t pos, size_t n) const;
    size_t find_first_not_of(char c, size_t pos = 0) const;

    size_t find_last_not_of(const String& str, size_t pos = npos) const;
    size_t find_last_not_of(const_char_p s, size_t pos = npos) const;
    size_t find_last_not_of(const_char_p s, size_t pos, size_t n) const;
    size_t find_last_not_of(char c, size_t pos = npos) const;

    String substr(size_t pos = 0, size_t len = npos) const;

    int_t compare(String& str) const;
    int_t compare(const String& str) const;
    int_t compare(size_t pos, size_t len, const String& str) const;
    int_t compare(size_t pos, size_t len, const String& str,
                  size_t subpos, size_t sublen) const;
    int_t compare(const_char_p s) const;
    int_t compare(size_t pos, size_t len, const_char_p s) const;
    int_t compare(size_t pos, size_t len, const_char_p s, size_t n) const;

    String operator+ (const String& rhs);
    String operator+ (const_char_p rhs);
    String operator+ (char rhs);

    friend String operator+ (const String &lhs, const String& rhs);
    friend String operator+ (const String &lhs, const_char_p rhs);
    friend String operator+ (const_char_p lhs, const String& rhs);
    friend String operator+ (const String &lhs, char rhs);
    friend String operator+ (char lhs, const String &rhs);

    bool_t operator== (const String& rhs);
    bool_t operator== (const_char_p rhs);
    friend bool_t operator== (const String& lhs, const String& rhs);
    friend bool_t operator== (const_char_p lhs, const String& rhs);
    friend bool_t operator== (const String& lhs, const_char_p rhs);

    bool_t operator!= (const String& rhs);
    bool_t operator!= (const_char_p rhs);
    friend bool_t operator!= (const String& lhs, const String& rhs);
    friend bool_t operator!= (const_char_p lhs, const String& rhs);
    friend bool_t operator!= (const String& lhs, const_char_p rhs);

    bool_t operator< (const String& rhs);
    bool_t operator< (const_char_p rhs);
    friend bool_t operator<  (const String& lhs, const String& rhs);
    friend bool_t operator<  (const_char_p lhs, const String& rhs);
    friend bool_t operator<  (const String& lhs, const_char_p rhs);

    bool_t operator<= (const String& rhs);
    bool_t operator<= (const_char_p rhs);
    friend bool_t operator<= (const String& lhs, const String& rhs);
    friend bool_t operator<= (const_char_p lhs, const String& rhs);
    friend bool_t operator<= (const String& lhs, const_char_p rhs);

    bool_t operator> (const String& rhs);
    bool_t operator> (const_char_p rhs);
    friend bool_t operator>  (const String& lhs, const String& rhs);
    friend bool_t operator>  (const_char_p lhs, const String& rhs);
    friend bool_t operator>  (const String& lhs, const_char_p rhs);

    bool_t operator>= (const String& rhs);
    bool_t operator>= (const_char_p rhs);
    friend bool_t operator>= (const String& lhs, const String& rhs);
    friend bool_t operator>= (const_char_p   lhs, const String& rhs);
    friend bool_t operator>= (const String& lhs, const_char_p rhs);

    String remove_char(char ch);
};

inline String::String() {}

inline String::String(
    const String & str
) {
    element = str.element;
}

inline String::String(
    const String & str,
    size_t pos,
    size_t len
) : element(str.element, pos, len) {

}

inline String::String(
    const_char_p s
) : element(s) {}

inline String::String(
    const_char_p s,
    size_t n
) : element(s, n) {}

inline String::String(
    size_t n,
    char c
) : element(n, c) {}

inline String &
String::operator=(
    const String & str
) {
    element = str.element;
    return *this;
}

inline String &
String::operator=(
    const_char_p s
) {
    element = s;
    return *this;
}

inline String &
String::operator=(
    char c
) {
    element = c;
    return *this;
}

inline String::iterator_t
String::begin() {
    return element.begin();
}

inline String::const_iterator_t
String::begin() const {
    return element.begin();
}

inline String::iterator_t
String::end() {
    return element.end();
}

inline String::const_iterator_t
String::end() const {
    return element.end();
}

inline String::reverse_iterator_t
String::rbegin() {
    return element.rbegin();
}

inline String::const_reverse_iterator_t
String::rbegin() const {
    return element.rbegin();
}

inline String::reverse_iterator_t
String::rend() {
    return element.rend();
}

inline String::const_reverse_iterator_t
String::rend() const {
    return element.rend();
}

inline size_t
String::size() const {
    return element.size();
}

inline size_t
String::length() const {
    return element.length();
}

inline size_t
String::max_size() const {
    return element.max_size();
}

inline void_t
String::resize(size_t n) {
    element.resize(n);
}

inline void_t
String::resize(size_t n, char c) {
    element.resize(n, c);
}

inline size_t
String::capacity() const {
    return element.capacity();
}

inline void_t
String::reserve(size_t n) {
    element.reserve(n);
}

inline void_t
String::clear() {
    element.clear();
}

inline bool_t
String::empty() const {
    return element.empty();
}

inline char &
String::operator[](
    size_t pos
    ) {
    return element[pos];
}

inline const char &
String::operator[](
    size_t pos
) const {
    return element[pos];
}

inline char &
String::at(
    size_t pos
) {
    return element.at(pos);
}

inline const char &
String::at(
    size_t pos
) const {
    return element.at(pos);
}

inline String &
String::operator+= (
    const String & str
) {
    element += str.element;
    return *this;
}

inline String &
String::operator+= (
    const_char_p s
) {
    element += s;
    return *this;
}

inline String &
String::operator+= (
    char c
) {
    element += c;
    return *this;
}

inline String&
String::append(
    const String& str
) {
    element.append(str.element);
    return *this;
}
inline String &
String::append(
    const_char_p s
) {
    element.append(s);
    return *this;
}
inline String &
String::append(
    const_char_p s,
    size_t n
) {
    element.append(s, n);
    return *this;
}
inline String &
String::append(
    size_t n,
    char c
) {
    element.append(n, c);
    return *this;
}

inline void_t
String::push_back(char c) {
    element.push_back(c);
}

inline String &
String::assign(
    const String & str
) {
    element.assign(str.element);
    return *this;
}

inline String &
String::assign(
    const String & str,
    size_t subpos,
    size_t sublen
) {
    element.assign(str.element, subpos, sublen);
    return *this;
}

inline String &
String::assign(
    const_char_p s
) {
    element.assign(s);
    return *this;
}

inline String &
String::assign(
    const_char_p s,
    size_t n
) {
    element.assign(s, n);
    return *this;
}

inline String &
String::assign(
    size_t n,
    char c
) {
    element.assign(n, c);
    return *this;
}

inline String &
String::insert(
    size_t pos,
    String & str
) {
    element.insert(pos, str.element);
    return *this;
}

inline String &
String::insert(
    size_t pos,
    String & str,
    size_t subpos,
    size_t sublen
) {
    element.insert(pos, str.element, subpos, sublen);
    return *this;
}

inline String &
String::insert(
    size_t pos,
    const_char_p s
) {
    element.insert(pos, s);
    return *this;
}

inline String &
String::insert(
    size_t pos,
    const_char_p s,
    size_t n
) {
    element.insert(pos, s, n);
    return *this;
}

inline String &
String::insert(
    size_t pos,
    size_t n,
    char c
) {
    element.insert(pos, n, c);
    return *this;
}

inline void_t
String::insert(
    String::iterator_t p,
    size_t n,
    char c
) {
    element.insert(p, n, c);
}

inline String::iterator_t
String::insert(
    String::iterator_t p,
    char c
) {
    return element.insert(p, c);
}


inline String &
String::erase(
    size_t pos,
    size_t len
) {
    element.erase(pos, len);
    return *this;
}

inline String::iterator_t
String::erase(
    String::iterator_t p
) {
    return element.erase(p);
}

inline String::iterator_t
String::erase(
    String::iterator_t first,
    String::iterator_t last
) {
    return element.erase(first, last);
}

inline String &
String::replace(
    size_t pos,
    size_t len,
    const String & str
) {
    element.replace(pos, len, str.element);
    return *this;
}

inline String &
String::replace(
    String::iterator_t i1,
    String::iterator_t i2,
    const String & str
) {
    element.replace(i1, i2, str.element);
    return *this;
}

inline String &
String::replace(
    size_t pos,
    size_t len,
    const String & str,
    size_t subpos,
    size_t sublen
) {
    element.replace(pos, len, str.element, subpos, sublen);
    return *this;
}

inline String &
String::replace(
    size_t pos,
    size_t len,
    const_char_p s
) {
    element.replace(pos, len, s);
    return *this;
}

inline String &
String::replace(
    String::iterator_t i1,
    String::iterator_t i2,
    const_char_p s
) {
    element.replace(i1, i2, s);
    return *this;
}

inline String &
String::replace(
    size_t pos,
    size_t len,
    const_char_p s,
    size_t n
) {
    element.replace(pos, len, s, n);
    return *this;
}

inline String &
String::replace(
    String::iterator_t i1,
    String::iterator_t i2,
    const_char_p s,
    size_t n
) {
    element.replace(i1, i2, s, n);
    return *this;
}

inline String &
String::replace(
    size_t pos,
    size_t len,
    size_t n,
    char c
) {
    element.replace(pos, len, n, c);
    return *this;
}

inline String &
String::replace(
    String::iterator_t i1,
    String::iterator_t i2,
    size_t n,
    char c
) {
    element.replace(i1, i2, n, c);
    return *this;
}

inline void_t
String::swap(
    String & str
) {
    element.swap(str.element);
}

inline const char*
String::c_str() const {
    return element.c_str();
}

inline const char*
String::data() const {
    return element.data();
}

inline String::allocator_type_t
String::get_allocator() const {
    return element.get_allocator();
}

inline size_t
String::copy(
    char_p s,
    size_t len,
    size_t pos
) const {
    return element.copy(s, len, pos);
}

inline size_t
String::find(
    const String & str,
    size_t pos
) const {
    return element.find(str.element, pos);
}

inline size_t
String::find(
    const_char_p s,
    size_t pos
) const {
    return element.find(s, pos);
}

inline size_t
String::find(
    const_char_p s,
    size_t pos,
    size_t n
) const {
    return element.find(s, pos, n);
}

inline size_t
String::find(
    char c,
    size_t pos
) const {
    return element.find(c, pos);
}

inline size_t
String::rfind(
    const String & str,
    size_t pos
) const {
    return element.rfind(str.element, pos);
}

inline size_t
String::rfind(
    const_char_p s,
    size_t pos
) const {
    return element.rfind(s, pos);
}

inline size_t
String::rfind(
    const_char_p s,
    size_t pos,
    size_t n
) const {
    return element.rfind(s, pos, n);
}

inline size_t
String::rfind(
    char c,
    size_t pos
) const {
    return element.rfind(c, pos);
}

inline size_t
String::find_first_of(
    const String & str,
    size_t pos
) const {
    return element.find_first_of(str.element, pos);
}

inline size_t
String::find_first_of(
    const_char_p s,
    size_t pos
) const {
    return element.find_first_of(s, pos);
}

inline size_t
String::find_first_of(
    const_char_p s,
    size_t pos,
    size_t n
) const {
    return element.find_first_of(s, pos, n);
}

inline size_t
String::find_first_of(
    char c,
    size_t pos
) const {
    return element.find_first_of(c, pos);
}

inline size_t
String::find_last_of(
    const String & str,
    size_t pos
) const {
    return element.find_last_of(str.element, pos);
}

inline size_t
String::find_last_of(
    const_char_p s,
    size_t pos
) const {
    return element.find_last_of(s, pos);
}

inline size_t
String::find_last_of(
    const_char_p s,
    size_t pos,
    size_t n
) const {
    return element.find_last_of(s, pos, n);
}

inline size_t
String::find_last_of(
    char c,
    size_t pos
) const {
    return element.find_last_of(c, pos);
}

inline size_t
String::find_first_not_of(
    const String & str,
    size_t pos
) const {
    return element.find_first_not_of(str.element, pos);
}

inline size_t
String::find_first_not_of(
    const_char_p s,
    size_t pos
) const {
    return element.find_first_not_of(s, pos);
}

inline size_t
String::find_first_not_of(
    const_char_p s,
    size_t pos,
    size_t n
) const {
    return element.find_first_not_of(s, pos, n);
}

inline size_t
String::find_first_not_of(
    char c,
    size_t pos
) const {
    return element.find_first_not_of(c, pos);
}

inline size_t
String::find_last_not_of(
    const String & str,
    size_t pos
) const {
    return element.find_last_not_of(str.element, pos);
}

inline size_t
String::find_last_not_of(
    const_char_p s,
    size_t pos
) const {
    return element.find_last_not_of(s, pos);
}

inline size_t
String::find_last_not_of(
    const_char_p s,
    size_t pos,
    size_t n
) const {
    return element.find_last_not_of(s, pos, n);
}

inline size_t
String::find_last_not_of(
    char c,
    size_t pos
) const {
    return element.find_last_not_of(c, pos);
}

inline String
String::substr(
    size_t pos,
    size_t len
) const {
    String s;
    s.element = element.substr(pos, len);

    return s;
}

inline int_t
String::compare(
    String & str
) const {
    return element.compare(str.element);
}

inline int_t
String::compare(
    const String & str
) const {
    return element.compare(str.element);
}

inline int_t
String::compare(
    size_t pos,
    size_t len,
    const String & str
) const {
    return element.compare(pos, len, str.element);
}

inline int_t
String::compare(
    size_t pos,
    size_t len,
    const String & str,
    size_t subpos,
    size_t sublen
) const {
    return element.compare(pos, len, str.element, subpos, sublen);
}

inline int_t
String::compare(
    const_char_p s
) const {
    return element.compare(s);
}

inline int_t
String::compare(
    size_t pos,
    size_t len,
    const_char_p s
) const {
    return element.compare(pos, len, s);
}

inline int_t
String::compare(
    size_t pos,
    size_t len,
    const_char_p s,
    size_t n
) const {
    return element.compare(pos, len, s, n);
}

inline String
String::operator+ (
    const String & rhs
) {
    String str(*this);
    return (str += rhs);
}

inline String
String::operator+ (
    const_char_p rhs
) {
    String str(*this);
    return (str += rhs);
}

inline String
String::operator+ (
    char rhs
) {
    String str(*this);
    return (str += rhs);
}

inline String operator+ (
    const String &lhs,
    const String& rhs
) {
    String str(lhs);
    return (str += rhs);
}

inline String operator+ (
    const String &lhs,
    const_char_p rhs
) {
    String str(lhs);
    return (str += rhs);
}

inline String operator+ (
    const_char_p lhs,
    const String& rhs
) {
    String str(lhs);
    return (str += rhs);
}

inline String operator+ (
    const String &lhs,
    char rhs
) {
    String str(lhs);
    return (str += rhs);
}

inline String operator+ (
    char lhs,
    const String &rhs
) {
    String str(1, lhs);
    return (str += rhs);
}

inline bool_t
String::operator== (
    const String & rhs
) {
    return element == rhs.element;
}

inline bool_t
String::operator== (
    const_char_p rhs
) {
    return element == rhs;
}

inline bool_t
operator== (
    const String& lhs,
    const String& rhs
) {
   return lhs.element == rhs.element;
}

inline bool_t
operator== (
    const_char_p lhs,
    const String& rhs
) {
    return lhs == rhs.element;
}

inline bool_t
operator== (
    const String& lhs,
    const_char_p rhs
) {
   return lhs.element == rhs;
}

inline bool_t
String::operator!= (
    const String & rhs
) {
    return element != rhs.element;
}

inline bool_t
String::operator!= (
    const_char_p rhs
) {
    return element != rhs;
}

inline bool_t
operator!= (
    const String& lhs,
    const String& rhs
) {
    return lhs.element != rhs.element;
}

inline bool_t
operator!= (
    const_char_p lhs,
    const String& rhs
) {
    return lhs != rhs.element;
}

inline bool_t
operator!= (
    const String& lhs,
    const_char_p rhs
) {
    return lhs.element != rhs;
}

inline bool_t
String::operator< (
    const String & rhs
) {
    return element < rhs.element;
}

inline bool_t
String::operator< (
    const_char_p rhs
) {
    return element < rhs;
}

inline bool_t
operator< (
    const String& lhs,
    const String& rhs
) {
    return lhs.element < rhs.element;
}

inline bool_t
operator< (
    const_char_p lhs,
    const String& rhs
) {
    return lhs < rhs.element;
}

inline bool_t
operator< (
    const String& lhs,
    const_char_p rhs
) {
    return lhs.element < rhs;
}

inline bool_t
String::operator<= (
    const String & rhs
) {
    return element <= rhs.element;
}

inline bool_t
String::operator<= (
    const_char_p rhs
) {
    return element <= rhs;
}

inline bool_t
operator<= (
    const String& lhs,
    const String& rhs
) {
    return lhs.element <= rhs.element;
}

inline bool_t
operator<= (
    const_char_p lhs,
    const String& rhs
) {
    return lhs <= rhs.element;
}

inline bool_t operator<= (
    const String& lhs,
    const_char_p rhs
) {
    return lhs.element <= rhs;
}

inline bool_t
String::operator> (
    const String & rhs
) {
    return element > rhs.element;
}

inline bool_t
String::operator> (
    const_char_p rhs
) {
    return element > rhs;
}

inline bool_t operator> (
    const String& lhs,
    const String& rhs
) {
    return lhs.element > rhs.element;
}

inline bool_t operator> (
    const_char_p lhs,
    const String& rhs
) {
    return lhs > rhs.element;
}

inline bool_t operator> (
    const String& lhs,
    const_char_p rhs
) {
   return lhs.element > rhs;
}

inline bool_t
String::operator>= (
    const String & rhs
) {
    return element >= rhs.element;
}

inline bool_t
String::operator>=(
    const_char_p rhs
) {
    return element >= rhs;
}

inline bool_t
operator>= (
    const String& lhs,
    const String& rhs
) {
    return lhs.element >= rhs.element;
}

inline bool_t operator>= (
    const_char_p lhs,
    const String& rhs
) {
   return lhs >= rhs.element;
}

inline bool_t
operator>= (
    const String& lhs,
    const_char_p rhs
) {
   return lhs.element >= rhs;
}

inline String
String::remove_char(
    char ch
) {
    erase(std::remove(begin(), end(), ch), end());
    return *this;
}

#endif /* !STRING_HPP_ */
