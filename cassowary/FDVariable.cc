// $Id: FDVariable.cc,v 1.1.1.1 2003/01/15 14:06:20 svilen Exp $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// FDVariable.cc

#include "FDVariable.h"
#include "Solver.h" // for list<FDNumber> printing

#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif

#if 0 /* GJB:FIXME:: */
  FDVariable( string name, FDNumber Value) :
    AbstractVariable( name),
    _value( Value),
    _fSet( true),
    _desired_value( Value),
    _plfdnInitialDomain( NULL)
    { init_flags(); }
#endif

FDVariable::FDVariable( string name, FDNumber Value, const list<FDNumber> & initial_domain) 
    : AbstractVariable( name),
    _value( Value),
    _fSet( true),
    _desired_value( Value),
    _plfdnInitialDomain( new list<FDNumber>())
{
    init_flags();
    *_plfdnInitialDomain = initial_domain;
}

// Use < > for FDVariable-s, instead of [ ]
#ifndef CL_NO_IO
ostream & FDVariable::PrintOn( ostream & xo) const
{  
  xo << "<" << Name() << "=" << Value() << ":" << * PlfdnDomain() << ">";
  return xo;
}
#endif
