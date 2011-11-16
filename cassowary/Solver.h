// $Id: Solver.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Solver.h

#ifndef Solver_H
#define Solver_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "Errors.h"
#include "Typedefs.h"
#include <list>

class Variable;

// Solver is an abstract base class
class Solver {
 public:

  Solver() : _pv( 0), _fAutosolve( true), _pfnChangevCallback( 0) { }

  virtual ~Solver()
    { } 

  // Add the constraint cn to the solver
  virtual Solver & AddConstraint( P_Constraint ) = 0;

  // Remove the constraint cn from the solver
  virtual Solver & RemoveConstraint( P_Constraint ) = 0;

  // Same as above, but returns false if the constraint cannot be solved
  // ( i.e., the resulting system would be unsatisfiable)
  // The above function "AddConstraint" throws an exception in that case
  // which may be inconvenient
  bool AddConstraintNoException( P_Constraint pcn)
    {
      try {
          AddConstraint( pcn);
          return true;
      }
      catch ( const ExCLRequiredFailure & e)
        { return false; }
      catch ( const ExCLTooDifficult & e)
        { return false; }
    }

#ifdef CL_NO_DEPRECATED
  // Deprecated --02/22/99 gjb
  bool AddConstraintNoException( Constraint & cn)
    { return AddConstraintNoException(&cn); }
#endif

  bool RemoveConstraintNoException( P_Constraint pcn)
    {
      try {
        RemoveConstraint( pcn);
        return true;
      }
      catch ( const ExCLConstraintNotFound & e)
        { return false; }
    }

#ifdef CL_NO_DEPRECATED
  // Deprecated --02/22/99 gjb
  bool RemoveConstraintNoException( Constraint & cn)
    { return RemoveConstraintNoException(&cn); }
#endif


  virtual Solver & Solve()
    { assert( false); return * this; }

  bool SolveNoException()
    {
      try {
        Solve();
        return true;
      }
      catch ( const ExCLTooDifficult & e)
        { return false; }
      catch ( const ExCLRequiredFailure & e)
        { return false; }
    }


  virtual void Resolve()
    { assert( false); }

#ifdef CL_PV
  void SetPv( void * pv) { _pv = pv; } 
  void * Pv() const { return _pv; }
#endif
  
  typedef void (*PfnChangevCallback)( Variable * pclv, Solver * psolver);

  void SetChangevCallback( PfnChangevCallback pfn)
    { _pfnChangevCallback = pfn; }

  // Control whether optimization and setting of external variables
  // is done automatically or not.  By default it is done
  // automatically and solve() never needs to be explicitly
  // called by client code; if SetAutosolve is put to false,
  // then solve() needs to be invoked explicitly before using
  // variables' values
  // ( Turning off autosolve while adding lots and lots of
  // constraints [ala the addDel test in Tests] saved
  // about 20% in runtime, from 68sec to 54sec for 900 constraints,
  // with 126 failed adds)
  Solver & SetAutosolve( bool f)
    { _fAutosolve = f; if ( f) Solve(); return * this; }

  // Tell whether we are autosolving
  bool FIsAutosolving() const
    { return _fAutosolve; }


#ifndef CL_NO_IO
  friend ostream & operator<<( ostream & xo, const Solver & solver);

  virtual ostream & PrintOn( ostream & xo) const = 0;

#endif  

 protected:

  // C-style extension mechanism so I
  // don't have to wrap ScwmSolver separately
  void * _pv;

  bool _fAutosolve;

  PfnChangevCallback _pfnChangevCallback;
};


#ifndef CL_NO_IO
ostream & PrintTo( ostream & xo, const VarVector & varlist);
ostream & operator<<( ostream & xo, const VarVector & varlist);

ostream & PrintTo( ostream & xo, const ConstraintToVarSetMap & mapCnToVarSet);
ostream & operator<<( ostream & xo, const ConstraintToVarSetMap & mapCnToVarSet);

ostream & PrintTo( ostream & xo, const ConstraintSet & setCn);
ostream & operator<<( ostream & xo, const ConstraintSet & setCn);

ostream & PrintTo( ostream & xo, const list<FDNumber> & listFDN);
ostream & operator<<( ostream & xo, const list<FDNumber> & listFDN);

#endif

#endif
