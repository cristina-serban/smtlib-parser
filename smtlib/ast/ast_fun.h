/**
 * \file smt_fun.h
 * \brief Function declaration and definition.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_FUN_H
#define SMTLIB_PARSER_AST_FUN_H

#include "ast_abstract.h"
#include "ast_basic.h"
#include "ast_interfaces.h"
#include "ast_sort.h"
#include "ast_var.h"

#include "ast/visitor/ast_visitor.h"

#include <memory>
#include <vector>

namespace smtlib {
    namespace ast {
        /* =============================== FunctionDeclaration ================================ */
        /**
         * An SMT-LIB function declaration.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class FunctionDeclaration : public AstNode,
                                    public std::enable_shared_from_this<FunctionDeclaration> {
        private:
            sptr_t<Symbol> symbol;
            sptr_v<SortedVariable> params;
            sptr_t<Sort> sort;
        public:
            /**
             * \param symbol    Name of the function
             * \param params    List of parameters
             * \param sort      Sort of the return value
             */
            FunctionDeclaration(sptr_t<Symbol> symbol,
                                sptr_v<SortedVariable>& params,
                                sptr_t<Sort> sort);

            inline sptr_t<Symbol> getSymbol() { return symbol; }

            inline void setSymbol(sptr_t<Symbol> symbol) { this->symbol = symbol; }

            inline sptr_v<SortedVariable>& getParams() { return params; }

            inline sptr_t<Sort> getSort() { return sort; }

            inline void setSort(sptr_t<Sort> sort) { this->sort = sort;}

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================ FunctionDefinition ================================ */
        /**
         * An SMT-LIB function definition.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class FunctionDefinition : public AstNode,
                                   public std::enable_shared_from_this<FunctionDefinition> {
        private:
            sptr_t<FunctionDeclaration> signature;
            sptr_t<Term> body;
        public:
            /**
             * \param signature    Function signature
             * \param body         Function body
             */
            FunctionDefinition(sptr_t<FunctionDeclaration> signature,
                               sptr_t<Term> body)
                    : signature(signature), body(body) { }

            /**
             * \param symbol    Name of the function
             * \param params    List of parameters
             * \param type      Sort of the return value
             * \param body      Function body
             */
            FunctionDefinition(sptr_t<Symbol> symbol,
                               sptr_v<SortedVariable>& params,
                               sptr_t<Sort> sort,
                               sptr_t<Term> body);

            inline sptr_t<FunctionDeclaration> getSignature() { return signature; }

            inline void setSignature(sptr_t<FunctionDeclaration> signature) { this->signature = signature; }

            inline sptr_t<Term> getBody() { return body; }

            inline void setBody(sptr_t<Term> body) { this->body = body; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };
    }
}

#endif //SMTLIB_PARSER_AST_FUN_H