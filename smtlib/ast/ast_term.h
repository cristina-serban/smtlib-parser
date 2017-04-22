/**
 * \file smt_term.h
 * \brief SMT-LIB terms.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_TERM_H
#define SMTLIB_PARSER_AST_TERM_H

#include "ast_attribute.h"
#include "ast_interfaces.h"
#include "ast_match.h"
#include "ast_var.h"

#include <memory>
#include <vector>

namespace smtlib {
    namespace ast {
        /* ================================== QualifiedTerm =================================== */
        /**
         * A list of terms preceded by a qualified identifier.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class QualifiedTerm : public Term,
                              public std::enable_shared_from_this<QualifiedTerm> {
        private:
            sptr_t<Identifier> identifier;
            sptr_v<Term> terms;

        public:
            /**
             * \param identifier    Qualified identifier
             * \param terms         List of terms
             */
            QualifiedTerm(sptr_t<Identifier> identifier,
                          sptr_v<Term>& terms);

            inline sptr_t<Identifier> getIdentifier() { return identifier; }

            inline void setIdentifier(sptr_t<Identifier> identifier) { this->identifier = identifier; }

            inline sptr_v<Term>& getTerms() { return terms; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ===================================== LetTerm ====================================== */
        /**
         * Term preceded by a let binder.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class LetTerm : public Term,
                        public std::enable_shared_from_this<LetTerm> {
        private:
            sptr_v<VarBinding> bindings;
            sptr_t<Term> term;

        public:
            /**
             * \param bindings  List of bound variables
             * \param term      Inner term
             */
            LetTerm(sptr_v<VarBinding>& bindings,
                    sptr_t<Term> term);

            inline sptr_t<Term> getTerm() { return term; }

            inline void setTerm(sptr_t<Term> term) { this->term = term; }

            inline sptr_v<VarBinding>& getBindings() { return bindings; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ==================================== ForallTerm ==================================== */
        /**
         * Term preceded by a forall binder.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class ForallTerm : public Term,
                           public std::enable_shared_from_this<ForallTerm> {
        private:
            sptr_v<SortedVariable> bindings;
            sptr_t<Term> term;

        public:
            /**
             * \param bindings  List of bound variables
             * \param term      Inner term
             */
            ForallTerm(sptr_v<SortedVariable>& bindings,
                       sptr_t<Term> term);

            inline sptr_t<Term> getTerm() { return term; }

            inline void setTerm(sptr_t<Term> term) { this->term = term; }

            inline sptr_v<SortedVariable>& getBindings() { return bindings; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ==================================== ExistsTerm ==================================== */
        /**
         * Term preceded by an exists binder.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class ExistsTerm : public Term,
                           public std::enable_shared_from_this<ExistsTerm> {
        private:
            sptr_v<SortedVariable> bindings;
            sptr_t<Term> term;

        public:
            /**
             * \param bindings  List of bound variables
             * \param term      Inner term
             */
            ExistsTerm(sptr_v<SortedVariable>& bindings,
                       sptr_t<Term> term);

            inline sptr_t<Term> getTerm() { return term; }

            inline void setTerm(sptr_t<Term> term) { this->term = term; }

            inline sptr_v<SortedVariable>& getBindings() { return bindings; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ==================================== MatchTerm ===================================== */
        class MatchTerm : public Term,
                          public std::enable_shared_from_this<MatchTerm> {
        private:
            sptr_t<Term> term;
            sptr_v<MatchCase> cases;
        public:
            MatchTerm(sptr_t<Term> term,
                      sptr_v<MatchCase>& cases);

            inline sptr_t<Term> getTerm() { return term; }

            void setTerm(sptr_t<Term> term) { this->term = term; }

            sptr_v<MatchCase>& getCases() { return cases; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================== AnnotatedTerm =================================== */
        /**
         * An annotated term.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class AnnotatedTerm : public Term,
                              public std::enable_shared_from_this<AnnotatedTerm> {
        private:
            sptr_t<Term> term;
            sptr_v<Attribute> attributes;

        public:
            /**
             * \param term  Inner term
             * \param attr  Attributes
             */
            AnnotatedTerm(sptr_t<Term> term,
                          sptr_v<Attribute>& attributes);

            inline sptr_t<Term> getTerm() { return term; }

            inline void setTerm(sptr_t<Term> term) { this->term = term; }

            inline sptr_v<Attribute>& getAttributes() { return attributes; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };
    }
}

#endif //SMTLIB_PARSER_AST_TERM_H