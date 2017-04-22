/**
 * \file ast_abstract.h
 * \brief Abstract SMT-LIB data structures.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_ABSTRACT_H
#define SMTLIB_PARSER_AST_ABSTRACT_H

#include "ast/visitor/ast_visitor.h"
#include "util/global_typedef.h"

#include <string>
#include <memory>

namespace smtlib {
    namespace ast {

        /** Node of the SMT-LIB abstract syntax tree */
        class AstNode {
        private:
            int rowLeft;
            int rowRight;
            int colLeft;
            int colRight;
            sptr_t<std::string> filename;

        public:
            AstNode() : rowLeft(0), colLeft(0), rowRight(0), colRight(0) { }

            /** Get row where node begins */
            inline int getRowLeft() { return rowLeft; }

            /** Set row where node begins */
            inline void setRowLeft(int rowLeft) { this->rowLeft = rowLeft; }

            /** Get column where node begins */
            inline int getColLeft() { return colLeft; }

            /** Set column where node begins */
            inline void setColLeft(int colLeft) { this->colLeft = colLeft; }

            /** Get row where node ends */
            inline int getRowRight() { return rowRight; }

            /** Set row where node ends */
            inline void setRowRight(int rowRight) { this->rowRight = rowRight; }

            /** Get column where node ends */
            inline int getColRight() { return colRight; }

            /** Set column where node ends */
            inline void setColRight(int colRight) { this->colRight = colRight; }

            /** Get name of the source file */
            inline sptr_t<std::string> getFilename() { return filename; }

            /** Set name of the source file */
            inline void setFilename(sptr_t<std::string> filename) { this->filename = filename; }

            /** Accept a visitor */
            virtual void accept(class AstVisitor0* visitor) = 0;

            /** Get string representation of the node */
            virtual std::string toString() = 0;
        };

        /** Root of the SMT-LIB abstract syntax tree */
        class AstRoot : public AstNode {
        };
    }
}

#endif //SMTLIB_PARSER_AST_ABSTRACT_H