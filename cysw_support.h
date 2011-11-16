#include <string>
#include <sstream>
#include "cassowary/SimplexSolver.h"
#include "cassowary/Errors.h"
#include "cassowary/LinearExpression.h"
#include "cassowary/Constraint.h"



std::string get_cpp_exception_message();
std::string solver_str(SimplexSolver *solver);
P_Constraint *newLinearEquation(const P_LinearExpression &lhs, const P_LinearExpression &rhs, const Strength &strength, double weight);
P_Constraint *newLinearInequality(const P_LinearExpression &lhs, CnRelation op, const P_LinearExpression &rhs, const Strength &strength, double weight);
P_LinearExpression newLinearExpression(double constant);
void delete_P_Constraint(P_Constraint *pcn);

