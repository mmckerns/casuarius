// s.dobrev - may 2k1
#ifndef REF_CNT_H
#define REF_CNT_H

 // reference counter - to go with RefCountPtr<>
 // inherit as public or make member and export inc/dec/n()
class RefCount {
    int _n;
public:    void ctor()          { _n=0; }       //use instead of ctor!
public:
    RefCount()                  { _n=0; }
    RefCount( const RefCount &) { _n=0; }
    void operator = (const RefCount &)  {}      //nothing!
// ~RefCount()                  {}              // assert/message if still used
    int  nref() const           { return _n; }
    void incref()               { _n++; }
    void decref()               { _n--; }
};
        //put inside your class
#define REFCOUNT_DEF    RefCount _refcnt;       \
public: void incref()       { _refcnt.incref(); }  \
        void decref()       { _refcnt.decref(); }  \
        int  nref() const   { return _refcnt.nref(); }

        //put outside - global scope, external
#define REFCOUNT_INST(Type)     \
void incref( Type * p)  { p->incref(); }  \
void decref( Type * p, int del)  { p->decref(); if (del && !p->nref()) delete p; }
//use template<> ...func... if func is declared as template in refcntp.h

#ifdef TRACE_REFCOUNT_DIE
# define REFCOUNT_DIE(Type)  printf(" DIE "#Type" %p: %d refs\n", this,nref());
# define REFCOUNT_DIE0(Type) if (nref()) REFCOUNT_DIE(Type);
#else
# define REFCOUNT_DIE(Type)
# define REFCOUNT_DIE0(Type) if (0) {}
#endif

#endif  //REF_CNT_H
