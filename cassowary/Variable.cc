// $Id: Variable.cc,v 1.1.1.1 2003/01/15 14:06:21 svilen Exp $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Variable.cc

#include "Variable.h"

#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif

StringToVarMap * Variable::pmapStrPclv = NULL;
Variable clvNil( static_cast<AbstractVariable*>( 0));

