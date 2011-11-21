// $Id: SymbolicWeight.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// SymbolicWeight.h

#ifndef SymbolicWeight_H
#define SymbolicWeight_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "Errors.h"
#include <vector>

#ifdef USE_GC_WEIGHT
class SymbolicWeight : public gc {
#else
class SymbolicWeight {
#endif
 public:
  SymbolicWeight();
  SymbolicWeight( int CLevels /* = 3*/, Number value /* = 0.0*/);

  SymbolicWeight( Number w1, Number w2, Number w3);

  SymbolicWeight( const vector<Number> & weights);

  static SymbolicWeight & Zero();

  SymbolicWeight & negated();

  SymbolicWeight & MultiplyMe( Number n);

  SymbolicWeight Times( Number n) const
    { SymbolicWeight cl = * this; cl.MultiplyMe( n); return cl; }

  SymbolicWeight DivideBy( Number n) const;

  SymbolicWeight & addtoMe( const SymbolicWeight & cl);

  SymbolicWeight Add( const SymbolicWeight & cl) const
    { SymbolicWeight clRet = * this; clRet.addtoMe( cl); return clRet; }

  SymbolicWeight Subtract( const SymbolicWeight & cl) const;

  SymbolicWeight operator*( const Number & n) const
    { return Times( n); }

  SymbolicWeight operator/( const Number & n) const
    { return DivideBy( n); }

  // FIXGJB: can we express this statically?
  SymbolicWeight operator*( SymbolicWeight & w) const
    { throw ExCLInternalError("Multiplication of symbolic weights encountered"); 
      return w; }
  SymbolicWeight & operator*=( SymbolicWeight & w)
    { throw ExCLInternalError("Multiplicative assignment of symbolic weights encountered"); 
      return w; }

  // FIXGJB: can we express this statically?
  SymbolicWeight operator-() const
    { throw ExCLInternalError("Can not negate a symbolic weight");
      return SymbolicWeight::Zero(); }

  friend SymbolicWeight ReciprocalOf( const SymbolicWeight & );

  SymbolicWeight & operator*=( const Number & n)
    { return MultiplyMe( n); }

  SymbolicWeight operator+( const SymbolicWeight & cl) const
    { return Add( cl); }

  SymbolicWeight operator+=( const SymbolicWeight & cl)
    { return addtoMe( cl); }

  SymbolicWeight operator*( const Number & n)
    { SymbolicWeight answer(*this);
      answer *= n;
      return answer; }

  bool lessThan( const SymbolicWeight & cl) const;
  bool lessThanOrEqual( const SymbolicWeight & cl) const;
  bool equal( const SymbolicWeight & cl) const;
  bool greaterThan( const SymbolicWeight & cl) const;
  bool greaterThanOrEqual( const SymbolicWeight & cl) const;
  bool isNegative() const;

  friend bool operator==( const SymbolicWeight & cl1, const SymbolicWeight & cl2)
    { return cl1.equal( cl2); }

  friend bool operator!=( const SymbolicWeight & cl1, const SymbolicWeight & cl2)
    { return !( cl1 == cl2); }

  friend bool operator<( const SymbolicWeight & cl1, const SymbolicWeight & cl2)
    { return cl1.lessThan( cl2); }

  friend bool operator>( const SymbolicWeight & cl1, const SymbolicWeight & cl2)
  { return ( cl2 < cl1); }

  // function.h provides operator>, >=, <= from operator<

  double AsDouble() const
    {
    vector<Number>::const_reverse_iterator i = _values.rbegin();
    Number sum  = 0;
    Number factor = 1;
    Number multiplier = 1000;
    for ( ; i != _values.rend(); ++i) 
      {
      sum += * i * factor;
      factor *= multiplier;
      }
    return sum;
    }

#ifndef CL_NO_IO
  ostream & PrintOn( ostream & xo) const
    { 
    vector<Number>::const_iterator i = _values.begin();
    if ( i == _values.end())
      return xo;

    xo << * i;
    for (++i; i != _values.end(); ++i) 
      {
      xo << "," << * i;
      }
    return xo;
    }

  // FIXGJB: use a template function to generate these automatically
  friend ostream& operator<<( ostream & xos, const SymbolicWeight & clsw)
    { clsw.PrintOn( xos); return xos; }
#endif

  int CLevels() const
    { return _values.size(); }

//  friend bool Approx( const SymbolicWeight & cl, Number n);
//  friend bool Approx( const SymbolicWeight & cl1, const SymbolicWeight & cl2);
    bool Approx( Number n) const;
    bool Approx( const SymbolicWeight & cl2) const;

 private:
  vector<Number> _values;

  void push_back( Number d) 
    { _values.push_back( d); }

};

inline bool Approx( const SymbolicWeight & cl, Number n)
{   return cl.Approx( n); }

inline bool Approx( const SymbolicWeight & cl1, const SymbolicWeight & cl2)
{   return cl1.Approx( cl2); }

inline SymbolicWeight ReciprocalOf( const SymbolicWeight & )
{ throw( ExCLInternalError("Cannot take ReciprocalOf symbolic weight"));
  return SymbolicWeight::Zero(); }

#endif
