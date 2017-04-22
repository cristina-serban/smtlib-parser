/**
 * \file smt_logic
 * \brief SMT-LIB logic.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_LOGIC_H
#define SMTLIB_PARSER_AST_LOGIC_H

#include "ast_abstract.h"
#include "ast_attribute.h"

#include <memory>
#include <vector>

namespace smtlib {
    namespace ast {
        /**
         * SMT-LIB logic.
         * Node and (possible) root of the SMT abstract syntax tree.
         * Represents the contents of a logic file.
         */
        class Logic : public AstRoot, public std::enable_shared_from_this<Logic> {
        private:
            sptr_t<Symbol> name;
            sptr_v<Attribute> attributes;

        public:
            /**
             * Constructs logic without attributes.
             * \param name          Logic name
             */
            inline Logic(sptr_t<Symbol> name) : name(name) { }

            /**
             * Constructs logic with attributes.
             * \param name          Logic name
             * \param attributes    Logic attributes
             */
            Logic(sptr_t<Symbol> name, sptr_v<Attribute>& attributes);

            inline sptr_t<Symbol> getName() { return name; }

            inline void setName(sptr_t<Symbol> name) { this->name = name; }

            inline sptr_v<Attribute>& getAttributes() { return attributes; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };
    }
}

#endif //SMTLIB_PARSER_AST_LOGIC_H