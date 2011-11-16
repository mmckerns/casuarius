// $Id: LinearEquation.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// LinearEquation.h

#ifndef LinearEquation_H
#define LinearEquation_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "LinearConstraint.h"
#include "LinearExpression.h"

class Strength;
class Variable;

class LinearEquation : public LinearConstraint {
 private: typedef LinearConstraint super;

 public:
 //// Constructors

 // LinearEquation( expr,...)    is   expr == 0
 LinearEquation( const LinearExpression & cle,
		  const Strength & strength = sRequired(),
		  double weight = 1.0) :
   LinearConstraint( cle,strength, weight)
   { }

 // LinearEquation( var,expr,...)  is   var == expr
 LinearEquation( Variable clv,
		  const LinearExpression & cle,
		  const Strength & strength = sRequired(),
		  double weight = 1.0) :
   LinearConstraint( cle,strength,weight)
   { _expression.AddVariable( clv,-1.0); }

 // LinearEquation( var,const,...)  is   var == const
 LinearEquation( Variable clv,
		  Number n,
		  const Strength & strength = sRequired(),
		  double weight = 1.0) :
   LinearConstraint( n,strength,weight)
   { _expression.AddVariable( clv,-1.0); }

 // LinearEquation( expr,var,...) is   var == expr
 LinearEquation( const LinearExpression & cle,
		  Variable clv,
		  const Strength & strength = sRequired(),
		  double weight = 1.0) :
   LinearConstraint( cle,strength,weight)
   { _expression.AddVariable( clv,-1.0); }

 // LinearEquation( expr,expr,...) is   expr == expr
 LinearEquation( const LinearExpression & cle1,
		  const LinearExpression & cle2,
		  const Strength & strength = sRequired(),
		  double weight = 1.0) :
   LinearConstraint( cle1,strength,weight)
   { _expression.AddExpression( cle2,-1.0); }

 // LinearEquation( expr,const,...) is   expr == const
 LinearEquation( const LinearExpression & cle1,
		  Number n,
		  const Strength & strength = sRequired(),
		  double weight = 1.0) :
   LinearConstraint( cle1,strength,weight)
   { _expression.AddExpression( n,-1.0); }

#ifndef CL_NO_IO
 virtual ostream & PrintOn( ostream & xo) const
   {  super::PrintOn( xo); xo << " = 0 )"; return xo; }
#endif

  virtual bool FIsSatisfied() const
    { return ( _expression.Evaluate() == 0); }

};

#include "my/refcntp.h"
typedef RefCountPtr_Subclass< LinearEquation, LinearConstraint> P_LinearEquation;

#endif
