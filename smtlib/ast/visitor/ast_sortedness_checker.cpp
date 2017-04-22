#include "ast_sortedness_checker.h"
#include "ast_syntax_checker.h"
#include "ast/ast_logic.h"
#include "ast/ast_script.h"
#include "ast/ast_theory.h"
#include "smtlib/parser/smtlib_parser.h"
#include "util/error_messages.h"
#include "util/global_values.h"
#include "exec/execution.h"

using namespace std;
using namespace smtlib;
using namespace smtlib::ast;

/* ============================= SortednessCheckerContext ============================= */

SortednessCheckerContext::SortednessCheckerContext() {
    stack = make_shared<SymbolStack>();
    config = make_shared<Configuration>();
}

SortednessCheckerContext::SortednessCheckerContext(sptr_t<SymbolStack> stack)
        : stack(stack) { }

sptr_t<SymbolStack> SortednessCheckerContext::getStack() {
    return stack;
}

vector<string>& SortednessCheckerContext::getCurrentTheories() {
    return currentTheories;
}

string SortednessCheckerContext::getCurrentLogic() {
    return currentLogic;
}

sptr_t<Configuration> SortednessCheckerContext::getConfiguration() {
    return config;
}

void SortednessCheckerContext::setCurrentLogic(string logic) {
    currentLogic = logic;
}

/* ================================ SortednessChecker ================================= */

sptr_t<SortednessChecker::NodeError>
SortednessChecker::addError(string message, sptr_t<AstNode> node,
                            sptr_t<SortednessChecker::NodeError> err) {
    if (!err) {
        sptr_t<Error> errInfo =
                make_shared<Error>(message);
        err = make_shared<NodeError>(errInfo, node);
        if (node && node->getFilename())
            errors[string(node->getFilename()->c_str())].push_back(err);
        else
            errors[""].push_back(err);
    } else {
        sptr_t<Error> errInfo =
                make_shared<Error>(message);
        err->errs.push_back(errInfo);
    }

    return err;
}

sptr_t<SortednessChecker::NodeError>
SortednessChecker::addError(string message, sptr_t<AstNode> node,
                            sptr_t<SymbolInfo> info,
                            sptr_t<SortednessChecker::NodeError> err) {
    if (!err) {
        sptr_t<Error> errInfo =
                make_shared<Error>(message, info);
        err = make_shared<NodeError>(errInfo, node);
        if (node && node->getFilename())
            errors[string(node->getFilename()->c_str())].push_back(err);
        else
            errors[""].push_back(err);
    } else {
        sptr_t<Error> errInfo =
                make_shared<Error>(message, info);
        err->errs.push_back(errInfo);
    }

    return err;
}

void SortednessChecker::addError(string message, sptr_t<AstNode> node) {
    sptr_t<Error> errInfo =
            make_shared<Error>(message);
    sptr_t<NodeError> err =
            make_shared<NodeError>(errInfo, node);
    if (node && node->getFilename())
        errors[string(node->getFilename()->c_str())].push_back(err);
    else
        errors[""].push_back(err);
}

void SortednessChecker::addError(string message, sptr_t<AstNode> node,
                                 sptr_t<SymbolInfo> info) {
    sptr_t<Error> errInfo =
            make_shared<Error>(message, info);
    sptr_t<NodeError> err =
            make_shared<NodeError>(errInfo, node);
    errors[string(node->getFilename()->c_str())].push_back(err);
}

sptr_t<SortInfo> SortednessChecker::getInfo(sptr_t<DeclareSortCommand> node) {
    return make_shared<SortInfo>(node->getSymbol()->toString(), node->getArity()->getValue(), node);
}

sptr_t<SortInfo> SortednessChecker::getInfo(sptr_t<DefineSortCommand> node) {
    return make_shared<SortInfo>(node->getSymbol()->toString(), node->getParams().size(),
                                 node->getParams(), ctx->getStack()->expand(node->getSort()), node);
}


sptr_t<SortInfo> SortednessChecker::getInfo(sptr_t<SortSymbolDeclaration> node) {
    return make_shared<SortInfo>(node->getIdentifier()->toString(),
                                 node->getArity()->getValue(),
                                 node->getAttributes(), node);
}

sptr_t<FunInfo> SortednessChecker::getInfo(sptr_t<SpecConstFunDeclaration> node) {
    sptr_v<Sort> sig;
    sig.push_back(ctx->getStack()->expand(node->getSort()));

    return make_shared<FunInfo>(node->getConstant()->toString(), sig, node->getAttributes(), node);
}

sptr_t<FunInfo> SortednessChecker::getInfo(sptr_t<MetaSpecConstFunDeclaration> node) {
    sptr_v<Sort> sig;
    sig.push_back(ctx->getStack()->expand(node->getSort()));
    return make_shared<FunInfo>(node->getConstant()->toString(), sig, node->getAttributes(), node);
}

sptr_t<FunInfo> SortednessChecker::getInfo(sptr_t<SimpleFunDeclaration> node) {
    sptr_v<Sort> &sig = node->getSignature();
    sptr_v<Sort> newsig;

    for (auto sortIt = sig.begin(); sortIt != sig.end(); sortIt++) {
        newsig.push_back(ctx->getStack()->expand(*sortIt));
    }

    sptr_t<FunInfo> funInfo = make_shared<FunInfo>(node->getIdentifier()->toString(), newsig,
                                                       node->getAttributes(), node);

    sptr_v<Attribute> attrs = node->getAttributes();
    for (auto attr = attrs.begin(); attr != attrs.end(); attr++) {
        if ((*attr)->toString() == KW_RIGHT_ASSOC) {
            funInfo->assocR = true;
        }

        if ((*attr)->toString() == KW_LEFT_ASSOC) {
            funInfo->assocL = true;
        }

        if ((*attr)->toString() == KW_CHAINABLE) {
            funInfo->chainable = true;
        }

        if ((*attr)->toString() == KW_PAIRWISE) {
            funInfo->pairwise = true;
        }
    }

    return funInfo;
}

sptr_t<FunInfo> SortednessChecker::getInfo(sptr_t<ParametricFunDeclaration> node) {
    sptr_v<Sort> &sig = node->getSignature();
    sptr_v<Sort> newsig;

    for (auto sortIt = sig.begin(); sortIt != sig.end(); sortIt++) {
        newsig.push_back(ctx->getStack()->expand(*sortIt));
    }

    sptr_t<FunInfo> funInfo = make_shared<FunInfo>(node->getIdentifier()->toString(), newsig,
                                                       node->getParams(), node->getAttributes(), node);

    sptr_v<Attribute> attrs = node->getAttributes();
    for (auto attr = attrs.begin(); attr != attrs.end(); attr++) {
        if ((*attr)->toString() == KW_RIGHT_ASSOC) {
            funInfo->assocR = true;
        }

        if ((*attr)->toString() == KW_LEFT_ASSOC) {
            funInfo->assocL = true;
        }

        if ((*attr)->toString() == KW_CHAINABLE) {
            funInfo->chainable = true;
        }

        if ((*attr)->toString() == KW_PAIRWISE) {
            funInfo->pairwise = true;
        }
    }

    return funInfo;
}

sptr_t<FunInfo> SortednessChecker::getInfo(sptr_t<DeclareConstCommand> node) {
    sptr_v<Sort> sig;
    sig.push_back(ctx->getStack()->expand(node->getSort()));

    return make_shared<FunInfo>(node->getSymbol()->toString(), sig, node);
}

sptr_t<FunInfo> SortednessChecker::getInfo(sptr_t<DeclareFunCommand> node) {
    sptr_v<Sort> &sig = node->getParams();
    sptr_v<Sort> newsig;

    for (auto sortIt = sig.begin(); sortIt != sig.end(); sortIt++) {
        sptr_t<Sort> itsort = ctx->getStack()->expand(*sortIt);
        newsig.push_back(itsort);
    }
    sptr_t<Sort> retsort = ctx->getStack()->expand(node->getSort());
    newsig.push_back(retsort);

    return make_shared<FunInfo>(node->getSymbol()->toString(), newsig, node);
}

sptr_t<FunInfo> SortednessChecker::getInfo(sptr_t<DefineFunCommand> node) {
    sptr_v<Sort> newsig;
    sptr_v<SortedVariable> &params = node->getDefinition()->getSignature()->getParams();
    for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++) {
        newsig.push_back(ctx->getStack()->expand((*paramIt)->getSort()));
    }
    newsig.push_back(ctx->getStack()->expand(node->getDefinition()->getSignature()->getSort()));

    return make_shared<FunInfo>(node->getDefinition()->getSignature()->getSymbol()->toString(),
                                newsig, node->getDefinition()->getBody(), node);
}

sptr_t<FunInfo> SortednessChecker::getInfo(sptr_t<DefineFunRecCommand> node) {
    sptr_v<Sort> newsig;
    sptr_v<SortedVariable> &params = node->getDefinition()->getSignature()->getParams();
    for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++) {
        newsig.push_back(ctx->getStack()->expand((*paramIt)->getSort()));
    }
    newsig.push_back(ctx->getStack()->expand(node->getDefinition()->getSignature()->getSort()));

    return make_shared<FunInfo>(node->getDefinition()->getSignature()->getSymbol()->toString(),
                                newsig, node->getDefinition()->getBody(), node);
}

sptr_v<FunInfo> SortednessChecker::getInfo(sptr_t<DefineFunsRecCommand> node) {
    sptr_v<FunInfo> infos;
    for (unsigned long i = 0; i < node->getDeclarations().size(); i++) {
        sptr_v<Sort> newsig;
        sptr_v<SortedVariable> &params = node->getDeclarations()[i]->getParams();
        for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++) {
            newsig.push_back(ctx->getStack()->expand((*paramIt)->getSort()));
        }
        newsig.push_back(ctx->getStack()->expand(node->getDeclarations()[i]->getSort()));

        infos.push_back(make_shared<FunInfo>(node->getDeclarations()[i]->getSymbol()->toString(),
                                             newsig, node->getBodies()[i], node));
    }

    return infos;
}

sptr_v<SymbolInfo> SortednessChecker::getInfo(sptr_t<DeclareDatatypeCommand> node) {
    sptr_v<SymbolInfo> infos;
    string typeName = node->getSymbol()->toString();

    sptr_t<ParametricDatatypeDeclaration> pdecl =
            dynamic_pointer_cast<ParametricDatatypeDeclaration>(node->getDeclaration());

    if (pdecl) {
        // Add datatype (parametric) sort info
        infos.push_back(make_shared<SortInfo>(typeName, pdecl->getParams().size(), node));

        // Build a sort representing the datatype (to be used in the signatures of the constructors and selectors)
        sptr_t<Sort> typeSort = make_shared<Sort>(make_shared<SimpleIdentifier>(node->getSymbol()));

        sptr_v<Symbol> params = pdecl->getParams();
        for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++) {
            typeSort->getArgs().push_back(make_shared<Sort>(make_shared<SimpleIdentifier>(*paramIt)));
        }

        typeSort = ctx->getStack()->expand(typeSort);

        sptr_v<ConstructorDeclaration> constructors = pdecl->getConstructors();
        for (auto consIt = constructors.begin(); consIt != constructors.end(); consIt++) {
            // Start building function info for current constructor
            string consName = (*consIt)->getSymbol()->toString();
            sptr_v<Sort> consSig;

            sptr_v<SelectorDeclaration> selectors = (*consIt)->getSelectors();
            for (auto selIt = selectors.begin(); selIt != selectors.end(); selIt++) {
                // Add sort of current selector to current constructor signature
                consSig.push_back(ctx->getStack()->expand((*selIt)->getSort()));

                // Build function info for current selector
                string selName = (*selIt)->getSymbol()->toString();
                sptr_v<Sort> selSig;
                selSig.push_back(typeSort);
                selSig.push_back(ctx->getStack()->expand((*selIt)->getSort()));

                // Add selector function info
                infos.push_back(make_shared<FunInfo>(selName, selSig, pdecl->getParams(), node));
            }

            // Add constructor function info
            consSig.push_back(typeSort);
            infos.push_back(make_shared<FunInfo>(consName, consSig, pdecl->getParams(), node));
        }

    } else {
        // Add datatype (non-parametric) sort info
        infos.push_back(make_shared<SortInfo>(typeName, 0, node));

        // Build a sort representing the datatype (to be used in the signatures of the constructors and selectors)
        sptr_t<Sort> typeSort = make_shared<Sort>(make_shared<SimpleIdentifier>(node->getSymbol()));
        typeSort = ctx->getStack()->expand(typeSort);

        sptr_t<SimpleDatatypeDeclaration> sdecl =
                dynamic_pointer_cast<SimpleDatatypeDeclaration>(node->getDeclaration());

        sptr_v<ConstructorDeclaration> constructors = sdecl->getConstructors();

        for (auto consIt = constructors.begin(); consIt != constructors.end(); consIt++) {
            // Start building function info for current constructor
            string consName = (*consIt)->getSymbol()->toString();
            sptr_v<Sort> consSig;
            sptr_v<SelectorDeclaration> selectors = (*consIt)->getSelectors();

            for (auto selIt = selectors.begin(); selIt != selectors.end(); selIt++) {
                // Add sort of current selector to current constructor signature
                consSig.push_back(ctx->getStack()->expand((*selIt)->getSort()));

                // Build function info for current selector
                string selName = (*selIt)->getSymbol()->toString();
                sptr_v<Sort> selSig;
                selSig.push_back(typeSort);
                selSig.push_back(ctx->getStack()->expand((*selIt)->getSort()));

                // Add selector function info
                infos.push_back(make_shared<FunInfo>(selName, selSig, node));
            }

            // Add constructor function info
            consSig.push_back(typeSort);
            infos.push_back(make_shared<FunInfo>(consName, consSig, node));
        }
    }

    return infos;
}

sptr_v<SymbolInfo> SortednessChecker::getInfo(sptr_t<DeclareDatatypesCommand> node) {
    sptr_v<SymbolInfo> infos;

    sptr_v<SortDeclaration> datatypeSorts = node->getSorts();
    for (auto sortIt = datatypeSorts.begin(); sortIt != datatypeSorts.end(); sortIt++) {
        string typeName = (*sortIt)->getSymbol()->toString();
        unsigned long arity = (unsigned long) (*sortIt)->getArity()->getValue();

        // Add datatype sort info
        infos.push_back(make_shared<SortInfo>(typeName, arity, node));
    }

    for (unsigned long i = 0; i < node->getSorts().size(); i++) {
        sptr_t<ParametricDatatypeDeclaration> pdecl =
                dynamic_pointer_cast<ParametricDatatypeDeclaration>(node->getDeclarations()[i]);
        if (pdecl) {
            // Build a sort representing the datatype (to be used in the signatures of the constructors and selectors)
            sptr_t<Sort> typeSort =
                    make_shared<Sort>(make_shared<SimpleIdentifier>(node->getSorts()[i]->getSymbol()));
            typeSort = ctx->getStack()->expand(typeSort);

            sptr_v<Symbol> params = pdecl->getParams();
            for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++) {
                typeSort->getArgs().push_back(make_shared<Sort>(make_shared<SimpleIdentifier>(*paramIt)));
            }

            sptr_v<ConstructorDeclaration> constructors = pdecl->getConstructors();

            for (auto consIt = constructors.begin(); consIt != constructors.end(); consIt++) {
                // Start building function info for current constructor
                string consName = (*consIt)->getSymbol()->toString();
                sptr_v<Sort> consSig;
                sptr_v<SelectorDeclaration> selectors = (*consIt)->getSelectors();

                for (auto selIt = selectors.begin(); selIt != selectors.end(); selIt++) {
                    // Add sort of current selector to current constructor signature
                    consSig.push_back(ctx->getStack()->expand((*selIt)->getSort()));

                    // Build function info for current selector
                    string selName = (*selIt)->getSymbol()->toString();
                    sptr_v<Sort> selSig;
                    selSig.push_back(typeSort);
                    selSig.push_back(ctx->getStack()->expand((*selIt)->getSort()));

                    // Add selector function info
                    infos.push_back(make_shared<FunInfo>(selName, selSig, pdecl->getParams(), node));
                }

                // Add constructor function info
                consSig.push_back(typeSort);
                infos.push_back(make_shared<FunInfo>(consName, consSig, pdecl->getParams(), node));
            }
        } else {
            // Build a sort representing the datatype (to be used in the signatures of the constructors and selectors)
            sptr_t<Sort> typeSort =
                    make_shared<Sort>(make_shared<SimpleIdentifier>(node->getSorts()[i]->getSymbol()));
            typeSort = ctx->getStack()->expand(typeSort);

            sptr_t<SimpleDatatypeDeclaration> sdecl =
                    dynamic_pointer_cast<SimpleDatatypeDeclaration>(node->getDeclarations()[i]);

            sptr_v<ConstructorDeclaration> constructors = sdecl->getConstructors();

            for (auto consIt = constructors.begin(); consIt != constructors.end(); consIt++) {
                // Start building function info for current constructor
                string consName = (*consIt)->getSymbol()->toString();
                sptr_v<Sort> consSig;

                sptr_v<SelectorDeclaration> selectors = (*consIt)->getSelectors();

                for (auto selIt = selectors.begin(); selIt != selectors.end(); selIt++) {
                    // Add sort of current selector to current constructor signature
                    consSig.push_back(ctx->getStack()->expand((*selIt)->getSort()));

                    // Build function info for current selector
                    string selName = (*selIt)->getSymbol()->toString();
                    sptr_v<Sort> selSig;
                    selSig.push_back(typeSort);
                    selSig.push_back(ctx->getStack()->expand((*selIt)->getSort()));

                    // Add selector function info
                    infos.push_back(make_shared<FunInfo>(selName, selSig, node));
                }

                // Add constructor function info
                consSig.push_back(typeSort);
                infos.push_back(make_shared<FunInfo>(consName, consSig, node));
            }
        }
    }

    return infos;
}

void SortednessChecker::loadTheory(string theory) {
    sptr_t<AstNode> node;
    sptr_t<NodeError> err;
    loadTheory(theory, node, err);
}

void SortednessChecker::loadTheory(string theory,
                                   sptr_t<AstNode> node,
                                   sptr_t<NodeError> err) {
    string path = ctx->getConfiguration()->get(Configuration::Property::LOC_THEORIES) + theory
                  + ctx->getConfiguration()->get(Configuration::Property::FILE_EXT_THEORY);
    FILE *f = fopen(path.c_str(), "r");
    if (f) {
        fclose(f);

        sptr_t<SmtExecutionSettings> settings = make_shared<SmtExecutionSettings>();
        settings->setInputFromFile(path);
        settings->setCoreTheoryEnabled(false);
        settings->setSortCheckContext(ctx);

        SmtExecution exec(settings);
        if(exec.parse()) {
            if(exec.checkSortedness()) {
                //currentTheories[theory] = true;
            }
        } else {
            addError(ErrorMessages::buildTheoryUnloadable(theory), node, err);
        }
    } else {
        addError(ErrorMessages::buildTheoryUnknown(theory), node, err);
    }
}

void SortednessChecker::loadLogic(string logic,
                                  sptr_t<AstNode> node,
                                  sptr_t<NodeError> err) {
    string path = ctx->getConfiguration()->get(Configuration::Property::LOC_LOGICS) + logic
                  + ctx->getConfiguration()->get(Configuration::Property::FILE_EXT_LOGIC);
    FILE *f = fopen(path.c_str(), "r");
    if (f) {
        fclose(f);

        sptr_t<SmtExecutionSettings> settings = make_shared<SmtExecutionSettings>();
        settings->setInputFromFile(path);
        settings->setCoreTheoryEnabled(false);
        settings->setSortCheckContext(ctx);

        SmtExecution exec(settings);
        if(exec.parse()) {
            exec.checkSortedness();
        } else {
            addError(ErrorMessages::buildLogicUnloadable(logic), node, err);
        }
    } else {
        addError(ErrorMessages::buildLogicUnknown(logic), node, err);
    }
}

sptr_t<SortednessChecker::NodeError>
SortednessChecker::checkSort(sptr_t<Sort> sort,
                             sptr_t<AstNode> source,
                             sptr_t<SortednessChecker::NodeError> err) {
    string name = sort->getIdentifier()->toString();
    sptr_t<SortInfo> info = ctx->getStack()->getSortInfo(name);
    if (!info) {
        err = addError(ErrorMessages::buildSortUnknown(name, sort->getRowLeft(), sort->getColLeft(),
                                                       sort->getRowRight(), sort->getColRight()), source, err);

        sptr_v<Sort> argSorts = sort->getArgs();
        for (auto sortIt = argSorts.begin(); sortIt != argSorts.end(); sortIt++) {
            checkSort(*sortIt, source, err);
        }
    } else {
        if (sort->getArgs().size() != info->arity) {
            err = addError(ErrorMessages::buildSortArity(name, info->arity, sort->getArgs().size(),
                                                         sort->getRowLeft(), sort->getColLeft(),
                                                         sort->getRowRight(), sort->getColRight()),
                           source, info, err);
        } else {
            sptr_v<Sort> argSorts = sort->getArgs();
            for (auto sortIt = argSorts.begin(); sortIt != argSorts.end(); sortIt++) {
                checkSort(*sortIt, source, err);
            }
        }
    }
    return err;
}

sptr_t<SortednessChecker::NodeError>
SortednessChecker::checkSort(sptr_v<Symbol> &params,
                             sptr_t<Sort> sort,
                             sptr_t<AstNode> source,
                             sptr_t<SortednessChecker::NodeError> err) {
    string name = sort->getIdentifier()->toString();
    bool isParam = false;
    for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++) {
        if (name == (*paramIt)->toString())
            isParam = true;
    }

    if (!isParam) {
        sptr_t<SortInfo> info = ctx->getStack()->getSortInfo(name);
        if (!info) {
            err = addError(ErrorMessages::buildSortUnknown(name, sort->getRowLeft(), sort->getColLeft(),
                                                           sort->getRowRight(), sort->getColRight()), source, err);

            sptr_v<Sort> argSorts = sort->getArgs();
            for (auto sortIt = argSorts.begin(); sortIt != argSorts.end(); sortIt++) {
                checkSort(params, *sortIt, source, err);
            }
        } else {
            if (sort->getArgs().empty())
                return err;

            if (sort->getArgs().size() != info->arity) {
                err = addError(ErrorMessages::buildSortArity(name, info->arity, sort->getArgs().size(),
                                                             sort->getRowLeft(), sort->getColLeft(),
                                                             sort->getRowRight(), sort->getColRight()),
                               source, info, err);
            } else {
                sptr_v<Sort> argSorts = sort->getArgs();
                for (auto sortIt = argSorts.begin(); sortIt != argSorts.end(); sortIt++) {
                    checkSort(params, *sortIt, source, err);
                }
            }
        }
    }

    return err;
}

void SortednessChecker::visit(sptr_t<AssertCommand> node) {
    TermSorter sorter(shared_from_this());
    sptr_t<Sort> result = sorter.run(node->getTerm());
    if (result) {
        string resstr = result->toString();
        if (resstr != SORT_BOOL) {
            sptr_t<Term> term = node->getTerm();
            addError(ErrorMessages::buildAssertTermNotBool(term->toString(), resstr,
                                                           term->getRowLeft(), term->getColLeft(),
                                                           term->getRowRight(), term->getColRight()), node);
        }
    } else {
        sptr_t<Term> term = node->getTerm();
        addError(ErrorMessages::buildAssertTermNotWellSorted(term->toString(),
                                                             term->getRowLeft(), term->getColLeft(),
                                                             term->getRowRight(), term->getColRight()), node);
    }
}

void SortednessChecker::visit(sptr_t<DeclareConstCommand> node) {
    sptr_t<NodeError> err;
    err = checkSort(node->getSort(), node, err);

    sptr_t<FunInfo> nodeInfo = getInfo(node);
    sptr_t<FunInfo> dupInfo = ctx->getStack()->tryAdd(nodeInfo);

    if (dupInfo) {
        addError(ErrorMessages::buildConstAlreadyExists(nodeInfo->name), node, dupInfo, err);
    }
}

void SortednessChecker::visit(sptr_t<DeclareFunCommand> node) {
    sptr_t<NodeError> err;

    sptr_v<Sort> params = node->getParams();
    for (auto paramIt = params.begin(); paramIt != params.end(); paramIt++) {
        err = checkSort(*paramIt, node, err);
    }

    err = checkSort(node->getSort(), node, err);

    sptr_t<FunInfo> nodeInfo = getInfo(node);
    sptr_t<FunInfo> dupInfo = ctx->getStack()->tryAdd(nodeInfo);

    if (dupInfo) {
        addError(ErrorMessages::buildFunAlreadyExists(nodeInfo->name), node, dupInfo, err);
    }
}

void SortednessChecker::visit(sptr_t<DeclareDatatypeCommand> node) {
    sptr_t<NodeError> err;

    sptr_t<ParametricDatatypeDeclaration> pdecl =
            dynamic_pointer_cast<ParametricDatatypeDeclaration>(node->getDeclaration());

    sptr_v<SymbolInfo> infos = getInfo(node);
    for (auto infoIt = infos.begin(); infoIt != infos.end(); infoIt++) {
        sptr_t<SortInfo> sortInfo = dynamic_pointer_cast<SortInfo>(*infoIt);
        if (sortInfo) {
            sptr_t<SortInfo> dupInfo = ctx->getStack()->tryAdd(sortInfo);

            if (dupInfo) {
                err = addError(ErrorMessages::buildSortAlreadyExists(sortInfo->name), node, dupInfo, err);
            }
        }
    }

    if (pdecl) {
        sptr_v<ConstructorDeclaration> constructors = pdecl->getConstructors();

        for (auto consIt = constructors.begin(); consIt != constructors.end(); consIt++) {
            sptr_v<SelectorDeclaration> selectors = (*consIt)->getSelectors();

            for (auto selIt = selectors.begin(); selIt != selectors.end(); selIt++) {
                err = checkSort(pdecl->getParams(), (*selIt)->getSort(), node, err);
            }
        }
    } else {
        sptr_t<SimpleDatatypeDeclaration> sdecl =
                dynamic_pointer_cast<SimpleDatatypeDeclaration>(node->getDeclaration());
        sptr_v<ConstructorDeclaration> constructors = sdecl->getConstructors();

        for (auto consIt = constructors.begin(); consIt != constructors.end(); consIt++) {
            sptr_v<SelectorDeclaration> selectors = (*consIt)->getSelectors();

            for (auto selIt = selectors.begin(); selIt != selectors.end(); selIt++) {
                err = checkSort((*selIt)->getSort(), node, err);
            }
        }
    }

    infos = getInfo(node);
    for (auto infoIt = infos.begin(); infoIt != infos.end(); infoIt++) {
        sptr_t<FunInfo> funInfo = dynamic_pointer_cast<FunInfo>(*infoIt);
        if (funInfo) {
            sptr_t<FunInfo> dupInfo = ctx->getStack()->tryAdd(funInfo);

            if (dupInfo) {
                err = addError(ErrorMessages::buildFunAlreadyExists(funInfo->name), node, dupInfo, err);
            }

        }
    }
}

void SortednessChecker::visit(sptr_t<DeclareDatatypesCommand> node) {
    sptr_t<NodeError> err;

    sptr_v<SymbolInfo> infos = getInfo(node);
    for (auto infoIt = infos.begin(); infoIt != infos.end(); infoIt++) {
        sptr_t<SortInfo> sortInfo = dynamic_pointer_cast<SortInfo>((*infoIt));
        if (sortInfo) {
            sptr_t<SortInfo> dupInfo = ctx->getStack()->tryAdd(sortInfo);
            if (dupInfo) {
                err = addError(ErrorMessages::buildSortAlreadyExists(sortInfo->name), node, dupInfo, err);
            }
        }
    }

    for (unsigned long i = 0; i < node->getSorts().size(); i++) {
        sptr_t<NodeError> declerr;

        sptr_t<ParametricDatatypeDeclaration> pdecl =
                dynamic_pointer_cast<ParametricDatatypeDeclaration>(node->getDeclarations()[i]);

        if (pdecl) {
            sptr_v<ConstructorDeclaration> constructors = pdecl->getConstructors();

            for (auto consIt = constructors.begin(); consIt != constructors.end(); consIt++) {
                sptr_v<SelectorDeclaration> selectors = (*consIt)->getSelectors();

                for (auto selIt = selectors.begin(); selIt != selectors.end(); selIt++) {
                    declerr = checkSort(pdecl->getParams(), (*selIt)->getSort(), pdecl, declerr);
                }
            }
        } else {
            sptr_t<SimpleDatatypeDeclaration> sdecl =
                    dynamic_pointer_cast<SimpleDatatypeDeclaration>(node->getDeclarations()[i]);
            sptr_v<ConstructorDeclaration> constructors = sdecl->getConstructors();

            for (auto consIt = constructors.begin(); consIt != constructors.end(); consIt++) {
                sptr_v<SelectorDeclaration> selectors = (*consIt)->getSelectors();

                for (auto selIt = selectors.begin(); selIt != selectors.end(); selIt++) {
                    declerr = checkSort((*selIt)->getSort(), sdecl, declerr);
                }
            }
        }
    }

    infos = getInfo(node);
    for (auto infoIt = infos.begin(); infoIt != infos.end(); infoIt++) {
        sptr_t<FunInfo> funInfo = dynamic_pointer_cast<FunInfo>(*infoIt);

        if (funInfo) {
            sptr_t<FunInfo> dupInfo = ctx->getStack()->tryAdd(funInfo);
            if (dupInfo) {
                err = addError(ErrorMessages::buildFunAlreadyExists(funInfo->name), node, dupInfo, err);
            }
        }
    }
}

void SortednessChecker::visit(sptr_t<DeclareSortCommand> node) {
    sptr_t<SortInfo> nodeInfo = getInfo(node);
    sptr_t<SortInfo> dupInfo = ctx->getStack()->tryAdd(nodeInfo);

    if (dupInfo) {
        addError(ErrorMessages::buildSortAlreadyExists(nodeInfo->name), node, dupInfo);
    }
}

void SortednessChecker::visit(sptr_t<DefineFunCommand> node) {
    sptr_t<NodeError> err;

    sptr_v<SortedVariable> sig = node->getDefinition()->getSignature()->getParams();
    for (auto sortIt = sig.begin(); sortIt != sig.end(); sortIt++) {
        err = checkSort((*sortIt)->getSort(), node, err);
    }
    err = checkSort(node->getDefinition()->getSignature()->getSort(), node, err);

    sptr_t<FunInfo> nodeInfo = getInfo(node);
    sptr_t<FunInfo> dupInfo = ctx->getStack()->findDuplicate(nodeInfo);

    if (dupInfo) {
        addError(ErrorMessages::buildFunAlreadyExists(nodeInfo->name), node, dupInfo, err);
    } else {
        ctx->getStack()->push();

        sptr_v<SortedVariable> &bindings = node->getDefinition()->getSignature()->getParams();
        for (auto bindingIt = bindings.begin(); bindingIt != bindings.end(); bindingIt++) {
            ctx->getStack()->tryAdd(make_shared<VarInfo>((*bindingIt)->getSymbol()->toString(),
                                               ctx->getStack()->expand((*bindingIt)->getSort()), node));
        }

        TermSorter sorter(shared_from_this());
        sptr_t<Sort> result = sorter.run(node->getDefinition()->getBody());

        if (result) {
            string retstr = nodeInfo->signature[nodeInfo->signature.size() - 1]->toString();
            string resstr = result->toString();
            if (resstr != retstr) {
                sptr_t<Term> body = node->getDefinition()->getBody();
                addError(ErrorMessages::buildFunBodyWrongSort(body->toString(), resstr, retstr,
                                                              body->getRowLeft(), body->getColLeft(),
                                                              body->getRowRight(), body->getColRight()), node);
            }
        } else {
            sptr_t<Term> body = node->getDefinition()->getBody();
            addError(ErrorMessages::buildFunBodyNotWellSorted(body->toString(),
                                                              body->getRowLeft(), body->getColLeft(),
                                                              body->getRowRight(), body->getColRight()), node);
        }

        ctx->getStack()->pop();        ctx->getStack()->tryAdd(nodeInfo);
    }
}

void SortednessChecker::visit(sptr_t<DefineFunRecCommand> node) {
    sptr_t<NodeError> err;

    sptr_v<SortedVariable> sig = node->getDefinition()->getSignature()->getParams();
    for (auto sortIt = sig.begin(); sortIt != sig.end(); sortIt++) {
        err = checkSort((*sortIt)->getSort(), node, err);
    }
    err = checkSort(node->getDefinition()->getSignature()->getSort(), node, err);

    sptr_t<FunInfo> nodeInfo = getInfo(node);
    sptr_t<FunInfo> dupInfo = ctx->getStack()->findDuplicate(nodeInfo);

    if (dupInfo) {
        addError(ErrorMessages::buildFunAlreadyExists(nodeInfo->name), node, dupInfo, err);
    } else {
        ctx->getStack()->push();
        ctx->getStack()->tryAdd(nodeInfo);

        sptr_v<SortedVariable> &bindings = node->getDefinition()->getSignature()->getParams();
        for (auto bindingIt = bindings.begin(); bindingIt != bindings.end(); bindingIt++) {
            ctx->getStack()->tryAdd(make_shared<VarInfo>((*bindingIt)->getSymbol()->toString(),
                                               ctx->getStack()->expand((*bindingIt)->getSort()), node));
        }

        TermSorter sorter(shared_from_this());
        sptr_t<Sort> result = sorter.run(node->getDefinition()->getBody());

        if (result) {
            string retstr = nodeInfo->signature[nodeInfo->signature.size() - 1]->toString();
            string resstr = result->toString();
            if (resstr != retstr) {
                sptr_t<Term> body = node->getDefinition()->getBody();
                addError(ErrorMessages::buildFunBodyWrongSort(body->toString(), resstr, retstr,
                                                              body->getRowLeft(), body->getColLeft(),
                                                              body->getRowRight(), body->getColRight()), node);
            }
        } else {
            sptr_t<Term> body = node->getDefinition()->getBody();
            addError(ErrorMessages::buildFunBodyNotWellSorted(body->toString(),
                                                              body->getRowLeft(), body->getColLeft(),
                                                              body->getRowRight(), body->getColRight()), node);
        }

        ctx->getStack()->pop();
        ctx->getStack()->tryAdd(nodeInfo);
    }
}

void SortednessChecker::visit(sptr_t<DefineFunsRecCommand> node) {
    sptr_t<NodeError> err;
    sptr_v<FunctionDeclaration> &decls = node->getDeclarations();
    sptr_v<Term> &bodies = node->getBodies();

    for (auto declIt = decls.begin(); declIt != decls.end(); declIt++) {
        sptr_v<SortedVariable> sig = (*declIt)->getParams();
        for (auto itt = sig.begin(); itt != sig.end(); itt++) {
            err = checkSort((*itt)->getSort(), node, err);
        }
        err = checkSort((*declIt)->getSort(), node, err);
    }

    sptr_v<FunInfo> infos = getInfo(node);

    bool dup = false;
    for (auto infoIt = infos.begin(); infoIt != infos.end(); infoIt++) {
        sptr_t<FunInfo> dupInfo = ctx->getStack()->findDuplicate(*infoIt);
        if (dupInfo) {
            dup = true;
            err = addError(ErrorMessages::buildFunAlreadyExists((*infoIt)->name), node, *infoIt, err);
        }
    }

    if (!dup) {
        ctx->getStack()->push();

        for (unsigned long i = 0; i < decls.size(); i++) {
            ctx->getStack()->tryAdd(infos[i]);
        }

        for (unsigned long i = 0; i < decls.size(); i++) {
            ctx->getStack()->push();
            sptr_v<SortedVariable> &bindings = decls[i]->getParams();
            for (auto bindingIt = bindings.begin(); bindingIt != bindings.end(); bindingIt++) {
                ctx->getStack()->tryAdd(make_shared<VarInfo>((*bindingIt)->getSymbol()->toString(),
                                                   ctx->getStack()->expand((*bindingIt)->getSort()), node));
            }

            TermSorter sorter(shared_from_this());
            sptr_t<Sort> result = sorter.run(bodies[i]);

            if (result) {
                string retstr = infos[i]->signature[infos[i]->signature.size() - 1]->toString();
                string resstr = result->toString();
                if (resstr != retstr) {
                    err = addError(ErrorMessages::buildFunBodyWrongSort(infos[i]->name, infos[i]->body->toString(),
                                                                        resstr, retstr, infos[i]->body->getRowLeft(),
                                                                        infos[i]->body->getColLeft(),
                                                                        infos[i]->body->getRowRight(),
                                                                        infos[i]->body->getColRight()), node, err);
                }
            } else {
                err = addError(ErrorMessages::buildFunBodyNotWellSorted(infos[i]->name, infos[i]->body->toString(),
                                                                        infos[i]->body->getRowLeft(),
                                                                        infos[i]->body->getColLeft(),
                                                                        infos[i]->body->getRowRight(),
                                                                        infos[i]->body->getColRight()), node, err);
            }
            ctx->getStack()->pop();
        }

        ctx->getStack()->pop();
        for (unsigned long i = 0; i < infos.size(); i++) {
            ctx->getStack()->tryAdd(infos[i]);
        }
    }
}

void SortednessChecker::visit(sptr_t<DefineSortCommand> node) {
    sptr_t<NodeError> err;
    err = checkSort(node->getParams(), node->getSort(), node, err);

    sptr_t<SortInfo> nodeInfo = getInfo(node);
    sptr_t<SortInfo> dupInfo = ctx->getStack()->tryAdd(nodeInfo);

    if (dupInfo) {
        addError(ErrorMessages::buildSortAlreadyExists(nodeInfo->name), node, dupInfo, err);
    }
}

void SortednessChecker::visit(sptr_t<GetValueCommand> node) {
    sptr_t<NodeError> err;

    sptr_v<Term> terms = node->getTerms();
    for (auto termIt = terms.begin(); termIt != terms.end(); termIt++) {
        TermSorter sorter(shared_from_this());
        sptr_t<Sort> result = sorter.run(*termIt);
        if (!result) {
            err = addError(ErrorMessages::buildTermNotWellSorted(
                    (*termIt)->toString(), (*termIt)->getRowLeft(),
                    (*termIt)->getColLeft(), (*termIt)->getRowRight(),
                    (*termIt)->getColRight()), node, err);
        }
    }
}

void SortednessChecker::visit(sptr_t<PopCommand> node) {
    unsigned long levels = (unsigned long) node->getNumeral()->getValue();
    if (!ctx->getStack()->pop(levels)) {
        addError(ErrorMessages::buildStackUnpoppable(levels), node);
    }
}

void SortednessChecker::visit(sptr_t<PushCommand> node) {
    ctx->getStack()->push((unsigned long) node->getNumeral()->getValue());
}

void SortednessChecker::visit(sptr_t<ResetCommand> node) {
    ctx->getStack()->reset();
    ctx->setCurrentLogic("");
    ctx->getCurrentTheories().clear();
}

void SortednessChecker::visit(sptr_t<SetLogicCommand> node) {
    sptr_t<NodeError> err;
    if (ctx->getCurrentLogic() != "") {
        addError(ErrorMessages::buildLogicAlreadySet(ctx->getCurrentLogic()), node);
    } else {
        string logic = node->getLogic()->toString();
        ctx->setCurrentLogic(logic);
        loadLogic(logic, node, err);
    }
}

void SortednessChecker::visit(sptr_t<Logic> node) {
    sptr_v<Attribute> attrs = node->getAttributes();
    for (auto attrIt = attrs.begin(); attrIt != attrs.end(); attrIt++) {
        sptr_t<Attribute> attr = *attrIt;
        if (attr->getKeyword()->getValue() == KW_THEORIES) {
            sptr_t<NodeError> err;

            sptr_t<CompAttributeValue> attrValue =
                    dynamic_pointer_cast<CompAttributeValue>(attr->getValue());
            sptr_v<AttributeValue> compValues = attrValue->getValues();

            for (auto valIt = compValues.begin(); valIt != compValues.end(); valIt++) {
                string theory = dynamic_cast<Symbol *>((*valIt).get())->toString();
                auto found = find(ctx->getCurrentTheories().begin(), ctx->getCurrentTheories().end(), theory);

                if (found != ctx->getCurrentTheories().end()) {
                    err = addError(ErrorMessages::buildTheoryAlreadyLoaded(theory), attr, err);
                } else {
                    ctx->getCurrentTheories().push_back(theory);
                    loadTheory(theory, attr, err);
                }
            }
        }
    }
}

void SortednessChecker::visit(sptr_t<Theory> node) {
    sptr_v<Attribute> attrs = node->getAttributes();
    for (auto attrIt = attrs.begin(); attrIt != attrs.end(); attrIt++) {
        sptr_t<Attribute> attr = *attrIt;

        if (attr->getKeyword()->getValue() == KW_SORTS || attr->getKeyword()->getValue() == KW_FUNS) {
            CompAttributeValue *val = dynamic_cast<CompAttributeValue *>(attr->getValue().get());
            visit0(val->getValues());
        }
    }
}

void SortednessChecker::visit(sptr_t<Script> node) {
    visit0(node->getCommands());
}

void SortednessChecker::visit(sptr_t<SortSymbolDeclaration> node) {
    sptr_t<SortInfo> nodeInfo = getInfo(node);
    sptr_t<SortInfo> dupInfo = ctx->getStack()->tryAdd(nodeInfo);

    if (dupInfo) {
        addError(ErrorMessages::buildSortAlreadyExists(nodeInfo->name), node, dupInfo);
    }
}

void SortednessChecker::visit(sptr_t<SpecConstFunDeclaration> node) {
    sptr_t<NodeError> err;
    err = checkSort(node->getSort(), node, err);

    sptr_t<FunInfo> nodeInfo = getInfo(node);
    sptr_t<FunInfo> dupInfo = ctx->getStack()->tryAdd(nodeInfo);

    if (dupInfo) {
        addError(ErrorMessages::buildSpecConstAlreadyExists(nodeInfo->name), node, dupInfo, err);
    }
}

void SortednessChecker::visit(sptr_t<MetaSpecConstFunDeclaration> node) {
    sptr_t<NodeError> err;
    err = checkSort(node->getSort(), node, err);

    sptr_t<FunInfo> nodeInfo = getInfo(node);
    sptr_v<FunInfo> dupInfo = ctx->getStack()->getFunInfo(nodeInfo->name);

    if (!dupInfo.empty()) {
        err = addError(ErrorMessages::buildMetaSpecConstAlreadyExists(nodeInfo->name), node, dupInfo[0], err);
    } else {
        ctx->getStack()->tryAdd(nodeInfo);
    }
}

void SortednessChecker::visit(sptr_t<SimpleFunDeclaration> node) {
    sptr_t<NodeError> err;

    sptr_v<Sort> sig = node->getSignature();
    for (auto sortIt = sig.begin(); sortIt != sig.end(); sortIt++) {
        err = checkSort(*sortIt, node, err);
    }

    sptr_t<FunInfo> nodeInfo = getInfo(node);

    if (nodeInfo->assocL) {
        if (sig.size() != 3) {
            err = addError(ErrorMessages::buildLeftAssocParamCount(nodeInfo->name), node, err);
            nodeInfo->assocL = false;
        } else {
            sptr_t<Sort> firstSort = sig[0];
            sptr_t<Sort> returnSort = sig[2];

            if (firstSort->toString() != returnSort->toString()) {
                err = addError(ErrorMessages::buildLeftAssocRetSort(nodeInfo->name), node, err);
                nodeInfo->assocL = false;
            }
        }
    }

    if (nodeInfo->assocR) {
        if (sig.size() != 3) {
            err = addError(ErrorMessages::buildRightAssocParamCount(nodeInfo->name), node, err);
            nodeInfo->assocR = false;
        } else {
            sptr_t<Sort> secondSort = sig[1];
            sptr_t<Sort> returnSort = sig[2];

            if (secondSort->toString() != returnSort->toString()) {
                err = addError(ErrorMessages::buildRightAssocRetSort(nodeInfo->name), node, err);
                nodeInfo->assocR = false;
            }
        }
    }

    if (nodeInfo->chainable && nodeInfo->pairwise) {
        err = addError(ErrorMessages::buildChainableAndPairwise(nodeInfo->name), node, err);
        nodeInfo->chainable = false;
        nodeInfo->pairwise = false;
    } else if (nodeInfo->chainable) {
        if (sig.size() != 3) {
            err = addError(ErrorMessages::buildChainableParamCount(nodeInfo->name), node, err);
            nodeInfo->chainable = false;
        } else {
            sptr_t<Sort> firstSort = sig[0];
            sptr_t<Sort> secondSort = sig[1];
            sptr_t<Sort> returnSort = sig[2];

            if (firstSort->toString() != secondSort->toString()) {
                err = addError(ErrorMessages::buildChainableParamSort(nodeInfo->name), node, err);
                nodeInfo->chainable = false;
            }

            if (returnSort->toString() != SORT_BOOL) {
                err = addError(ErrorMessages::buildChainableRetSort(nodeInfo->name), node, err);
                nodeInfo->chainable = false;
            }
        }
    } else if (nodeInfo->pairwise) {
        if (sig.size() != 3) {
            err = addError(ErrorMessages::buildPairwiseParamCount(nodeInfo->name), node, err);
            nodeInfo->pairwise = false;
        } else {
            sptr_t<Sort> firstSort = sig[0];
            sptr_t<Sort> secondSort = sig[1];
            sptr_t<Sort> returnSort = sig[2];

            if (firstSort->toString() != secondSort->toString()) {
                err = addError(ErrorMessages::buildPairwiseParamSort(nodeInfo->name), node, err);
                nodeInfo->pairwise = false;
            }

            if (returnSort->toString() != SORT_BOOL) {
                err = addError(ErrorMessages::buildPairwiseRetSort(nodeInfo->name), node, err);
                nodeInfo->pairwise = false;
            }
        }
    }

    sptr_t<FunInfo> dupInfo = ctx->getStack()->tryAdd(nodeInfo);

    if (dupInfo) {
        addError(ErrorMessages::buildFunAlreadyExists(nodeInfo->name), node, dupInfo, err);
    }
}

void SortednessChecker::visit(sptr_t<ParametricFunDeclaration> node) {
    sptr_t<NodeError> err;

    sptr_v<Sort> sig = node->getSignature();
    for (auto sortIt = sig.begin(); sortIt != sig.end(); sortIt++) {
        err = checkSort(node->getParams(), *sortIt, node, err);
    }

    sptr_t<FunInfo> nodeInfo = getInfo(node);

    if (nodeInfo->assocL) {
        if (sig.size() != 3) {
            err = addError(ErrorMessages::buildLeftAssocParamCount(nodeInfo->name), node, err);
            nodeInfo->assocL = false;
        } else {
            sptr_t<Sort> firstSort = sig[0];
            sptr_t<Sort> returnSort = sig[2];

            if (firstSort->toString() != returnSort->toString()) {
                err = addError(ErrorMessages::buildLeftAssocRetSort(nodeInfo->name), node, err);
                nodeInfo->assocL = false;
            }
        }
    }

    if (nodeInfo->assocR) {
        if (sig.size() != 3) {
            err = addError(ErrorMessages::buildRightAssocParamCount(nodeInfo->name), node, err);
            nodeInfo->assocR = false;
        } else {
            sptr_t<Sort> secondSort = sig[1];
            sptr_t<Sort> returnSort = sig[2];

            if (secondSort->toString() != returnSort->toString()) {
                err = addError(ErrorMessages::buildRightAssocRetSort(nodeInfo->name), node, err);
                nodeInfo->assocR = false;
            }
        }
    }

    if (nodeInfo->chainable && nodeInfo->pairwise) {
        err = addError(ErrorMessages::buildChainableAndPairwise(nodeInfo->name), node, err);
        nodeInfo->chainable = false;
        nodeInfo->pairwise = false;
    } else if (nodeInfo->chainable) {
        if (sig.size() != 3) {
            err = addError(ErrorMessages::buildChainableParamCount(nodeInfo->name), node, err);
            nodeInfo->chainable = false;
        } else {
            sptr_t<Sort> firstSort = sig[0];
            sptr_t<Sort> secondSort = sig[1];
            sptr_t<Sort> returnSort = sig[2];

            if (firstSort->toString() != secondSort->toString()) {
                err = addError(ErrorMessages::buildChainableParamSort(nodeInfo->name), node, err);
                nodeInfo->chainable = false;
            }

            if (returnSort->toString() != SORT_BOOL) {
                err = addError(ErrorMessages::buildChainableRetSort(nodeInfo->name), node, err);
                nodeInfo->chainable = false;
            }
        }
    } else if (nodeInfo->pairwise) {
        if (sig.size() != 3) {
            err = addError(ErrorMessages::buildPairwiseParamCount(nodeInfo->name), node, err);
            nodeInfo->pairwise = false;
        } else {
            sptr_t<Sort> firstSort = sig[0];
            sptr_t<Sort> secondSort = sig[1];
            sptr_t<Sort> returnSort = sig[2];

            if (firstSort->toString() != secondSort->toString()) {
                err = addError(ErrorMessages::buildPairwiseParamSort(nodeInfo->name), node, err);
                nodeInfo->pairwise = false;
            }

            if (returnSort->toString() != SORT_BOOL) {
                err = addError(ErrorMessages::buildPairwiseRetSort(nodeInfo->name), node, err);
                nodeInfo->pairwise = false;
            }
        }
    }

    sptr_t<FunInfo> dupInfo = ctx->getStack()->tryAdd(nodeInfo);

    if (dupInfo) {
        addError(ErrorMessages::buildFunAlreadyExists(nodeInfo->name), node, dupInfo, err);
    }
}

bool SortednessChecker::check(sptr_t<AstNode> node) {
    if (node) {
        visit0(node);
    } else {
        Logger::warning("SortednessChecker::run()", "Attempting to check an empty abstract syntax tree");
        return false;
    }
    return errors.empty();
}

string SortednessChecker::getErrors() {
    stringstream ss;

    for (auto errIt = errors.begin(); errIt != errors.end(); errIt++) {
        string file = errIt->first;
        sptr_v<NodeError> errs = errIt->second;

        if (file != "") {
            long length = 11 + file.length();
            for (long i = 0; i < length; i++)
                ss << "-";
            ss << endl << "In file '" << file << "':" << endl;
            for (long i = 0; i < length; i++)
                ss << "-";
            ss << endl;
        }

        for (auto itt = errs.begin(); itt != errs.end(); itt++) {
            sptr_t<NodeError> err = *itt;
            if (err->node) {
                ss << err->node->getRowLeft() << ":" << err->node->getColLeft()
                << " - " << err->node->getRowRight() << ":" << err->node->getColRight() << "   ";

                string nodestr = err->node->toString();
                if (nodestr.length() > 100)
                    ss << string(nodestr, 0, 100) << "[...]";
                else
                    ss << nodestr;

                ss << endl;
            }

            for (auto infoIt = err->errs.begin(); infoIt != err->errs.end(); infoIt++) {
                sptr_t<AstNode> source;

                if ((*infoIt)->info) {
                    source = (*infoIt)->info->source;
                }

                if (infoIt != err->errs.begin() && source)
                    ss << endl;

                ss << "\t" << (*infoIt)->message << "." << endl;

                if (source) {
                    ss << "\t\tPreviously, in file '" << source->getFilename()->c_str() << "'\n\t\t"
                    << source->getRowLeft() << ":" << source->getColLeft() << " - "
                    << source->getRowRight() << ":" << source->getColRight() << "   ";

                    string sourcestr = source->toString();
                    if (sourcestr.length() > 100)
                        ss << string(sourcestr, 0, 100);
                    else
                        ss << sourcestr;

                    ss << endl;

                    if (infoIt + 1 != err->errs.end())
                        ss << endl;
                }
            }

            ss << endl;
        }
    }

    ss << endl;

    return ss.str();
}

sptr_t<SymbolStack> SortednessChecker::getStack() {
    return ctx->getStack();
}

sptr_t<SortednessChecker> SortednessChecker::getChecker() {
    return shared_from_this();
}

sptr_t<Configuration> SortednessChecker::getConfiguration() {
    return ctx->getConfiguration();
}

