#ifndef P_CONSTRAINT_H
#define P_CONSTRAINT_H

class Constraint;                   //no need to be known
#include "my/refcntp.h"
REFCOUNT_DECL( Constraint)          //from refcntp.h
typedef RefCountPtr< Constraint> P_Constraint;
//static/stack vars
typedef RefCountPtr_static_holder< Constraint> P_Constraint_holder;

#endif   //P_CONSTRAINT_H
