// $Id: SlackVariable.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// SlackVariable.h

#ifndef SlackVariable_H
#define SlackVariable_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "AbstractVariable.h"

class SlackVariable : public AbstractVariable {
public:
  ~SlackVariable();
  SlackVariable( string Name = "") :
    AbstractVariable( Name)
    { }

  SlackVariable( long number, char * prefix) :
    AbstractVariable( number,prefix)
    { }

#ifndef CL_NO_IO
  ostream & PrintOn( ostream & xo) const
  {  
    xo << "[" << Name() << ":slack]";
    return xo;
  }
#endif

  bool IsPivotable()  const { return true; } 
  bool IsRestricted() const { return true; }
};

#endif
