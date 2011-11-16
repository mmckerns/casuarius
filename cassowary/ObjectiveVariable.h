// $Id: ObjectiveVariable.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// ObjectiveVariable.h

#ifndef ObjectiveVariable_H
#define ObjectiveVariable_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "AbstractVariable.h"

class ObjectiveVariable : public AbstractVariable {
public:
  ObjectiveVariable( string name = "") :
    AbstractVariable( name)
    {} 

  ObjectiveVariable( long number, char * prefix) :
    AbstractVariable( number,prefix)
    {}

#ifndef CL_NO_IO
  ostream & PrintOn( ostream & xo) const
  {  
    xo << "[" << Name() << ":obj]";
    return xo;
  }
#endif

  // We don't need to give such variables a Value after solving is complete.
  //IsExternal() { return false; }

  // Return true if we can Pivot on this variable.
  bool IsPivotable() const { return false; }

  // Return true if this is a restricted (or slack) variable.  Such
  // variables are constrained to be non-negative and occur only
  // internally to the simplex solver.
  bool IsRestricted() const { return false; }
};

#endif
