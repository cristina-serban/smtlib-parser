/**
 * \file smt_identif.h
 * \brief SMT-LIB identifiers.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_IDENTIFIER_H
#define SMTLIB_PARSER_AST_IDENTIFIER_H

#include "ast_basic.h"
#include "ast_interfaces.h"
#include "ast_sort.h"

#include <memory>
#include <vector>

namespace smtlib {
    namespace ast {
        class Sort;

        /* ==================================== SimpleIdentifier ==================================== */
        /** Simple identifier (e.g. "Real", "|John Brown|", "_ BitVec 32"). */
        class SimpleIdentifier : public Identifier,
                                 public std::enable_shared_from_this<SimpleIdentifier> {
        private:
            sptr_t<Symbol> symbol;
            sptr_v<Index> indices;

        public:
            /**
             * Constuctor for unindexed identifier.
             * \param symbol    Identifier symbol
             */
            SimpleIdentifier(sptr_t<Symbol> symbol) : symbol(symbol) { }

            /**
             * Constuctor for indexed identifier.
             * \param symbol    Identifier symbol
             * \param indices   Identifier indices
             */
            SimpleIdentifier(sptr_t<Symbol> symbol, sptr_v<Index>& indices);

            inline sptr_t<Symbol> getSymbol() { return symbol; }

            inline void setSymbol(sptr_t<Symbol> symbol) { this->symbol = symbol; }

            inline sptr_v<Index>& getIndices() { return indices; }

            /** Checks whether the identifier is indexed (i.e. the list of indices is not empty). */
            bool isIndexed();

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =============================== QualifiedIdentifier ================================ */
        /**
         * Qualified identifier (e.g. "(as f Sigma)").
         */
        class QualifiedIdentifier : public Identifier,
                                    public std::enable_shared_from_this<QualifiedIdentifier> {
        private:
            sptr_t<SimpleIdentifier> identifier;
            sptr_t<Sort> sort;
        public:
            /**
             * \param identifier    SimpleIdentifier
             * \param sort          Result sort
             */
            inline QualifiedIdentifier(sptr_t<SimpleIdentifier> identifier,
                                       sptr_t<Sort> sort) :
                    identifier(identifier), sort(sort) { }

            inline sptr_t<SimpleIdentifier> getIdentifier() { return identifier; }

            inline void setIdentifier(sptr_t<SimpleIdentifier> identifier) { this->identifier = identifier; }

            inline sptr_t<Sort> getSort() { return sort; }

            inline void setSort(sptr_t<Sort> sort) { this->sort = sort; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };
    }
}

#endif //SMTLIB_PARSER_AST_IDENTIFIER_H