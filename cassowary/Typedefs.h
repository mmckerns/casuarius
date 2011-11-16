// $Id: Typedefs.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Typedefs.h

#ifndef CL_TYPEDEFS_H__
#define CL_TYPEDEFS_H__

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Map.h"
#include "Set.h"
#include "LinearExpression_fwd.h"
#include <set> // Since TableauVarSet is always a set ( never a hash_set)
#include <vector>

using namespace std;
class Variable;
#include "Constraint_P.h"
//class Constraint;
class EditInfo;

// For Tableau
// TableauVarSet is not a Set, for performance reasons 
// ( Steve Wolfman discovered this, and seems to be true --02/17/99 gjb)
// I have not observed any big performance gains from using the hashtable based containers 
typedef Set<Variable> VarSet;  
typedef Map<Variable, VarSet > TableauColumnsMap;
typedef Map<Variable, P_LinearExpression > TableauRowsMap;

// For Solver
typedef Map<P_Constraint, VarSet> ConstraintToVarSetMap;
typedef Map<P_Constraint, Variable> ConstraintToVarMap;
typedef Map<Variable, P_Constraint > VarToConstraintMap;
typedef vector<Variable> VarVector;

typedef Set<P_Constraint > ConstraintSet;

// For FDSolver
typedef Map<Variable, ConstraintSet> VarToConstraintSetMap;

#endif
