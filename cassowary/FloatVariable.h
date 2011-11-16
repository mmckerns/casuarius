// $Id: FloatVariable.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// FloatVariable.h

#ifndef FloatVariable_H
#define FloatVariable_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include <stdio.h>
#include <map>
#include <string>
#include "Cassowary.h"
#include "AbstractVariable.h"

class FloatVariable : public AbstractVariable {
  void init_flags() {  
    _flags |= E_IsFloatVariable | E_IsExternal; 
  }
public:
  typedef AbstractVariable super;

  FloatVariable( string name, Number Value = 0.0) :
    AbstractVariable( name),
    _value( Value)
    { init_flags(); }

  FloatVariable( Number Value = 0.0) :
    AbstractVariable(""),
    _value( Value)
    { init_flags(); }

  FloatVariable( long number, char * prefix, Number Value = 0.0) :
    AbstractVariable( number,prefix),
    _value( Value)
    { init_flags(); }

#ifndef CL_NO_IO
  // Prints a semi-descriptive representation to the stream, using the
  // name if there is one, and otherwise the hash number of this
  // object.
  //	EXAMPLE
  //	  [x:10.0]		-- name = "x", Value = 10.0
  ostream & PrintOn( ostream & xo) const;
#endif
  
  // Return true if we can Pivot on this variable.
  bool IsPivotable() const { return false; }

  // Return true if this is a restricted (or slack) variable.  Such
  // variables are constrained to be non-negative and occur only
  // internally to the simplex solver.
  bool IsRestricted() const { return false; }
  
  // Return the current Value I hold.
  Number Value() const { return _value; }

  // Round the Value to an integer and return it
  int IntValue() const { return int( _value + 0.5); }

  // change the Value held -- should * not* use this if the variable is
  // in a solver -- instead use AddEditVar() and SuggestValue() interface
  void SetValue( Number Value) { _value = Value; }

  // permit overriding in subclasses in case something needs to be
  // done when the Value is changed by the solver
  // may be called when the Value hasn't actually changed -- just 
  // means the solver is setting the external variable
  void ChangeValue( Number Value) { _value = Value; }

private:

  // similar to SetValue -- see caveat above -- made private for now
  // since it's probably the wrong thing and is too easy to invoke
  //Number operator=( Number Value) { _value = Value; return Value; }

  // Copy constructor left undefined since we want to
  // outlaw passing by Value!  
  //FloatVariable( const FloatVariable & );

  Number _value;
};



#endif
