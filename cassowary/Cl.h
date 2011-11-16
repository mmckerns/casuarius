// $Id: Cl.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// .h
// This is the top level include file for external clients

#ifndef CL_H
#define CL_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#ifdef CL_NO_IO
#undef CL_TRACE
#undef CL_SOLVER_STATS
#undef CL_DEBUG_FAILURES
#undef CL_TRACE_VERBOSE
#endif

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "cassowary/Variable.h"
#include "cassowary/SimplexSolver.h"
#include "cassowary/LinearEquation.h"
#include "cassowary/LinearInequality.h"
#include "cassowary/Errors.h"
#include "cassowary/EditConstraint.h"
#include "cassowary/StayConstraint.h"
//#include "cassowary/Reader.h"
#include "cassowary/Constraint.h"
#if defined( CL_HAVE_GTL) && defined( CL_BUILD_FD_SOLVER)
#include "cassowary/FDBinaryOneWayConstraint.h"
#include "cassowary/FDSolver.h"
#endif

extern const char * szCassowaryVersion;

#endif
