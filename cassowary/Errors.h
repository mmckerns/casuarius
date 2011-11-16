// $Id: Errors.h 172 2007-11-23 11:00:57Z svilen_dobrev $
//
// Cassowary Incremental Constraint Solver
// Original Smalltalk Implementation by Alan Borning
// This C++ Implementation by Greg J. Badros, <gjb@cs.washington.edu>
// http://www.cs.washington.edu/homes/gjb
// ( C) 1998, 1999 Greg J. Badros and Alan Borning
// See ../LICENSE for legal details regarding this software
//
// Errors.h

#ifndef Errors_H
#define Errors_H

#if defined( HAVE_CONFIG_H) && !defined( CONFIG_H_INCLUDED) && !defined( CONFIG_INLINE_H_INCLUDED)
#include <cassowary/config-inline.h>
#define CONFIG_INLINE_H_INCLUDED
#endif

#include "Cassowary.h"
#include "Set.h"
#include "Typedefs.h"
#include <stdexcept>

class ExCLError : public exception
#ifdef USE_GC
, public gc
#endif
{
 public:
  ExCLError( const char * def, string msg =string());
  ~ExCLError() throw ();
  virtual const ConstraintSet * explanation() const { return 0; }
  string description() const;
 protected:
  string _msg;
  const char * _def;
};

class ExCLInternalError : public ExCLError {
 public:
  ExCLInternalError( string sz) : ExCLError(
        "ExCLInternalError: An internal error has occurred"
        , sz) {}
};

class ExCLBadResolve : public ExCLError {
 public:
  ExCLBadResolve( string sz) : ExCLError(
        "ExCLBadResolve: Number of resolve values did not match number of edit vars"
        , sz) {}
};

class ExCLEditMisuse : public ExCLError {
 public:
  ExCLEditMisuse( string sz) : ExCLError(
        "ExCLEditMisuse: Edit protocol usage violation"
        , sz) {}
};


class ExCLTooDifficult : public ExCLError {
 protected:
     ExCLTooDifficult( const char * def, string sz = string())
         : ExCLError( def,sz ) {}
 public:
     ExCLTooDifficult() : ExCLError(
        "ExCLTooDifficult: The constraints are too difficult to solve"
        ) {}
};

class ExCLTooDifficultSpecial : public ExCLTooDifficult {
 public:
  ExCLTooDifficultSpecial( string sz) : ExCLTooDifficult(
        "ExCLTooDifficultSpecial: Solver requirements are not satisfied"
        , sz) {}
};

class ExCLReadOnlyNotAllowed : public ExCLTooDifficult {
 public:
    ExCLReadOnlyNotAllowed() : ExCLTooDifficult(
        "ExCLReadOnlyNotAllowed: The read-only annotation is not permitted by the solver"
        ) {}
};

class ExCLCycleNotAllowed : public ExCLTooDifficult {
 public:
    ExCLCycleNotAllowed() : ExCLTooDifficult(
        "ExCLCycleNotAllowed: A cyclic constraint graph is not permitted by the solver"
        ) {}
};

class ExCLStrictInequalityNotAllowed : public ExCLTooDifficult {
 public:
    ExCLStrictInequalityNotAllowed() : ExCLTooDifficult(
        "ExCLStrictInequalityNotAllowed: The strict inequality is not permitted by the solver"
        ) {}
};

class ExCLRequiredFailure : public ExCLError {
 protected:
    ExCLRequiredFailure( const char * def) : ExCLError( def ) {}
 public:
    ExCLRequiredFailure() : ExCLError(
        "ExCLRequiredFailure: A required constraint cannot be satisfied"
        ) {}
};

class ExCLNotEnoughStays : public ExCLError {
 public:
    ExCLNotEnoughStays() : ExCLError(
        "ExCLNotEnoughStays: There are not enough stays to give specific values to every variable"
        ) {}
};

class ExCLNonlinearExpression : public ExCLError {
 public:
    ExCLNonlinearExpression() : ExCLError(
        "ExCLNonlinearExpression: The resulting expression would be nonlinear"
        ) {}
};

class ExCLConstraintNotFound : public ExCLError {
 public:
    ExCLConstraintNotFound( P_Constraint cnExpl ) : ExCLError(
        "ExCLConstraintNotFound: Tried to remove a constraint that was never added"
        ) { AddConstraint( cnExpl ); }

    void AddConstraint( P_Constraint cnExpl);
};

class ExCLParseError : public ExCLError {
 public:
    ExCLParseError() : ExCLError(
        "ExCLParseError"
        ) {}
};

class ExCLParseErrorMisc : public ExCLParseError {
 public:
    ExCLParseErrorMisc( const string & s) {
        _msg += "Misc: ";
        _msg += s;
    }
};

class ExCLParseErrorBadIdentifier : public ExCLParseError {
 public:
    ExCLParseErrorBadIdentifier( const string & id) {
        _msg += "BadIdentifier: Did not recognize identifier '";
        _msg += id;
        _msg += "'";
    }
};


class ExCLRequiredFailureWithExplanation : public ExCLRequiredFailure {
public:
    ExCLRequiredFailureWithExplanation() : ExCLRequiredFailure(
        "ExCLRequiredFailureWithExplanation: A required constraint cannot be satisfied"
        ) {}
  ~ExCLRequiredFailureWithExplanation() throw () {}
  void AddConstraint( P_Constraint cnExpl);
  const ConstraintSet * explanation() const { return & _explanation; }
protected:
  ConstraintSet _explanation;
};

#endif
