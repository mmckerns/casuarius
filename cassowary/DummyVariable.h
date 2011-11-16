// $Id: DummyVariable.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// DummyVariable.h

#ifndef DummyVariable_H
#define DummyVariable_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "AbstractVariable.h"

class DummyVariable : public AbstractVariable {
  void init_flags() {  
    _flags |= E_IsDummy; 
  }

public:
  ~DummyVariable();
  DummyVariable( string Name = "") :
    AbstractVariable( Name)
    {  init_flags(); }

  DummyVariable( long number, char * prefix) :
    AbstractVariable( number,prefix)
    {  init_flags(); }

#ifndef CL_NO_IO
  ostream & PrintOn( ostream & xo) const
  {  
    xo << "[" << Name() << ":dummy]";
    return xo;
  }
#endif

  // Return true if we can Pivot on this variable.
  bool IsPivotable() const { return false; }
  // Return true if this is a restricted ( or slack) variable.  Such
  // variables are constrained to be non-negative and occur only
  // internally to the simplex solver.
  bool IsRestricted() const { return true; }
};

#endif
