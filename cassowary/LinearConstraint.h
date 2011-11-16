// $Id: LinearConstraint.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// LinearConstraint.h

#ifndef LinearConstraint_H
#define LinearConstraint_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "Constraint.h"
#include "LinearExpression.h"

// Add the LinearExpression member variable needed for both
// LinearEquation and LinearInequality

class LinearConstraint : public Constraint {
 public:

    LinearConstraint( const LinearExpression & cle,
                      const Strength & strength = sRequired(),
                      double weight = 1.0)
        : Constraint( strength, weight)
        , _expression( cle)       //my own copy
        , _holder( &_expression )
        { }

  // Return my linear Expression.  ( For linear equations, this
  // constraint represents Expression=0; for linear inequalities it
  // represents Expression>=0.)
    LinearExpression Expression() const { return _expression; }

  // do not do this if * this is inside a solver
    void ChangeConstant( Number constant) { _expression.Set_constant( constant); }

 protected:
    LinearExpression _expression;
    virtual void setExpression( const LinearExpression & expr) { _expression = expr; }
 private:
    P_LinearExpression_holder _holder;
};

#include "my/refcntp.h"
typedef RefCountPtr_Subclass< LinearConstraint, Constraint> P_LinearConstraint;

#endif
