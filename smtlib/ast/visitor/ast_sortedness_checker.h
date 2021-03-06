/**
 * \file ast_term_sorter.h
 * \brief Visitor for checking sortedness.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_SORTEDNESS_CHECKER_H
#define SMTLIB_PARSER_AST_SORTEDNESS_CHECKER_H

#include "ast_visitor_extra.h"
#include "ast_term_sorter.h"
#include "ast/ast_symbol_decl.h"
#include "ast/ast_command.h"
#include "ast/stack/ast_symbol_stack.h"
#include "util/logger.h"
#include "util/configuration.h"

#include <map>

namespace smtlib {
    namespace ast {
        /* ============================= SortednessCheckerContext ============================= */
        /** Context for checking sortedness */
        class ISortCheckContext {
        public:
            virtual sptr_t<SymbolStack> getStack() = 0;
            virtual std::vector<std::string>& getCurrentTheories() = 0;
            virtual std::string getCurrentLogic() = 0;
            virtual sptr_t<Configuration> getConfiguration() = 0;
            virtual void setCurrentLogic(std::string logic) = 0;
        };

        class SortednessCheckerContext : public ISortCheckContext,
                                         public std::enable_shared_from_this<SortednessCheckerContext> {
        private:
            sptr_t<smtlib::SymbolStack> stack;
            std::vector<std::string> currentTheories;
            std::string currentLogic;
            sptr_t<Configuration> config;
        public:
            SortednessCheckerContext();

            SortednessCheckerContext(sptr_t<smtlib::SymbolStack> stack);

            virtual sptr_t<SymbolStack> getStack();
            virtual std::vector<std::string>& getCurrentTheories();
            virtual std::string getCurrentLogic();
            virtual sptr_t<Configuration> getConfiguration();
            virtual void setCurrentLogic(std::string logic);
        };

        /* ================================ SortednessChecker ================================= */
        /** Visitor for checking sortedness */
        class SortednessChecker : public DummyAstVisitor0,
                                  public ITermSorterContext,
                                  public std::enable_shared_from_this<SortednessChecker>{
        public:
            struct Error {
                std::string message;
                sptr_t<SymbolInfo> info;

                Error(std::string message) : message(message) { }

                Error(std::string message, sptr_t<SymbolInfo> info)
                        : message(message), info(info) { }
            };

            struct NodeError {
                sptr_v<Error> errs;
                sptr_t<AstNode> node;

                NodeError() { }

                NodeError(sptr_t<Error> err, sptr_t<AstNode> node) : node(node) {
                    errs.push_back(err);
                }

                NodeError(sptr_v<Error> &errs, sptr_t<AstNode> node) : node(node) {
                    this->errs.insert(this->errs.begin(), errs.begin(), errs.end());
                }
            };

        private:
            sptr_t<ISortCheckContext> ctx;
            std::map<std::string, sptr_v<NodeError>> errors;

            sptr_t<SortInfo> getInfo(sptr_t<SortSymbolDeclaration> node);
            sptr_t<SortInfo> getInfo(sptr_t<DeclareSortCommand> node);
            sptr_t<SortInfo> getInfo(sptr_t<DefineSortCommand> node);

            sptr_t<FunInfo> getInfo(sptr_t<SpecConstFunDeclaration> node);
            sptr_t<FunInfo> getInfo(sptr_t<MetaSpecConstFunDeclaration> node);
            sptr_t<FunInfo> getInfo(sptr_t<SimpleFunDeclaration> node);
            sptr_t<FunInfo> getInfo(sptr_t<ParametricFunDeclaration> node);
            sptr_t<FunInfo> getInfo(sptr_t<DeclareConstCommand> node);
            sptr_t<FunInfo> getInfo(sptr_t<DeclareFunCommand> node);
            sptr_t<FunInfo> getInfo(sptr_t<DefineFunCommand> node);
            sptr_t<FunInfo> getInfo(sptr_t<DefineFunRecCommand> node);
            sptr_v<FunInfo> getInfo(sptr_t<DefineFunsRecCommand> node);

            sptr_v<SymbolInfo> getInfo(sptr_t<DeclareDatatypeCommand> node);
            sptr_v<SymbolInfo> getInfo(sptr_t<DeclareDatatypesCommand> node);

            void loadTheory(std::string theory, sptr_t<AstNode> node, sptr_t<NodeError> err);

            void loadLogic(std::string logic, sptr_t<AstNode> node, sptr_t<NodeError> err);

        public:
            inline SortednessChecker() : ctx(std::make_shared<SortednessCheckerContext>()) { }

            inline SortednessChecker(sptr_t<ISortCheckContext> ctx) : ctx(ctx) { }

            sptr_t<NodeError> addError(std::string message, sptr_t<AstNode> node, sptr_t<NodeError> err);

            sptr_t<NodeError> addError(std::string message, sptr_t<AstNode> node,
                                       sptr_t<SymbolInfo> symbolInfo, sptr_t<NodeError> err);

            void addError(std::string message, sptr_t<AstNode> node);
            void addError(std::string message, sptr_t<AstNode> node, sptr_t<SymbolInfo> err);

            void loadTheory(std::string theory);

            sptr_t<NodeError> checkSort(sptr_t<Sort> sort, sptr_t<AstNode> source,
                                        sptr_t<NodeError> err);

            sptr_t<NodeError> checkSort(sptr_v<Symbol> &params, sptr_t<Sort> sort,
                                        sptr_t<AstNode> source, sptr_t<NodeError> err);

            virtual void visit(sptr_t<AssertCommand> node);
            virtual void visit(sptr_t<DeclareConstCommand> node);
            virtual void visit(sptr_t<DeclareFunCommand> node);
            virtual void visit(sptr_t<DeclareDatatypeCommand> node);
            virtual void visit(sptr_t<DeclareDatatypesCommand> node);
            virtual void visit(sptr_t<DeclareSortCommand> node);
            virtual void visit(sptr_t<DefineFunCommand> node);
            virtual void visit(sptr_t<DefineFunRecCommand> node);
            virtual void visit(sptr_t<DefineFunsRecCommand> node);
            virtual void visit(sptr_t<DefineSortCommand> node);
            virtual void visit(sptr_t<GetValueCommand> node);
            virtual void visit(sptr_t<PopCommand> node);
            virtual void visit(sptr_t<PushCommand> node);
            virtual void visit(sptr_t<ResetCommand> node);
            virtual void visit(sptr_t<SetLogicCommand> node);

            virtual void visit(sptr_t<Logic> node);
            virtual void visit(sptr_t<Theory> node);
            virtual void visit(sptr_t<Script> node);

            virtual void visit(sptr_t<SortSymbolDeclaration> node);
            virtual void visit(sptr_t<SpecConstFunDeclaration> node);
            virtual void visit(sptr_t<MetaSpecConstFunDeclaration> node);
            virtual void visit(sptr_t<SimpleFunDeclaration> node);
            virtual void visit(sptr_t<ParametricFunDeclaration> node);

            bool check(sptr_t<AstNode> node);

            std::string getErrors();

            // ITermSorterContext implementation
            virtual sptr_t<SymbolStack> getStack();

            virtual sptr_t<SortednessChecker> getChecker();

            virtual sptr_t<Configuration> getConfiguration();
        };
    }
}

#endif //SMTLIB_PARSER_AST_SORTEDNESS_CHECKER_H
