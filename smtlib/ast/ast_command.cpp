#include "ast_command.h"

#include <sstream>

using namespace std;
using namespace smtlib::ast;

/* ================================== AssertCommand =================================== */

void AssertCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
}

string AssertCommand::toString() {
    stringstream ss;
    ss << "(assert " << term->toString() << ")";
    return ss.str();
}

/* ================================= CheckSatCommand ================================== */

void CheckSatCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
}

string CheckSatCommand::toString() {
    return "(check-sat)";
}

/* =============================== CheckSatAssumCommand =============================== */

CheckSatAssumCommand::CheckSatAssumCommand(sptr_v<PropLiteral>& assumptions) {
    this->assumptions.insert(this->assumptions.end(), assumptions.begin(), assumptions.end());
}

void CheckSatAssumCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
}

string CheckSatAssumCommand::toString() {
    stringstream ss;
    ss << "(check-sat-assuming (";

    for (auto assumIt = assumptions.begin(); assumIt != assumptions.end(); assumIt++) {
        if(assumIt != assumptions.begin())
            ss << " ";
        ss << (*assumIt)->toString();
    }

    ss << "))";

    return ss.str();
}

/* =============================== DeclareConstCommand ================================ */

void DeclareConstCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
}

string DeclareConstCommand::toString() {
    stringstream ss;
    ss << "(declare-const " << symbol->toString() << " " << sort->toString() << ")";
    return ss.str();
}

/* ============================== DeclareDatatypeCommand ============================== */
void DeclareDatatypeCommand::accept(AstVisitor0* visitor) {
    visitor->visit(shared_from_this());
}

string DeclareDatatypeCommand::toString() {
    stringstream ss;
    ss << "(declare-datatype " << symbol->toString() << " " << declaration->toString() << ")";
    return ss.str();
}

/* ============================= DeclareDatatypesCommand ============================== */
DeclareDatatypesCommand::DeclareDatatypesCommand(sptr_v<SortDeclaration>& sorts,
                                                 sptr_v<DatatypeDeclaration>& declarations) {
    this->sorts.insert(this->sorts.begin(), sorts.begin(), sorts.end());
    this->declarations.insert(this->declarations.begin(), declarations.begin(), declarations.end());
}

void DeclareDatatypesCommand::accept(AstVisitor0* visitor) {
    visitor->visit(shared_from_this());
}

string DeclareDatatypesCommand::toString() {
    stringstream ss;
    ss << "( declare-datatypes (";

    bool first = true;
    for (auto sortIt = sorts.begin(); sortIt != sorts.end(); sortIt++) {
        if(first)
            first = false;
        else
            ss << " ";
        ss << (*sortIt)->toString();
    }

    ss << ") (";

    first = true;
    for (auto declIt = declarations.begin(); declIt != declarations.end(); declIt++) {
        if(first)
            first = false;
        else
            ss << " ";
        ss << (*declIt)->toString();
    }

    return ss.str();
}
/* =============================== DeclareFunCommand ================================ */

DeclareFunCommand::DeclareFunCommand(sptr_t<Symbol> symbol,
                                     sptr_v<Sort>& params,
                                     sptr_t<Sort> sort)
        : symbol(symbol), sort(sort) {
    this->params.insert(this->params.end(), params.begin(), params.end());
}

void DeclareFunCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
}

string DeclareFunCommand::toString() {
    stringstream ss;
    ss << "(declare-fun " << symbol->toString() << " (";

    for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++) {
        if(paramIt != params.begin())
            ss << " ";
        ss << (*paramIt)->toString();
    }

    ss << ") " << sort->toString() << ")";

    return ss.str();
}

/* =============================== DeclareSortCommand ================================ */

void DeclareSortCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
}

string DeclareSortCommand::toString() {
    stringstream ss;
    ss << "(declare-sort " << symbol->toString() << " " << arity->toString() << ")";
    return ss.str();
}

/* ================================= DefineFunCommand ================================= */

DefineFunCommand::DefineFunCommand(sptr_t<Symbol> symbol,
                                   sptr_v<SortedVariable>& params,
                                   sptr_t<Sort> sort,
                                   sptr_t<Term> body) {
    definition = make_shared<FunctionDefinition>(symbol, params, sort, body);
}

void DefineFunCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
}

string DefineFunCommand::toString() {
    stringstream ss;
    ss << "(define-fun " << definition->toString() << ")";
    return ss.str();
}

/* ================================ DefineFunRecCommand =============================== */

DefineFunRecCommand::DefineFunRecCommand(sptr_t<Symbol> symbol,
                                         sptr_v<SortedVariable>& params,
                                         sptr_t<Sort> sort,
                                         sptr_t<Term> body) {
    definition = make_shared<FunctionDefinition>(symbol, params, sort, body);
}

void DefineFunRecCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
}

string DefineFunRecCommand::toString() {
    stringstream ss;
    ss << "(define-fun-rec " << definition->toString() << ")";
    return ss.str();
}

/* =============================== DefineFunsRecCommand =============================== */

DefineFunsRecCommand::DefineFunsRecCommand(sptr_v<FunctionDeclaration>& declarations,
                                           sptr_v<Term>& bodies) {
    this->declarations.insert(this->declarations.end(), declarations.begin(), declarations.end());
    this->bodies.insert(this->bodies.end(), bodies.begin(), bodies.end());
}

void DefineFunsRecCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
}

string DefineFunsRecCommand::toString() {
    stringstream ss;
    ss << "(define-funs-rec (";
    for (auto declIt = declarations.begin(); declIt != declarations.end(); declIt++) {
        if(declIt != declarations.begin())
            ss << " ";
        ss << "(" << (*declIt)->toString() << ")";
    }

    ss << ") (";
    for (auto bodyIt = bodies.begin(); bodyIt != bodies.end(); bodyIt++) {
        if(bodyIt != bodies.begin())
            ss << " ";
        ss << "(" << (*bodyIt)->toString() << ")";
    }

    ss << "))";
    return ss.str();
}

/* ================================ DefineSortCommand ================================= */

DefineSortCommand::DefineSortCommand(sptr_t<Symbol> symbol,
                                     sptr_v<Symbol>& params,
                                     sptr_t<Sort> sort)
        : symbol(symbol), sort(sort) {
    this->params.insert(this->params.end(), params.begin(), params.end());
}

void DefineSortCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
}

string DefineSortCommand::toString() {
    stringstream ss;
    ss << "(define-sort " << symbol->toString() << " (";
    for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++) {
        if(paramIt != params.begin())
            ss << " ";
        ss << (*paramIt)->toString();
    }

    ss << ") " << sort->toString() << ")";
    return ss.str();
}

/* =================================== EchoCommand ==================================== */

void EchoCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string EchoCommand::toString() {
    stringstream ss;
    ss << "(echo " << message << ")";
    return ss.str();
}

/* =================================== ExitCommand ==================================== */

void ExitCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string ExitCommand::toString() {
    return "(exit)";
}

/* ================================ GetAssertsCommand ================================= */

void GetAssertsCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string GetAssertsCommand::toString() {
    return "(get-assertions)";
}

/* ================================ GetAssignsCommand ================================= */

void GetAssignsCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string GetAssignsCommand::toString() {
    return "(get-assignments)";
}

/* ================================== GetInfoCommand ================================== */

void GetInfoCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string GetInfoCommand::toString() {
    stringstream ss;
    ss << "(get-info " << flag->toString() << ")";
    return ss.str();
}

/* ================================= GetModelCommand ================================== */

void GetModelCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string GetModelCommand::toString() {
    return "(get-model)";
}

/* ================================= GetOptionCommand ================================= */

void GetOptionCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string GetOptionCommand::toString() {
    stringstream ss;
    ss << "(get-option " << option->toString() << ")";
    return ss.str();
}

/* ================================= GetProofCommand ================================== */

void GetProofCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string GetProofCommand::toString() {
    return "(get-proof)";
}

/* ============================== GetUnsatAssumsCommand =============================== */

void GetUnsatAssumsCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string GetUnsatAssumsCommand::toString() {
    return "(get-unsat-assumptions)";
}

/* =============================== GetUnsatCoreCommand ================================ */

void GetUnsatCoreCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string GetUnsatCoreCommand::toString() {
    return "(get-unsat-core)";
}

/* ================================= GetValueCommand ================================== */

GetValueCommand::GetValueCommand(sptr_v<Term>& terms) {
    this->terms.insert(this->terms.end(), terms.begin(), terms.end());
}

void GetValueCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string GetValueCommand::toString() {
    stringstream ss;
    ss << "(get-value (";

    for (auto termIt = terms.begin(); termIt != terms.end(); termIt++) {
        if(termIt != terms.begin())
            ss << " ";
        ss << (*termIt)->toString();
    }

    ss << "))";
    return ss.str();
}

/* =================================== PopCommand ==================================== */

void PopCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string PopCommand::toString() {
    stringstream ss;
    ss << "(pop " << numeral->toString() << ")";
    return ss.str();
}

/* =================================== PushCommand ==================================== */

void PushCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string PushCommand::toString() {
    stringstream ss;
    ss << "(push " << numeral->toString() << ")";
    return ss.str();
}

/* =================================== ResetCommand =================================== */

void ResetCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string ResetCommand::toString() {
    return "(reset)";
}

/* =============================== ResetAssertsCommand ================================ */

void ResetAssertsCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string ResetAssertsCommand::toString() {
    return "(reset-assertions)";
}

/* ================================== SetInfoCommand ================================== */

void SetInfoCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string SetInfoCommand::toString() {
    stringstream ss;
    ss << "(set-info " << info->getKeyword()->toString()
       << " " << info->getValue()->toString() << ")";
    return ss.str();
}

/* ================================= SetLogicCommand ================================== */

void SetLogicCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string SetLogicCommand::toString() {
    stringstream ss;
    ss << "(set-logic " << logic->toString() << ")";
    return ss.str();
}

/* ================================= SetOptionCommand ================================= */

void SetOptionCommand::accept(AstVisitor0* visitor){
    visitor->visit(shared_from_this());
} 

string SetOptionCommand::toString() {
    stringstream ss;
    ss << "(set-option " << option->getKeyword()->toString()
       << " " << option->getValue()->toString() << ")";
    return ss.str();
}