// $Id: Variable.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Variable.h
// A handle on AbstractVariable-s

#ifndef Variable_H
#define Variable_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include <stdio.h>
#include <map>
#include <string>
#include "Cassowary.h"
#include "FloatVariable.h"
#include "FDVariable.h"

class Variable;
typedef map<const string,Variable> StringToVarMap;

class Variable {
  P_AbstractVariable pclv;
public:
  // converters from raw AbstractVariable
  Variable( P_AbstractVariable pclv_) : pclv( pclv_) { }
  Variable( AbstractVariable * pclv_) : pclv( pclv_) { }
  Variable( AbstractVariable & clv_) : pclv(&clv_) { }

  // Copy ctr
  Variable( const Variable & clv_) : pclv( clv_.pclv) { }

  /// These ctrs build FloatVariable-s
  Variable( string name, Number Value = 0.0) 
      : pclv( new FloatVariable( name,Value)) 
    { if ( pmapStrPclv) { (*pmapStrPclv)[name] = * this; }  }
  Variable( Number Value = 0.0) 
      : pclv( new FloatVariable( Value)) { }
  Variable( long number, char * prefix, Number Value = 0.0)
      : pclv( new FloatVariable( number,prefix,Value)) { }

  // This one builds a FDVariable
  Variable( FDVariable * pcfv)
      : pclv( pcfv) 
    { if ( pmapStrPclv) { (*pmapStrPclv)[pcfv->Name()] = * this; } }

  /// permit Variables to be used as pointers to pclvs
  AbstractVariable * operator->() { return pclv.ptr(); }
  const AbstractVariable * operator->() const { return pclv.ptr(); }

  /// and also forward the function calls along
  bool IsFloatVariable() const { assert( pclv); return pclv->IsFloatVariable(); }
  bool IsFDVariable() const { assert( pclv); return pclv->IsFDVariable(); }
  bool IsDummy() const { assert( pclv); return pclv->IsDummy(); }
  bool IsExternal() const { assert( pclv); return pclv->IsExternal(); }
  bool IsPivotable() const { assert( pclv); return pclv->IsPivotable(); }
  bool IsRestricted() const { assert( pclv); return pclv->IsRestricted(); }

  string Name() const { assert( pclv); return pclv->Name(); }

  Number Value() const { assert( pclv); return pclv->Value(); }
  int IntValue() const { assert( pclv); return pclv->IntValue(); }
  void SetValue( Number Value) 
    { assert( pclv); pclv->SetValue( Value); }
  void ChangeValue( Number Value) 
    { assert( pclv); pclv->ChangeValue( Value); }
#ifdef CL_PV
  void SetPv( void * pv) { assert( pclv); pclv->SetPv( pv); }
  void * Pv() const { assert( pclv); return pclv->Pv(); }
#endif
  void SetName( const string & nm) {
    assert( pclv);
    if ( pmapStrPclv) {
      pmapStrPclv->erase( Name());
      (*pmapStrPclv)[nm] = *this;
    }
    pclv->SetName( nm);
  }

  AbstractVariable * get_pclv() const { return pclv.ptr(); } 
  bool IsNil() const { return pclv == NULL; }
/*
  //virtual 
  FDNumber DesiredValue() const { assert( false); return 0; } 
  //virtual 
  list<FDNumber> * PlfdnDomain() { assert( false); return NULL; }
*/
  
  static void SetVarMap( StringToVarMap * pmap) { pmapStrPclv = pmap; }
  static StringToVarMap * VarMap() { return pmapStrPclv; }
  static StringToVarMap * pmapStrPclv;
#ifndef CL_NO_IO
  ostream & PrintOn( ostream & xo) const
    { 
      if ( pclv) return pclv->PrintOn( xo); /* return xo << "@" << pclv << endl; */
      return xo << "clvNil";
    }
#endif

  friend bool operator<( const Variable & cl1, const Variable & cl2)
    { return cl1.pclv.ptr() < cl2.pclv.ptr(); }

  bool operator == ( const Variable & cl2) const { return pclv == cl2.pclv; } 
  bool operator != ( const Variable & cl2) const { return pclv != cl2.pclv; }
};

#ifndef CL_NO_IO
inline ostream & operator<<( ostream & xo, const Variable & clv)
{ return clv.PrintOn( xo); }
#endif

#ifdef CL_USE_HASH_MAP_AND_SET
struct hash<Variable> { 
  size_t operator()( const Variable & v) const
    { return size_t(( unsigned long)v.get_pclv()/CL_PTR_HASH_DIVISOR);  }
};
#endif


#include <math.h>

// Compare two double-s approximately, since equality is no good
inline bool Approx( double a, double b)
{
  const double epsilon = 1.0e-8;
  if ( a > b) {
    return ( a - b) < epsilon;
  } else {
    return ( b - a) < epsilon;
  }
}

// Can remove these if I decide to 
// autoconvert from Variable-s to double-s
inline bool Approx( const Variable & clv, double b)
{
  return Approx( clv->Value(),b);
}

inline bool Approx( double a, const Variable & clv)
{
  return Approx( a,clv->Value());
}

extern Variable clvNil;

#endif
