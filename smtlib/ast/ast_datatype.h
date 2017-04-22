/**
 * \file smt_datatype.h
 * \brief SMT-LIB datatype declarations and their components.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_DATATYPE_H
#define SMTLIB_PARSER_AST_DATATYPE_H

#include "ast_abstract.h"
#include "ast_basic.h"
#include "ast_literal.h"
#include "ast_sort.h"

namespace smtlib {
    namespace ast {
        /* ================================= SortDeclaration ================================== */
        /**
         * A sort declaration (used by the declare-datatypes command).
         * Node of the SMT-LIB abstract syntax tree.
         */
        class SortDeclaration : public AstNode,
                                public std::enable_shared_from_this<SortDeclaration> {
        private:
            sptr_t<Symbol> symbol;
            sptr_t<NumeralLiteral> arity;
        public:
            /**
             * \param symbol    Datatype (sort) name
             * \param arity     Arity
             */
            inline SortDeclaration(sptr_t<Symbol> symbol,
                                   sptr_t<NumeralLiteral> arity)
                    : symbol(symbol), arity(arity) { }

            inline sptr_t<Symbol> getSymbol() { return symbol; }

            inline void setSymbol(sptr_t<Symbol> symbol) { this->symbol = symbol; }

            inline sptr_t<NumeralLiteral> getArity() { return arity; }

            inline void setArity(sptr_t<NumeralLiteral> arity) { this->arity = arity; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =============================== SelectorDeclaration ================================ */
        /**
         * A selector declaration (used by constructor declarations).
         * Node of the SMT-LIB abstract syntax tree.
         */
        class SelectorDeclaration : public AstNode,
                                    public std::enable_shared_from_this<SelectorDeclaration> {
        private:
            sptr_t<Symbol> symbol;
            sptr_t<Sort> sort;
        public:
            /**
             * \param symbol    Selector name
             * \param sort      Selector sort
             */
            inline SelectorDeclaration(sptr_t<Symbol> symbol,
                                       sptr_t<Sort> sort)
                    : symbol(symbol), sort(sort) { }

            inline sptr_t<Symbol> getSymbol() { return symbol; }

            inline void setSymbol(sptr_t<Symbol> symbol) { this->symbol = symbol; }

            inline sptr_t<Sort> getSort() { return sort; }

            inline void setSort(sptr_t<Sort> sort) { this->sort = sort; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =============================== ConstructorDeclaration ============================== */
        /**
         * A sort declaration (used by the declare-datatypes command).
         * Node of the SMT-LIB abstract syntax tree.
         */
        class ConstructorDeclaration : public AstNode,
                                       public std::enable_shared_from_this<ConstructorDeclaration> {
        private:
            sptr_t<Symbol> symbol;
            sptr_v<SelectorDeclaration> selectors;
        public:
            /**
             * \param symbol        Constructor name
             * \param selectors     Selectors for the constructor
             */
            ConstructorDeclaration(sptr_t<Symbol> symbol,
                                   sptr_v<SelectorDeclaration>& selectors);

            inline sptr_t<Symbol> getSymbol() { return symbol; }

            inline void setSymbol(sptr_t<Symbol> symbol) { this->symbol = symbol; }

            inline sptr_v<SelectorDeclaration>& getSelectors() { return selectors; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================ DatatypeDeclaration =============================== */
        /**
         * A datatype declaration (used by the declare-datatype and declare-datatypes commands).
         * Node of the SMT-LIB abstract syntax tree.
         */
        class DatatypeDeclaration : public AstNode { };

        /* ============================= SimpleDatatypeDeclaration ============================ */
        /**
         * A simple (non-parametric) datatype declaration.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class SimpleDatatypeDeclaration : public DatatypeDeclaration,
                                          public std::enable_shared_from_this<SimpleDatatypeDeclaration>  {
        private:
            sptr_v<ConstructorDeclaration> constructors;
        public:
            /**
             * \param constructors  Constructors for this datatype
             */
            SimpleDatatypeDeclaration(sptr_v<ConstructorDeclaration>& constructors);

            inline sptr_v<ConstructorDeclaration>& getConstructors() { return constructors; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =========================== ParametricDatatypeDeclaration ========================== */
        /**
         * A parametric datatype declaration.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class ParametricDatatypeDeclaration : public DatatypeDeclaration,
                                              public std::enable_shared_from_this<ParametricDatatypeDeclaration> {
        private:
            sptr_v<Symbol> params;
            sptr_v<ConstructorDeclaration> constructors;
        public:
            /**
             * \param params        Parameters for the declaration
             * \param constructors  Constructors for this datatype
             */
            ParametricDatatypeDeclaration(sptr_v<Symbol>& params,
                                          sptr_v<ConstructorDeclaration>& constructors);

            inline sptr_v<Symbol>& getParams() { return params; }

            inline sptr_v<ConstructorDeclaration>& getConstructors() { return constructors; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };
    }
}

#endif //SMTLIB_PARSER_AST_DATATYPE_H
