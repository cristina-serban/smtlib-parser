/**
 * \file ast_term_sorter.h
 * \brief Visitor for determining the sort of a term.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_TERM_SORTER_H
#define SMTLIB_PARSER_AST_TERM_SORTER_H

#include "ast_visitor_extra.h"

#include "ast/stack/ast_symbol_stack.h"
#include "util/configuration.h"

#include <algorithm>

namespace smtlib {
    namespace ast {
        class SortednessChecker;

        /** Context for determining the sort of a term */
        class ITermSorterContext {
        public:
            virtual sptr_t<SymbolStack> getStack() = 0;
            virtual sptr_t<SortednessChecker> getChecker() = 0;
            virtual sptr_t<Configuration> getConfiguration() = 0;
        };

        /** Visitor for determining the sort of a term */
        class TermSorter : public DummyAstVisitor1<sptr_t<Sort>> {
        private:
            sptr_t<ITermSorterContext> ctx;

            bool getParamMapping(std::vector<std::string> &params,
                                 sptr_um2<std::string, Sort> &mapping,
                                 sptr_t<Sort> sort1,
                                 sptr_t<Sort> sort2);

        public:
            inline TermSorter(sptr_t<ITermSorterContext> ctx) : ctx(ctx) { }

            virtual void visit(sptr_t<SimpleIdentifier> node);
            virtual void visit(sptr_t<QualifiedIdentifier> node);

            virtual void visit(sptr_t<DecimalLiteral> node);
            virtual void visit(sptr_t<NumeralLiteral> node);
            virtual void visit(sptr_t<StringLiteral> node);

            virtual void visit(sptr_t<QualifiedTerm> node);
            virtual void visit(sptr_t<LetTerm> node);
            virtual void visit(sptr_t<ForallTerm> node);
            virtual void visit(sptr_t<ExistsTerm> node);
            virtual void visit(sptr_t<MatchTerm> node);
            virtual void visit(sptr_t<AnnotatedTerm> node);

            sptr_t<Sort> run(sptr_t<AstNode> node) {
                return wrappedVisit(node);
            }
        };
    }
}


#endif //SMTLIB_PARSER_AST_TERM_SORTER_H
