/**
 * \file smt_var.h
 * \brief SMT-LIB sorted variable and variable binding.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_VAR_H
#define SMTLIB_PARSER_AST_VAR_H

#include "ast_abstract.h"
#include "ast_basic.h"
#include "ast_interfaces.h"
#include "ast_sort.h"

#include <memory>

namespace smtlib {
    namespace ast {
        /* ================================== SortedVariable ================================== */
        /**
         * A sorted variable.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class SortedVariable : public AstNode,
                               public std::enable_shared_from_this<SortedVariable> {
        private:
            sptr_t<Symbol> symbol;
            sptr_t<Sort> sort;

        public:
            /**
             * \param symbol    Variable name
             * \param sort      Variable sort
             */
            inline SortedVariable(sptr_t<Symbol> symbol, sptr_t<Sort> sort)
                    : symbol(symbol), sort(sort) { }

            inline sptr_t<Symbol> getSymbol() { return symbol; }

            inline void setSymbol(sptr_t<Symbol> symbol) { this->symbol = symbol; }

            inline sptr_t<Sort> getSort() { return sort; }

            inline void setSort(sptr_t<Sort> sort) { this->sort = sort; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ==================================== VarBinding ==================================== */
        /**
         * A variable binding.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class VarBinding : public AstNode,
                           public std::enable_shared_from_this<VarBinding> {
        private:
            sptr_t<Symbol> symbol;
            sptr_t<Term> term;

        public:
            /**
             * \param symbol    Variable name
             * \param term      Binding
             */
            VarBinding(sptr_t<Symbol> symbol, sptr_t<Term> term)
                    : symbol(symbol), term(term) { }

            inline sptr_t<Symbol> getSymbol() { return symbol; }

            inline void setSymbol(sptr_t<Symbol> symbol) { this->symbol = symbol; }

            inline sptr_t<Term> getTerm() { return term; }

            inline void setTerm(sptr_t<Term> term) { this->term = term; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };
    }
}

#endif //SMTLIB_PARSER_AST_VAR_H