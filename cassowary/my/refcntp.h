// s.dobrev - may 2k1
#ifndef REF_CNT_PTR_H
#define REF_CNT_PTR_H

 // pointer to a reference counted object - it must have inc()/dec()/n() funcs
 // BUT:
 //  - no const ptr (RefCountPtrConst/RefCountPtr may be _eventualy_ usable)
 //  - never delete objs yourself / automaticaly
 //  - use RefCountPtr_static_holder for non-newed objects

/* expected hypotetical interface about class Type:
  ::incref( Type * p);           e.g. { p->incref(); }
  ::decref( Type * p, int del);  e.g. { p->decref(); if (del&&!p->nref()) delete p; }

example usage:
...
///your MyType.h
#include "refcnt.h"       //see for example refcounter
class MyType { ...
    REFCOUNT_DEF                 //from nref.h
...
}; ...

///your refCntPtrMyType_user.h:
class MyType;                   //no need to be known
#include "refcntp.h"
REFCOUNT_DECL( MyType)          //from refcntp.h
typedef RefCountPtr< MyType> P_MyType;
...
///your MyType.cpp or  refCntPtrMyType_user.cpp
#include "MyType.h"
#include "refcnt.h"
REFCOUNT_INST( MyType )         //from refcnt.h
...
*/

    //put at global scope, external, BEFORE using templates below
#define REFCOUNT_DECL( Type)    void incref( Type * p), decref( Type * p, int del);

#ifndef NEED_RefCountPtrConst
#define RefCountPtrConst RefCountPtr
#endif
/*
 * the external incref/decref' calls below work for newer c++ (e.g. gcc 4.x) if either
 * used as ::incref() AND the incref()s are templated
 * or used without ::. THIS one is chosen here, hence templates below are commented.
 *
template< class Type> void incref( Type * p);
template< class Type> void decref( Type * p, int del);
these need template<> ...func... at definition (refcnt.h/REFCOUNT_INST)
*/
template< class Type>
class RefCountPtrConst {
protected:
    Type * _ptr;
private:
 static void inc( Type * p) {
        void incref( Type * p);
        incref(p); }
    void inc() const {
        void incref( Type * p);
        if (_ptr) incref( _ptr); }
    void dec() const {
        void decref( Type * p, int del);
        if (_ptr) decref( _ptr, 1); }
public:
    RefCountPtrConst()                          { _ptr= 0; }
    RefCountPtrConst( Type * p )                { _ptr= p; inc(); }
    RefCountPtrConst( const RefCountPtrConst&s) { s.inc(); _ptr= s._ptr; }
   ~RefCountPtrConst()                          { dec(); }
        //careful with x=x
    const RefCountPtrConst & operator = ( Type * p)                     { if (p) inc(p); dec(); _ptr= p; return *this; }
    const RefCountPtrConst & operator = ( const RefCountPtrConst &s)    { s.inc(); dec();  _ptr= s._ptr; return *this; }

    int operator == (const RefCountPtrConst &s) const   { return _ptr == s._ptr; }
    int operator != (const RefCountPtrConst &s) const   { return _ptr != s._ptr; }
    int operator == (const Type * p) const              { return _ptr == p; }
    int operator != (const Type * p) const              { return _ptr != p; }

#ifdef NEED_RefCountPtrConst
    const Type * ptr()         const    { return _ptr; }
    const Type * operator-> () const    { return _ptr; }
    operator  const  Type * () const    { return _ptr; }
    operator  const  Type & () const    { return *_ptr; }
};

template< class Type>
class RefCountPtr : public RefCountPtrConst<Type> {
public:
    RefCountPtr() {}
    RefCountPtr( Type * p ) : RefCountPtrConst(p) {}
    RefCountPtr( const RefCountPtr & s) : RefCountPtrConst(s) {}
#endif
    Type * ptr()         const          { return _ptr; }
    Type * operator-> () const          { return ptr(); }
    operator   Type * () const          { return ptr(); }
    operator   Type & () const          { return *ptr(); }
};

    //for object with larger lifetime than refcounting pointers to it:
    //this keeps it alive and does not kill it when dying
    //so obj_lifetime > this_lifespan > lifespan_of_refcounting
    //e.g. for static obj instances, put a static instance of this just after it
template< class Type>
class RefCountPtr_static_holder : public RefCountPtrConst<Type> {
public:
    RefCountPtr_static_holder( Type * p =0 ) : RefCountPtrConst<Type>(p) {}
   ~RefCountPtr_static_holder() {
        void decref( Type * p, int del);
        if (this->_ptr) decref( this->_ptr,0);
        this->_ptr=0;
   }
};

template< class Type, class superType>
class RefCountPtr_Subclass : public RefCountPtr<Type> {
public:
    RefCountPtr_Subclass()                          {}
    RefCountPtr_Subclass( Type * p ) : RefCountPtr<Type> (p) {}
/*    use defaults
    //RefCountPtr_Subclass( const RefCountPtr_Subclass&s)
        //careful with x=x
    const RefCountPtr_Subclass & operator = ( Type * p) {
        return RefCountPtr<superType>::operator =(p); }
    const RefCountPtr_Subclass & operator = ( const RefCountPtr_Subclass &s)
        return RefCountPtr<superType>::operator =(s); }
*/
    int operator == (const Type * p) const          { return this->_ptr == p; }
    int operator != (const Type * p) const          { return this->_ptr != p; }
    int operator == (const superType * p) const     { return this->_ptr == p; }
    int operator != (const superType * p) const     { return this->_ptr != p; }

    operator RefCountPtr<superType> () const { return RefCountPtr<superType>( this->_ptr ); }
};

/** allow overloading of these -- no, won't work (const RefCountPtrConst &..)
template< class Type>
 void inc( Type * p)             { ::incref(p); }
template< class Type>
 void inc( RefCountPtrConst<Type> & self ) const  { if (self._ptr) ::incref( self._ptr); }
template< class Type>
 void dec( RefCountPtrConst<Type> & self ) const  { if (self._ptr) ::decref( self._ptr, 1); }
**/

#endif  //REF_CNT_PTR_H
