/**
 * \file execution_settings.h
 * \brief Settings for execution handling.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_EXECUTION_SETTINGS_H
#define SMTLIB_PARSER_EXECUTION_SETTINGS_H

#include "ast/ast_abstract.h"
#include "ast/stack/ast_symbol_stack.h"
#include "ast/visitor/ast_sortedness_checker.h"
#include "util/global_typedef.h"

#include <memory>

namespace smtlib {
    /** Manages settings for execution handling */
    class SmtExecutionSettings {
    public:
        enum InputMethod { INPUT_NONE = 0, INPUT_FILE, INPUT_AST };

    private:
        bool coreTheoryEnabled;
        std::string filename;
        sptr_t<smtlib::ast::AstNode> ast;
        sptr_t<smtlib::ast::ISortCheckContext> sortCheckContext;
        InputMethod inputMethod;

    public:
        SmtExecutionSettings();

        SmtExecutionSettings(sptr_t<SmtExecutionSettings> settings);

        inline bool isCoreTheoryEnabled() { return coreTheoryEnabled; }
        inline void setCoreTheoryEnabled(bool enabled) { coreTheoryEnabled = enabled; }

        void setInputFromFile(std::string filename);

        void setInputFromAst(sptr_t<smtlib::ast::AstNode> ast);

        inline void setSortCheckContext(sptr_t<smtlib::ast::ISortCheckContext> ctx) {
            this->sortCheckContext = ctx;
        }

        inline std::string getFilename() { return filename; }

        inline sptr_t<smtlib::ast::AstNode> getAst() { return ast; }

        inline sptr_t<smtlib::ast::ISortCheckContext> getSortCheckContext() {
            return sortCheckContext;
        }

        inline InputMethod getInputMethod() { return inputMethod; }
    };
}

#endif //SMTLIB_PARSER_ENGINE_SETTINGS_H
