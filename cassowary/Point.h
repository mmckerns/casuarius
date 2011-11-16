// $Id: Point.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Point.h

#ifndef Point_H
#define Point_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "Variable.h"

// Point is just a convenience class for pairs of
// Variables -- often useful for coordinate pairs in 2-space
class Point {
 public:
  Point( Number x, Number y)
    : _clv_x( x), _clv_y( y)
    { }

  Point()
    { }

  Variable X()
    { return _clv_x; }

  Variable Y()
    { return _clv_y; }

  const Variable X() const
    { return _clv_x; }

  const Variable Y() const
    { return _clv_y; }

  void SetXY( Number x, Number y)
    { _clv_x.SetValue( x); _clv_y.SetValue( y); }

  Number Xvalue() const
    { return X().Value(); }

  Number Yvalue() const
    { return Y().Value(); }

 private:
  Variable _clv_x;
  Variable _clv_y;

#ifndef CL_NO_IO
  friend ostream & operator<<( ostream & xo, const Point & clp);
#endif
  
};

#ifndef CL_NO_IO
inline ostream & 
operator<<( ostream & xo, const Point & clp)
{
  xo << "(" << clp._clv_x << ", " << clp._clv_y << ")";
  return xo;
}
#endif

#endif
