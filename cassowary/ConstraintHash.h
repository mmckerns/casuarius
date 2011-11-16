// $Id: ConstraintHash.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Hash.h

#ifndef CL_HASH_H__
#define CL_HASH_H__

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#ifdef CL_USE_HASH_MAP_AND_SET

#include <hash_map>

//class Constraint;
#include "Constraint_P.h"

struct hash< P_Constraint > {
  size_t operator()( P_const Constraint p) const 
  {   p = p.ptr();
        return size_t(( unsigned long)p/CL_PTR_HASH_DIVISOR); }
};

struct hash<P_Constraint > {
  size_t operator()( P_Constraint p) const 
  {   p = p.ptr();
        return size_t(( unsigned long)p/CL_PTR_HASH_DIVISOR); }
};

#endif // CL_USE_HASH_MAP_AND_SET


#endif
