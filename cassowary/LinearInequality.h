// $Id: LinearInequality.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// LinearInequality.h

#ifndef LinearInequality_H
#define LinearInequality_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "LinearConstraint.h"

class Variable;

class LinearInequality : public LinearConstraint {
 private: typedef LinearConstraint super;

 public:
 //// Constructors
 // LinearInequality( expr,...)  is expr >= 0
 LinearInequality( const LinearExpression & cle,
		    const Strength & strength = sRequired(),
		    double weight = 1.0) :
   LinearConstraint( cle,strength, weight),
   _fStrictInequality( false)
   { }

 // LinearInequality( var,OP,expr) is  var >= expr
 LinearInequality( const Variable clv,
		    CnRelation op,
		    const LinearExpression & cle,
		    const Strength & strength = sRequired(),
		    double weight = 1.0) :
   LinearConstraint( cle, strength, weight),
   _fStrictInequality( false)
   { 
   if ( op == cnGEQ || op == cnGT)
     {
     _expression.MultiplyMe(-1.0);
     _expression.AddVariable( clv,1.0);
     }
   else if ( op == cnLEQ || op == cnGEQ)
     {
     _expression.AddVariable( clv,-1.0);
     }
   else
     {
       throw ExCLEditMisuse("Cannot use that operator for LinearInequality objects");
     }
   if ( op == cnLT || op == cnGT) {
     _fStrictInequality = true;
   }
   }

#ifdef FIXGJB_AMBIGUOUS
 // LinearInequality( expr,OP,var) is  var ?<>? expr
 LinearInequality( const LinearExpression & cle,
		    CnRelation op,
		    const Variable clv,
		    const Strength & strength = sRequired(),
		    double weight = 1.0) :
   LinearConstraint( cle, strength, weight),
   _fStrictInequality( false)
   { 
   if ( op == cnLEQ || op == cnLT)
     {
     _expression.MultiplyMe(-1.0);
     _expression.AddVariable( clv,1.0);
     }
   else if ( op == cnGEQ || op == cnGT)
     {
     _expression.AddVariable( clv,-1.0);
     }
   if ( op == cnLT || op == cnGT) {
     _fStrictInequality = true;
   }
   }
#endif

 // LinearInequality( expr,OP,expr) is  expr >= expr
 LinearInequality( const LinearExpression & cle1,
		    CnRelation op,
		    const LinearExpression & cle2,
		    const Strength & strength = sRequired(),
		    double weight = 1.0) :
   LinearConstraint( cle2, strength, weight),
   _fStrictInequality( false)
   { 
   if ( op == cnGEQ || op == cnGT)
     {
     _expression.MultiplyMe(-1.0);
     _expression.AddExpression( cle1);
     }
   else if ( op == cnLEQ || op == cnLT)
     {
     _expression.AddExpression( cle1,-1.0);
     }
   if ( op == cnLT || op == cnGT) {
     _fStrictInequality = true;
   }
   }

#ifdef FIXGJB_AMBIGUOUS
 // LinearInequality( var,OP,var) is  var ?<>? var
 LinearInequality( const Variable clv1,
		    CnRelation op,
		    const Variable clv2,
		    const Strength & strength = sRequired(),
		    double weight = 1.0) :
   LinearConstraint( clv2, strength, weight),
   _fStrictInequality( false)
   { 
   if ( op == cnGEQ || op == cnGT)
     {
     _expression.MultiplyMe(-1.0);
     _expression.AddVariable( clv1,1.0);
     }
   else if ( op == cnLEQ || op == cnLT)
     {
     _expression.AddVariable( clv1,-1.0);
     }
   if ( op == cnLT || op == cnGT) {
     _fStrictInequality = true;
   }
   }
#endif

 
 // Return true if this is an inequality constraint and
 // false if it is an equality constraint.  The default is
 // that it is not.
 virtual bool IsInequality() const
   { return true; }

 virtual bool IsStrictInequality() const
   { return _fStrictInequality; }

#ifndef CL_NO_IO 
 virtual ostream & PrintOn( ostream & xo) const
   {  super::PrintOn( xo); xo << " >= 0 )"; return xo; }
#endif

  virtual bool FIsSatisfied() const
    { 
      Number v = _expression.Evaluate();
      if ( _fStrictInequality) 
        return ( v > 0);
      else 
        return ( v >= 0);
    }

 private:

  bool _fStrictInequality;
};

#endif
