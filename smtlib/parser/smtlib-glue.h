/**
 * \file smtlib-glue.h
 * \brief Glue between the generated C parser and the C++ implementation.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_GLUE_H
#define SMTLIB_PARSER_GLUE_H

#ifdef __cplusplus
#include "ast/ast_abstract.h"
namespace smtlib {
    namespace ast {
        class AstNode;
    }
    class Parser;
}
class ParserInternalList;

typedef class smtlib::ast::AstNode *SmtPtr;
typedef class ParserInternalList *SmtList;
typedef class smtlib::Parser *SmtPrsr;
#else
typedef void *SmtPtr, *SmtList, *SmtPrsr;
#endif

#ifdef __cplusplus
extern "C" {
#endif

int yylex (void);
int yyparse(SmtPrsr);

void smt_print(SmtPtr ptr);

void smt_setAst(SmtPrsr parser, SmtPtr ast);
void smt_reportError(SmtPrsr parser, unsigned int rowLeft, unsigned int colLeft,
                     unsigned int rowRight, unsigned int colRight, const char* msg);

SmtList smt_listCreate();
void smt_listAdd(SmtList list, SmtPtr item);
void smt_listDelete(SmtList list);

void smt_setLocation(SmtPrsr parser, SmtPtr ptr, int rowLeft, int colLeft, int rowRight, int colRight);

int smt_bool_value(SmtPtr ptr);

// ast_attribute.h
SmtPtr smt_newAttribute1(SmtPtr keyword);
SmtPtr smt_newAttribute2(SmtPtr keyword, SmtPtr attr_value);
SmtPtr smt_newCompAttributeValue(SmtList values);

// ast_basic.h
SmtPtr smt_newSymbol(char const* value);
SmtPtr smt_newKeyword(char const* value);
SmtPtr smt_newMetaSpecConstant(int value);
SmtPtr smt_newBooleanValue(int value);
SmtPtr smt_newPropLiteral(SmtPtr symbol, int negated);

// ast_command.h
SmtPtr smt_newAssertCommand(SmtPtr term);
SmtPtr smt_newCheckSatCommand();
SmtPtr smt_newCheckSatAssumCommand(SmtList assumptions);
SmtPtr smt_newDeclareConstCommand(SmtPtr symbol, SmtPtr sort);
SmtPtr smt_newDeclareDatatypeCommand(SmtPtr symbol, SmtPtr declaration);
SmtPtr smt_newDeclareDatatypesCommand(SmtList sorts, SmtList declarations);
SmtPtr smt_newDeclareFunCommand(SmtPtr symbol, SmtList params, SmtPtr sort);
SmtPtr smt_newDeclareSortCommand(SmtPtr symbol, SmtPtr arity);
SmtPtr smt_newDefineFunCommand(SmtPtr definition);
SmtPtr smt_newDefineFunRecCommand(SmtPtr definition);
SmtPtr smt_newDefineFunsRecCommand(SmtList declarations, SmtList bodies);
SmtPtr smt_newDefineSortCommand(SmtPtr symbol, SmtList params, SmtPtr sort);
SmtPtr smt_newEchoCommand(SmtPtr);
SmtPtr smt_newExitCommand();
SmtPtr smt_newGetAssertsCommand();
SmtPtr smt_newGetAssignsCommand();
SmtPtr smt_newGetInfoCommand(SmtPtr keyword);
SmtPtr smt_newGetModelCommand();
SmtPtr smt_newGetOptionCommand(SmtPtr keyword);
SmtPtr smt_newGetProofCommand();
SmtPtr smt_newGetUnsatAssumsCommand();
SmtPtr smt_newGetUnsatCoreCommand();
SmtPtr smt_newGetValueCommand(SmtList terms);
SmtPtr smt_newPopCommand(SmtPtr numeral);
SmtPtr smt_newPushCommand(SmtPtr numeral);
SmtPtr smt_newResetCommand();
SmtPtr smt_newResetAssertsCommand();
SmtPtr smt_newSetInfoCommand(SmtPtr info);
SmtPtr smt_newSetLogicCommand(SmtPtr logic);
SmtPtr smt_newSetOptionCommand(SmtPtr option);

//smt_datatype.h
SmtPtr smt_newSortDeclaration(SmtPtr symbol, SmtPtr numeral);
SmtPtr smt_newSelectorDeclaration(SmtPtr symbol, SmtPtr sort);
SmtPtr smt_newConstructorDeclaration(SmtPtr symbol, SmtList selectors);
SmtPtr smt_newSimpleDatatypeDeclaration(SmtList constructors);
SmtPtr smt_newParametricDatatypeDeclaration(SmtList params, SmtList constructors);

// ast_fun.h
SmtPtr smt_newFunctionDeclaration(SmtPtr symbol, SmtList params, SmtPtr sort);
SmtPtr smt_newFunctionDefinition(SmtPtr signature, SmtPtr body);

// ast_identifier.h
SmtPtr smt_newSimpleIdentifier1(SmtPtr symbol);
SmtPtr smt_newSimpleIdentifier2(SmtPtr symbol, SmtList indices);
SmtPtr smt_newQualifiedIdentifier(SmtPtr identifier, SmtPtr sort);

// ast_literal.h
SmtPtr smt_newNumeralLiteral(long value, unsigned int base);
SmtPtr smt_newDecimalLiteral(double value);
SmtPtr smt_newStringLiteral(char const* value);

// ast_logic.h
SmtPtr smt_newLogic(SmtPtr name, SmtList attributes);

// ast_match.h
SmtPtr smt_newQualifiedConstructor(SmtPtr symbol, SmtPtr sort);
SmtPtr smt_newQualifiedPattern(SmtPtr constructor, SmtList symbols);
SmtPtr smt_newMatchCase(SmtPtr pattern, SmtPtr term);

// ast_s_exp.h
SmtPtr smt_newCompSExpression(SmtList exprs);

// ast_script.h
SmtPtr smt_newSmtScript(SmtList cmds);

// ast_sort.h
SmtPtr smt_newSort1(SmtPtr identifier);
SmtPtr smt_newSort2(SmtPtr identifier, SmtList params);

// ast_symbol_decl.h
SmtPtr smt_newSortSymbolDeclaration(SmtPtr identifier, SmtPtr arity, SmtList attributes);
SmtPtr smt_newSpecConstFunDeclaration(SmtPtr constant, SmtPtr sort, SmtList attributes);
SmtPtr smt_newMetaSpecConstFunDeclaration(SmtPtr constant, SmtPtr sort, SmtList attributes);
SmtPtr smt_newSimpleFunDeclaration(SmtPtr identifier, SmtList signature, SmtList attributes);
SmtPtr smt_newParametricFunDeclaration(SmtList params, SmtPtr identifier, SmtList signature, SmtList attributes);

// ast_term.h
SmtPtr smt_newQualifiedTerm(SmtPtr identifier, SmtList terms);
SmtPtr smt_newLetTerm(SmtList bindings, SmtPtr term);
SmtPtr smt_newForallTerm(SmtList bindings, SmtPtr term);
SmtPtr smt_newExistsTerm(SmtList bindings, SmtPtr term);
SmtPtr smt_newMatchTerm(SmtPtr term, SmtList cases);
SmtPtr smt_newAnnotatedTerm(SmtPtr term, SmtList attrs);

// ast_theory.h
SmtPtr smt_newTheory(SmtPtr name, SmtList attributes);

// ast_var.h
SmtPtr smt_newSortedVariable(SmtPtr symbol, SmtPtr sort);
SmtPtr smt_newVarBinding(SmtPtr symbol, SmtPtr term);

#ifdef __cplusplus
}; // extern "C"
#endif

#endif //SMTLIB_PARSER_GLUE_H