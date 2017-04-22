/**
 * \file ast_match.h
 * \brief Classes for match terms.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_MATCH_H
#define SMTLIB_PARSER_AST_MATCH_H

#include "ast_abstract.h"
#include "ast_basic.h"
#include "ast_interfaces.h"

#include <vector>

namespace smtlib {
    namespace ast {
        /* =============================== QualifiedConstructor =============================== */
        /** A qualified constructor for match terms */
        class QualifiedConstructor : public Constructor,
                                     public std::enable_shared_from_this<QualifiedConstructor> {
        private:
            sptr_t<Symbol> symbol;
            sptr_t<Sort> sort;
        public:
            inline QualifiedConstructor(sptr_t<Symbol> symbol, sptr_t<Sort> sort)
                    : symbol(symbol), sort(sort) { }

            inline sptr_t<Symbol> getSymbol() { return symbol; }

            inline void setSymbol(sptr_t<Symbol> symbol) { this->symbol = symbol; }

            inline sptr_t<Sort> getSort() { return sort; }

            inline void setSort(sptr_t<Sort> sort) { this->sort = sort; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================= QualifiedPattern ================================= */
        /** A qualified pattern for match terms */
        class QualifiedPattern : public Pattern,
                                 public std::enable_shared_from_this<QualifiedPattern> {
        private:
            sptr_t<Constructor> constructor;
            sptr_v<Symbol> symbols;
        public:
            QualifiedPattern(sptr_t<Constructor> constructor,
                             sptr_v<Symbol>& symbols);

            inline sptr_t<Constructor> getConstructor() { return constructor; }

            inline void setConstructor(sptr_t<Constructor> constructor) { this->constructor = constructor; }

            inline sptr_v<Symbol>& getSymbols() { return symbols; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ===================================== MatchCase ==================================== */
        /** A match case for match terms */
        class MatchCase : public AstNode,
                          public std::enable_shared_from_this<MatchCase> {
        private:
            sptr_t<Pattern> pattern;
            sptr_t<Term> term;
        public:
            inline MatchCase(sptr_t<Pattern> pattern,
                             sptr_t<Term> term) : pattern(pattern), term(term) { }

            inline sptr_t<Pattern> getPattern() { return pattern; }

            inline void setPattern(sptr_t<Pattern> pattern) { this->pattern = pattern; }

            inline sptr_t<Term> getTerm() { return term; }

            inline void setTerm(sptr_t<Term> term) { this->term = term; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };
    }
}

#endif //SMTLIB_PARSER_AST_MATCH_H
