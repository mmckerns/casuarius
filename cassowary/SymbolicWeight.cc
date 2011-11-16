// $Id: SymbolicWeight.cc,v 1.1.1.1 2003/01/15 14:06:21 svilen Exp $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// SymbolicWeight.cc

#include "Cassowary.h"
#include "SymbolicWeight.h"
#include "Variable.h"

#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif

// Help g++ out, tell it to instantiate this
//template vector<double> & vector<double>::operator =( const vector<double> & );

SymbolicWeight::SymbolicWeight() :
  _values( 3,0)
{ 
  assert( _values.size() == 3);
}
SymbolicWeight::SymbolicWeight( int CLevels, double value) :
  _values( CLevels, value)
{ 
  assert( _values.size() == CLevels);
}

SymbolicWeight::SymbolicWeight( double w1, double w2, double w3)
{
  _values.push_back( w1);
  _values.push_back( w2);
  _values.push_back( w3);
  assert( _values.size() == 3);
}

SymbolicWeight::SymbolicWeight( const vector<double> & weights) :
  _values( weights)
{ }

SymbolicWeight & 
SymbolicWeight::Zero()
{
  static SymbolicWeight Zero( 0.0, 0.0, 0.0);
  return Zero;
}


SymbolicWeight & 
SymbolicWeight::negated()
{
  vector<double>::iterator it = _values.begin();
  for (; it != _values.end(); ++it)
    {
    * it = -*it;
    }
  return * this;
}

SymbolicWeight & 
SymbolicWeight::MultiplyMe( Number n)
{
  vector<double>::iterator it = _values.begin();
  for (; it != _values.end(); ++it)
    {
    * it *= n;
    }
  return * this;
}


SymbolicWeight 
SymbolicWeight::DivideBy( Number n) const
{
  assert( n!=0);
  SymbolicWeight clsw( 0,0);
  vector<double>::const_iterator i = _values.begin();
  for (; i != _values.end(); ++i)
    {
    clsw.push_back(*i / n);
    }
  return clsw;
}

SymbolicWeight & 
SymbolicWeight::addtoMe( const SymbolicWeight & cl)
{
  assert( cl.CLevels() == CLevels());

  vector<double>::iterator i1 = _values.begin();
  vector<double>::const_iterator i2 = cl._values.begin();
  for (; i1 != _values.end(); ++i1, ++i2)
    {
    * i1 += * i2;
    }
  return * this;
}

SymbolicWeight 
SymbolicWeight::Subtract( const SymbolicWeight & cl) const
{
  assert( cl.CLevels() == CLevels());

  SymbolicWeight clsw( 0,0);
  vector<double>::const_iterator i1 = _values.begin();
  vector<double>::const_iterator i2 = cl._values.begin();
  for (; i1 != _values.end(); ++i1, ++i2)
    {
    clsw.push_back(*i1 - * i2);
    }
  return clsw;
}


bool 
SymbolicWeight::lessThan( const SymbolicWeight & cl) const
{
  return _values < cl._values;
}

bool 
SymbolicWeight::lessThanOrEqual( const SymbolicWeight & cl) const
{
  return _values <= cl._values;
}

bool 
SymbolicWeight::equal( const SymbolicWeight & cl) const
{
  return _values == cl._values;
}

bool 
SymbolicWeight::greaterThan( const SymbolicWeight & cl) const
{
  return _values > cl._values;
}

bool 
SymbolicWeight::greaterThanOrEqual( const SymbolicWeight & cl) const
{
  return _values >= cl._values;
}

bool 
SymbolicWeight::isNegative() const
{
  return _values < Zero()._values;
}


bool SymbolicWeight::Approx( Number n) const 
{ const SymbolicWeight & cl = * this;
  vector<Number>::const_iterator it = cl._values.begin();
  if (!::Approx(*it,n))
    return false;

  ++it;
  for (; it != cl._values.end(); ++it)
    {
    if (!::Approx(*it,0))
      return false;
    }

  return true;
}

bool SymbolicWeight::Approx( const SymbolicWeight & cl2) const 
{ const SymbolicWeight & cl1= * this;
  vector<Number>::const_iterator it1 = cl1._values.begin();
  vector<Number>::const_iterator it2 = cl2._values.begin();

  for (; it1 != cl1._values.end() && it2 != cl2._values.end(); 
       ++it1, ++it2)
    {
    if (!::Approx(*it1,*it2))
      return false;
    }

  return ( it1 == cl1._values.end() && it2 == cl2._values.end());
}
