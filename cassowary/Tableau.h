// $Id: Tableau.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Tableau.h

#ifndef Tableau_H
#define Tableau_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "LinearExpression.h"
#include "Variable.h"
#include "Typedefs.h"


#ifndef CL_NO_IO
class Tableau;

ostream & operator<<( ostream & xo, const Tableau & clt); 
ostream & operator<<( ostream & xo, const VarSet & varset); 
ostream & operator<<( ostream & xo, const TableauColumnsMap & varmap); 
ostream & operator<<( ostream & xo, const TableauRowsMap & rows); 
ostream & operator<<( ostream & xo, const VarVector & varlist);
#endif // CL_NO_IO

class Tableau {

 public:
  // No public constructor, since this does nothing but support
  // an ADT for the SimplexSolver

  // Variable v has been removed from an Expression.  If the
  // Expression is in a tableau the corresponding basic variable is
  // subject ( or if subject is nil then it's in the objective function).
  // Update the column cross-indices.
  void NoteRemovedVariable( const Variable & v, const Variable & subject);

  // v has been added to the linear Expression for subject
  // update column cross indices
  void NoteAddedVariable( const Variable & v, const Variable & subject);

#ifndef CL_NO_IO
  ostream & PrintOn( ostream & xo) const; 
  ostream & PrintInternalInfo( ostream & xo) const; 
  ostream & printExternalVariablesTo( ostream & xo) const; 
#endif

  // Check integrity of the tableau
  // not complete, yet, but a start, at least
  // Guard calls to AssertValid with CL_SOLVER_CHECK_INTEGRITY,
  // since this is expensive
  virtual void AssertValid() const;
  
 protected:
  // Constructor -- want to start with empty objects so not much to do
  Tableau()
    { }

  virtual ~Tableau();

  // Add v=expr to the tableau, update column cross indices
  // v becomes a basic variable
  void addRow( const Variable & v, P_LinearExpression );

  // Remove v from the tableau -- remove the column cross indices for v
  // and remove v from every Expression in rows in which v occurs
  // returns a pointer to the variable ( since we often want to delete
  // the variable)
  Variable RemoveColumn( const Variable & v);

  // Remove the basic variable v from the tableau row v=expr
  // Then update column cross indices
  // Probably want to call delete on the LinearExpression * returned
  // unless you're adding that same Expression back into the 
  // tableau
  P_LinearExpression RemoveRow( const Variable & v);

  // Replace all occurrences of oldVar with expr, and update column cross indices
  // oldVar should now be a basic variable
  void SubstituteOut( const Variable & oldVar, P_LinearExpression );

  const TableauColumnsMap & Columns() { return _columns; }  
  const TableauRowsMap &    Rows()    { return _rows; }  

  // return true iff the variable subject is in the Columns keys
  bool ColumnsHasKey( const Variable & subject) const
    { 
    TableauColumnsMap::const_iterator i = _columns.find( subject);
    return ( i != _columns.end());
    }

  P_LinearExpression RowExpression( const Variable & v) const
    {
    TableauRowsMap::const_iterator i = _rows.find( v);
    if ( i != _rows.end())
      return (*i).second;
    else
      return NULL;
    }
/*
  LinearExpression * RowExpression( Variable v)
    {
      const Tableau * pthis = const_cast<const Tableau * >( this);
      return const_cast<LinearExpression * >( pthis->RowExpression( v));
    }
*/

  bool FIsBasicVar( const Variable & v) const
    { return RowExpression( v) != NULL; }

  // private: FIXGJB: can I improve the encapsulation?

  // _columns is a mapping from variables which occur in expressions to the
  // set of basic variables whose expressions contain them
  // i.e., it's a mapping from variables in expressions ( a column) to the 
  // set of rows that contain them
  TableauColumnsMap _columns;

  // _rows maps basic variables to the expressions for that row in the tableau
  TableauRowsMap _rows;

  // the collection of basic variables that have infeasible rows
  // ( used when reoptimizing)
  VarSet _infeasibleRows;

  // the set of rows where the basic variable is external
  // this was added to the C++ version to reduce time in SetExternalVariables()
  VarSet _externalRows;

  // the set of external variables which are parametric
  // this was added to the C++ version to reduce time in SetExternalVariables()
  VarSet _externalParametricVars;

};

#endif
