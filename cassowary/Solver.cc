// $Id: Solver.cc,v 1.1.1.1 2003/01/15 14:06:21 svilen Exp $

#ifdef HAVE_CONFIG_H
#include <cassowary/config.h>
#define CONFIG_H_INCLUDED
#endif

#include "Cassowary.h"
#include "Solver.h"
#include "Constraint.h"
#include "Errors.h"
#include "Typedefs.h"


#ifndef CL_NO_IO
ostream & 
PrintTo( ostream & xo, const ConstraintSet & setCn)
{
  ConstraintSet::const_iterator it = setCn.begin();
  for (; it != setCn.end(); ++it) {
    const Constraint * pcn = *it;
    xo << *pcn << endl;
  }
  return xo;
}  

ostream & 
PrintTo( ostream & xo, const list<FDNumber> & listFDN)
{
  list<FDNumber>::const_iterator it = listFDN.begin();
  for (; it != listFDN.end(); ) {
    FDNumber n = * it;
    xo << n;
    ++it;
    if ( it != listFDN.end())
      xo << ",";
  }
  return xo;
}  


ostream & operator<<( ostream & xo, const ConstraintSet & setCn)
{ return PrintTo( xo,setCn); }


ostream & operator<<( ostream & xo, const Solver & solver)
{ return solver.PrintOn( xo); }

ostream & operator<<( ostream & xo, const list<FDNumber> & listFDN)
{ return PrintTo( xo,listFDN); }

#endif
