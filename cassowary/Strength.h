// $Id: Strength.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Strength.h

#ifndef Strength_H
#define Strength_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "SymbolicWeight.h"

class Strength;

const Strength & sRequired();
const Strength & sStrong();
const Strength & sMedium();
const Strength & sWeak();

#ifdef USE_GC_STRENGTH
class Strength : public gc {
#else
class Strength {
#endif
 public:

  Strength( const string & Name, const SymbolicWeight & symbolicWeight, bool isRequired=false) :
    _name( Name), _symbolicWeight( symbolicWeight), _isRequired( isRequired)
    { }

  // special case for when nLevels = 3, should assert nLevels() == 3
  Strength( const string & Name, double w1, double w2, double w3, bool isRequired=false);

  virtual ~Strength()
    { }

  virtual bool IsRequired() const
    { return _isRequired; }

#ifndef CL_NO_IO
  virtual ostream & PrintOn( ostream & xo) const
    { 
    xo << Name(); 
    if (!IsRequired())
      xo << ":" << symbolicWeight(); 
    return xo; 
    }

  friend ostream& operator<<( ostream & xos, const Strength & s)
    { s.PrintOn( xos); return xos; }

#endif

  virtual const SymbolicWeight & symbolicWeight() const
    { return _symbolicWeight; }

#ifdef CL_PV
  void SetPv( void * pv) { _pv = pv; } 
  void * Pv() const { return _pv; }
#endif

 private:
  string Name() const
    { return _name; }

  void SetName( string Name)
    { _name = Name; }

  void SetSymbolicWeight( const SymbolicWeight & symbolicWeight)
    { _symbolicWeight = symbolicWeight; }

  // instance variables
  string _name;
  SymbolicWeight _symbolicWeight;
  bool _isRequired;
#ifdef CL_PV
  void * _pv;
#endif
};

#endif
