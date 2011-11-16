// $Id: AbstractVariable.cc,v 1.1.1.1 2003/01/15 14:06:20 svilen Exp $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// AbstractVariable.cc

#include "AbstractVariable.h"

#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif
#include <stdio.h> /* for sprintf */

#include "my/refcnt.h"
REFCOUNT_INST( AbstractVariable)         //from refcnt.h

long AbstractVariable::iVariableNumber = 0;

AbstractVariable::AbstractVariable( string Name )
    : _flags(0)
    , _name( Name)
#ifdef CL_PV
    , _pv( 0)
#endif    
    { 
    ++iVariableNumber;
    if ( Name.length() == 0)
      {
      char sz[16];
      sprintf( sz,"v%ld",iVariableNumber);
      _name = string( sz);
      }
    }

AbstractVariable::AbstractVariable( long varnumber, char * prefix) 
    : _flags(0)
#ifdef CL_PV
    , _pv( 0)
#endif    
    {
    iVariableNumber++;
    char pch[16]; sprintf( pch,"%ld",varnumber);
    _name = string( prefix ) + string( pch);
    }

AbstractVariable::~AbstractVariable() {
//    REFCOUNT_DIE( AbstractVariable)
}
