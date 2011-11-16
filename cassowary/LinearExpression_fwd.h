// $Id: LinearExpression_fwd.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// LinearExpression.h

#ifndef LinearExpression_fwd_H
#define LinearExpression_fwd_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"

template <class T> class GenericLinearExpression;
typedef GenericLinearExpression<Number> LinearExpression;

#include "my/refcntp.h"
REFCOUNT_DECL( LinearExpression)          //from refcntp.h
typedef RefCountPtr< LinearExpression> P_LinearExpression;
//static/stack vars
typedef RefCountPtr_static_holder< LinearExpression> P_LinearExpression_holder; 

#endif
