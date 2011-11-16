// $Id: StayConstraint.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// StayConstraint.h

#ifndef StayConstraint_H
#define StayConstraint_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "EditOrStayConstraint.h"

class StayConstraint : public EditOrStayConstraint {
     typedef EditOrStayConstraint super;
 public:

  StayConstraint( const Variable & var,
		   const Strength & strength = sWeak(), double weight = 1.0 ) :
    EditOrStayConstraint( var,strength,weight)
    { }

  virtual bool isStayConstraint() const
    { return true; }

#ifndef CL_NO_IO  
  virtual ostream & PrintOn( ostream & xo) const 
    { super::PrintOn( xo); return xo << " STAY)"; }
#endif
  
 private:
};

#endif
