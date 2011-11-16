// $Id: FDVariable.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// FDVariable.h

#ifndef FDVariable_H
#define FDVariable_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include <stdio.h>
#include <map>
#include <string>
#include <list>
#include "Cassowary.h"
#include "AbstractVariable.h"

class FDVariable : public AbstractVariable {
  void init_flags() {  
    _flags |= E_IsFDVariable | E_IsExternal;  
  }
public:
  typedef AbstractVariable super;

#if 0 /* GJB:FIXME:: */
  FDVariable( string name, FDNumber Value);
#endif

  FDVariable( string name, FDNumber Value, const list<FDNumber> & initial_domain);

#ifndef CL_NO_IO
  // Prints a semi-descriptive representation to the stream, using the
  // name if there is one, and otherwise the hash number of this
  // object.
  //	EXAMPLE
  //	  [x:10.0]		-- name = "x", Value = 10.0
  ostream & PrintOn( ostream & xo) const;
#endif
  
  // Return the current Value I hold.
  Number Value() const { return _value; }

  // Round the Value to an integer and return it
  int IntValue() const { return _value; }

  // change the Value held -- should * not* use this if the variable is
  // in a solver -- instead use AddEditVar() and SuggestValue() interface
  void SetValue( FDNumber Value) { _value = Value; }

  // permit overriding in subclasses in case something needs to be
  // done when the Value is changed by the solver
  // may be called when the Value hasn't actually changed -- just 
  // means the solver is setting the external variable
  void ChangeValue( FDNumber Value) { _value = Value; }

  virtual bool FIsSet() { return _fSet; } 
  virtual void SetFIsSet( bool f) { _fSet = f; }

  FDNumber DesiredValue() const { return _desired_value; }

  const list<FDNumber> * PlfdnDomain() const { return _plfdnInitialDomain; }

protected:

  // similar to SetValue -- see caveat above -- made private for now
  // since it's probably the wrong thing and is too easy to invoke
  FDNumber operator=( FDNumber Value)
    { _value = Value; return Value; }

  // Copy constructor left undefined since we want to
  // outlaw passing by Value!  Will get a link error if you
  // try to use within FDVariable.c, compile-time error everywhere else
  FDVariable( const FDVariable & );

  FDNumber _value;

  // has the _value been set?  Used during solves.
  bool _fSet;

  FDNumber _desired_value;

  list<FDNumber> * _plfdnInitialDomain;
};

#endif
