#include "execution.h"

#include "ast/visitor/ast_syntax_checker.h"
#include "util/global_values.h"


using namespace std;
using namespace smtlib;
using namespace smtlib::ast;

SmtExecution::SmtExecution()
        : settings(make_shared<SmtExecutionSettings>()) {
    parseAttempted = false;
    parseSuccessful = false;
    syntaxCheckAttempted = false;
    syntaxCheckSuccessful = false;
    sortednessCheckAttempted = false;
    sortednessCheckSuccessful = false;
}

SmtExecution::SmtExecution(sptr_t<SmtExecutionSettings> settings)
        : settings(make_shared<SmtExecutionSettings>(settings)) {
    if (settings->getInputMethod() == SmtExecutionSettings::InputMethod::INPUT_AST) {
        ast = settings->getAst();
        parseAttempted = true;
        parseSuccessful = true;
    } else {
        parseAttempted = false;
        parseSuccessful = false;
    }

    syntaxCheckAttempted = false;
    syntaxCheckSuccessful = false;
    sortednessCheckAttempted = false;
    sortednessCheckSuccessful = false;
}

bool SmtExecution::parse() {
    if (parseAttempted)
        return parseSuccessful;

    parseAttempted = true;

    if (settings->getInputMethod() == SmtExecutionSettings::InputMethod::INPUT_NONE) {
        Logger::error("SmtExecution::parse()", "No input provided");
        return false;
    }

    if (settings->getInputMethod() == SmtExecutionSettings::InputMethod::INPUT_FILE) {
        sptr_t<Parser> parser = make_shared<Parser>();
        ast = parser->parse(settings->getFilename().c_str());
        if (ast) {
            parseSuccessful = true;
        } else {
            //Logger::error("SmtExecution::parse()", "Stopped due to previous errors");
        }
    }

    return parseSuccessful;
}

bool SmtExecution::checkSyntax() {
    if (syntaxCheckAttempted)
        return syntaxCheckSuccessful;

    syntaxCheckAttempted = true;

    if (!parse()) {
        //Logger::error("SmtExecution::checkSyntax()", "Stopped due to previous errors");
        return false;
    }

    sptr_t<SyntaxChecker> chk = make_shared<SyntaxChecker>();
    syntaxCheckSuccessful = chk->check(ast);

    if (!syntaxCheckSuccessful) {
        if (settings->getInputMethod() == SmtExecutionSettings::InputMethod::INPUT_AST) {
            Logger::syntaxError("SmtExecution::checkSyntax()", chk->getErrors().c_str());
        } else {
            Logger::syntaxError("SmtExecution::checkSyntax()",
                                settings->getFilename().c_str(), chk->getErrors().c_str());
        }
    }

    return syntaxCheckSuccessful;
}

bool SmtExecution::checkSortedness() {
    if (sortednessCheckAttempted)
        return sortednessCheckSuccessful;

    sortednessCheckAttempted = true;

    if (!checkSyntax()) {
        //Logger::error("SmtExecution::checkSortedness()", "Stopped due to previous errors");
        return false;
    }

    sptr_t<SortednessChecker> chk;

    if (settings->getSortCheckContext())
        chk = make_shared<SortednessChecker>(settings->getSortCheckContext());
    else
        chk = make_shared<SortednessChecker>();

    if (settings->isCoreTheoryEnabled())
        chk->loadTheory(THEORY_CORE);
    sortednessCheckSuccessful = chk->check(ast);

    if (!sortednessCheckSuccessful) {
        if (settings->getInputMethod() == SmtExecutionSettings::InputMethod::INPUT_AST) {
            Logger::sortednessError("SmtExecution::checkSortedness()", chk->getErrors().c_str());
        } else {
            Logger::sortednessError("SmtExecution::checkSortedness()",
                                    settings->getFilename().c_str(), chk->getErrors().c_str());
        }
    }

    return sortednessCheckSuccessful;
}