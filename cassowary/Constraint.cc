// $Id: Constraint.cc,v 1.1.1.1 2003/01/15 14:06:20 svilen Exp $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Constraint.cc

#include "Constraint.h"
#include "debug.h"

#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif

#include "my/refcnt.h"
REFCOUNT_INST( Constraint)         //from refcnt.h
/*
void incref( Constraint * p)  { p->incref(); }  
void decref( Constraint * p, int del)  { 
    cout << "dele Constraint" << p << '/' << p->nref() <<endl;
    p->decref(); if (del && !p->nref()) delete p; }
*/

Constraint::Constraint( const Strength & strength, double weight ) :
    _strength( strength),
    _readOnlyVars(),
    _weight( weight),
    _pv( 0),
    _times_added( 0)
{ 
    CtrTracer( __FUNCTION__,this);
}
Constraint::~Constraint()
{ 
    REFCOUNT_DIE( Constraint)
    DtrTracer( __FUNCTION__,this);
}
    
#ifndef CL_NO_IO
#include "Tableau.h" // for VarSet printing

ostream & 
Constraint::PrintOn( ostream & xo) const 
{
  // Note that the trailing "= 0)" or ">= 0)" is missing, as derived classes will
  // print the right thing after calling this function
  xo << strength() << " w{" << weight() << "} ta{" 
     << _times_added << "} RO" << _readOnlyVars << " " << "(" << Expression();
  return xo;
}

#endif

//#include "LinearConstraint.h"
//REFCOUNT_INST( LinearConstraint)         //from refcnt.h

