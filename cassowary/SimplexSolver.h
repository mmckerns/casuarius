// $Id: SimplexSolver.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// SimplexSolver.h

#ifndef SimplexSolver_H
#define SimplexSolver_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "Solver.h"
#include "Tableau.h"
#include "LinearExpression_fwd.h"
#include "Strength.h"
#include "Constraint.h"
#include "Typedefs.h"
#include <stack>

class Variable;
class Point;
class ExCLRequiredFailureWithExplanation;


// SimplexSolver encapsulates the solving behaviour
// of the cassowary algorithm
class SimplexSolver : public Solver, public Tableau {
public:
  class EditInfo;
  typedef RefCountPtr< EditInfo> P_EditInfo;
  typedef list<P_EditInfo > EditInfoList;

 protected: 
  typedef Tableau super;
  P_EditInfo PEditInfoFromv( const Variable & );

 public:

  // Constructor
  SimplexSolver();
  virtual ~SimplexSolver();
  
  // Add constraints so that lower<=var<=upper.  ( nil means no  bound.)
  SimplexSolver & AddLowerBound( Variable v, Number lower);
  SimplexSolver & AddUpperBound( Variable v, Number upper);
  SimplexSolver & AddBounds( Variable v, Number lower, Number upper)
    { AddLowerBound( v,lower); AddUpperBound( v,upper); return *this; }

  // Add the constraint cn to the tableau
  SimplexSolver & AddConstraint( P_Constraint );

#ifdef CL_NO_DEPRECATED
  // Deprecated! --02/19/99 gjb
  SimplexSolver & AddConstraint( Constraint & cn) 
    { return AddConstraint(&cn); }
#endif

  // Add an edit constraint for "v" with given strength
  SimplexSolver & AddEditVar( const Variable &, const Strength & strength = sStrong(),
                              double weight = 1.0 );

  SimplexSolver & RemoveEditVar( const Variable &);

  // BeginEdit() should be called before sending
  // Resolve() messages, after adding the appropriate edit variables
  SimplexSolver & BeginEdit();

  // EndEdit should be called after editing has finished
  // for now, it just removes edit variables added from before the last BeginEdit
  SimplexSolver & EndEdit();

  // RemoveAllEditVars() just eliminates all the edit constraints
  // that were added
  SimplexSolver & RemoveAllEditVars() { RemoveEditVarsTo( 0); return *this; }

  // remove the last added edit vars to leave only n edit vars left
  SimplexSolver & RemoveEditVarsTo( int n);

  int numEditVars() const { return _editInfoList.size(); }

  // Add weak stays to the x and y parts of each point. These have
  // increasing weights so that the solver will try to satisfy the x
  // and y stays on the same point, rather than the x stay on one and
  // the y stay on another.
  SimplexSolver & AddPointStays( const vector<const Point * > & listOfPoints,
                                 const Strength & strength = sWeak());

  SimplexSolver & AddPointStay( const Variable & vx, const Variable & vy, 
                                const Strength & strength = sWeak(),
                                double weight = 1.0)
    { AddStay( vx,strength,weight); AddStay( vy,strength,weight); return *this; }

  SimplexSolver & AddPointStay( const Point & clp,
                                const Strength & strength = sWeak(),
                                double weight = 1.0);


  // Add a stay of the given strength ( default to weak) of v to the tableau
  SimplexSolver & AddStay( const Variable &,
			   const Strength & strength = sWeak(), double weight = 1.0 );

  // Remove the constraint cn from the tableau
  // Also remove any error variable associated with cn
  SimplexSolver & RemoveConstraint( P_Constraint pcn)
    { RemoveConstraintInternal( pcn); pcn->removedFrom(*this); return *this; }

#ifdef CL_NO_DEPRECATED
  // Deprecated! --02/19/99 gjb
  SimplexSolver & RemoveConstraint( Constraint & cn) 
    { return RemoveConstraint(&cn); }
#endif

  Variable RemoveColumn( const Variable & v)     { return Tableau::RemoveColumn( v); }

  // Re-initialize this solver from the original constraints, thus
  // getting rid of any accumulated numerical problems.  ( Actually, we
  // haven't definitely observed any such problems yet)
  void Reset();

  // Re-solve the current collection of constraints, given the new
  // values for the edit variables that have already been
  // suggested ( see SuggestValue() method)
  // This is not guaranteed to work if you remove an edit constraint
  // from the middle of the edit constraints you added
  // ( e.g., edit A, edit B, edit C, remove B -> this will fail!)
  // DEPRECATED
  void Resolve();

  // Re-solve the current collection of constraints for new values for
  // the constants of the edit variables.
  // This is implemented in terms of SuggestValue-s, and is
  // less efficient than that more natural interface
  void Resolve( const vector<Number> & newEditConstants);

  // Convenience function for Resolve-s of two variables
  void Resolve( Number x, Number y)
    {
    vector<Number> vals( 2);
    vals.push_back( x);
    vals.push_back( y);
    Resolve( vals);
    }

  // Suggest a new value for an edit variable
  // the variable needs to be added as an edit variable
  // and BeginEdit() needs to be called before this is called.
  // The tableau will not be solved completely until
  // after Resolve() has been called
  SimplexSolver & SuggestValue( const Variable & v, Number x);

  // Set and check whether or not the solver will attempt to compile
  // an explanation of failure when a required constraint conflicts
  // with another required constraint
  SimplexSolver & SetExplaining( bool f)
    { _fExplainFailure = f; return *this; }

  bool FIsExplaining() const
    { return _fExplainFailure; }

  // If autosolving has been turned off, client code needs
  // to explicitly call solve() before accessing variables
  // values
  SimplexSolver & Solve();

  SimplexSolver & SetEditedValue( Variable v, double n);

  // Solver contains the variable if it's in either the columns
  // list or the rows list
  bool FContainsVariable( const Variable & v)
    { return ColumnsHasKey( v) || RowExpression( v); }

  SimplexSolver & AddVar( const Variable & v)
    { if (!FContainsVariable( v)) 
        {
        AddStay( v); 
#ifdef CL_TRACE
        cerr << "added initial stay on " << v << endl;
#endif
        }
      return *this; }

  typedef void (*PfnResolveCallback)( SimplexSolver * psolver);

  void SetResolveCallback( PfnResolveCallback pfn)
    { _pfnResolveCallback = pfn; }

  typedef void (*PfnCnSatCallback)( SimplexSolver * psolver, 
                                   P_Constraint pcn, bool fSatisfied);

  void SetCnSatCallback( PfnCnSatCallback pfn)
    { _pfnCnSatCallback = pfn; }

#ifndef CL_NO_IO
  friend ostream & operator<<( ostream & xo, const SimplexSolver & tableau);

  ostream & PrintOn( ostream & xo) const; 
  ostream & PrintInternalInfo( ostream & xo) const; 
  ostream & PrintOnVerbose( ostream & xo) const 
    { PrintOn( xo); PrintInternalInfo( xo); xo << endl; return xo; }

#endif

  const ConstraintToVarMap & ConstraintMap() const
    { return _markerVars; }

  const VarToConstraintMap & MarkerMap() const
    { return _constraintsMarked; }

  bool FIsConstraintSatisfied( P_Constraint pcn) const;

#if CL_NO_DEPRECATED
  bool FIsConstraintSatisfied( const Constraint & pcn) const
    { return FIsConstraintSatisfied(&pcn); }
#endif

  // re-set all the external variables to their current values
  // most importantly, this re-calls all the Changev callbacks
  // ( which might be used to copy the Variable's value to another
  // variable)
  void UpdateExternalVariables() 
    { SetExternalVariables(); }

  // A. Beurive' Tue Jul  6 17:05:39 CEST 1999
  void ChangeStrengthAndWeight( P_Constraint , const Strength & , double weight);
  void ChangeStrength( P_Constraint , const Strength & );
  void ChangeWeight( P_Constraint , double weight);
  // void DisplayObjective();

  // Each of the non-required stays will be represented by an equation
  // of the form
  //     v = c + eplus - eminus
  // where v is the variable with the stay, c is the previous value of
  // v, and eplus and eminus are slack variables that hold the error
  // in satisfying the stay constraint.  We are about to change
  // something, and we want to fix the constants in the equations
  // representing the stays.  If both eplus and eminus are nonbasic
  // they have value 0 in the current solution, meaning the previous
  // stay was exactly satisfied.  In this case nothing needs to be
  // changed.  Otherwise one of them is basic, and the other must
  // occur only in the Expression for that basic error variable.
  // Reset the Constant in this Expression to 0.
  void ResetStayConstants();

  SimplexSolver & SetAutoResetStayConstants( bool f)
    { _fResetStayConstantsAutomatically = f; if ( f) ResetStayConstants(); return *this; }

  bool FIsAutoResetStayConstants() const
    { return _fResetStayConstantsAutomatically; }

 public:
  
  
 protected:
  // Add the constraint expr=0 to the inequality tableau using an
  // artificial variable.  To do this, create an artificial variable
  // av and Add av=expr to the inequality tableau, then make av be 0.
  // ( Raise an exception if we can't attain av=0.)
  // ( Raise an exception if we can't attain av=0.) If the Add fails,
  // prepare an explanation in e that describes why it failed ( note
  // that an empty explanation is considered to mean the explanation
  // encompasses all active constraints.
  bool AddWithArtificialVariable( P_LinearExpression , 
                                 ExCLRequiredFailureWithExplanation & e);
  
  // Using the given equation ( av = cle) build an explanation which
  // implicates all constraints used to construct the equation. That
  // is, everything for which the variables in the equation are markers.
  // Thanks to Steve Wolfman for the implementation of the explanation feature
  void BuildExplanation( ExCLRequiredFailureWithExplanation & e, 
                        Variable av,
                        P_LinearExpression );

  // We are trying to Add the constraint expr=0 to the appropriate
  // tableau.  Try to Add expr directly to the tableax without
  // creating an artificial variable.  Return true if successful and
  // false if not.
  bool TryAddingDirectly( P_LinearExpression );

  // We are trying to Add the constraint expr=0 to the tableaux.  Try
  // to choose a subject ( a variable to become basic) from among the
  // current variables in expr.  If expr contains any unrestricted
  // variables, then we must choose an unrestricted variable as the
  // subject.  Also, if the subject is new to the solver we won't have
  // to do any substitutions, so we prefer new variables to ones that
  // are currently noted as parametric.  If expr contains only
  // restricted variables, if there is a restricted variable with a
  // negative coefficient that is new to the solver we can make that
  // the subject.  Otherwise we can't find a subject, so return nil.
  // ( In this last case we have to Add an artificial variable and use
  // that variable as the subject -- this is done outside this method
  // though.)
  // 
  // Note: in checking for variables that are new to the solver, we
  // ignore whether a variable occurs in the objective function, since
  // new slack variables are added to the objective function by
  // 'NewExpression:', which is called before this method.
  Variable ChooseSubject( P_LinearExpression );
  
  // Each of the non-required edits will be represented by an equation
  // of the form
  //    v = c + eplus - eminus 
  // where v is the variable with the edit, c is the previous edit
  // value, and eplus and eminus are slack variables that hold the
  // error in satisfying the edit constraint.  We are about to change
  // something, and we want to fix the constants in the equations
  // representing the edit constraints.  If one of eplus and eminus is
  // basic, the other must occur only in the Expression for that basic
  // error variable.  ( They can't both be basic.)  Fix the Constant in
  // this Expression.  Otherwise they are both nonbasic.  Find all of
  // the expressions in which they occur, and fix the constants in
  // those.  See the UIST paper for details.  
  // ( This comment was for resetEditConstants(), but that is now
  // gone since it was part of the screwey vector-based interface
  // to resolveing. --02/15/99 gjb)
  void DeltaEditConstant( Number delta, Variable pv1, Variable pv2);
  
  // We have set new values for the constants in the edit constraints.
  // Re-Optimize using the dual simplex algorithm.
  void DualOptimize();

  // Make a new linear Expression representing the constraint cn,
  // replacing any basic variables with their defining expressions.
  // Normalize if necessary so that the Constant is non-negative.  If
  // the constraint is non-required give its error variables an
  // appropriate weight in the objective function.
  P_LinearExpression NewExpression( P_Constraint ,
                                    /* output to */
                                    Variable & clvEplus,
                                    Variable & clvEminus,
                                    Number & prevEConstant);

  // Minimize the value of the objective.  ( The tableau should already
  // be feasible.)
  void Optimize( const Variable & zVar);

  // Do a Pivot.  Move entryVar into the basis ( i.e. make it a basic variable),
  // and move exitVar out of the basis ( i.e., make it a parametric variable)
  void Pivot( const Variable & entryVar, const Variable & exitVar);

  // Set the external variables known to this solver to their appropriate values.
  // Set each external basic variable to its value, and set each
  // external parametric variable to 0.  ( It isn't clear that we will
  // ever have external parametric variables -- every external
  // variable should either have a stay on it, or have an equation
  // that defines it in terms of other external variables that do have
  // stays.  For the moment I'll put this in though.)  Variables that
  // are internal to the solver don't actually store values -- their
  // values are just implicit in the tableu -- so we don't need to set
  // them.
  void SetExternalVariables();

  // this gets called by RemoveConstraint and by AddConstraint when the
  // contraint we're trying to Add is inconsistent
  SimplexSolver & RemoveConstraintInternal( P_Constraint );

  void Changev( Variable clv, Number n) {
    clv.ChangeValue( n); 
    if ( _pfnChangevCallback) 
      _pfnChangevCallback(&clv,this);
  }

  /// instance variables

  // the arrays of positive and negative error vars for the stay constraints
  // ( need both positive and negative since they have only non-negative values)
  VarVector _stayMinusErrorVars;
  VarVector _stayPlusErrorVars;

  // give error variables for a non required constraint,
  // maps to SlackVariable-s
  ConstraintToVarSetMap _errorVars;

  // Return a lookup table giving the marker variable for each
  // constraint ( used when deleting a constraint).
  ConstraintToVarMap _markerVars;

  // Reverse of the above-- a lookup table giving the constraint
  // for each marker variable ( used when building failure explanations)
  VarToConstraintMap _constraintsMarked;

  Variable _objective;

  // Map edit variables to their constraints, errors, and prior
  // values
  EditInfoList _editInfoList;

  int _slackCounter;
  int _artificialCounter;
#ifdef CL_FIND_LEAK
  int _cArtificialVarsDeleted;
#endif
  int _dummyCounter;
  const double _epsilon;

  bool _fResetStayConstantsAutomatically;
  bool _fNeedsSolving;
  bool _fExplainFailure;

  PfnResolveCallback _pfnResolveCallback;
  PfnCnSatCallback _pfnCnSatCallback;

#ifdef CL_PV
  // C-style extension mechanism so I
  // don't have to wrap ScwmSolver separately
  void * _pv;
#endif
  // a stack of the number of edit constraints
  // that existed at the prior BeginEdit.
  // an EndEdit needs to pop off the top value,
  // then remove constraints to get down
  // to the # of constraints as in _stkCedcns.top()
  stack<int> _stkCedcns;

};


#endif
