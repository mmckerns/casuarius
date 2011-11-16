// $Id: Strength.cc,v 1.1.1.1 2003/01/15 14:06:21 svilen Exp $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Strength.cc

#include "Strength.h"

#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif

// Use the singleton pattern for the strength objects
const Strength & sRequired()
{
  // required is distinct by the isRequired flag,
  // but I still use an especially high symbolic weight, just in case
  static Strength required_strength("<Required>", 1000, 1000, 1000, true);
  return required_strength;
}

const Strength & sStrong()
{
  static Strength strong_strength("strong", 1.0, 0.0, 0.0);
  return strong_strength;
}

const Strength & sMedium()
{
  static Strength medium_strength("medium", 0.0, 1.0, 0.0);
  return medium_strength;
}


const Strength & sWeak()
{
  static Strength weak_strength("weak", 0.0, 0.0, 1.0);
  return weak_strength;
}

// special case for when nLevels = 3, should assert nLevels() == 3
Strength::Strength( const string & Name, double w1, double w2, double w3, bool isRequired) :
  _name( Name), _symbolicWeight( w1, w2, w3), _isRequired(isRequired)
{ 
}
