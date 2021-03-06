/**
 * \file ast_term_sorter.h
 * \brief Visitor for checking syntax correctness.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_SYNTAX_CHECKER_H
#define SMTLIB_PARSER_AST_SYNTAX_CHECKER_H

#include "ast_visitor.h"

#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

namespace smtlib {
    namespace ast {
        /** Visitor for checking syntax correctness */
        class SyntaxChecker : public DummyAstVisitor0 {
        private:
            struct Error {
                std::vector<std::string> messages;
                sptr_t<AstNode> node;

                Error() { }

                Error(std::string message, sptr_t<AstNode> node) : node(node) {
                    messages.push_back(message);
                }

                Error(std::vector<std::string>& messages, sptr_t<AstNode> node) : node(node) {
                    this->messages.insert(this->messages.begin(), messages.begin(), messages.end());
                }
            };

            sptr_v<Error> errors;

            const std::regex regexSymbol = std::regex(
                    "^([a-zA-Z+\\-/*=%?!.$_~&^<>@][a-zA-Z0-9+\\-/*=%?!.$_~&^<>@]*)"
                            "|(\\|[\\x20-\\x5B\\x5D-\\x7B\\x7D\\x7E\\xA0-\\xFF\\x09\\r\\n \\xA0]*\\|)$"
            );
            const std::regex regexKeyword = std::regex(
                    "^:([a-zA-Z+\\-/*=%?!.$_~&^<>@][a-zA-Z0-9+\\-/*=%?!.$_~&^<>@]*)"
                            "|(\\|[\\x20-\\x5B\\x5D-\\x7B\\x7D\\x7E\\xA0-\\xFF\\x09\\r\\n \\xA0]*\\|)$"
            );

            sptr_t<Error> addError(std::string message, sptr_t<AstNode> node,
                                                       sptr_t<Error> err);

            sptr_t<Error> checkParamUsage(sptr_v<Symbol> &params,
                                          std::unordered_map<std::string, bool> &paramUsage,
                                          sptr_t<Sort> sort,
                                          sptr_t<AstNode> source,
                                          sptr_t<Error> err);
        public:
            virtual void visit(sptr_t<Attribute> node);
            virtual void visit(sptr_t<CompAttributeValue> node);

            virtual void visit(sptr_t<Symbol> node);
            virtual void visit(sptr_t<Keyword> node);
            virtual void visit(sptr_t<MetaSpecConstant> node);
            virtual void visit(sptr_t<BooleanValue> node);
            virtual void visit(sptr_t<PropLiteral> node);

            virtual void visit(sptr_t<AssertCommand> node);
            virtual void visit(sptr_t<CheckSatCommand> node);
            virtual void visit(sptr_t<CheckSatAssumCommand> node);
            virtual void visit(sptr_t<DeclareConstCommand> node);
            virtual void visit(sptr_t<DeclareDatatypeCommand> node);
            virtual void visit(sptr_t<DeclareDatatypesCommand> node);
            virtual void visit(sptr_t<DeclareFunCommand> node);
            virtual void visit(sptr_t<DeclareSortCommand> node);
            virtual void visit(sptr_t<DefineFunCommand> node);
            virtual void visit(sptr_t<DefineFunRecCommand> node);
            virtual void visit(sptr_t<DefineFunsRecCommand> node);
            virtual void visit(sptr_t<DefineSortCommand> node);
            virtual void visit(sptr_t<EchoCommand> node);
            virtual void visit(sptr_t<ExitCommand> node);
            virtual void visit(sptr_t<GetAssertsCommand> node);
            virtual void visit(sptr_t<GetAssignsCommand> node);
            virtual void visit(sptr_t<GetInfoCommand> node);
            virtual void visit(sptr_t<GetModelCommand> node);
            virtual void visit(sptr_t<GetOptionCommand> node);
            virtual void visit(sptr_t<GetProofCommand> node);
            virtual void visit(sptr_t<GetUnsatAssumsCommand> node);
            virtual void visit(sptr_t<GetUnsatCoreCommand> node);
            virtual void visit(sptr_t<GetValueCommand> node);
            virtual void visit(sptr_t<PopCommand> node);
            virtual void visit(sptr_t<PushCommand> node);
            virtual void visit(sptr_t<ResetCommand> node);
            virtual void visit(sptr_t<ResetAssertsCommand> node);
            virtual void visit(sptr_t<SetInfoCommand> node);
            virtual void visit(sptr_t<SetLogicCommand> node);
            virtual void visit(sptr_t<SetOptionCommand> node);

            virtual void visit(sptr_t<FunctionDeclaration> node);
            virtual void visit(sptr_t<FunctionDefinition> node);

            virtual void visit(sptr_t<SimpleIdentifier> node);
            virtual void visit(sptr_t<QualifiedIdentifier> node);

            virtual void visit(sptr_t<DecimalLiteral> node);
            virtual void visit(sptr_t<NumeralLiteral> node);
            virtual void visit(sptr_t<StringLiteral> node);

            virtual void visit(sptr_t<Logic> node);
            virtual void visit(sptr_t<Theory> node);
            virtual void visit(sptr_t<Script> node);

            virtual void visit(sptr_t<Sort> node);

            virtual void visit(sptr_t<CompSExpression> node);

            virtual void visit(sptr_t<SortSymbolDeclaration> node);
            virtual void visit(sptr_t<SpecConstFunDeclaration> node);
            virtual void visit(sptr_t<MetaSpecConstFunDeclaration> node);
            virtual void visit(sptr_t<SimpleFunDeclaration> node);
            virtual void visit(sptr_t<ParametricFunDeclaration> node);
            virtual void visit(sptr_t<SortDeclaration> node);

            virtual void visit(sptr_t<SelectorDeclaration> node);
            virtual void visit(sptr_t<ConstructorDeclaration> node);
            virtual void visit(sptr_t<SimpleDatatypeDeclaration> node);
            virtual void visit(sptr_t<ParametricDatatypeDeclaration> node);

            virtual void visit(sptr_t<QualifiedConstructor> node);
            virtual void visit(sptr_t<QualifiedPattern> node);
            virtual void visit(sptr_t<MatchCase> node);

            virtual void visit(sptr_t<QualifiedTerm> node);
            virtual void visit(sptr_t<LetTerm> node);
            virtual void visit(sptr_t<ForallTerm> node);
            virtual void visit(sptr_t<ExistsTerm> node);
            virtual void visit(sptr_t<MatchTerm> node);
            virtual void visit(sptr_t<AnnotatedTerm> node);

            virtual void visit(sptr_t<SortedVariable> node);
            virtual void visit(sptr_t<VarBinding> node);

            bool check(sptr_t<AstNode> node);

            std::string getErrors();
        };
    }
}

#endif //SMTLIB_PARSER_AST_SYNTAX_CHECKER_H
