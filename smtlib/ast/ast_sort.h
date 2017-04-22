/**
 * \file smt_sort.h
 * \brief SMT-LIB sort.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_SORT_H
#define SMTLIB_PARSER_AST_SORT_H

#include "ast_basic.h"
#include "ast_identifier.h"

#include <memory>
#include <vector>

namespace smtlib {
    namespace ast {
        class SimpleIdentifier;
        /**
         * An SMT-LIB sort.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class Sort : public AstNode,
                     public std::enable_shared_from_this<Sort> {
        protected:
            sptr_t<SimpleIdentifier> identifier;
            sptr_v<Sort> args;
        public:

            /**
             * Constructor for a simple sort
             * \param identifier    Sort name
             */
            inline Sort(sptr_t<SimpleIdentifier> identifier) : identifier(identifier) { }

            /**
             * Constructor for a parametric sort
             * \param identifier    Sort name
             * \param args          Sort arguments
             */
            Sort(sptr_t<SimpleIdentifier> identifier,
                 sptr_v<Sort>& args);

            inline sptr_t<SimpleIdentifier> getIdentifier() { return identifier; }

            inline void setIdentifier(sptr_t<SimpleIdentifier> identifier) { this->identifier = identifier; }

            inline sptr_v<Sort>& getArgs() { return args; }

            /** Checks whether the sort is parametrized (i.e. the list of sort parameters is not empty). */
            bool hasArgs();

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };
    }
}

#endif //SMTLIB_PARSER_AST_SORT_H