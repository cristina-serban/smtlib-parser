/**
 * \file smt_interfaces.h
 * \brief Interfaces that need to be implemented by some of the SMT-LIB AST nodes.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_INTERFACES_H
#define SMTLIB_PARSER_AST_INTERFACES_H

#include "ast_abstract.h"

namespace smtlib {
    namespace ast {

        class AttributeValue : public virtual AstNode { };

        class SExpression : public virtual AstNode { };

        class Term : public virtual AstNode { };

        class Identifier : public virtual AstNode,
                           public Term { };

        class Index : public virtual AstNode { };

        class SpecConstant : public virtual AstNode,
                             public SExpression,
                             public Term,
                             public AttributeValue { };

        class Pattern : public virtual AstNode { };

        class Constructor : public virtual AstNode,
                            public Pattern { };


    }
}

#endif //SMTLIB_PARSER_AST_INTERFACES_H