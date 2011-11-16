// $Id: Constraint.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Constraint.h

#ifndef Constraint_H
#define Constraint_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif


#include "Cassowary.h"
#include "LinearExpression.h"
#include "Strength.h"
#include <set>
#include <string>
#include "my/refcnt.h"

class SimplexSolver;
class FDSolver;
class BlueSolver;

// enum setup so additive inverse flips the direction of the inequality
enum CnRelation {cnEQ = 0, cnNEQ = 100, cnLEQ = 2, cnGEQ = -2, cnLT = 3, cnGT = -3 };

inline enum CnRelation
ReverseInequality( enum CnRelation c)
{
  if ( c != cnNEQ)
    c = ( enum CnRelation) (- int( c));
  return c;
}

inline string
StrCnRelation( CnRelation rel) {
  switch ( rel) {
  case cnEQ: return "=";
  case cnNEQ: return "=/=";
  case cnLEQ: return "<=";
  case cnGEQ: return ">=";
  case cnLT: return "<";
  case cnGT: return ">";
  default: assert( false);
  }
}



#ifdef USE_GC_CONSTRAINT
class Constraint : public gc {
#else
class Constraint {
#endif
    REFCOUNT_DEF                 //from nref.h
public:

  Constraint( const Strength & strength = sRequired(), double weight = 1.0 );
  virtual ~Constraint();

  // Return (copyof) my linear Expression.
  // For linear equations, this constraint represents Expression=0;
  // for linear inequalities it represents Expression>=0.
  virtual LinearExpression Expression() const
    { assert( false); return 0; }

  // Returns true if this is an edit constraint
  virtual bool IsEditConstraint() const
    { return false; }

  // Return true if this is an inequality constraint and
  // false if it is an equality constraint.  The default is
  // that it is not.
  virtual bool IsInequality() const
    { return false; }

  virtual bool IsStrictInequality() const
    { return false; }

  virtual bool IsRequired() const
    { return _strength.IsRequired(); }

  virtual bool isStayConstraint() const
    { return false; }

  virtual const Strength & strength() const
    { return _strength; }

  virtual double weight() const
    { return _weight; }

#ifndef CL_NO_IO
  virtual ostream & PrintOn( ostream & xo) const = 0;
  friend ostream& operator<<( ostream & xos, const Constraint & constraint)
    { constraint.PrintOn( xos); return xos; }
#endif


#ifdef CL_PV
  void SetPv( void * pv) { _pv = pv; }
  void * Pv() const { return _pv; }
#endif

  virtual bool FIsSatisfied() const { return false; }
  virtual bool FIsInSolver() const { return _times_added != 0; }
  virtual bool FIsOkayForSimplexSolver() const { return true; }

  void ChangeStrength( const Strength & strength)
    {
      if ( _times_added == 0) {
        setStrength( strength);
      } else {
        throw ExCLTooDifficult();
      }
    }

  void ChangeWeight( double weight )
    {
      if ( _times_added == 0) {
        setWeight( weight);
      } else {
        throw ExCLTooDifficult();
      }
    }

  bool FIsReadOnlyVar( Variable v) const {
    return !( _readOnlyVars.find( v) == _readOnlyVars.end());
  }

  const VarSet & ReadOnlyVars() const {
    return _readOnlyVars;
  }

  Constraint & AddROVars( const VarSet & setv) {
    for ( VarSet::const_iterator it = setv.begin();
          it != setv.end(); ++it) {
      _readOnlyVars.insert(*it);
    }
    return * this;
  }

  friend class SimplexSolver;
  friend class FDSolver;
  friend class BlueSolver;
private:

  SymbolicWeight symbolicWeight() const {
    return _strength.symbolicWeight();
  }

  void addedTo( const SimplexSolver & )
    { ++_times_added; }

  void removedFrom( const SimplexSolver & )
    { --_times_added; }

  void setStrength( const Strength & strength )
    { _strength = strength; }

  void setWeight( double weight )
    { _weight = weight; }

  /// instance variables
  Strength _strength;

  VarSet _readOnlyVars;

  double _weight;

  void * _pv;

  int _times_added;
};

#include "Constraint_P.h"

#endif
