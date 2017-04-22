/**
 * \file ast_visitor_extra.h
 * \brief Additional visitors for the SMT-LIB AST.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_VISITOR_EXTRA_H
#define SMTLIB_PARSER_AST_VISITOR_EXTRA_H

#include "ast_visitor.h"

#include "ast/ast_abstract.h"
#include "util/logger.h"

namespace smtlib {
    namespace ast {
        template<class RetT>
        class AstVisitor1 : public virtual AstVisitor0 {
        protected:
            RetT ret;
            RetT wrappedVisit(sptr_t<AstNode> node) {
                RetT oldRet = ret;
                visit0(node);
                RetT newRet = ret;
                ret = oldRet;
                return newRet;
            }
        public:
            virtual RetT run (sptr_t<AstNode> node) {
                return wrappedVisit(node);
            }
        };

        template<class RetT, class ArgT>
        class AstVisitor2 : public virtual AstVisitor0 {
        protected:
            ArgT arg;
            RetT ret;

            RetT wrappedVisit(ArgT arg, sptr_t<AstNode> node) {
                RetT oldRet = ret;
                ArgT oldArg = this->arg;
                this->arg = arg;
                visit0(node);
                RetT newRet = ret;
                ret = oldRet;
                this->arg = oldArg;
                return newRet;
            }
        public:
            virtual RetT run(ArgT arg, sptr_t<AstNode> node) {
                return wrappedVisit(arg, node);
            }
        };

        template<class RetT>
        class DummyAstVisitor1 : public AstVisitor1<RetT>,
                                 public DummyAstVisitor0 { };

        template<class RetT, class ArgT>
        class DummyAstVisitor2 : public AstVisitor2<RetT, ArgT>,
                                 public DummyAstVisitor0 { };
    }
}

#endif //SMTLIB_PARSER_AST_VISITOR_EXTRA_H