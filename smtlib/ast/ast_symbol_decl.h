/**
 * \file smt_symbol_decl.h
 * \brief SMT-LIB sort and function symbol declarations.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_SYMBOL_DECL_H
#define SMTLIB_PARSER_AST_SYMBOL_DECL_H

#include "ast_abstract.h"
#include "ast_attribute.h"
#include "ast_basic.h"
#include "ast_identifier.h"
#include "ast_interfaces.h"
#include "ast_literal.h"
#include "ast_sort.h"

#include <memory>
#include <vector>

namespace smtlib {
    namespace ast {
        /* =============================== SortSymbolDeclaration ============================== */
        /**
         * Declaration of a sort symbol.
         * Node of the SMT-LIB abstract syntax tree.
         * Can act as an attribute value.
         */
        class SortSymbolDeclaration : public virtual AstNode,
                                      public AttributeValue,
                                      public std::enable_shared_from_this<SortSymbolDeclaration> {
        private:
            sptr_t<SimpleIdentifier> identifier;
            sptr_t<NumeralLiteral> arity;
            sptr_v<Attribute> attributes;
        public:
            /**
             * Constructs declaration without attributes.
             * \param identifier    Sort symbol identiier
             * \param arity         Sort arity
             */
            inline SortSymbolDeclaration(sptr_t<SimpleIdentifier> identifier,
                                         sptr_t<NumeralLiteral> arity)
                    : identifier(identifier), arity(arity) { }

            /**
             * Constructs declaration with attributes.
             * \param identifier    Sort symbol identiier
             * \param arity         Sort arity
             * \param attributes    Sort symbol declaration attributes
             */
            SortSymbolDeclaration(sptr_t<SimpleIdentifier> identifier,
                                  sptr_t<NumeralLiteral> arity,
                                  sptr_v<Attribute>& attributes);

            inline sptr_t<SimpleIdentifier> getIdentifier() { return identifier; }

            inline void setIdentifier(sptr_t<SimpleIdentifier> identifier) { this->identifier = identifier; }

            inline sptr_t<NumeralLiteral> getArity() { return arity; }

            inline void setArity(sptr_t<NumeralLiteral> arity) { this->arity = arity; }

            inline sptr_v<Attribute>& getAttributes() { return attributes; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =============================== FunSymbolDeclaration =============================== */
        /**
         * Abstract class for a function symbol declaration.
         * Node of the SMT-LIB abstract syntax tree.
         * Can act as an attribute value.
         */
        class FunSymbolDeclaration : public virtual AstNode,
                                     public AttributeValue {
        };

        /* ============================= SpecConstFunDeclaration ============================== */
        /**
         * Specification constant function symbol declaration.
         * Node of the SMT-LIB abstract syntax tree.
         * Can act as an attribute value.
         */
        class SpecConstFunDeclaration : public FunSymbolDeclaration,
                                        public std::enable_shared_from_this<SpecConstFunDeclaration> {
        private:
            sptr_t<SpecConstant> constant;
            sptr_t<Sort> sort;
            sptr_v<Attribute> attributes;

        public:
            /**
            * Constructs declaration without attributes.
            * \param constant      Specification constant
            * \param sort          Function sort
            */
            inline SpecConstFunDeclaration(sptr_t<SpecConstant> constant, sptr_t<Sort> sort)
                    : constant(constant), sort(sort) { }

            /**
             * Constructs declaration with attributes.
             * \param constant      Specification constant
             * \param sort          Function sort
             * \param attributes    Function symbol declaration attributes
             */
            SpecConstFunDeclaration(sptr_t<SpecConstant> constant,
                                    sptr_t<Sort> sort,
                                    sptr_v<Attribute>& attributes);

            inline sptr_t<SpecConstant> getConstant() { return constant; }

            inline void setConstant(sptr_t<SpecConstant> constant) { this->constant = constant; }

            inline sptr_t<Sort> getSort() { return sort; }

            inline void setSort(sptr_t<Sort> sort) { this->sort = sort; }

            inline sptr_v<Attribute>& getAttributes() { return attributes; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ========================== MetaSpecConstFunDeclaration =========================== */

        /**
         * Meta specification constant function symbol declaration.
         * Node of the SMT-LIB abstract syntax tree.
         * Can act as an attribute value.
         */
        class MetaSpecConstFunDeclaration : public FunSymbolDeclaration,
                                            public std::enable_shared_from_this<MetaSpecConstFunDeclaration> {
        private:
            sptr_t<MetaSpecConstant> constant;
            sptr_t<Sort> sort;
            sptr_v<Attribute> attributes;

        public:
            /**
            * Constructs declaration without attributes.
            * \param constant      Meta specification constant
            * \param sort          Function sort
            */
            inline MetaSpecConstFunDeclaration(sptr_t<MetaSpecConstant> constant, sptr_t<Sort> sort)
                    : constant(constant), sort(sort) { }

            /**
             * Constructs declaration with attributes.
             * \param constant      Meta specification constant
             * \param sort          Function sort
             * \param attributes    Function symbol declaration attributes
             */
            MetaSpecConstFunDeclaration(sptr_t<MetaSpecConstant> constant,
                                        sptr_t<Sort> sort,
                                        sptr_v<Attribute>& attributes);

            inline sptr_t<MetaSpecConstant> getConstant() { return constant; }

            inline void setConstant(sptr_t<MetaSpecConstant> constant) { this->constant = constant; }

            inline sptr_t<Sort> getSort() { return sort; }

            inline void setSort(sptr_t<Sort> sort) { this->sort = sort; }

            inline sptr_v<Attribute>& getAttributes() { return attributes; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ============================== SimpleFunDeclaration =============================== */

        /**
         * Identifier function symbol declaration.
         * Node of the SMT-LIB abstract syntax tree.
         * Can act as an attribute value.
         */
        class SimpleFunDeclaration : public FunSymbolDeclaration,
                                     public std::enable_shared_from_this<SimpleFunDeclaration> {
        protected:
            sptr_t<SimpleIdentifier> identifier;
            sptr_v<Sort> signature;
            sptr_v<Attribute> attributes;

            SimpleFunDeclaration() { }

        public:
            /**
             * Constructs declaration without attributes.
             * \param identifier    Function identifier
             * \param signature     Function signature
             */
            SimpleFunDeclaration(sptr_t<SimpleIdentifier> identifier,
                                 sptr_v<Sort>& signature);

            /**
             * Constructs declaration with attributes.
             * \param identifier    Function identifier
             * \param signature     Function signature
             * \param attributes    Function symbol declaration attributes
             */
            SimpleFunDeclaration(sptr_t<SimpleIdentifier> identifier,
                                 sptr_v<Sort>& signature,
                                 sptr_v<Attribute>& attributes);

            inline sptr_t<SimpleIdentifier> getIdentifier() { return identifier; }

            inline void setIdentifier(sptr_t<SimpleIdentifier> identifier) { this->identifier = identifier; }

            inline sptr_v<Sort>& getSignature() { return signature; }

            inline sptr_v<Attribute>& getAttributes() { return attributes; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =============================== ParametricFunDeclaration ================================ */

        /**
        * Parametric function symbol declaration.
        * Node of the SMT-LIB abstract syntax tree.
        * Can act as an attribute value.
        */
        class ParametricFunDeclaration : public FunSymbolDeclaration,
                                         public std::enable_shared_from_this<ParametricFunDeclaration> {
        private:
            sptr_v<Symbol> params;
            sptr_t<SimpleIdentifier> identifier;
            sptr_v<Sort> signature;
            sptr_v<Attribute> attributes;

        public:
            /**
             * Constructs declaration without attributes.
             * \param params        Function parameters
             * \param identifier    Function identifier
             * \param signature     Function signature
             */
            ParametricFunDeclaration(sptr_v<Symbol>& params,
                                     sptr_t<SimpleIdentifier> identifier,
                                     sptr_v<Sort>& signature);

            /**
             * Constructs declaration with attributes.
             * \param params        Function parameters
             * \param identifier    Function identifier
             * \param signature     Function signature
             * \param attributes    Function symbol declaration attributes
             */
            ParametricFunDeclaration(sptr_v<Symbol>& params,
                                     sptr_t<SimpleIdentifier> identifier,
                                     sptr_v<Sort>& signature,
                                     sptr_v<Attribute>& attributes);

            inline sptr_v<Symbol>& getParams() { return params; }

            inline sptr_t<SimpleIdentifier> getIdentifier() { return identifier; }

            inline void setIdentifier(sptr_t<SimpleIdentifier> identifier) { this->identifier = identifier; }

            inline sptr_v<Sort>& getSignature() { return signature; }

            inline sptr_v<Attribute>& getAttributes() { return attributes; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };
    }
}

#endif //SMTLIB_PARSER_AST_SYMBOL_DECL_H