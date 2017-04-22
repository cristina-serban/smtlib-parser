/**
 * \file execution.h
 * \brief Execution handling.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_EXECUTION_H
#define SMTLIB_PARSER_EXECUTION_H

#include "execution_settings.h"

#include "parser/smtlib_parser.h"
#include "util/global_typedef.h"

#include <memory>

namespace smtlib {
    /** Handles execution. */
    class SmtExecution {
    private:
        sptr_t<SmtExecutionSettings> settings;
        sptr_t<smtlib::ast::AstNode> ast;

        bool parseAttempted, parseSuccessful;
        bool syntaxCheckAttempted, syntaxCheckSuccessful;
        bool sortednessCheckAttempted, sortednessCheckSuccessful;

    public:
        SmtExecution();

        SmtExecution(sptr_t<SmtExecutionSettings> settings);

        /** Parse the input */
        bool parse();

        /** Check syntax correctness of the input */
        bool checkSyntax();

        /** Check sortedness of the input */
        bool checkSortedness();
    };
}

#endif //SMTLIB_PARSER_ENGINE_H
