// $Id: AbstractVariable.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// AbstractVariable.h

#ifndef AbstractVariable_H
#define AbstractVariable_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "Errors.h"
#include "my/refcnt.h"

#ifdef USE_GC
class AbstractVariable : public gc {
#else
class AbstractVariable {
#endif
    REFCOUNT_DEF                 //from nref.h
protected:        
  int _flags;
  enum { E_IsFloatVariable =1,
         E_IsFDVariable =2,
         E_IsDummy =4,
         E_IsExternal =8,
  };
public:
  AbstractVariable( string Name = "");
  AbstractVariable( long varnumber, char * prefix);
  virtual ~AbstractVariable();
  /*struct Flags {
    bool IsFloatVariable :1;
    bool IsFDVariable    :1;
    bool IsDummy         :1;
    bool IsExternal      :1;
  } _flags;
  */
  
  // Return the Name of the variable
  string Name() const { return _name; }

  // Set the Name of the variable
  virtual void SetName( const string & Name) { _name = Name; }

  // Return true iff this variable is a FloatVariable
  bool IsFloatVariable() const { return _flags & E_IsFloatVariable; }

  // Return true iff this variable is a FDVariable
  bool IsFDVariable() const { return _flags & E_IsFDVariable; }

  // Return true if this a dummy variable ( used as a marker variable
  // for required equality constraints).  Such variables aren't
  // allowed to enter the basis when pivoting.
  bool IsDummy() const { return _flags & E_IsDummy; }

  // Return true if this a variable known outside the solver.  
  // ( We need to give such variables a Value after solving is complete.)
  bool IsExternal() const { return _flags & E_IsExternal; }

  // Return true if we can Pivot on this variable.
  virtual bool IsPivotable() const
    { throw ExCLTooDifficultSpecial("Variable not usable inside SimplexSolver"); return false; }

  // Return true if this is a restricted ( or slack) variable.  Such
  // variables are constrained to be non-negative and occur only
  // internally to the simplex solver.
  virtual bool IsRestricted() const
    { throw ExCLTooDifficultSpecial("Variable not usable inside SimplexSolver"); return false; }

#ifndef CL_NO_IO
  // Prints a semi-descriptive representation to the stream, using the
  // Name if there is one, and otherwise the hash number of this
  // object.
  //	EXAMPLES
  //	  x[10.0]		-- w/ Name
  //	  x[0.0,100]		-- w/ Name, bounds but no Value yet
  //	  CV#345( 10.0)		-- w/o Name
  virtual ostream & PrintOn( ostream & xo) const = 0;

  friend ostream& operator<<( ostream & xos, const AbstractVariable & clv)
    { clv.PrintOn( xos); return xos; }

#endif // CL_NO_IO

  friend bool operator<( const AbstractVariable & cl1, const AbstractVariable & cl2)
    { return & cl1 < & cl2; }

  bool operator == ( const AbstractVariable & cl2) const { return this == &cl2; } 
  bool operator != ( const AbstractVariable & cl2) const { return this != &cl2; }

  virtual Number Value() const { return 0; }
  virtual int IntValue() const { return 0; }
  virtual void SetValue( Number) { assert( false); } 
  virtual void ChangeValue( Number) { assert( false); }

#ifdef CL_PV
  void SetPv( void * pv) { _pv = pv; } 
  void * Pv() const { return _pv; }
#endif  

private:
  string _name;

  static long iVariableNumber;

#ifdef CL_PV
  // C-style extension mechanism so I
  // don't have to wrap ScwmVariables separately
  void * _pv;
#endif  
};

//class AbstractVariable;                   //no need to be known
#include "my/refcntp.h"
REFCOUNT_DECL( AbstractVariable)          //from refcntp.h
typedef RefCountPtr< AbstractVariable> P_AbstractVariable;
//static/stack vars
typedef RefCountPtr_static_holder< AbstractVariable> P_AbstractVariable_holder;

#endif
