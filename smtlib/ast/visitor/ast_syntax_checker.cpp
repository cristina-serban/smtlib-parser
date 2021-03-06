#include "ast_syntax_checker.h"
#include "ast/ast_attribute.h"
#include "ast/ast_command.h"
#include "ast/ast_logic.h"
#include "ast/ast_script.h"
#include "ast/ast_sexp.h"
#include "ast/ast_symbol_decl.h"
#include "ast/ast_term.h"
#include "ast/ast_theory.h"
#include "util/error_messages.h"
#include "util/global_values.h"

#include <iostream>

using namespace std;
using namespace smtlib;
using namespace smtlib::ast;

sptr_t<SyntaxChecker::Error>
SyntaxChecker::addError(string message, sptr_t<AstNode> node,
                        sptr_t<SyntaxChecker::Error> err) {
    if (!err) {
        err = make_shared<Error>(message, node);
        errors.push_back(err);
    } else {
        err->messages.push_back(message);
    }

    return err;
}

sptr_t<SyntaxChecker::Error>
SyntaxChecker::checkParamUsage(sptr_v<Symbol> &params,
                               unordered_map<string, bool> &paramUsage,
                               sptr_t<Sort> sort,
                               sptr_t<AstNode> source,
                               sptr_t<Error> err) {
    if (!sort)
        return err;

    string name = sort->getIdentifier()->toString();
    bool isParam = false;
    for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++) {
        if (name == (*paramIt)->toString())
            isParam = true;
    }

    if (isParam) {
        paramUsage[name] = true;

        if (!sort->getArgs().empty()) {
            err = addError(ErrorMessages::buildSortParamArity(sort->toString(), name), source, err);
        }
    } else {
        sptr_v<Sort> argSorts = sort->getArgs();
        for (auto argIt = argSorts.begin(); argIt != argSorts.end(); argIt++) {
            checkParamUsage(params, paramUsage, *argIt, source, err);
        }
    }

    return err;
}

void SyntaxChecker::visit(sptr_t<Attribute> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getKeyword()) {
        err = addError(ErrorMessages::ERR_ATTR_MISSING_KEYWORD, node, err);
    } else {
        visit0(node->getKeyword());
    }

    visit0(node->getValue());
}

void SyntaxChecker::visit(sptr_t<CompAttributeValue> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    visit0(node->getValues());
}

void SyntaxChecker::visit(sptr_t<Symbol> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!regex_match(node->getValue(), regexSymbol)) {
        err = addError(ErrorMessages::ERR_SYMBOL_MALFORMED, node, err);
    }
}

void SyntaxChecker::visit(sptr_t<Keyword> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!regex_match(node->getValue(), regexKeyword)) {
        err = addError(ErrorMessages::ERR_KEYWORD_MALFORMED, node, err);
    }
}

void SyntaxChecker::visit(sptr_t<MetaSpecConstant> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<BooleanValue> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<PropLiteral> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_PROP_LIT_MISSING_SYMBOL, node, err);
    } else {
        visit0(node->getSymbol());
    }
}

void SyntaxChecker::visit(sptr_t<AssertCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getTerm()) {
        err = addError(ErrorMessages::ERR_ASSERT_MISSING_TERM, node, err);
    } else {
        visit0(node->getTerm());
    }
}

void SyntaxChecker::visit(sptr_t<CheckSatCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<CheckSatAssumCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    visit0(node->getAssumptions());
}

void SyntaxChecker::visit(sptr_t<DeclareConstCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_DECL_CONST_MISSING_NAME, node, err);
    } else {
        visit0(node->getSymbol());
    }

    if (!node->getSort()) {
        err = addError(ErrorMessages::ERR_DECL_CONST_MISSING_SORT, node, err);
    } else {
        visit0(node->getSort());
    }
}


void SyntaxChecker::visit(sptr_t<DeclareDatatypeCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_DECL_DATATYPE_MISSING_NAME, node, err);
    } else {
        visit0(node->getSymbol());
    }

    if (!node->getDeclaration()) {
        err = addError(ErrorMessages::ERR_DECL_DATATYPE_MISSING_DECL, node, err);
    } else {
        visit0(node->getDeclaration());
    }
}

void SyntaxChecker::visit(sptr_t<DeclareDatatypesCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (node->getSorts().empty()) {
        err = addError(ErrorMessages::ERR_DECL_DATATYPES_MISSING_SORTS, node, err);
    }

    if (node->getDeclarations().empty()) {
        err = addError(ErrorMessages::ERR_DECL_DATATYPES_MISSING_DECLS, node, err);
    }

    unsigned long sortCount = node->getSorts().size();
    unsigned long declCount = node->getDeclarations().size();

    if (node->getSorts().size() != node->getDeclarations().size()) {
        err = addError(ErrorMessages::buildDeclDatatypesCount(sortCount, declCount), node, err);
    }

    unsigned long minCount = sortCount < declCount ? sortCount : declCount;
    for (unsigned long i = 0; i < minCount; i++) {
        unsigned long arity = (unsigned long)node->getSorts()[i]->getArity()->getValue();
        unsigned long paramCount = 0;
        sptr_t<ParametricDatatypeDeclaration> decl =
                dynamic_pointer_cast<ParametricDatatypeDeclaration>(node->getDeclarations()[i]);
        if (decl) {
            paramCount = decl->getParams().size();
        }

        if (arity != paramCount) {
            err = addError(ErrorMessages::buildDeclDatatypeArity(
                    node->getSorts()[i]->getSymbol()->toString(), arity, paramCount), node, err);
        }
    }

    visit0(node->getSorts());

    visit0(node->getDeclarations());
}

void SyntaxChecker::visit(sptr_t<DeclareFunCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_DECL_FUN_MISSING_NAME, node, err);
    } else {
        visit0(node->getSymbol());
    }

    visit0(node->getParams());

    if (!node->getSort()) {
        err = addError(ErrorMessages::ERR_DECL_FUN_MISSING_RET, node, err);
    } else {
        visit0(node->getSort());
    }
}

void SyntaxChecker::visit(sptr_t<DeclareSortCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_DECL_SORT_MISSING_NAME, node, err);
    } else {
        visit0(node->getSymbol());
    }

    if (!node->getArity()) {
        err = addError(ErrorMessages::ERR_DECL_SORT_MISSING_ARITY, node, err);
    } else {
        visit0(node->getArity());
    }
}

void SyntaxChecker::visit(sptr_t<DefineFunCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getDefinition()) {
        err = addError(ErrorMessages::ERR_DEF_FUN_MISSING_DEF, node, err);
    } else {
        visit0(node->getDefinition());
    }
}

void SyntaxChecker::visit(sptr_t<DefineFunRecCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getDefinition()) {
        err = addError(ErrorMessages::ERR_DEF_FUN_REC_MISSING_DEF, node, err);
    } else {
        visit0(node->getDefinition());
    }
}

void SyntaxChecker::visit(sptr_t<DefineFunsRecCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (node->getDeclarations().empty()) {
        err = addError(ErrorMessages::ERR_DEF_FUNS_REC_EMPTY_DECLS, node, err);
    }

    if (node->getBodies().empty()) {
        err = addError(ErrorMessages::ERR_DEF_FUNS_REC_EMPTY_BODIES, node, err);
    }

    unsigned long declCount = node->getDeclarations().size();
    unsigned long bodyCount = node->getBodies().size();


    if (declCount != bodyCount) {
        err = addError(ErrorMessages::buildDefFunsRecCount(declCount, bodyCount), node, err);
    }

    visit0(node->getDeclarations());

    visit0(node->getDeclarations());
}

void SyntaxChecker::visit(sptr_t<DefineSortCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    // Check symbol
    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_DEF_SORT_MISSING_NAME, node, err);
    } else {
        visit0(node->getSymbol());
    }

    // Check parameter list
    visit0(node->getParams());

    // Check definition
    if (!node->getSort()) {
        err = addError(ErrorMessages::ERR_DEF_SORT_MISSING_DEF, node, err);
    } else {
        visit0(node->getSort());
    }

    // Check parameter usage
    unordered_map<string, bool> paramUsage;
    err = checkParamUsage(node->getParams(), paramUsage, node->getSort(), node, err);

    if (paramUsage.size() != node->getParams().size()) {
        vector<string> unusedParams;
        sptr_v<Symbol> params = node->getParams();
        for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++) {
            string pname = (*paramIt)->toString();
            if (paramUsage.find(pname) == paramUsage.end()) {
                unusedParams.push_back("'" + pname + "'");
            }
        }
        err = addError(ErrorMessages::buildSortDefUnusedSortParams(unusedParams), node, err);
    }
}

void SyntaxChecker::visit(sptr_t<EchoCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (node->getMessage() == "") {
        err = addError(ErrorMessages::ERR_ECHO_EMPTY_STRING, node, err);
    }

}

void SyntaxChecker::visit(sptr_t<ExitCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<GetAssertsCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<GetAssignsCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<GetInfoCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getFlag()) {
        err = addError(ErrorMessages::ERR_GET_INFO_MISSING_FLAG, node, err);
    } else {
        visit0(node->getFlag());
    }
}

void SyntaxChecker::visit(sptr_t<GetModelCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<GetOptionCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getOption()) {
        err = addError(ErrorMessages::ERR_GET_OPT_MISSING_OPT, node, err);
    } else {
        visit0(node->getOption());
    }
}

void SyntaxChecker::visit(sptr_t<GetProofCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<GetUnsatAssumsCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<GetUnsatCoreCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<GetValueCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (node->getTerms().empty()) {
        err = addError(ErrorMessages::ERR_GET_VALUE_EMPTY_TERMS, node, err);
    } else {
        visit0(node->getTerms());
    }
}

void SyntaxChecker::visit(sptr_t<PopCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getNumeral()) {
        err = addError(ErrorMessages::ERR_POP_MISSING_NUMERAL, node, err);
    } else {
        visit0(node->getNumeral());
    }
}

void SyntaxChecker::visit(sptr_t<PushCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getNumeral()) {
        err = addError(ErrorMessages::ERR_PUSH_MISSING_NUMERAL, node, err);
    } else {
        visit0(node->getNumeral());
    }
}

void SyntaxChecker::visit(sptr_t<ResetCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<ResetAssertsCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<SetInfoCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getInfo()) {
        err = addError(ErrorMessages::ERR_SET_INFO_MISSING_INFO, node, err);
    } else {
        visit0(node->getInfo());
    }
}

void SyntaxChecker::visit(sptr_t<SetLogicCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getLogic()) {
        err = addError(ErrorMessages::ERR_SET_LOGIC_MISSING_LOGIC, node, err);
    }
}

void SyntaxChecker::visit(sptr_t<SetOptionCommand> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getOption()) {
        err = addError(ErrorMessages::ERR_SET_OPT_MISSING_OPT, node, err);
    } else {
        sptr_t<Attribute> option = node->getOption();
        if ((option->getKeyword()->getValue() == KW_DIAG_OUTPUT_CHANNEL
             || option->getKeyword()->getValue() == KW_REGULAR_OUTPUT_CHANNEL)
            && !dynamic_cast<StringLiteral *>(option->getValue().get())) {
            err = addError(ErrorMessages::ERR_OPT_VALUE_STRING, option, err);
        } else if ((option->getKeyword()->getValue() == KW_RANDOM_SEED
                    || option->getKeyword()->getValue() == KW_VERBOSITY
                    || option->getKeyword()->getValue() == KW_REPROD_RESOURCE_LIMIT)
                   && !dynamic_cast<NumeralLiteral *>(option->getValue().get())) {
            err = addError(ErrorMessages::ERR_OPT_VALUE_NUMERAL, option, err);
        } else if ((option->getKeyword()->getValue() == KW_EXPAND_DEFS
                    || option->getKeyword()->getValue() == KW_GLOBAL_DECLS
                    || option->getKeyword()->getValue() == KW_INTERACTIVE_MODE
                    || option->getKeyword()->getValue() == KW_PRINT_SUCCESS
                    || option->getKeyword()->getValue() == KW_PROD_ASSERTS
                    || option->getKeyword()->getValue() == KW_PROD_ASSIGNS
                    || option->getKeyword()->getValue() == KW_PROD_MODELS
                    || option->getKeyword()->getValue() == KW_PROD_PROOFS
                    || option->getKeyword()->getValue() == KW_PROD_UNSAT_ASSUMS
                    || option->getKeyword()->getValue() == KW_PROD_UNSAT_CORES)) {
            if (!option->getValue() || (option->getValue()->toString() != CONST_TRUE
                                        && option->getValue()->toString() != CONST_FALSE)) {
                err = addError(ErrorMessages::ERR_OPT_VALUE_BOOLEAN, option, err);
            }
        }

        visit0(node->getOption());
    }
}

void SyntaxChecker::visit(sptr_t<FunctionDeclaration> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_FUN_DECL_MISSING_NAME, node, err);
    } else {
        visit0(node->getSymbol());
    }

    visit0(node->getParams());

    if (!node->getSort()) {
        err = addError(ErrorMessages::ERR_FUN_DECL_MISSING_RET, node, err);
    } else {
        visit0(node->getSort());
    }
}

void SyntaxChecker::visit(sptr_t<FunctionDefinition> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSignature()) {
        err = addError(ErrorMessages::ERR_FUN_DEF_MISSING_SIG, node, err);
    } else {
        visit0(node->getSignature());
    }

    if (!node->getBody()) {
        err = addError(ErrorMessages::ERR_FUN_DEF_MISSING_BODY, node, err);
    } else {
        visit0(node->getBody());
    }
}

void SyntaxChecker::visit(sptr_t<SimpleIdentifier> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_ID_MISSING_SYMBOL, node, err);
    } else {
        visit0(node->getSymbol());
    }

    if (node->isIndexed() && node->getIndices().empty()) {
        err = addError(ErrorMessages::ERR_ID_EMPTY_INDICES, node, err);
    }

    visit0(node->getIndices());
}

void SyntaxChecker::visit(sptr_t<QualifiedIdentifier> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getIdentifier()) {
        err = addError(ErrorMessages::ERR_QUAL_ID_MISSING_ID, node, err);
    } else {
        visit0(node->getIdentifier());
    }

    if (!node->getSort()) {
        err = addError(ErrorMessages::ERR_QUAL_ID_MISSING_SORT, node, err);
    } else {
        visit0(node->getSort());
    }
}

void SyntaxChecker::visit(sptr_t<DecimalLiteral> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<NumeralLiteral> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<StringLiteral> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }
}

void SyntaxChecker::visit(sptr_t<Logic> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    sptr_v<Attribute> attrs = node->getAttributes();

    if (!node->getName()) {
        err = addError(ErrorMessages::ERR_LOGIC_MISSING_NAME, node, err);
    }

    if (attrs.empty()) {
        err = addError(ErrorMessages::ERR_LOGIC_EMPTY_ATTRS, node, err);
    }

    for (auto attrIt = attrs.begin(); attrIt != attrs.end(); attrIt++) {
        sptr_t<Attribute> attr = *attrIt;
        if (!attr)
            continue;

        sptr_t<Error> attrerr;

        if (attr->getKeyword()->getValue() == KW_LANGUAGE
            || attr->getKeyword()->getValue() == KW_EXTENSIONS
            || attr->getKeyword()->getValue() == KW_VALUES
            || attr->getKeyword()->getValue() == KW_NOTES) {
            if (!dynamic_cast<StringLiteral *>(attr->getValue().get())) {
                attrerr = addError(ErrorMessages::ERR_ATTR_VALUE_STRING, attr, attrerr);
            }
        } else if (attr->getKeyword()->getValue() == KW_THEORIES) {
            if (!dynamic_cast<CompAttributeValue *>(attr->getValue().get())) {
                err = addError(ErrorMessages::ERR_ATTR_VALUE_THEORIES, attr, err);
            } else {
                CompAttributeValue *val = dynamic_cast<CompAttributeValue *>(attr->getValue().get());
                sptr_v<AttributeValue> values = val->getValues();

                // Note: standard prohibits empty theory list, but there are logics that only use Core
                /*if (values.empty()) {
                    err = addError(ErrorMessages::ERR_ATTR_VALUE_THEORIES_EMPTY, attr, err);
                }*/

                for (auto valueIt = values.begin(); valueIt != values.begin(); valueIt++) {
                    if ((*valueIt) && !dynamic_cast<Symbol *>((*valueIt).get())) {
                        attrerr = addError(ErrorMessages::buildAttrValueSymbol((*valueIt)->toString()), attr, attrerr);
                    }
                }
            }
        }

        visit0(attr);
    }
}

void SyntaxChecker::visit(sptr_t<Theory> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    sptr_v<Attribute> attrs = node->getAttributes();

    if (!node->getName()) {
        err = addError(ErrorMessages::ERR_THEORY_MISSING_NAME, node, err);
    }

    if (attrs.empty()) {
        err = addError(ErrorMessages::ERR_THEORY_EMPTY_ATTRS, node, err);
    }

    for (auto attrIt = attrs.begin(); attrIt != attrs.end(); attrIt++) {
        sptr_t<Attribute> attr = *attrIt;
        if (!attr)
            continue;

        sptr_t<Error> attrerr;

        if (attr->getKeyword()->getValue() == KW_SORTS_DESC
            || attr->getKeyword()->getValue() == KW_FUNS_DESC
            || attr->getKeyword()->getValue() == KW_DEFINITION
            || attr->getKeyword()->getValue() == KW_VALUES
            || attr->getKeyword()->getValue() == KW_NOTES) {
            if (!dynamic_cast<StringLiteral *>(attr->getValue().get())) {
                attrerr = addError(ErrorMessages::ERR_ATTR_VALUE_STRING, attr, attrerr);
            }
        } else if (attr->getKeyword()->getValue() == KW_SORTS) {
            if (!dynamic_cast<CompAttributeValue *>(attr->getValue().get())) {
                attrerr = addError(ErrorMessages::ERR_ATTR_VALUE_SORTS, attr, attrerr);
            } else {
                CompAttributeValue *val = dynamic_cast<CompAttributeValue *>(attr->getValue().get());
                sptr_v<AttributeValue> values = val->getValues();

                if (values.empty()) {
                    attrerr = addError(ErrorMessages::ERR_ATTR_VALUE_SORTS_EMPTY, attr, attrerr);
                }

                for (auto valueIt = values.begin(); valueIt != values.begin(); valueIt++) {
                    if ((*valueIt) && !dynamic_cast<SortSymbolDeclaration *>((*valueIt).get())) {
                        attrerr = addError(
                                ErrorMessages::buildAttrValueSortDecl((*valueIt)->toString()), attr, attrerr);
                    }
                }
            }
        } else if (attr->getKeyword()->getValue() == KW_FUNS) {
            if (!dynamic_cast<CompAttributeValue *>(attr->getValue().get())) {
                attrerr = addError(ErrorMessages::ERR_ATTR_VALUE_FUNS, attr, attrerr);
            } else {
                CompAttributeValue *val = dynamic_cast<CompAttributeValue *>(attr->getValue().get());
                sptr_v<AttributeValue> values = val->getValues();

                if (values.empty()) {
                    attrerr = addError(ErrorMessages::ERR_ATTR_VALUE_FUNS_EMPTY, attr, attrerr);
                }

                for (auto valueIt = values.begin(); valueIt != values.begin(); valueIt++) {
                    if ((*valueIt) && !dynamic_cast<FunSymbolDeclaration *>((*valueIt).get())) {
                        attrerr = addError(
                                ErrorMessages::buildAttrValueFunDecl((*valueIt)->toString()), attr, attrerr);
                    }
                }
            }
        }

        visit0(attr);
    }
}

void SyntaxChecker::visit(sptr_t<Script> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    visit0(node->getCommands());
}

void SyntaxChecker::visit(sptr_t<Sort> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getIdentifier()) {
        err = addError(ErrorMessages::ERR_SORT_MISSING_ID, node, err);
    } else {
        visit0(node->getIdentifier());
    }

    if (node->hasArgs() && node->getArgs().empty()) {
        err = addError(ErrorMessages::ERR_SORT_EMPTY_ARGS, node, err);
    } else {
        visit0(node->getArgs());
    }
}

void SyntaxChecker::visit(sptr_t<CompSExpression> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    visit0(node->getExpressions());
}

void SyntaxChecker::visit(sptr_t<SortSymbolDeclaration> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getIdentifier()) {
        err = addError(ErrorMessages::ERR_SORT_SYM_DECL_MISSING_ID, node, err);
    } else {
        visit0(node->getIdentifier());
    }

    if (!node->getArity()) {
        err = addError(ErrorMessages::ERR_SORT_SYM_DECL_MISSING_ARITY, node, err);
    } else {
        visit0(node->getArity());
    }

    visit0(node->getAttributes());
}

void SyntaxChecker::visit(sptr_t<SpecConstFunDeclaration> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getConstant()) {
        err = addError(ErrorMessages::ERR_SPEC_CONST_DECL_MISSING_CONST, node, err);
    } else {
        visit0(node->getConstant());
    }

    if (!node->getSort()) {
        err = addError(ErrorMessages::ERR_SPEC_CONST_DECL_MISSING_SORT, node, err);
    } else {
        visit0(node->getSort());
    }

    visit0(node->getAttributes());
}

void SyntaxChecker::visit(sptr_t<MetaSpecConstFunDeclaration> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getConstant()) {
        err = addError(ErrorMessages::ERR_META_SPEC_CONST_DECL_MISSING_CONST, node, err);
    } else {
        visit0(node->getConstant());
    }

    if (!node->getSort()) {
        err = addError(ErrorMessages::ERR_META_SPEC_CONST_DECL_MISSING_SORT, node, err);
    } else {
        visit0(node->getSort());
    }

    visit0(node->getAttributes());
}

void SyntaxChecker::visit(sptr_t<SimpleFunDeclaration> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getIdentifier()) {
        err = addError(ErrorMessages::ERR_FUN_SYM_DECL_MISSING_ID, node, err);
    } else {
        visit0(node->getIdentifier());
    }

    if (node->getSignature().empty()) {
        err = addError(ErrorMessages::ERR_FUN_SYM_DECL_EMPTY_SIG, node, err);
    } else {
        visit0(node->getSignature());
    }

    visit0(node->getAttributes());
}

void SyntaxChecker::visit(sptr_t<ParametricFunDeclaration> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    // Check parameter list
    if (node->getParams().empty()) {
        err = addError(ErrorMessages::ERR_PARAM_FUN_SYM_DECL_EMPTY_PARAMS, node, err);
    } else {
        visit0(node->getParams());
    }

    // Check identifier
    if (!node->getIdentifier()) {
        err = addError(ErrorMessages::ERR_PARAM_FUN_SYM_DECL_MISSING_ID, node, err);
    } else {
        visit0(node->getIdentifier());
    }

    // Check signature
    if (node->getSignature().empty()) {
        err = addError(ErrorMessages::ERR_PARAM_FUN_SYM_DECL_EMPTY_SIG, node, err);
    } else {
        visit0(node->getSignature());
    }

    // Check parameter usage
    unordered_map<string, bool> paramUsage;
    sptr_v<Sort> sig = node->getSignature();
    for (auto sortIt = sig.begin(); sortIt != sig.end(); sortIt++) {
        err = checkParamUsage(node->getParams(), paramUsage, *sortIt, node, err);
    }

    if (paramUsage.size() != node->getParams().size()) {
        vector<string> unusedParams;
        sptr_v<Symbol> params = node->getParams();
        for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++) {
            string pname = (*paramIt)->toString();
            if (paramUsage.find(pname) == paramUsage.end()) {
                unusedParams.push_back("'" + pname + "'");
            }
        }

        err = addError(ErrorMessages::buildParamFunDeclUnusedSortParams(unusedParams), node, err);
    }

    // Check attribute list
    visit0(node->getAttributes());
}

void SyntaxChecker::visit(sptr_t<SortDeclaration> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_SORT_DECL_MISSING_SYMBOL, node, err);
    } else {
        visit0(node->getSymbol());
    }

    if (!node->getArity()) {
        err = addError(ErrorMessages::ERR_SORT_DECL_MISSING_ARITY, node, err);
    } else {
        visit0(node->getArity());
    }
}

void SyntaxChecker::visit(sptr_t<SelectorDeclaration> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_SEL_DECL_MISSING_SYMBOL, node, err);
    } else {
        visit0(node->getSymbol());
    }

    if (!node->getSort()) {
        err = addError(ErrorMessages::ERR_SEL_DECL_MISSING_SORT, node, err);
    } else {
        visit0(node->getSort());
    }
}

void SyntaxChecker::visit(sptr_t<ConstructorDeclaration> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_CONS_DECL_MISSING_SYMBOL, node, err);
    } else {
        visit0(node->getSymbol());
    }

    sptr_v<SelectorDeclaration> &selectors = node->getSelectors();
    for (auto selIt = selectors.begin(); selIt != selectors.end(); selIt++) {
        visit0((*selIt));
    }
}


void SyntaxChecker::visit(sptr_t<SimpleDatatypeDeclaration> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (node->getConstructors().empty()) {
        err = addError(ErrorMessages::ERR_DATATYPE_DECL_EMPTY_CONS, node, err);
    } else {
        visit0(node->getConstructors());
    }

}

void SyntaxChecker::visit(sptr_t<ParametricDatatypeDeclaration> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (node->getParams().empty()) {
        err = addError(ErrorMessages::ERR_PARAM_DATATYPE_DECL_EMPTY_PARAMS, node, err);
    } else {
        visit0(node->getParams());
    }

    if (node->getConstructors().empty()) {
        err = addError(ErrorMessages::ERR_PARAM_DATATYPE_DECL_EMPTY_CONS, node, err);
    } else {
        visit0(node->getConstructors());
    }

    std::unordered_map<std::string, bool> paramUsage;

    sptr_v<ConstructorDeclaration> constructors = node->getConstructors();
    for (auto consIt = constructors.begin(); consIt != constructors.end(); consIt++) {
        sptr_v<SelectorDeclaration> selectors = (*consIt)->getSelectors();
        for (auto selit = selectors.begin(); selit != selectors.end(); selit++) {
            err = checkParamUsage(node->getParams(), paramUsage, (*selit)->getSort(), node, err);
        }
    }

    if (paramUsage.size() != node->getParams().size()) {
        vector<string> unusedParams;
        sptr_v<Symbol> params = node->getParams();
        for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++) {
            string pname = (*paramIt)->toString();
            if (paramUsage.find(pname) == paramUsage.end()) {
                unusedParams.push_back("'" + pname + "'");
            }
        }
        err = addError(ErrorMessages::buildParamDatatypeDeclUnusedSortParams(unusedParams), node, err);
    }
}

void SyntaxChecker::visit(sptr_t<QualifiedConstructor> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_QUAL_CONS_MISSING_SYMBOL, node, err);
    } else {
        visit0(node->getSymbol());
    }

    if (!node->getSort()) {
        err = addError(ErrorMessages::ERR_QUAL_CONS_MISSING_SORT, node, err);
    } else {
        visit0(node->getSort());
    }
}

void SyntaxChecker::visit(sptr_t<QualifiedPattern> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getConstructor()) {
        err = addError(ErrorMessages::ERR_QUAL_PATTERN_MISSING_CONS, node, err);
    } else {
        visit0(node->getConstructor());
    }

    if (node->getSymbols().empty()) {
        err = addError(ErrorMessages::ERR_QUAL_PATTERN_EMPTY_SYMS, node, err);
    } else {
        visit0(node->getSymbols());
    }
}

void SyntaxChecker::visit(sptr_t<MatchCase> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getPattern()) {
        err = addError(ErrorMessages::ERR_MATCH_CASE_MISSING_PATTERN, node, err);
    } else {
        visit0(node->getPattern());
    }

    if (!node->getTerm()) {
        err = addError(ErrorMessages::ERR_MATCH_CASE_MISSING_TERM, node, err);
    } else {
        visit0(node->getTerm());
    }
}

void SyntaxChecker::visit(sptr_t<QualifiedTerm> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getIdentifier()) {
        err = addError(ErrorMessages::ERR_QUAL_TERM_MISSING_ID, node, err);
    } else {
        visit0(node->getIdentifier());
    }

    if (node->getTerms().empty()) {
        err = addError(ErrorMessages::ERR_QUAL_TERM_EMPTY_TERMS, node, err);
    } else {
        visit0(node->getTerms());
    }
}

void SyntaxChecker::visit(sptr_t<LetTerm> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (node->getBindings().empty()) {
        err = addError(ErrorMessages::ERR_LET_TERM_EMPTY_VARS, node, err);
    } else {
        sptr_v<VarBinding> &bindings = node->getBindings();
        for (auto bindingIt = bindings.begin(); bindingIt != bindings.end(); bindingIt++) {
            visit0((*bindingIt));
        }
    }

    if (!node->getTerm()) {
        err = addError(ErrorMessages::ERR_LET_TERM_MISSING_TERM, node, err);
    } else {
        visit0(node->getTerm());
    }
}

void SyntaxChecker::visit(sptr_t<ForallTerm> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (node->getBindings().empty()) {
        err = addError(ErrorMessages::ERR_FORALL_TERM_EMPTY_VARS, node, err);
    } else {
        sptr_v<SortedVariable> &bindings = node->getBindings();
        for (auto bindingIt = bindings.begin(); bindingIt != bindings.end(); bindingIt++) {
            visit0((*bindingIt));
        }
    }

    if (!node->getTerm()) {
        err = addError(ErrorMessages::ERR_FORALL_TERM_MISSING_TERM, node, err);
    } else {
        visit0(node->getTerm());
    }
}

void SyntaxChecker::visit(sptr_t<ExistsTerm> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (node->getBindings().empty()) {
        err = addError(ErrorMessages::ERR_EXISTS_TERM_EMPTY_VARS, node, err);
    } else {
        sptr_v<SortedVariable> &bindings = node->getBindings();
        for (auto bindingIt = bindings.begin(); bindingIt != bindings.end(); bindingIt++) {
            visit0((*bindingIt));
        }
    }

    if (!node->getTerm()) {
        err = addError(ErrorMessages::ERR_EXISTS_TERM_MISSING_TERM, node, err);
    } else {
        visit0(node->getTerm());
    }
}

void SyntaxChecker::visit(sptr_t<MatchTerm> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getTerm()) {
        err = addError(ErrorMessages::ERR_MATCH_TERM_MISSING_TERM, node, err);
    } else {
        visit0(node->getTerm());
    }

    if (node->getCases().empty()) {
        err = addError(ErrorMessages::ERR_MATCH_TERM_EMPTY_CASES, node, err);
    } else {
        sptr_v<MatchCase> &cases = node->getCases();
        for (auto caseIt = cases.begin(); caseIt != cases.end(); caseIt++) {
            visit0((*caseIt));
        }
    }
}

void SyntaxChecker::visit(sptr_t<AnnotatedTerm> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getTerm()) {
        err = addError(ErrorMessages::ERR_ANNOT_TERM_MISSING_TERM, node, err);
    } else {
        visit0(node->getTerm());
    }

    if (node->getAttributes().empty()) {
        err = addError(ErrorMessages::ERR_ANNOT_TERM_EMPTY_ATTRS, node, err);
    } else {
        visit0(node->getAttributes());
    }
}

void SyntaxChecker::visit(sptr_t<SortedVariable> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_SORTED_VAR_MISSING_SYMBOL, node, err);
    } else {
        visit0(node->getSymbol());
    }

    if (!node->getSort()) {
        err = addError(ErrorMessages::ERR_SORTED_VAR_MISSING_SORT, node, err);
    } else {
        visit0(node->getSort());
    }
}

void SyntaxChecker::visit(sptr_t<VarBinding> node) {
    sptr_t<Error> err;

    if (!node) {
        err = addError(ErrorMessages::ERR_NULL_NODE_VISIT, node, err);
        return;
    }

    if (!node->getSymbol()) {
        err = addError(ErrorMessages::ERR_VAR_BIND_MISSING_SYMBOL, node, err);
    } else {
        visit0(node->getSymbol());
    }

    if (!node->getTerm()) {
        err = addError(ErrorMessages::ERR_VAR_BIND_MISSING_SORT, node, err);
    } else {
        visit0(node->getTerm());
    }
}

bool SyntaxChecker::check(sptr_t<AstNode> node) {
    visit0(node);
    return errors.empty();
}

string SyntaxChecker::getErrors() {
    stringstream ss;
    for (auto errIt = errors.begin(); errIt != errors.end(); errIt++) {
        sptr_t<Error> err = *errIt;
        if (err->node) {
            ss << err->node->getRowLeft() << ":" << err->node->getColLeft()
            << " - " << err->node->getRowRight() << ":" << err->node->getColRight() << "   ";

            string nodestr = err->node->toString();
            if (nodestr.length() > 100)
                ss << string(nodestr, 0, 100) << "[...]";
            else
                ss << nodestr;
        } else {
            ss << "NULL";
        }

        ss << endl;
        for (auto itt = err->messages.begin(); itt != err->messages.end(); itt++) {
            ss << "\t" << *itt << "." << endl;
        }

        ss << endl;
    }

    return ss.str();
}