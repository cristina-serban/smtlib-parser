/**
 * \file smtlib-glue.h
 * \brief Glue between the generated C parser and the C++ implementation.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_H
#define SMTLIB_PARSER_H

#include "ast/ast_abstract.h"

#include <memory>
#include <string>

namespace smtlib {
    class Parser {
    private:
        sptr_t<ast::AstNode> ast;
        sptr_t<std::string> filename;
    public:
        sptr_t<ast::AstNode> parse(std::string filename);

        sptr_t<std::string> getFilename();

        void setAst(sptr_t<ast::AstNode> ast);

        sptr_t<ast::AstNode> getAst();

        void reportError(unsigned int lineLeft, unsigned int colLeft,
                         unsigned int lineRight, unsigned int colRight, const char* msg);
    };
}

#endif //SMTLIB_PARSER_H
