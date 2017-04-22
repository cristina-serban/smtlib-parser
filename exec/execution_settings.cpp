#include "execution_settings.h"

using namespace std;
using namespace smtlib;
using namespace smtlib::ast;

SmtExecutionSettings::SmtExecutionSettings()
        : coreTheoryEnabled(true), inputMethod(INPUT_NONE) {}

SmtExecutionSettings::SmtExecutionSettings(sptr_t<SmtExecutionSettings> settings) {
    this->coreTheoryEnabled = settings->coreTheoryEnabled;
    this->inputMethod = settings->inputMethod;
    this->filename = settings->filename;
    this->ast = settings->ast;
    this->sortCheckContext = settings->sortCheckContext;
}

void SmtExecutionSettings::setInputFromFile(std::string filename) {
    this->filename = filename;
    this->ast.reset();
    inputMethod = INPUT_FILE;
}

void SmtExecutionSettings::setInputFromAst(sptr_t<smtlib::ast::AstNode> ast) {
    this->ast = ast;
    this->filename = "";
    inputMethod = INPUT_AST;
}