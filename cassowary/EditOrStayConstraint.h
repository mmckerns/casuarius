// $Id: EditOrStayConstraint.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// EditOrStayConstraint.h

#ifndef EditOrStayConstraint_H
#define EditOrStayConstraint_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Constraint.h"
#include "LinearExpression.h"

class Variable;

class EditOrStayConstraint : public Constraint {
 public:

  EditOrStayConstraint( const Variable & var,
			 const Strength & strength = sRequired(), double weight = 1.0 ) :
    Constraint( strength,weight),
    _variable( var)
    { }

  const Variable & variable() const
    { return _variable; }

  LinearExpression Expression() const
    { return LinearExpression( _variable,-1,_variable.Value()); }

 private:

  void setVariable( const Variable & v)
    { _variable = v; }

  /// instance variables
  Variable _variable;
};

#endif
