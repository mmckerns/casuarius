// $Id: Tableau.cc,v 1.1.1.1 2003/01/15 14:06:21 svilen Exp $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Tableau.cc

#include "Tableau.h"
#include "debug.h"

#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif


void Tableau::NoteRemovedVariable( const Variable & v, const Variable & subject)
    { 
#ifdef CL_TRACE
    Tracer TRACER( __FUNCTION__);
    cerr << "(" << v << ", " << subject << ")" << endl;
#endif
    VarSet & column = _columns[v];
    VarSet::const_iterator it = column.find( subject);
    assert( it != column.end());
    column.erase( it);
#ifdef CL_TRACE_VERBOSE
    cerr << "v = " << v << " and Columns[v].size() = "
         << column.size() << endl;
#endif
    if ( column.size() == 0)
      {
      _columns.erase( v);
      _externalRows.erase( v);
      _externalParametricVars.erase( v);
      }
    }

  // v has been added to the linear Expression for subject
  // update column cross indices
void Tableau::NoteAddedVariable( const Variable & v, const Variable & subject)
    { 
#ifdef CL_TRACE
    Tracer TRACER( __FUNCTION__);
    cerr << "(" << v << ", " << subject << ")" << endl;
#endif
    _columns[v].insert( subject); 
    if ( v.IsExternal() && !FIsBasicVar( v))
      {
      _externalParametricVars.insert( v);
      }
    }

void Tableau::AssertValid() const {
#ifndef NDEBUG
    // all external basic variables are in _externalRows
    // and all external parametric variables are in _externalParametricVars
    TableauRowsMap::const_iterator itRow = _rows.begin();
    for (; itRow != _rows.end(); ++itRow)
      {
      const Variable clv = (*itRow).first;
      if ( clv.IsExternal())
        {
        if ( _externalRows.find( clv) == _externalRows.end()) 
          {
#ifndef CL_NO_IO
          cerr << "External basic variable " << clv
               << " is not in _externalRows" << endl;
#endif
          }
        }
      P_LinearExpression pcle = RowExpression( clv);
      assert( pcle);
      VarToNumberMap::const_iterator it = pcle->Terms().begin();
      for (; it != pcle->Terms().end(); ++it)
        {
        Variable clv = (*it).first;
        if ( clv.IsExternal()) 
          {
          if ( _externalParametricVars.find( clv) == _externalParametricVars.end())
            {
#ifndef CL_NO_IO
            cerr << "External parametric variable " << clv 
                 << " is not in _externalParametricVars" << endl;
#endif
            }
          }
        }
      }
#endif /* !NDEBUG */
  }


// delete the linear expressions
// let SimplexSolver worry about deleting the variables
Tableau::~Tableau()
{
#ifdef CL_TRACE
  TableauRowsMap::iterator it = _rows.begin();
  for (; it != _rows.end(); ++it)
    {
    // free the LinearExpression that we new-ed 
    cerr << "Deleting row  delete@ " << ((*it).second.ptr()) << endl;
    }
#endif
}

#ifndef CL_NO_IO
// Some extra debugging info
ostream & 
Tableau::PrintInternalInfo( ostream & xo) const
{
  xo << "ncns:" << _rows.size() -1
     << "; cols:" << _columns.size()
     << "; infrows:" << _infeasibleRows.size() 
     << "; ebvars:" << _externalRows.size()
     << "; epvars:" << _externalParametricVars.size();
  return xo;
}


ostream & 
Tableau::printExternalVariablesTo( ostream & xo) const
{
  xo << "Parametric: ";
  VarSet::const_iterator itParVars = _externalParametricVars.begin();
  for ( ; itParVars != _externalParametricVars.end(); ++itParVars ) {
    Variable v = * itParVars;
    xo << v << " ";
  }
  xo << "\nBasic: ";
  VarSet::const_iterator itRowVars = _externalRows.begin();
  for ( ; itRowVars != _externalRows.end() ; ++itRowVars ) {
    Variable v = * itRowVars;
    xo << v << " ";
  }
  return xo << endl;
}

#endif


// Add v, update column cross indices
// v becomes a basic variable
// expr is now owned by Tableau class, 
// and Tableau is responsible for deleting it
// ( also, expr better be allocated on the heap!)
void Tableau::addRow( const Variable & var, P_LinearExpression expr)
{
#ifdef CL_TRACE
  Tracer TRACER( __FUNCTION__);
  cerr << "(" << var << ", " << expr << ")" << endl;
#endif
  _rows[var] = expr;//const_cast<LinearExpression * >(&expr);
  // for each variable in expr, Add var to the set of rows which have that variable
  // in their Expression
  VarToNumberMap::const_iterator it = expr->Terms().begin();
  for (; it != expr->Terms().end(); ++it)
    {
    Variable v = (*it).first;
    _columns[v].insert( var);
    if ( v.IsExternal() && !FIsBasicVar( v))
      {
      _externalParametricVars.insert( v);
      }
    }
  if ( var.IsExternal())
    {
    _externalRows.insert( var);
    }
#ifdef CL_TRACE
  cerr << *this << endl;
#endif
}

// Remove var from the tableau -- remove the column cross indices for var
// and remove var from every Expression in rows in which v occurs
// Remove the parametric variable var, updating the appropriate column and row entries.
// ( Renamed from Smalltalk implementation's `removeParametricVar')
Variable
Tableau::RemoveColumn( const Variable & var)
{
#ifdef CL_TRACE
  Tracer TRACER( __FUNCTION__);
  cerr << "(" << var << ")" << endl;
#endif
  TableauColumnsMap::iterator it_var = _columns.find( var);
  if ( it_var == _columns.end())
    return var;  // nothing to do

  VarSet & varset = (*it_var).second;
  // remove the rows with the variables in varset
  VarSet::iterator it = varset.begin();
  for (; it != varset.end(); ++it)
    {
    Variable v = (*it);
    VarToNumberMap & Terms = _rows[v]->Terms();
    Terms.erase( Terms.find( var));
    }
  if ( var.IsExternal())
    {
    _externalRows.erase( var);
    _externalParametricVars.erase( var);
    }
  _columns.erase( it_var);
  return var;
}

// Remove the basic variable v from the tableau row v=expr
// Then update column cross indices
P_LinearExpression Tableau::RemoveRow( const Variable & var)
{
#ifdef CL_TRACE
  Tracer TRACER( __FUNCTION__);
  cerr << "(" << var << ")" << endl;
#endif
  TableauRowsMap::iterator it = _rows.find( var);
  assert( it != _rows.end());
  P_LinearExpression pexpr = (*it).second;
  VarToNumberMap & Terms = pexpr->Terms();
  VarToNumberMap::iterator it_term = Terms.begin();
  for (; it_term != Terms.end(); ++it_term)
    {
    const Variable & v = (*it_term).first;
    _columns[v].erase( var);
    if ( _columns[v].size() == 0)
      {
      _columns.erase( v);
      _externalParametricVars.erase( v);
      }
    }

  _infeasibleRows.erase( var);

  if ( var.IsExternal())
    {
    _externalRows.erase( var);
    _externalParametricVars.erase( var);
    }

  _rows.erase( it);
#ifdef CL_TRACE
  cerr << "- returning " << *pexpr << endl;
#endif
  return pexpr;
}

// Replace all occurrences of oldVar with expr, and update column cross indices
// oldVar should now be a basic variable
// Uses the Columns data structure and calls SubstituteOut on each
// row that has oldVar in it
// oldVar is leaving the basis, and becoming parametric
void 
Tableau::SubstituteOut( const Variable & oldVar, P_LinearExpression expr)
{
#ifdef CL_TRACE
  cerr << "* Tableau::";
  Tracer TRACER( __FUNCTION__);
  cerr << "(" << oldVar << ", " << *expr << ")" << endl;
  cerr << (*this) << endl;
#endif

  TableauColumnsMap::iterator it_oldVar = _columns.find( oldVar);
  if ( it_oldVar == _columns.end())
    return;

  VarSet & varset = (*it_oldVar).second;
  VarSet::iterator it = varset.begin();
  for (; it != varset.end(); ++it)
    {
    const Variable & v = (*it);
    P_LinearExpression prow = _rows[v];
    prow->SubstituteOut( oldVar,*expr,v,*this);
    if ( v.IsRestricted() && prow->Constant() < 0.0)
      {
      _infeasibleRows.insert( v);
      }
    }
  _columns.erase( it_oldVar);
  if ( oldVar.IsExternal())
    {
    if ( _columns[oldVar].size() > 0) 
      {
      _externalRows.insert( oldVar);
      }
    _externalParametricVars.erase( oldVar);
    }
}


#ifndef CL_NO_IO

ostream & 
PrintTo( ostream & xo, const VarSet & varset)
{
  VarSet::const_iterator it = varset.begin();
  xo << "{ ";
  if ( it != varset.end())
    {
    xo << * it;
    ++it;
    }
  for (; it != varset.end(); ++it) 
    {
    xo << ", " << * it;
    }
  xo << " }";
  return xo;
}  

ostream & operator<<( ostream & xo, const VarSet & varset)
{ return PrintTo( xo,varset); }

ostream & 
PrintTo( ostream & xo, const TableauColumnsMap & varmap)
{
  TableauColumnsMap::const_iterator it = varmap.begin();
  for (; it != varmap.end(); ++it) 
    {
    xo << (*it).first << " -> " << (*it).second << endl;
    }
  return xo;
}

ostream & operator<<( ostream & xo, const TableauColumnsMap & varmap)
{ return PrintTo( xo,varmap); }

ostream & 
PrintTo( ostream & xo, const TableauRowsMap & rows)
{
  TableauRowsMap::const_iterator it = rows.begin();
  for (; it != rows.end(); ++it) 
    {
    Variable v = it->first;
    P_LinearExpression pe = it->second;
    xo << v << " <-=-> ";
    if ( pe) xo << * pe; else xo << "NilExpr";
    xo << endl;
    }
  return xo;
}

ostream & operator<<( ostream & xo, const TableauRowsMap & rows)
{ return PrintTo( xo,rows); }

ostream & 
Tableau::PrintOn( ostream & xo) const
{
  xo << "Tableau:\n" 
     << _rows << endl;
  xo << "Columns:\n" 
     << _columns << endl;
  xo << "Infeasible rows: " 
     << _infeasibleRows << endl;
  xo << "External basic variables: "
     << _externalRows << endl;
  xo << "External parametric variables: "
     << _externalParametricVars << endl;
  return xo;
}

ostream & operator<<( ostream & xo, const Tableau & clt)
{ return clt.PrintOn( xo); }

#endif
