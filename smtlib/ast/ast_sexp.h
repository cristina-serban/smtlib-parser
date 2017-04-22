/**
 * \file smt_sexp.h
 * \brief Compound S-expression.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_S_EXPR_H
#define SMTLIB_PARSER_AST_S_EXPR_H

#include "ast_interfaces.h"

#include <memory>
#include <vector>

namespace smtlib {
    namespace ast {
        /**
         * Compound S-expression.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class CompSExpression : public SExpression,
                                public AttributeValue,
                                public std::enable_shared_from_this<CompSExpression> {
        private:
            sptr_v<SExpression> exprs;
        public:
            /**
             * \param exprs     Subexpressions
             */
            CompSExpression(sptr_v<SExpression>& exprs);

            inline sptr_v<SExpression>& getExpressions() { return exprs; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };
    }
}

#endif //SMTLIB_PARSER_AST_S_EXPR_H