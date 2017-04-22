/**
 * \file ast_attribute.h
 * \brief SMT-LIB attribute.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_ATTRIBUTE_H
#define SMTLIB_PARSER_AST_ATTRIBUTE_H

#include "ast_abstract.h"
#include "ast_basic.h"
#include "ast_interfaces.h"

#include "ast/visitor/ast_visitor.h"

#include <vector>

namespace smtlib {
    namespace ast {
        /* ==================================== Attribute ===================================== */
        /** An SMT-LIB attribute */
        class Attribute : public AstNode,
                          public std::enable_shared_from_this<Attribute> {
        private:
            sptr_t<Keyword> keyword;
            sptr_t<AttributeValue> value;

        public:
            /** Default constructor. */
            inline Attribute() { }

            /**
             * Constructs keyword without attribute value.
             * \param keyword   Keyword of the attribute
             */
            inline Attribute(sptr_t<Keyword> keyword) : keyword(keyword) { }

            /**
             * Constructs keyword with attribute value.
             * \param keyword   Keyword of the attribute
             * \param value     Value of the attribute
             */
            inline Attribute(sptr_t<Keyword> keyword,
                             sptr_t<AttributeValue> value)
                    : keyword(keyword), value(value) { }

            inline sptr_t<Keyword> getKeyword() { return keyword; }

            inline void setKeyword(sptr_t<Keyword> keyword) { this->keyword = keyword; }

            inline sptr_t<AttributeValue> getValue() { return value; }

            inline void setValue(sptr_t<AttributeValue> value) { this->value = value; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ============================== CompAttributeValue ============================== */
        /** A compound value for an SMT-LIB attribute */
        class CompAttributeValue : public AttributeValue,
                                   public std::enable_shared_from_this<CompAttributeValue> {
        private:
            sptr_v<AttributeValue> values;
        public:
            /**
             * Constructs a composite attribute value from a vector of attribute values
             * \param values    Vector of attribute values
             */
            CompAttributeValue(sptr_v<AttributeValue>& values);

            inline sptr_v<AttributeValue>& getValues() { return values; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };
    }
}

#endif //SMTLIB_PARSER_AST_ATTRIBUTE_H