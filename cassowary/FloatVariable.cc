// $Id: FloatVariable.cc,v 1.2 2003/05/23 11:10:45 svilen Exp $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// FloatVariable.cc

#include "FloatVariable.h"

#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif

#ifndef CL_NO_IO
ostream & FloatVariable::PrintOn( ostream & xo) const
{  
  xo << "[" << Name() << ":" << _value << "]";
//    xo << this <<"#";
  return xo;
}
#endif
