// $Id: LinearExpression.h 182 2007-12-02 08:18:49Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// LinearExpression.h

#ifndef LinearExpression_H
#define LinearExpression_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Map.h"
#include "Cassowary.h"
#include "Variable.h"
#include "LinearExpression_fwd.h"
#include "my/refcnt.h"

using namespace std;

class Tableau;
class SymbolicWeight;

template <class T>
#ifdef USE_GC_EXP
class GenericLinearExpression : public gc {
#else
class GenericLinearExpression  {
#endif
    REFCOUNT_DEF                 //from nref.h
public:

  typedef Map<Variable,T> VarToCoeffMap;

  // convert Number-s into LinearExpression-s
  GenericLinearExpression( T num = 0.0);

  // Convert from Variable to a LinearExpression
  // this replaces Variable::asLinearExpression
  GenericLinearExpression( Variable clv, T value = 1.0, T constant = 0.0);

  // copy ctr
  GenericLinearExpression( const GenericLinearExpression<T> & expr) :
    _constant( expr._constant),
    _terms( expr._terms)
    { }

  virtual ~GenericLinearExpression();

  // Return a new linear expression formed by multiplying self by x.
  // ( Note that this result must be linear.)
  GenericLinearExpression<T> Times( Number x) const;

  // Return a new linear expression formed by multiplying self by x.
  // ( Note that this result must be linear.)
  GenericLinearExpression<T> Times( const GenericLinearExpression<T> & expr) const;

  // Return a new linear expression formed by adding x to self.
  GenericLinearExpression<T> Plus( const GenericLinearExpression<T> & expr) const;

  // Return a new linear expression formed by subtracting x from self.
  GenericLinearExpression<T> Minus( const GenericLinearExpression<T> & expr) const;

  // Return a new linear expression formed by dividing self by x.
  // ( Note that this result must be linear.)
  GenericLinearExpression<T> Divide( Number x) const;



  // Return a new linear expression formed by multiplying self by x.
  // ( Note that this result must be linear.)
  GenericLinearExpression<T> * P_times( Number x) const
    { return new GenericLinearExpression<T>( Times( x)); }

  GenericLinearExpression<T> *P_times(const GenericLinearExpression<T> &expr) const
    { return new GenericLinearExpression<T>(Times(expr)); }

  // Return a new linear expression formed by adding x to self.
  GenericLinearExpression<T> * P_plus( const GenericLinearExpression<T> & expr) const
    { return new GenericLinearExpression<T>( Plus( expr)); }

  // Return a new linear expression formed by subtracting x from self.
  GenericLinearExpression<T> * P_minus( const GenericLinearExpression<T> & expr) const
    { return new GenericLinearExpression<T>( Minus( expr)); }

  // Return a new linear expression formed by dividing self by x.
  // ( Note that this result must be linear.)
  GenericLinearExpression<T> * P_divide( Number x) const
    { return new GenericLinearExpression<T>( Divide( x)); }

  GenericLinearExpression<T> *P_divide(const GenericLinearExpression<T> &expr) const
    { return new GenericLinearExpression<T>(Divide(expr)); }

  // Return a new linear expression formed by dividing self by x.
  // ( Note that this result must be linear.)
  GenericLinearExpression<T> Divide( const GenericLinearExpression<T> & expr) const;

  // Return a new linear expression ( aNumber/this).  Since the result
  // must be linear, this is permissible only if 'this' is a constant.
  GenericLinearExpression<T> DivFrom( const GenericLinearExpression<T> & expr) const;

  // Return a new linear expression ( aNumber-this).
  GenericLinearExpression<T> SubtractFrom( const GenericLinearExpression<T> & expr) const
  { return expr.Minus(*this); }

  // Add n*expr to this expression from another expression expr.
  GenericLinearExpression<T> & AddExpression( const GenericLinearExpression<T> & expr,
				    Number n = 1.0);

  // Add n*expr to this expression from another expression expr.
  // Notify the solver if a variable is added or deleted from this
  // expression.
  GenericLinearExpression<T> & AddExpression( const GenericLinearExpression<T> & expr, Number n,
				    Variable subject,
				    Tableau & solver);

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, Add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.
  GenericLinearExpression<T> & AddVariable( Variable v, T c = 1.0);

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, Add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.
  GenericLinearExpression<T> & setVariable( Variable v, T c)
    {assert( c != 0.0);  _terms[v] = c; return * this; }

  // Add a term c*v to this expression.  If the expression already
  // contains a term involving v, Add c to the existing coefficient.
  // If the new coefficient is approximately 0, delete v.  Notify the
  // solver if v appears or disappears from this expression.
  GenericLinearExpression<T> & AddVariable( Variable v, T c,
				  Variable subject,
				  Tableau & solver);

  // Return a pivotable variable in this expression.  ( It is an error
  // if this expression is constant -- signal ExCLInternalError in
  // that case).  Return NULL if no pivotable variables
  Variable AnyPivotableVariable() const;

  // Replace var with a symbolic expression expr that is equal to it.
  // If a variable has been added to this expression that wasn't there
  // before, or if a variable has been dropped from this expression
  // because it now has a coefficient of 0, inform the solver.
  // PRECONDITIONS:
  //   var occurs with a non-Zero coefficient in this expression.
  void SubstituteOut( const Variable & v,
		     const GenericLinearExpression<T> & expr,
		     const Variable & subject,
		     Tableau & solver);

  // This linear expression currently represents the equation
  // oldSubject=self.  Destructively modify it so that it represents
  // the equation NewSubject=self.
  //
  // Precondition: NewSubject currently has a nonzero coefficient in
  // this expression.
  //
  // NOTES
  //   Suppose this expression is c + a*NewSubject + a1*v1 + ... + an*vn.
  //
  //   Then the current equation is
  //       oldSubject = c + a*NewSubject + a1*v1 + ... + an*vn.
  //   The new equation will be
  //        NewSubject = -c/a + oldSubject/a - ( a1/a)*v1 - ... - ( an/a)*vn.
  //   Note that the term involving NewSubject has been dropped.
  void ChangeSubject( Variable old_subject,
		     Variable new_subject);

  // This linear expression currently represents the equation self=0.  Destructively modify it so
  // that subject=self represents an equivalent equation.
  //
  // Precondition: subject must be one of the variables in this expression.
  // NOTES
  //   Suppose this expression is
  //     c + a*subject + a1*v1 + ... + an*vn
  //   representing
  //     c + a*subject + a1*v1 + ... + an*vn = 0
  // The modified expression will be
  //    subject = -c/a - ( a1/a)*v1 - ... - ( an/a)*vn
  //   representing
  //    subject = -c/a - ( a1/a)*v1 - ... - ( an/a)*vn
  //
  // Note that the term involving subject has been dropped.
  // Returns the reciprocal, so ChangeSubject can use it, too
  T NewSubject( Variable subject);

  // Return the value of the linear expression
  // given the current assignments of values to contained variables
  T Evaluate() const;

  // Return the coefficient corresponding to variable var, i.e.,
  // the 'ci' corresponding to the 'vi' that var is:
  //     v1*c1 + v2*c2 + .. + vn*cn + c
  T CoefficientFor( Variable var) const
    {
    typename VarToCoeffMap::const_iterator it = _terms.find( var);
    if ( it != _terms.end())
      return (*it).second;
    return 0.0;
    }

  T Constant() const
    { return _constant; }

  void Set_constant( T c)
    { _constant = c; }

  const VarToCoeffMap & Terms() const
    { return _terms; }

  VarToCoeffMap & Terms()
    { return _terms; }

  void IncrementConstant( T c)
    { _constant += c; }

  bool IsConstant() const
    { return _terms.size() == 0; }

#ifndef CL_NO_IO
  virtual ostream & PrintOn( ostream & xo) const;

  friend ostream & operator<<( ostream & xo,const GenericLinearExpression<T> & cle)
    { return cle.PrintOn( xo); }
#endif

  GenericLinearExpression<T> operator+( const GenericLinearExpression<T> & e2) { return Plus( e2); }
  GenericLinearExpression<T> operator-( const GenericLinearExpression<T> & e2) { return Minus( e2); }
  GenericLinearExpression<T> operator*( const GenericLinearExpression<T> & e2) { return Times( e2); }
  GenericLinearExpression<T> operator/( const GenericLinearExpression<T> & e2) { return Divide( e2); }
/*
  bool operator == ( const GenericLinearExpression<T> & e2) const {
    return _constant == e2._constant and _terms == e2._terms; }

  GenericLinearExpression<T> operator+( T e2 ) { return Plus( e2); }
  GenericLinearExpression<T> operator-( T e2 ) { return Minus( e2); }
  GenericLinearExpression<T> operator*( T e2 ) { return Times( e2); }
  GenericLinearExpression<T> operator/( T e2 ) { return Divide( e2); }

  GenericLinearExpression<T> operator+( Variable e2 ) { return Plus( e2); }
  GenericLinearExpression<T> operator-( Variable e2 ) { return Minus( e2); }
  GenericLinearExpression<T> operator*( Variable e2 ) { return Times( e2); }
  GenericLinearExpression<T> operator/( Variable e2 ) { return Divide( e2); }
*/

  friend GenericLinearExpression<T> operator+( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2) { return e1.Plus( e2); }
  friend GenericLinearExpression<T> operator-( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2) { return e1.Minus( e2); }
  friend GenericLinearExpression<T> operator*( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2) { return e1.Times( e2); }
  friend GenericLinearExpression<T> operator/( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2) { return e1.Divide( e2); }

  // FIXGJB -- this may be wrong -- should test underlying expression for equality
  friend bool operator==( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2)
    { return & e1 == & e2; }

  /// Named versions of the operator functions for ease of
  /// wrapping, or expressing using prefix notation

  friend GenericLinearExpression<T> Plus( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2) { return e1.Plus( e2); }
  friend GenericLinearExpression<T> Minus( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2) { return e1.Minus( e2); }
  friend GenericLinearExpression<T> Times( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2) { return e1.Times( e2); }
  friend GenericLinearExpression<T> Divide( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2) { return e1.Divide( e2); }
  friend GenericLinearExpression<T> * p_Plus( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2) { return e1.P_plus( e2); }
  friend GenericLinearExpression<T> * p_Minus( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2) { return e1.P_minus( e2); }
  friend GenericLinearExpression<T> * p_Times( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2) { return e1.P_times( e2); }
  friend GenericLinearExpression<T> * p_Divide( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2) { return e1.P_divide( e2); }


  // FIXGJB -- this may be wrong -- should test underlying expression for equality
  friend bool FEquals( const GenericLinearExpression<T> & e1, const GenericLinearExpression<T> & e2)
    { return & e1 == & e2; }

  GenericLinearExpression<T> & MultiplyMe( T x);

 private:

  T _constant;
  VarToCoeffMap _terms;

};

typedef GenericLinearExpression<Number>::VarToCoeffMap VarToNumberMap;

//because "no match for operator- in 'var - const'" for gcc-4.*
//template<> does not help, be it here or as friend's above
inline
GenericLinearExpression<Number> operator+( const Variable & e1, Number e2) { return GenericLinearExpression<Number>(e1).Plus( e2); }
inline
GenericLinearExpression<Number> operator-( const Variable & e1, Number e2) { return GenericLinearExpression<Number>(e1).Minus( e2); }
inline
GenericLinearExpression<Number> operator*( const Variable & e1, Number e2) { return GenericLinearExpression<Number>(e1).Times( e2); }
inline
GenericLinearExpression<Number> operator/( const Variable & e1, Number e2) { return GenericLinearExpression<Number>(e1).Divide( e2); }

inline
GenericLinearExpression<Number> operator+( Number e1, const Variable & e2) { return GenericLinearExpression<Number>(e1).Plus( e2); }
inline
GenericLinearExpression<Number> operator-( Number e1, const Variable & e2) { return GenericLinearExpression<Number>(e1).Minus( e2); }
inline
GenericLinearExpression<Number> operator*( Number e1, const Variable & e2) { return GenericLinearExpression<Number>(e1).Times( e2); }
inline
GenericLinearExpression<Number> operator/( Number e1, const Variable & e2) { return GenericLinearExpression<Number>(e1).Divide( e2); }

#endif
