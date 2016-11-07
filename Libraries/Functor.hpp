/*******************************************************************************
*
* Copyright (c) 2016
* Lumi, JSC.
* All Rights Reserved
*
*
* Description:      Include file for application
*
* Author:
*
* Last Changed By:  TrungTQ
* Revision:         Revision: 1.0
* Last Changed:     Date: 2016-08-08 10:00:00 (Mon, 08 Aug 2016)
*
* Note:             
*
*******************************************************************************/
#ifndef FUNCTOR_HPP_
#define FUNCTOR_HPP_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stddef.h>
#include <string.h>
/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                          */
/******************************************************************************/
#define Funtor0_t                       CBFunctor0
#define Funtor0_p                       CBFunctor0*
#define Functor1_t(P1)                  CBFunctor1<P1>
#define Functor1_p(P1)                  CBFunctor1<P1>*
#define Functor2_t(P1, P2)              CBFunctor2<P1, P2>
#define Functor2_p(P1, P2)              CBFunctor2<P1, P2>*
#define Functor3_t(P1, P2, P3)          CBFunctor3<P1, P2, P3>
#define Functor3_p(P1, P2, P3)          CBFunctor3<P1, P2, P3>*
#define Functor1Ret_t(P1, RT)           CBFunctor1wRet<P1, RT>
#define Functor1Ret_p(P1, RT)           CBFunctor1wRet<P1, RT>*

class CBFunctorBase {
public:
    typedef void (CBFunctorBase::*_MemFunc)();
    typedef void(*_Func)();

    // Contructors
    // Default contructor
    CBFunctorBase() : callee(NULL), func(NULL) {}
    // Contructor with input parameters
    CBFunctorBase(const void* c, const void* f, size_t sz) {
        if (c != NULL) {
            callee = (void*) c;
            memcpy(memFunc, f, sz);
        } else {
            func = f;
        }
    }
    // Use to check Functor initialized or not
    operator int() const {
        return func || callee;
    }
    // Property of class
    class DummyInit { };
    void* callee;
    union {
        const void* func;
        char memFunc[sizeof(_MemFunc)];
    };
};

/******************************************************************************/
/*                        FUNCTOR: NO ARG, NO RETURN                          */
/******************************************************************************/
class CBFunctor0 : protected CBFunctorBase {
public:
    CBFunctor0(DummyInit* = NULL) { thunk = NULL; }
    void operator()() const {
        if (thunk != NULL) {
            thunk(*this);
        }
    }
    using CBFunctorBase::operator int;
protected:
    typedef void(*Thunk)(const CBFunctorBase&);
    CBFunctor0(Thunk t, const void *c, const void *f, size_t sz) :
        CBFunctorBase(c, f, sz), thunk(t) { }
private:
    Thunk thunk;
};

template<typename Callee, typename MemFunc>
class CBMemberTranslator0 : public CBFunctor0 {
public:
    CBMemberTranslator0(Callee &c, const MemFunc &m) :
        CBFunctor0(thunk, &c, &m, sizeof(MemFunc)) { }
    static void thunk(const CBFunctorBase &ftor) {
        Callee *callee = (Callee *) ftor.callee;
        MemFunc &memFunc(*(MemFunc*) (void *) (ftor.memFunc));
        (callee->*memFunc)();
    }
};

template<typename Func>
class CBFunctionTranslator0 : public CBFunctor0 {
public:
    CBFunctionTranslator0(Func f) : CBFunctor0(thunk, NULL, f, 0) { }
    static void thunk(const CBFunctorBase &ftor) {
        (Func(ftor.func))();
    }
};

/******************************************************************************/
/*                        FUNCTOR: ONE ARG, NO RETURN                         */
/******************************************************************************/
template<typename P1>
class CBFunctor1 : protected CBFunctorBase {
public:
    CBFunctor1(DummyInit* = NULL) { thunk = NULL; }
    void operator()(P1 p1) const {
        if (thunk != NULL) {
            thunk(*this, p1);
        }
    }
    using CBFunctorBase::operator int;
    typedef P1 argument_type;
    typedef void result_type;
protected:
    typedef void(*Thunk)(const CBFunctorBase&, P1);
    CBFunctor1(Thunk t, const void *c, const void *f, size_t sz) :
        CBFunctorBase(c, f, sz), thunk(t) { }
private:
    Thunk thunk;
};

template<typename P1, typename Callee, typename MemFunc>
class CBMemberTranslator1 : public CBFunctor1<P1> {
public:
    CBMemberTranslator1(Callee &c, const MemFunc &m) :
        CBFunctor1<P1>(thunk, &c, &m, sizeof(MemFunc)) { }
    static void thunk(const CBFunctorBase &ftor, P1 p1) {
        Callee *callee = (Callee *) ftor.callee;
        MemFunc &memFunc(*(MemFunc*) (void *) (ftor.memFunc));
        (callee->*memFunc)(p1);
    }
};

template<typename P1, typename Func>
class CBFunctionTranslator1 : public CBFunctor1<P1> {
public:
    CBFunctionTranslator1(Func f) : CBFunctor1<P1>(thunk, NULL, f, 0) { }
    static void thunk(const CBFunctorBase &ftor, P1 p1) {
        (Func(ftor.func))(p1);
    }
};

template<typename P1, typename MemFunc>
class CBMemberOf1stArgTranslator1 : public CBFunctor1<P1> {
public:
    CBMemberOf1stArgTranslator1(const MemFunc &m) :
        CBFunctor1<P1>(thunk, (void*) 1, &m, sizeof(MemFunc)) { }
    static void thunk(const CBFunctorBase &ftor, P1 p1) {
        MemFunc &memFunc(*(MemFunc*) (void *) (ftor.memFunc));
        (p1.*memFunc)();
    }
};

/******************************************************************************/
/*                        FUNCTOR: TWO ARG, NO RETURN                         */
/******************************************************************************/
template <typename P1, typename P2>
class CBFunctor2 : protected CBFunctorBase {
public:
    CBFunctor2(DummyInit * = NULL) { thunk = NULL; }
    void operator()(P1 p1, P2 p2)const {
        if (thunk != NULL) {
            thunk(*this, p1, p2);
        }
    }
    using CBFunctorBase::operator int;
    typedef P1 first_argument_type;
    typedef P2 second_argument_type;
    typedef void result_type;
protected:
    typedef void (*Thunk)(const CBFunctorBase &, P1, P2);
    CBFunctor2(Thunk t,const void *c, const void *f, size_t sz) :
        CBFunctorBase(c,f,sz),thunk(t) { }
private:
    Thunk thunk;
};

template <typename P1, typename P2, typename Callee, typename MemFunc>
class CBMemberTranslator2 : public CBFunctor2<P1, P2> {
public:
    CBMemberTranslator2(Callee &c, const MemFunc &m):
        CBFunctor2<P1, P2>(thunk, &c, &m, sizeof(MemFunc)) { }
    static void thunk(const CBFunctorBase &ftor, P1 p1, P2 p2) {
        Callee *callee = (Callee*) ftor.callee;
        MemFunc &memFunc(*(MemFunc*)(void *)(ftor.memFunc));
        (callee->*memFunc)(p1, p2);
    }
};

template <typename P1, typename P2, typename Func>
class CBFunctionTranslator2 : public CBFunctor2<P1, P2> {
public:
    CBFunctionTranslator2(Func f):CBFunctor2<P1, P2>(thunk, 0, f, 0) { }
    static void thunk(const CBFunctorBase &ftor, P1 p1, P2 p2) {
        (Func(ftor.func))(p1, p2);
    }
};

template <typename P1, typename P2, typename MemFunc>
class CBMemberOf1stArgTranslator2 : public CBFunctor2<P1, P2> {
public:
    CBMemberOf1stArgTranslator2(const MemFunc &m):
        CBFunctor2<P1, P2>(thunk, (void *)1, &m, sizeof(MemFunc)) { }
    static void thunk(const CBFunctorBase &ftor, P1 p1, P2 p2) {
        MemFunc &memFunc(*(MemFunc*)(void *)(ftor.memFunc));
        (p1.*memFunc)(p2);
    }
};

/******************************************************************************/
/*                      FUNCTOR: THREE ARG, NO RETURN                         */
/******************************************************************************/
template <typename P1, typename P2, typename P3>
class CBFunctor3 : protected CBFunctorBase {
public:
    CBFunctor3(DummyInit * = NULL) { thunk = NULL; }
    void operator()(P1 p1, P2 p2, P3 p3)const {
        if (thunk != NULL) {
            thunk(*this, p1, p2, p3);
        }
    }
    using CBFunctorBase::operator int;
    typedef P1 first_argument_type;
    typedef P2 second_argument_type;
    typedef P3 third_argument_type;
    typedef void result_type;
protected:
    typedef void (*Thunk)(const CBFunctorBase &, P1, P2, P3);
    CBFunctor3(Thunk t,const void *c, const void *f, size_t sz) :
        CBFunctorBase(c,f,sz),thunk(t) { }
private:
    Thunk thunk;
};

template <typename P1, typename P2, typename P3, typename Callee, typename MemFunc>
class CBMemberTranslator3 : public CBFunctor3<P1, P2, P3> {
public:
    CBMemberTranslator3(Callee &c, const MemFunc &m):
        CBFunctor3<P1, P2, P3>(thunk, &c, &m, sizeof(MemFunc)) { }
    static void thunk(const CBFunctorBase &ftor, P1 p1, P2 p2, P3 p3) {
        Callee *callee = (Callee*) ftor.callee;
        MemFunc &memFunc(*(MemFunc*)(void *)(ftor.memFunc));
        (callee->*memFunc)(p1, p2, p3);
    }
};

template <typename P1, typename P2, typename P3, typename Func>
class CBFunctionTranslator3 : public CBFunctor3<P1, P2, P3> {
public:
    CBFunctionTranslator3(Func f):CBFunctor3<P1, P2, P3>(thunk, 0, f, 0) { }
    static void thunk(const CBFunctorBase &ftor, P1 p1, P2 p2, P3 p3) {
        (Func(ftor.func))(p1, p2, p3);
    }
};

template <typename P1, typename P2, typename P3, typename MemFunc>
class CBMemberOf1stArgTranslator3 : public CBFunctor3<P1, P2, P3> {
public:
    CBMemberOf1stArgTranslator3(const MemFunc &m):
        CBFunctor3<P1, P2, P3>(thunk, (void *)1, &m, sizeof(MemFunc)) { }
    static void thunk(const CBFunctorBase &ftor, P1 p1, P2 p2, P3 p3) {
        MemFunc &memFunc(*(MemFunc*)(void *)(ftor.memFunc));
        (p1.*memFunc)(p2, p3);
    }
};

/******************************************************************************/
/*                        FUNCTOR: ONE ARG, ONE RETURN                        */
/******************************************************************************/
template <class P1, class RT>
class CBFunctor1wRet : protected CBFunctorBase{
public:
    CBFunctor1wRet(DummyInit* = NULL) { thunk = NULL; }
    RT operator()(P1 p1) const {
            return thunk(*this, p1);
    }
    using CBFunctorBase::operator int;
    //for STL
    typedef P1 argument_type;
    typedef RT result_type;
protected:
    typedef RT (*Thunk)(const CBFunctorBase &, P1);
    CBFunctor1wRet(Thunk t, const void *c, const void *f, size_t sz) :
        CBFunctorBase(c, f, sz), thunk(t) { }
private:
    Thunk thunk;
};

template <typename P1, typename RT, typename Callee, typename MemFunc>
class CBMemberTranslator1wRet : public CBFunctor1wRet<P1,RT> {
public:
    CBMemberTranslator1wRet(Callee &c,const MemFunc &m):
        CBFunctor1wRet<P1, RT>(thunk, &c, &m, sizeof(MemFunc)){}

    static RT thunk(const CBFunctorBase &ftor, P1 p1) {
        Callee *callee = (Callee *)ftor.callee;
        MemFunc &memFunc(*(MemFunc*)(void *)(ftor.memFunc));
        return (callee->*memFunc)(p1);
    }
};

template <typename P1, typename RT, typename Func>
class CBFunctionTranslator1wRet : public CBFunctor1wRet<P1, RT> {
public:
    CBFunctionTranslator1wRet(Func f):CBFunctor1wRet<P1, RT>(thunk, 0, (void*)f, 0){}
    static RT thunk(const CBFunctorBase &ftor, P1 p1) {
        return (Func(ftor.func))(p1);
    }
};

template <typename P1, typename RT, typename MemFunc>
class CBMemberOf1stArgTranslator1wRet : public CBFunctor1wRet<P1, RT> {
public:
    CBMemberOf1stArgTranslator1wRet(const MemFunc &m):
        CBFunctor1wRet<P1,RT>(thunk,(void *)1,&m,sizeof(MemFunc)){}
    static RT thunk(const CBFunctorBase &ftor, P1 p1) {
        MemFunc &memFunc(*(MemFunc*)(void *)(ftor.memFunc));
        return (p1.*memFunc)();
    }
};

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                             EXPORTED METHODS                               */
/******************************************************************************/

/******************************************************************************/
/*                        FUNCTOR: NO ARG, NO RETURN                          */
/******************************************************************************/
template<typename Callee, typename TRT, typename CallType>
inline CBMemberTranslator0<Callee, TRT (CallType::*)()>
makeFunctor(CBFunctor0*, Callee &c, TRT (CallType::* const &f)()) {
    typedef TRT (CallType::*MemFunc)();
    return CBMemberTranslator0<Callee, MemFunc>(c, f);
}

template<typename Callee, typename TRT, typename CallType>
inline CBMemberTranslator0<const Callee, TRT (CallType::*)() const>
makeFunctor(CBFunctor0*, const Callee &c, TRT (CallType::* const &f)() const) {
    typedef TRT (CallType::*MemFunc)() const;
    return CBMemberTranslator0<const Callee, MemFunc>(c, f);
}

template<typename TRT>
inline CBFunctionTranslator0<TRT(*)()>
makeFunctor(CBFunctor0*, TRT (*f)()) {
    return CBFunctionTranslator0<TRT(*)()>(f);
}

/******************************************************************************/
/*                        FUNCTOR: ONE ARG, NO RETURN                         */
/******************************************************************************/
template<typename P1, typename Callee, typename TRT, typename CallType, typename TP1>
inline CBMemberTranslator1<P1, Callee, TRT (CallType::*)(TP1)>
makeFunctor(CBFunctor1<P1>*, Callee &c, TRT (CallType::* const &f)(TP1)) {
    typedef TRT (CallType::*MemFunc)(TP1);
    return CBMemberTranslator1<P1,Callee, MemFunc>(c, f);
}

template<typename P1, typename Callee, typename TRT, typename CallType, typename TP1>
inline CBMemberTranslator1<P1, const Callee, TRT (CallType::*)(TP1) const>
makeFunctor(CBFunctor1<P1>*, const Callee &c, TRT (CallType::* const &f)(TP1) const) {
    typedef TRT (CallType::*MemFunc)(TP1) const;
    return CBMemberTranslator1<P1, const Callee, MemFunc>(c, f);
}

template<typename P1, typename TRT, typename TP1>
inline CBFunctionTranslator1<P1, TRT (*)(TP1)>
makeFunctor(CBFunctor1<P1>*, TRT (*f)(TP1)) {
    return CBFunctionTranslator1<P1, TRT (*)(TP1)>(f);
}

template <typename P1, typename TRT, typename CallType>
inline CBMemberOf1stArgTranslator1<P1, TRT (CallType::*)()>
makeFunctor(CBFunctor1<P1>*, TRT (CallType::* const &f)()) {
    typedef TRT (CallType::*MemFunc)();
    return CBMemberOf1stArgTranslator1<P1, MemFunc>(f);
}

template <typename P1, typename TRT, typename CallType>
inline CBMemberOf1stArgTranslator1<P1, TRT (CallType::*)() const>
makeFunctor(CBFunctor1<P1>*, TRT (CallType::* const &f)() const) {
    typedef TRT (CallType::*MemFunc)() const;
    return CBMemberOf1stArgTranslator1<P1, MemFunc>(f);
}

/******************************************************************************/
/*                        FUNCTOR: TWO ARG, NO RETURN                         */
/******************************************************************************/
template <typename P1, typename P2, typename Callee,
          typename TRT, typename CallType, typename TP1, typename TP2>
inline CBMemberTranslator2<P1, P2, Callee, TRT (CallType::*)(TP1, TP2)>
makeFunctor(CBFunctor2<P1, P2>*, Callee &c, TRT (CallType::* const &f)(TP1, TP2)) {
    typedef TRT (CallType::*MemFunc)(TP1, TP2);
    return CBMemberTranslator2<P1, P2, Callee, MemFunc>(c, f);
}

template <typename P1, typename P2, typename Callee,
          typename TRT, typename CallType, typename TP1, typename TP2>
inline CBMemberTranslator2<P1, P2, const Callee, TRT (CallType::*)(TP1, TP2) const>
makeFunctor(CBFunctor2<P1, P2>*, const Callee &c, TRT (CallType::* const &f)(TP1, TP2) const) {
    typedef TRT (CallType::*MemFunc)(TP1, TP2) const;
    return CBMemberTranslator2<P1, P2, const Callee, MemFunc>(c, f);
}

template <typename P1, typename P2, typename TRT, typename TP1, typename TP2>
inline CBFunctionTranslator2<P1, P2, TRT (*)(TP1, TP2)>
makeFunctor(CBFunctor2<P1, P2>*, TRT (*f)(TP1, TP2)) {
    return CBFunctionTranslator2<P1, P2, TRT (*)(TP1, TP2)>(f);
}

template <typename P1, typename P2, typename TRT, typename CallType, typename TP1>
inline CBMemberOf1stArgTranslator2<P1, P2, TRT (CallType::*)(TP1)>
makeFunctor(CBFunctor2<P1, P2>*, TRT (CallType::* const &f)(TP1)) {
    typedef TRT (CallType::*MemFunc)(TP1);
    return CBMemberOf1stArgTranslator2<P1, P2, MemFunc>(f);
}

template <typename P1, typename P2, typename TRT, typename CallType, typename TP1>
inline CBMemberOf1stArgTranslator2<P1, P2, TRT (CallType::*)(TP1) const>
makeFunctor(CBFunctor2<P1, P2>*, TRT (CallType::* const &f)(TP1) const) {
    typedef TRT (CallType::*MemFunc)(TP1)const;
    return CBMemberOf1stArgTranslator2<P1, P2, MemFunc>(f);
}


/******************************************************************************/
/*                      FUNCTOR: THREE ARG, NO RETURN                         */
/******************************************************************************/
template <typename P1, typename P2, typename P3, typename Callee,
          typename TRT, typename CallType, typename TP1, typename TP2, typename TP3>
inline CBMemberTranslator3<P1, P2, P3, Callee, TRT (CallType::*)(TP1, TP2, TP3)>
makeFunctor(CBFunctor3<P1, P2, P3>*, Callee &c, TRT (CallType::* const &f)(TP1, TP2, TP3)) {
    typedef TRT (CallType::*MemFunc)(TP1, TP2, TP3);
    return CBMemberTranslator3<P1, P2, P3, Callee, MemFunc>(c, f);
}

template <typename P1, typename P2, typename P3, typename Callee,
          typename TRT, typename CallType, typename TP1, typename TP2, typename TP3>
inline CBMemberTranslator3<P1, P2, P3, const Callee, TRT (CallType::*)(TP1, TP2, TP3) const>
makeFunctor(CBFunctor3<P1, P2, P3>*, const Callee &c, TRT (CallType::* const &f)(TP1, TP2, TP3) const) {
    typedef TRT (CallType::*MemFunc)(TP1, TP2, TP3) const;
    return CBMemberTranslator3<P1, P2, P3, const Callee, MemFunc>(c, f);
}

template <typename P1, typename P2, typename P3, typename TRT, typename TP1, typename TP2, typename TP3>
inline CBFunctionTranslator3<P1, P2, P3, TRT (*)(TP1, TP2, TP3)>
makeFunctor(CBFunctor3<P1, P2, P3>*, TRT (*f)(TP1, TP2, TP3)) {
    return CBFunctionTranslator3<P1, P2, P3, TRT (*)(TP1, TP2, TP3)>(f);
}

template <typename P1, typename P2, typename P3, typename TRT, typename CallType, typename TP1, typename TP2>
inline CBMemberOf1stArgTranslator3<P1, P2, P3, TRT (CallType::*)(TP1, TP2)>
makeFunctor(CBFunctor3<P1, P2, P3>*, TRT (CallType::* const &f)(TP1)) {
    typedef TRT (CallType::*MemFunc)(TP1);
    return CBMemberOf1stArgTranslator3<P1, P2, P3, MemFunc>(f);
}

template <typename P1, typename P2, typename P3, typename TRT, typename CallType, typename TP1, typename TP2>
inline CBMemberOf1stArgTranslator3<P1, P2, P3, TRT (CallType::*)(TP1, TP2) const>
makeFunctor(CBFunctor3<P1, P2, P3>*, TRT (CallType::* const &f)(TP1, TP2) const) {
    typedef TRT (CallType::*MemFunc)(TP1, TP2) const;
    return CBMemberOf1stArgTranslator3<P1, P2, P3, MemFunc>(f);
}

/******************************************************************************/
/*                        FUNCTOR: ONE ARG, ONE RETURN                        */
/******************************************************************************/
template <typename P1, typename RT,
          typename Callee, typename TRT, typename CallType, typename TP1>
inline CBMemberTranslator1wRet<P1, RT, Callee, TRT (CallType::*)(TP1)>
makeFunctor(CBFunctor1wRet<P1, RT>*, Callee &c, TRT (CallType::* const &f)(TP1)) {
    typedef TRT (CallType::*MemFunc)(TP1);
    return CBMemberTranslator1wRet<P1, RT, Callee, MemFunc>(c, f);
}

template <typename P1, typename RT,
          typename Callee, typename TRT, typename CallType, typename TP1>
inline CBMemberTranslator1wRet<P1, RT, const Callee, TRT (CallType::*)(TP1) const>
makeFunctor(CBFunctor1wRet<P1, RT>*, const Callee &c, TRT (CallType::* const &f)(TP1)const) {
    typedef TRT (CallType::*MemFunc)(TP1)const;
    return CBMemberTranslator1wRet<P1, RT, const Callee, MemFunc>(c, f);
}

template <typename P1, typename RT, typename TRT, typename TP1>
inline CBFunctionTranslator1wRet<P1, RT, TRT (*)(TP1)>
makeFunctor(CBFunctor1wRet<P1, RT>*, TRT (*f)(TP1)) {
    return CBFunctionTranslator1wRet<P1, RT, TRT (*)(TP1)>(f);
}

template <typename P1, typename RT, typename TRT, typename CallType>
inline CBMemberOf1stArgTranslator1wRet<P1, RT, TRT (CallType::*)()>
makeFunctor(CBFunctor1wRet<P1, RT>*, TRT (CallType::* const &f)()) {
    typedef TRT (CallType::*MemFunc)();
    return CBMemberOf1stArgTranslator1wRet<P1, RT, MemFunc>(f);
}

template <typename P1, typename RT, typename TRT, typename CallType>
inline CBMemberOf1stArgTranslator1wRet<P1, RT, TRT (CallType::*)() const>
makeFunctor(CBFunctor1wRet<P1, RT>*, TRT (CallType::* const &f)() const) {
    typedef TRT (CallType::*MemFunc)() const;
    return CBMemberOf1stArgTranslator1wRet<P1, RT, MemFunc>(f);
}

#endif /* !FUNCTOR_HPP_ */
