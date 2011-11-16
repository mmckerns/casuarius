//s.dobrev '10.2k2
#include "Errors.h"
#include <string>

ExCLError::ExCLError( const char * def, string msg ) 
    : _def( def)
{
    if ( def) _msg = def;
	if ( def && !msg.empty()) _msg += "\n\t";
    _msg += msg;    //stupid <string> cant handle nullptrs
}
    
ExCLError::~ExCLError() throw() {} // if ( _msg) free( _msg); }

string ExCLError::description() const {
    if (!_msg.empty()) return _msg;
    if ( _def) return _def;
    return "ExCLError: An error has occured in CL"; 
}

#include <sstream>
#include "Constraint.h"

static string _AddConstraint( P_Constraint cnExpl) {
    ostringstream ss;
    ss << endl;
    if (cnExpl)
        ss << *cnExpl.ptr() << ends;
    else ss << "0!" << ends;
    return ss.str();
}

void ExCLConstraintNotFound::AddConstraint( P_Constraint cnExpl) {
    _msg += _AddConstraint( cnExpl); 
}

void ExCLRequiredFailureWithExplanation::AddConstraint( P_Constraint cnExpl) {
    _explanation.insert( cnExpl); 
    _msg += _AddConstraint( cnExpl); 
}
