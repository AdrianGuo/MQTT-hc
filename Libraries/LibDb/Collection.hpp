#ifndef COLLECTION_HPP_
#define COLLECTION_HPP_

#include <algorithm>
#include "Vector.hpp"
#include "SqlValueImpl.hpp"
#include "SqlStatement.hpp"
#include "DbContext.hpp"

template<class C>
class Collection {
private:
    int_t m_iSize;
    DbContext_p m_pDbContext;
    SqlStatement_p m_pSqlStatement;
    SqlStatement_p m_pSqlCountStatement;
    SqlStatement_p ExecuteStatement() const;
public:
    typedef C                   value_type;
    typedef value_type          key_type;
    typedef const value_type    const_value_type;
    typedef std::size_t         size_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;

    Collection(DbContext_p pDbContext = NULL,
               SqlStatement_p pSqlStatement = NULL,
               SqlStatement_p pSqlCountStatement = NULL);

    Collection(const Collection<C>& other);
    Collection& operator= (const Collection<C>& other);

    virtual ~Collection() {}

    class iterator : public std::iterator<std::input_iterator_tag, C> {
    public:
        iterator();
        iterator(const iterator& other);
        iterator(const Collection<C>& collection, SqlStatement_p pSqlStatement);

        struct core_impl {
            const Collection<C> m_collection;
            SqlStatement_p m_pSqlStatement;
            value_type m_current;
            int_t  m_idwCount;
            bool_t m_boIsLast;

            core_impl(
                const Collection<C>& collection,
                SqlStatement_p pSqlStatement
            ) : m_collection (collection),
                m_pSqlStatement (pSqlStatement),
                m_idwCount (0),
                m_boIsLast (FALSE) {
                m_pSqlStatement->reset();
                SetCurrent();
            }

            ~core_impl() {}

            value_type& Current() { return m_current; }
            void_t SetCurrent();
        };


        ~iterator();

        iterator& operator= (const iterator& other);

        C& operator* ();
        C* operator->();

        bool_t operator== (const iterator& other) const;
        bool_t operator!= (const iterator& other) const;

        iterator& operator++ ();
        iterator& operator++ (int_t);

        core_impl* m_pImpl;
    };

    class const_iterator : public std::iterator<std::input_iterator_tag, C> {
    public:
        typename Collection<C>::iterator m_impl;

        const_iterator();
        const_iterator(const Collection<C>& collection, SqlStatement_p pSqlStatement);
        const_iterator(const const_iterator& other);
        const_iterator(const typename Collection<C>::iterator& other);

        ~const_iterator() {}

        const_iterator& operator= (const const_iterator& other);
        const_iterator& operator= (const iterator& other);

        const C* operator->();
        C operator*();

        bool_t operator== (const const_iterator& other) const;
        bool_t operator!= (const const_iterator& other) const;

        const_iterator operator++ ();
        const_iterator operator++ (int_t);
    };

    void_t insert(C c);
    void_t erase(C c);
    bool_t empty() const;
    void_t clear();
    int_t size() const;
    int_t resize();

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    DbContext_p GetContext() const;
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline Collection<C>::Collection(
    DbContext_p pDbContext,
    SqlStatement_p pSqlStatement,
    SqlStatement_p pSqlCountStatement
) : m_pDbContext (pDbContext),
    m_pSqlStatement (pSqlStatement),
    m_pSqlCountStatement (pSqlCountStatement) {
    int_t iSize = 0;
    m_iSize = -1;

    if (m_pSqlCountStatement != NULL) {
        m_pSqlCountStatement->reset();
        m_pSqlCountStatement->execute();
        m_pSqlCountStatement->get(0, &iSize);
        m_iSize = iSize;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::Collection(
    const Collection<C>& other
) : m_iSize (other.m_iSize),
    m_pDbContext (other.m_pDbContext),
    m_pSqlStatement (other.m_pSqlStatement),
    m_pSqlCountStatement (other.m_pSqlCountStatement) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline Collection<C>&
Collection<C>::operator= (
    const Collection<C>& other
) {
    m_iSize = other.m_iSize;
    m_pDbContext = other.m_pDbContext;
    m_pSqlStatement = other.m_pSqlStatement;
    m_pSqlCountStatement = other.m_pSqlCountStatement;

    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline SqlStatement_p
Collection<C>::ExecuteStatement() const {
    return m_pSqlStatement;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
Collection<C>::insert(C c) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
Collection<C>::erase(C c) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
Collection<C>::empty() const {
    return size() == 0;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
Collection<C>::clear() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline int_t
Collection<C>::size() const {
    if (m_iSize != -1) { return m_iSize; }

    return 0;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline int_t
Collection<C>::resize() {
    int_t iCount = 0;

    if (m_pSqlCountStatement != NULL) {
        m_pSqlCountStatement->reset();
        m_pSqlCountStatement->execute();
        m_pSqlCountStatement->get(0, &iCount);
        m_iSize = iCount;
    }

    return iCount;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::iterator
Collection<C>::begin() {
    return iterator(*this, ExecuteStatement());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::iterator
Collection<C>::end() {
    return iterator();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::const_iterator
Collection<C>::begin() const {
    return const_iterator(*this, ExecuteStatement());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::const_iterator
Collection<C>::end() const {
    return const_iterator();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbContext_p
Collection<C>::GetContext() const {
    return m_pDbContext;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::const_iterator::const_iterator() : m_impl() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::const_iterator::const_iterator(
    const Collection<C>& collection,
    SqlStatement_p pSqlStatement
) : m_impl (collection, pSqlStatement) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::const_iterator::const_iterator(
    const const_iterator& other
) : m_impl (other.m_impl) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::const_iterator::const_iterator(
    const typename Collection<C>::iterator& other
) : m_impl (other) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::const_iterator&
Collection<C>::const_iterator::operator= (
    const const_iterator& other
) {
    m_impl = other.m_impl;

    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::const_iterator&
Collection<C>::const_iterator::operator= (
    const iterator& other
) {
    m_impl = other;

    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
Collection<C>::const_iterator::operator== (
    const const_iterator& other
) const {
    return m_impl == other.m_impl;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
Collection<C>::const_iterator::operator!= (
    const const_iterator& other
) const {
    return m_impl != other.m_impl;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::const_iterator
Collection<C>::const_iterator::operator++ () {
    ++m_impl;

    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::const_iterator
Collection<C>::const_iterator::operator++(int_t) {
    m_impl++;

    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
const C*
Collection<C>::const_iterator::operator->() {
    return m_impl.operator ->();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline C
Collection<C>::const_iterator::operator*() {
    return m_impl.operator *();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::iterator::iterator() : m_pImpl (NULL) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::iterator::iterator(
    const Collection<C>& collection,
    SqlStatement_p pSqlStatement
) {
    m_pImpl = new core_impl(collection, pSqlStatement);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::iterator::iterator(
    const iterator& other
) : m_pImpl (other.m_pImpl) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::iterator::~iterator() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::iterator&
Collection<C>::iterator::operator= (
    const iterator& other
) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline C&
Collection<C>::iterator::operator* () {
    if (m_pImpl != NULL) {
        return m_pImpl->Current();
    }
    return *(new C());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline C*
Collection<C>::iterator::operator->() {
    if (m_pImpl != NULL) {
        return &m_pImpl->Current();
    }
    return new C();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
Collection<C>::iterator::operator== (
    const iterator& other
) const {

    if (m_pImpl == other.m_pImpl) { return TRUE; }

    if ((m_pImpl->m_boIsLast) && (other.m_pImpl == NULL)) { return TRUE; }

    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
Collection<C>::iterator::operator!= (
    const iterator& other
) const {
    return !(*this == other);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::iterator&
Collection<C>::iterator::operator++ () {
    if ((m_pImpl != NULL) && !m_pImpl->m_boIsLast) { m_pImpl->SetCurrent(); }

    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::iterator&
Collection<C>::iterator::operator++ (int_t) {
    if ((m_pImpl != NULL) && !m_pImpl->m_boIsLast) { m_pImpl->SetCurrent(); }

    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
Collection<C>::iterator::core_impl::SetCurrent() {
    if (m_idwCount == m_collection.size()) { m_boIsLast = TRUE; }

    if (m_boIsLast) { return ; }

    DbContext_p pDbContext = m_collection.GetContext();

    if (m_idwCount < m_collection.size()) {
        int_t iColumn = 0;
        m_current = SqlValue<C>::Load(pDbContext, m_pSqlStatement, iColumn);
        m_idwCount++;
    }
}

#endif /* !COLLECTION_HPP_ */
