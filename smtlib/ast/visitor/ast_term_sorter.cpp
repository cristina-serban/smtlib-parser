#include "ast_term_sorter.h"
#include "ast_sortedness_checker.h"
#include "ast/ast_logic.h"
#include "ast/ast_script.h"
#include "ast/ast_theory.h"
#include "smtlib/parser/smtlib_parser.h"
#include "util/error_messages.h"
#include "util/global_values.h"

using namespace std;
using namespace smtlib;
using namespace smtlib::ast;

void TermSorter::visit(sptr_t<SimpleIdentifier> node) {
    sptr_t<VarInfo> varInfo = ctx->getStack()->getVarInfo(node->toString());
    if (varInfo) {
        ret = varInfo->sort;
    } else {
        sptr_v<FunInfo> infos = ctx->getStack()->getFunInfo(node->toString());
        sptr_v<Sort> possibleSorts;
        for (auto infoIt = infos.begin(); infoIt != infos.end(); infoIt++) {
            if ((*infoIt)->signature.size() == 1 && (*infoIt)->params.empty())
                possibleSorts.push_back((*infoIt)->signature[0]);
        }

        if (possibleSorts.size() == 1) {
            ret = possibleSorts[0];
        } else if (possibleSorts.empty()) {
            ctx->getChecker()->addError((node->toString()), node);
        } else {
            vector<string> possibleSortsStr;
            for (auto sort : possibleSorts) {
                possibleSortsStr.push_back(sort->toString());
            }
            ctx->getChecker()->addError(
                    ErrorMessages::buildConstMultipleSorts(node->toString(),
                                                           possibleSortsStr), node);
        }
    }
}

void TermSorter::visit(sptr_t<QualifiedIdentifier> node) {
    sptr_t<SortednessChecker::NodeError> err;
    err = ctx->getChecker()->checkSort(node->getSort(), node, err);

    sptr_v<FunInfo> infos = ctx->getStack()->getFunInfo(node->getIdentifier()->toString());
    sptr_t<Sort> retExpanded = ctx->getStack()->expand(node->getSort());

    sptr_v<Sort> retSorts;
    for (auto infoIt = infos.begin(); infoIt != infos.end(); infoIt++) {
        if ((*infoIt)->signature.size() == 1) {
            if ((*infoIt)->params.empty()) {
                retSorts.push_back((*infoIt)->signature[0]);
            } else {
                vector<string> pnames;
                for (auto p = (*infoIt)->params.begin(); p != (*infoIt)->params.end(); p++) {
                    pnames.push_back((*p)->toString());
                }

                unordered_map<string, sptr_t<Sort>> mapping;
                getParamMapping(pnames, mapping, (*infoIt)->signature[0], retExpanded);

                if (mapping.size() == pnames.size()) {
                    sptr_t<Sort> retSort = ctx->getStack()->replace((*infoIt)->signature[0], mapping);
                    if (retSort->toString() == retExpanded->toString()) {
                        ret = retSort;
                        return;
                    }
                    retSorts.push_back(retSort);
                }
            }
        }
    }

    if (retSorts.size() == 1 && retSorts[0]->toString() == retExpanded->toString()) {
        ret = retSorts[0];
    } else {
        if (retSorts.empty()) {
            err = ctx->getChecker()->addError(
                    ErrorMessages::buildConstUnknown(node->getIdentifier()->toString()), node, err);
        } else {
            vector<string> retSortsStr;
            for (auto sort : retSorts) {
                retSortsStr.push_back(sort->toString());
            }
            ctx->getChecker()->addError(
                    ErrorMessages::buildConstWrongSort(node->getIdentifier()->toString(),
                                                       retExpanded->toString(), retSortsStr), node, err);
        }
    }
}

void TermSorter::visit(sptr_t<DecimalLiteral> node) {
    sptr_v<FunInfo> infos = ctx->getStack()->getFunInfo(MSCONST_DECIMAL);
    if (infos.size() == 1) {
        if (infos[0]->signature.size() == 1) {
            ret = infos[0]->signature[0];
        }
    } else {
        if (infos.empty()) {
            ctx->getChecker()->addError(ErrorMessages::buildLiteralUnknownSort(MSCONST_DECIMAL_REF), node);
        } else {
            vector<string> possibleSorts;
            for (auto infoIt = infos.begin(); infoIt != infos.end(); infoIt++) {
                if ((*infoIt)->signature.size() == 1 && (*infoIt)->params.empty())
                    possibleSorts.push_back((*infoIt)->signature[0]->toString());
            }
            ctx->getChecker()->addError(
                    ErrorMessages::buildLiteralMultipleSorts(MSCONST_DECIMAL_REF, possibleSorts), node);
        }
    }
}

void TermSorter::visit(sptr_t<NumeralLiteral> node) {
    sptr_v<FunInfo> infos = ctx->getStack()->getFunInfo(MSCONST_NUMERAL);
    if (infos.size() == 1) {
        if (infos[0]->signature.size() == 1) {
            ret = infos[0]->signature[0];
        }
    } else {
        if (infos.empty()) {
            ctx->getChecker()->addError(ErrorMessages::buildLiteralUnknownSort(MSCONST_NUMERAL_REF), node);
        } else {
            vector<string> possibleSorts;
            for (auto infoIt = infos.begin(); infoIt != infos.end(); infoIt++) {
                if ((*infoIt)->signature.size() == 1 && (*infoIt)->params.empty())
                    possibleSorts.push_back((*infoIt)->signature[0]->toString());
            }
            ctx->getChecker()->addError(
                    ErrorMessages::buildLiteralMultipleSorts(MSCONST_NUMERAL_REF, possibleSorts), node);
        }
    }
}

void TermSorter::visit(sptr_t<StringLiteral> node) {
    sptr_v<FunInfo> infos = ctx->getStack()->getFunInfo(MSCONST_STRING);
    if (infos.size() == 1) {
        if (infos[0]->signature.size() == 1) {
            ret = infos[0]->signature[0];
        }
    } else {
        if (infos.empty()) {
            ctx->getChecker()->addError(ErrorMessages::buildLiteralUnknownSort(MSCONST_STRING_REF), node);
        } else {
            vector<string> possibleSorts;
            for (auto infoIt = infos.begin(); infoIt != infos.end(); infoIt++) {
                if ((*infoIt)->signature.size() == 1 && (*infoIt)->params.empty())
                    possibleSorts.push_back((*infoIt)->signature[0]->toString());
            }
            ctx->getChecker()->addError(
                    ErrorMessages::buildLiteralMultipleSorts(MSCONST_STRING_REF, possibleSorts), node);
        }
    }
}

void TermSorter::visit(sptr_t<QualifiedTerm> node) {
    sptr_t<SortednessChecker::NodeError> err;

    sptr_v<Sort> argSorts;
    sptr_v<Term> terms = node->getTerms();
    for (auto termIt = terms.begin(); termIt != terms.end(); termIt++) {
        sptr_t<Sort> result = wrappedVisit(*termIt);
        if (result)
            argSorts.push_back(result);
        else {
            return;
        }
    }

    sptr_t<SimpleIdentifier> id = dynamic_pointer_cast<SimpleIdentifier>(node->getIdentifier());
    sptr_t<QualifiedIdentifier> qid = dynamic_pointer_cast<QualifiedIdentifier>(node->getIdentifier());

    sptr_t<Sort> retExpanded;
    string name;

    if (id) {
        name = id->toString();
    } else {
        err = ctx->getChecker()->checkSort(qid->getSort(), node, err);
        name = qid->getIdentifier()->toString();
        retExpanded = ctx->getStack()->expand(qid->getSort());
    }

    sptr_v<FunInfo> infos = ctx->getStack()->getFunInfo(name);
    sptr_v<Sort> retSorts;


    for (auto infoIt = infos.begin(); infoIt != infos.end(); infoIt++) {
        sptr_v<Sort> funSig;
        if (argSorts.size() >= 2) {
            if ((*infoIt)->assocL) {
                funSig.push_back((*infoIt)->signature[0]);
                for (int i = 0; i < argSorts.size() - 1; i++) {
                    funSig.push_back((*infoIt)->signature[1]);
                }
                funSig.push_back((*infoIt)->signature[2]);
            } else if ((*infoIt)->assocR) {
                for (int i = 0; i < argSorts.size() - 1; i++) {
                    funSig.push_back((*infoIt)->signature[0]);
                }
                funSig.push_back((*infoIt)->signature[1]);
                funSig.push_back((*infoIt)->signature[2]);
            } else if ((*infoIt)->chainable || (*infoIt)->pairwise) {
                for (int i = 0; i < argSorts.size(); i++) {
                    funSig.push_back((*infoIt)->signature[0]);
                }
                funSig.push_back((*infoIt)->signature[2]);
            } else {
                funSig.insert(funSig.begin(), (*infoIt)->signature.begin(), (*infoIt)->signature.end());
            }
        } else {
            funSig.insert(funSig.begin(), (*infoIt)->signature.begin(), (*infoIt)->signature.end());
        }

        if (argSorts.size() == funSig.size() - 1) {
            bool fits = true;
            if ((*infoIt)->params.empty()) {
                for (unsigned long i = 0; i < funSig.size() - 1; i++) {
                    if (funSig[i]->toString() != argSorts[i]->toString())
                        fits = false;
                }

                if (id) {
                    if (fits)
                        retSorts.push_back(funSig[funSig.size() - 1]);
                } else {
                    sptr_t<Sort> retSort = funSig[funSig.size() - 1];
                    if (fits && retSort->toString() == retExpanded->toString()) {
                        ret = retSort;
                        return;
                    }
                }
            } else {
                vector<string> pnames;
                for (auto p = (*infoIt)->params.begin(); p != (*infoIt)->params.end(); p++) {
                    pnames.push_back((*p)->toString());
                }
                unordered_map<string, sptr_t<Sort>> mapping;

                for (unsigned long i = 0; i < funSig.size() - 1; i++) {
                    fits = fits && getParamMapping(pnames, mapping, funSig[i], argSorts[i]);
                }

                if (fits && mapping.size() == (*infoIt)->params.size()) {
                    sptr_t<Sort> retSort = funSig[funSig.size() - 1];
                    retSort = ctx->getStack()->replace(retSort, mapping);
                    string retSortString = retSort->toString();
                    if (id) {
                        retSorts.push_back(retSort);
                    } else {
                        if (retSortString == retExpanded->toString()) {
                            ret = retSort;
                            return;
                        }
                    }
                }
            }
        }
    }

    vector<string> argSortsStr;
    for (auto sort : argSorts) {
        argSortsStr.push_back(sort->toString());
    }

    vector<string> retSortsStr;
    for (auto sort : retSorts) {
        retSortsStr.push_back(sort->toString());
    }

    if (id) {
        if (retSorts.size() == 1) {
            ret = retSorts[0];
        } else if (retSorts.empty()) {
            err = ctx->getChecker()->addError(
                    ErrorMessages::buildFunUnknownDecl(name, argSortsStr), node, err);
        } else {
            err = ctx->getChecker()->addError
                    (ErrorMessages::buildFunMultipleDecls(name, argSortsStr, retSortsStr), node, err);
        }
    } else {
        err = ctx->getChecker()->addError(
                ErrorMessages::buildFunUnknownDecl(name, argSortsStr, retExpanded->toString()), node, err);
    }
}

void TermSorter::visit(sptr_t<LetTerm> node) {
    ctx->getStack()->push();

    sptr_v<VarBinding> &bindings = node->getBindings();
    for (auto bindingIt = bindings.begin(); bindingIt != bindings.end(); bindingIt++) {
        sptr_t<Sort> result = wrappedVisit((*bindingIt)->getTerm());
        if (result) {
            ctx->getStack()->tryAdd(
                    make_shared<VarInfo>((*bindingIt)->getSymbol()->toString(), result, node));
        } else {
            return;
        }
    }

    sptr_t<Sort> result = wrappedVisit(node->getTerm());
    if (result) {
        ret = result;
    }

    ctx->getStack()->pop();
}

void TermSorter::visit(sptr_t<ForallTerm> node) {
    ctx->getStack()->push();

    sptr_v<SortedVariable> &bindings = node->getBindings();
    for (auto bindingIt = bindings.begin(); bindingIt != bindings.end(); bindingIt++) {
        ctx->getStack()->tryAdd(
                make_shared<VarInfo>((*bindingIt)->getSymbol()->toString(),
                                     ctx->getStack()->expand((*bindingIt)->getSort()), node));
    }

    sptr_t<Sort> result = wrappedVisit(node->getTerm());
    if (result) {
        string resstr = result->toString();
        if (resstr == SORT_BOOL) {
            ret = result;
        } else {
            ctx->getChecker()->addError(
                    ErrorMessages::buildQuantTermWrongSort(node->getTerm()->toString(), resstr, SORT_BOOL,
                                                           node->getTerm()->getRowLeft(),
                                                           node->getTerm()->getColLeft(),
                                                           node->getTerm()->getRowRight(),
                                                           node->getTerm()->getColRight()), node);
        }
    }

    ctx->getStack()->pop();
}

void TermSorter::visit(sptr_t<ExistsTerm> node) {
    ctx->getStack()->push();

    sptr_v<SortedVariable> &bindings = node->getBindings();
    for (auto bindingIt = bindings.begin(); bindingIt != bindings.end(); bindingIt++) {
        ctx->getStack()->tryAdd(
                make_shared<VarInfo>((*bindingIt)->getSymbol()->toString(),
                                     ctx->getStack()->expand((*bindingIt)->getSort()), node));
    }

    sptr_t<Sort> result = wrappedVisit(node->getTerm());
    if (result) {
        string resstr = result->toString();
        if (resstr == SORT_BOOL) {
            ret = result;
        } else {
            ctx->getChecker()->addError(
                    ErrorMessages::buildQuantTermWrongSort(node->getTerm()->toString(), resstr, SORT_BOOL,
                                                           node->getTerm()->getRowLeft(),
                                                           node->getTerm()->getColLeft(),
                                                           node->getTerm()->getRowRight(),
                                                           node->getTerm()->getColRight()), node);
        }
    }

    ctx->getStack()->pop();
}

void TermSorter::visit(sptr_t<MatchTerm> node) {
    sptr_t<Sort> termSort = wrappedVisit(node->getTerm());
    sptr_v<Sort> caseSorts;

    if (termSort) {
        sptr_t<SortednessChecker::NodeError> err;
        string termSortStr = termSort->toString();

        sptr_v<MatchCase> cases = node->getCases();
        for (auto caseIt = cases.begin(); caseIt != cases.end(); caseIt++) {
            sptr_t<Pattern> pattern = (*caseIt)->getPattern();

            // symbol (constructor or variable)
            sptr_t<Symbol> spattern = dynamic_pointer_cast<Symbol>(pattern);
            // qualified constructor
            sptr_t<QualifiedConstructor> cpattern = dynamic_pointer_cast<QualifiedConstructor>(pattern);
            // qualified pattern
            sptr_t<QualifiedPattern> qpattern = dynamic_pointer_cast<QualifiedPattern>(pattern);

            sptr_t<Symbol> scons; // simple constructor for qualified pattern
            sptr_t<QualifiedConstructor> qcons; // qualified constructor for qualified pattern
            string caseId;

            // Init caseId, scons, qcons
            if (spattern) {
                caseId = spattern->toString();
            } else if (cpattern) {
                caseId = cpattern->getSymbol()->toString();
            } else if (qpattern) {
                sptr_t<Constructor> cons = qpattern->getConstructor();
                scons = dynamic_pointer_cast<Symbol>(cons);
                qcons = dynamic_pointer_cast<QualifiedConstructor>(cons);

                if (scons)
                    caseId = scons->toString();
                else
                    caseId = qcons->getSymbol()->toString();
            }

            // Get known information about functions with the name caseId
            sptr_v<FunInfo> funInfos = ctx->getStack()->getFunInfo(caseId);
            sptr_v<FunInfo> matchingInfos;
            vector<unordered_map<string, sptr_t<Sort>>> matchingMappings;

            for (auto info = funInfos.begin(); info != funInfos.end(); info++) {
                sptr_t<Sort> retSort = (*info)->signature[(*info)->signature.size() - 1];
                string retSortStr = retSort->toString();

                // If info is about a parametric function, map sort parameters to real sorts
                vector<string> pnames;
                unordered_map<string, sptr_t<Sort>> mapping;
                bool mapped = true;

                if (!(*info)->params.empty()) {
                    for (auto p = (*info)->params.begin(); p != (*info)->params.end(); p++) {
                        pnames.push_back((*p)->toString());
                    }
                    mapped = getParamMapping(pnames, mapping, retSort, termSort);
                }

                // Check if current function info fits
                if (spattern || cpattern) {
                    // Return sort mismatch in case of qualified constructor
                    if (cpattern && cpattern->getSort()->toString() != termSortStr) {
                        err = ctx->getChecker()->addError(
                                ErrorMessages::buildPatternMismatch(termSortStr, pattern->toString()), node, err);
                        continue;
                    }

                    // If return sorts were mapped correctly
                    if (mapped && (*info)->params.size() == mapping.size()) {
                        matchingInfos.push_back(*info);
                        matchingMappings.push_back(mapping);
                    }
                } else if (qpattern) {
                    // Return sort mismatch in case of qualified constructor
                    if (qcons && qcons->getSort()->toString() != termSortStr) {
                        err = ctx->getChecker()->addError(
                                ErrorMessages::buildPatternMismatch(termSortStr, pattern->toString()), node, err);
                        continue;
                    }

                    // If return sorts were mapped correctly
                    // and there are as many arguments to the fuction as there are symbols in the pattern
                    if (mapped && (*info)->params.size() == mapping.size()
                        && qpattern->getSymbols().size() == (*info)->signature.size() - 1) {
                        matchingInfos.push_back(*info);
                        matchingMappings.push_back(mapping);
                    }
                }
            }

            if (matchingInfos.empty()) {
                if (spattern && caseIt + 1 == node->getCases().end()) {
                    // If it's not a function, try to interpret it as a variable
                    ctx->getStack()->push();
                    ctx->getStack()->tryAdd(make_shared<VarInfo>(caseId, termSort, *caseIt));
                    sptr_t<Sort> caseSort = wrappedVisit((*caseIt)->getTerm());
                    if (caseSort) {
                        caseSorts.push_back(caseSort);
                    }
                    ctx->getStack()->pop();
                } else if (spattern || cpattern) {
                    err = ctx->getChecker()->addError(
                            ErrorMessages::buildFunUnknownDecl(caseId, termSort->toString()), node, err);
                } else if (qpattern) {
                    err = ctx->getChecker()->addError(
                            ErrorMessages::buildFunUnknownDecl(caseId, qpattern->getSymbols().size(),
                                                               termSort->toString()), node, err);
                }
            } else if (matchingInfos.size() > 1) {
                if (qpattern) {
                    err = ctx->getChecker()->addError(
                            ErrorMessages::buildFunMultipleDecls(caseId, qpattern->getSymbols().size(),
                                                                 termSort->toString()), node, err);
                }
            } else {
                sptr_t<FunInfo> match = matchingInfos[0];
                if (qpattern) {
                    ctx->getStack()->push();
                    for (unsigned long i = 0; i < match->signature.size() - 1; i++) {
                        sptr_t<Sort> paramSort = ctx->getStack()->replace(match->signature[i],
                                                                                    matchingMappings[0]);
                        ctx->getStack()->tryAdd(
                                make_shared<VarInfo>(qpattern->getSymbols()[i]->toString(), paramSort, *caseIt));
                    }
                }

                sptr_t<Sort> caseSort = wrappedVisit((*caseIt)->getTerm());
                if (caseSort) {
                    caseSorts.push_back(caseSort);
                }

                if (qpattern) {
                    ctx->getStack()->pop();
                }
            }
        }

        vector<string> caseSortsStr;
        for (auto caseSort : caseSorts) {
            caseSortsStr.push_back(caseSort->toString());
        }

        if (caseSorts.size() == node->getCases().size()) {
            string case1 = caseSorts[0]->toString();
            bool equalCases = true;
            for (unsigned long i = 1; i < caseSorts.size(); i++) {
                if (caseSorts[1]->toString() != case1) {
                    err = ctx->getChecker()->addError(
                            ErrorMessages::buildCasesMismatch(caseSortsStr), node, err);
                    equalCases = false;
                    break;
                }
            }
            if (equalCases)
                ret = caseSorts[0];
        }
    }
}

void TermSorter::visit(sptr_t<AnnotatedTerm> node) {
    visit0(node->getTerm());
}


bool TermSorter::getParamMapping(vector<string> &params,
                                 unordered_map<string, sptr_t<Sort>> &mapping,
                                 sptr_t<Sort> sort1,
                                 sptr_t<Sort> sort2) {
    if (!sort1 || !sort2)
        return false;

    string sort1Name = sort1->getIdentifier()->toString();
    bool isParam = (find(params.begin(), params.end(), sort1Name) != params.end());

    if (isParam) {
        if (mapping[sort1Name]) {
            return mapping[sort1Name]->toString() == sort2->toString();
        } else {
            mapping[sort1Name] = sort2;
            return true;
        }
    } else {
        if (sort1->getArgs().size() != sort2->getArgs().size()) {
            return false;
        } else if (sort1->getArgs().empty()) {
            return sort1Name == sort2->getIdentifier()->toString();
        } else {
            string sort2Name = sort2->getIdentifier()->toString();

            if (sort1Name != sort2Name || sort1->getArgs().size() != sort2->getArgs().size()) {
                return false;
            }

            bool fits = true;
            for (unsigned long i = 0; i < sort1->getArgs().size(); i++) {
                fits = fits && getParamMapping(params, mapping, sort1->getArgs()[i], sort2->getArgs()[i]);
            }

            return fits;
        }
    }
}