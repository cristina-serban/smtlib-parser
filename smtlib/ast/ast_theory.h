/**
 * \file smt_theory.h
 * \brief SMT-LIB theory.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_THEORY_H
#define SMTLIB_PARSER_AST_THEORY_H

#include "ast_abstract.h"
#include "ast_attribute.h"

#include <memory>
#include <vector>

namespace smtlib {
    namespace ast {

        /**
         * SMT-LIB theory.
         * Node and (possible) root of the SMT abstract syntax tree.
         * Represents the contents of a theory file.
         */
        class Theory : public AstRoot, public std::enable_shared_from_this<Theory> {
        private:
            sptr_t<Symbol> name;
            sptr_v<Attribute> attributes;

        public:
            /**
             * Constructs theory without attributes.
             * \param name  Theory name
             */
            inline Theory(sptr_t<Symbol> name) : name(name) { }

            /**
             * Constructs theory with attributes.
             * \param name          Theory name
             * \param attributes    Theory attributes
             */
            Theory(sptr_t<Symbol> name, sptr_v<Attribute>& attributes);

            inline sptr_t<Symbol> getName() { return name; }

            inline void setName(sptr_t<Symbol> name) { this->name = name; }

            inline sptr_v<Attribute>& getAttributes() { return attributes; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };
    }
}

#endif //SMTLIB_PARSER_AST_THEORY_H