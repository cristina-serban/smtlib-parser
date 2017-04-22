#include "smtlib-glue.h"
#include "smtlib_parser.h"

#include "ast/ast_attribute.h"
#include "ast/ast_basic.h"
#include "ast/ast_command.h"
#include "ast/ast_fun.h"
#include "ast/ast_identifier.h"
#include "ast/ast_literal.h"
#include "ast/ast_logic.h"
#include "ast/ast_sexp.h"
#include "ast/ast_script.h"
#include "ast/ast_sort.h"
#include "ast/ast_symbol_decl.h"
#include "ast/ast_term.h"
#include "ast/ast_theory.h"
#include "ast/ast_var.h"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace smtlib;
using namespace smtlib::ast;

unordered_map<smtlib::ast::AstNode*, sptr_t<smtlib::ast::AstNode>> nodemap;

template<class T>
sptr_t<T> share(SmtPtr nakedPtr) {
    return dynamic_pointer_cast<T>(nodemap[nakedPtr]);
}

template<>
sptr_t<SpecConstant> share(SmtPtr nakedPtr) {
    sptr_t<NumeralLiteral> option1 = dynamic_pointer_cast<NumeralLiteral>(nodemap[nakedPtr]);
    if (option1) {
        return option1->shared_from_this();
    }

    sptr_t<DecimalLiteral> option2 = dynamic_pointer_cast<DecimalLiteral>(nodemap[nakedPtr]);
    if (option2) {
        return option2->shared_from_this();
    }

    sptr_t<StringLiteral> option3 = dynamic_pointer_cast<StringLiteral>(nodemap[nakedPtr]);
    if (option3) {
        return option3->shared_from_this();
    }

    throw;
}

template<>
sptr_t<Command> share(SmtPtr nakedPtr) {
    sptr_t<AssertCommand> option1 = dynamic_pointer_cast<AssertCommand>(nodemap[nakedPtr]);
    if (option1) {
        return option1->shared_from_this();
    }

    sptr_t<CheckSatCommand> option2 = dynamic_pointer_cast<CheckSatCommand>(nodemap[nakedPtr]);
    if (option2) {
        return option2->shared_from_this();
    }

    sptr_t<CheckSatAssumCommand> option3 = dynamic_pointer_cast<CheckSatAssumCommand>(nodemap[nakedPtr]);
    if (option3) {
        return option3->shared_from_this();
    }

    sptr_t<DeclareConstCommand> option4 = dynamic_pointer_cast<DeclareConstCommand>(nodemap[nakedPtr]);
    if (option4) {
        return option4->shared_from_this();
    }

    sptr_t<DeclareFunCommand> option5 = dynamic_pointer_cast<DeclareFunCommand>(nodemap[nakedPtr]);
    if (option5) {
        return option5->shared_from_this();
    }

    sptr_t<DeclareSortCommand> option6 = dynamic_pointer_cast<DeclareSortCommand>(nodemap[nakedPtr]);
    if (option6) {
        return option6->shared_from_this();
    }

    sptr_t<DefineFunCommand> option7 = dynamic_pointer_cast<DefineFunCommand>(nodemap[nakedPtr]);
    if (option7) {
        return option7->shared_from_this();
    }

    sptr_t<DefineFunRecCommand> option8 = dynamic_pointer_cast<DefineFunRecCommand>(nodemap[nakedPtr]);
    if (option8) {
        return option8->shared_from_this();
    }

    sptr_t<DefineFunsRecCommand> option9 = dynamic_pointer_cast<DefineFunsRecCommand>(nodemap[nakedPtr]);
    if (option9) {
        return option9->shared_from_this();
    }

    sptr_t<DefineSortCommand> option10 = dynamic_pointer_cast<DefineSortCommand>(nodemap[nakedPtr]);
    if (option10) {
        return option10->shared_from_this();
    }

    sptr_t<EchoCommand> option11 = dynamic_pointer_cast<EchoCommand>(nodemap[nakedPtr]);
    if (option11) {
        return option11->shared_from_this();
    }

    sptr_t<ExitCommand> option12 = dynamic_pointer_cast<ExitCommand>(nodemap[nakedPtr]);
    if (option12) {
        return option12->shared_from_this();
    }

    sptr_t<SetOptionCommand> option13 = dynamic_pointer_cast<SetOptionCommand>(nodemap[nakedPtr]);
    if (option13) {
        return option13->shared_from_this();
    }

    sptr_t<GetAssertsCommand> option14 = dynamic_pointer_cast<GetAssertsCommand>(nodemap[nakedPtr]);
    if (option14) {
        return option14->shared_from_this();
    }

    sptr_t<GetAssignsCommand> option15 = dynamic_pointer_cast<GetAssignsCommand>(nodemap[nakedPtr]);
    if (option15) {
        return option15->shared_from_this();
    }

    sptr_t<GetInfoCommand> option16 = dynamic_pointer_cast<GetInfoCommand>(nodemap[nakedPtr]);
    if (option16) {
        return option16->shared_from_this();
    }

    sptr_t<GetModelCommand> option17 = dynamic_pointer_cast<GetModelCommand>(nodemap[nakedPtr]);
    if (option17) {
        return option17->shared_from_this();
    }

    sptr_t<GetOptionCommand> option18 = dynamic_pointer_cast<GetOptionCommand>(nodemap[nakedPtr]);
    if (option18) {
        return option18->shared_from_this();
    }

    sptr_t<GetProofCommand> option19 = dynamic_pointer_cast<GetProofCommand>(nodemap[nakedPtr]);
    if (option19) {
        return option19->shared_from_this();
    }

    sptr_t<GetUnsatAssumsCommand> option20 = dynamic_pointer_cast<GetUnsatAssumsCommand>(nodemap[nakedPtr]);
    if (option20) {
        return option20->shared_from_this();
    }

    sptr_t<GetUnsatCoreCommand> option21 = dynamic_pointer_cast<GetUnsatCoreCommand>(nodemap[nakedPtr]);
    if (option21) {
        return option21->shared_from_this();
    }

    sptr_t<GetValueCommand> option22 = dynamic_pointer_cast<GetValueCommand>(nodemap[nakedPtr]);
    if (option22) {
        return option22->shared_from_this();
    }

    sptr_t<PopCommand> option23 = dynamic_pointer_cast<PopCommand>(nodemap[nakedPtr]);
    if (option23) {
        return option23->shared_from_this();
    }

    sptr_t<PushCommand> option24 = dynamic_pointer_cast<PushCommand>(nodemap[nakedPtr]);
    if (option24) {
        return option24->shared_from_this();
    }

    sptr_t<ResetCommand> option25 = dynamic_pointer_cast<ResetCommand>(nodemap[nakedPtr]);
    if (option25) {
        return option25->shared_from_this();
    }

    sptr_t<ResetAssertsCommand> option26 = dynamic_pointer_cast<ResetAssertsCommand>(nodemap[nakedPtr]);
    if (option26) {
        return option26->shared_from_this();
    }

    sptr_t<SetInfoCommand> option27 = dynamic_pointer_cast<SetInfoCommand>(nodemap[nakedPtr]);
    if (option27) {
        return option27->shared_from_this();
    }

    sptr_t<SetLogicCommand> option28 = dynamic_pointer_cast<SetLogicCommand>(nodemap[nakedPtr]);
    if (option28) {
        return option28->shared_from_this();
    }

    sptr_t<DeclareDatatypeCommand> option29 = dynamic_pointer_cast<DeclareDatatypeCommand>(nodemap[nakedPtr]);
    if (option29) {
        return option29->shared_from_this();
    }

    sptr_t<DeclareDatatypesCommand> option30 = dynamic_pointer_cast<DeclareDatatypesCommand>(nodemap[nakedPtr]);
    if (option30) {
        return option30->shared_from_this();
    }

    throw;
}

template<>
sptr_t<FunSymbolDeclaration> share(SmtPtr nakedPtr) {
    sptr_t<SpecConstFunDeclaration> option6 = dynamic_pointer_cast<SpecConstFunDeclaration>(nodemap[nakedPtr]);
    if (option6) {
        return option6->shared_from_this();
    }

    sptr_t<MetaSpecConstFunDeclaration> option7 = dynamic_pointer_cast<MetaSpecConstFunDeclaration>(
            nodemap[nakedPtr]);
    if (option7) {
        return option7->shared_from_this();
    }

    sptr_t<SimpleFunDeclaration> option8 = dynamic_pointer_cast<SimpleFunDeclaration>(nodemap[nakedPtr]);
    if (option8) {
        return option8->shared_from_this();
    }

    sptr_t<ParametricFunDeclaration> option9 = dynamic_pointer_cast<ParametricFunDeclaration>(nodemap[nakedPtr]);
    if (option9) {
        return option9->shared_from_this();
    }

    throw;
}

template<>
sptr_t<Constructor> share(SmtPtr nakedPtr) {
    sptr_t<Symbol> option1 = dynamic_pointer_cast<Symbol>(nodemap[nakedPtr]);
    if (option1) {
        return option1->shared_from_this();
    }

    sptr_t<QualifiedConstructor> option2 = dynamic_pointer_cast<QualifiedConstructor>(nodemap[nakedPtr]);
    if (option2) {
        return option2->shared_from_this();
    }

    throw;
}

template<>
sptr_t<Pattern> share(SmtPtr nakedPtr) {
    if (dynamic_cast<Constructor*>(nakedPtr)) {
        return share<Constructor>(nakedPtr);
    }

    sptr_t<Symbol> option1 = dynamic_pointer_cast<Symbol>(nodemap[nakedPtr]);
    if (option1) {
        return option1->shared_from_this();
    }

    sptr_t<QualifiedPattern> option2 = dynamic_pointer_cast<QualifiedPattern>(nodemap[nakedPtr]);
    if (option2) {
        return option2->shared_from_this();
    }

    throw;
}

template<>
sptr_t<DatatypeDeclaration> share(SmtPtr nakedPtr) {
    sptr_t<SimpleDatatypeDeclaration> option1 =
            dynamic_pointer_cast<SimpleDatatypeDeclaration>(nodemap[nakedPtr]);
    if (option1) {
        return option1->shared_from_this();
    }

    sptr_t<ParametricDatatypeDeclaration> option2 =
            dynamic_pointer_cast<ParametricDatatypeDeclaration>(nodemap[nakedPtr]);
    if (option2) {
        return option2->shared_from_this();
    }

    throw;
}

template<>
sptr_t<AttributeValue> share(SmtPtr nakedPtr) {
    sptr_t<BooleanValue> option1 = dynamic_pointer_cast<BooleanValue>(nodemap[nakedPtr]);
    if (option1) {
        return option1->shared_from_this();
    }

    if (dynamic_cast<SpecConstant*>(nakedPtr)) {
        return share<SpecConstant>(nakedPtr);
    }

    sptr_t<SortSymbolDeclaration> option5 = dynamic_pointer_cast<SortSymbolDeclaration>(nodemap[nakedPtr]);
    if (option5) {
        return option5->shared_from_this();
    }

    if (dynamic_cast<FunSymbolDeclaration*>(nakedPtr)) {
        return share<FunSymbolDeclaration>(nakedPtr);
    }

    sptr_t<Symbol> option10 = dynamic_pointer_cast<Symbol>(nodemap[nakedPtr]);
    if (option10) {
        return option10->shared_from_this();
    }

    sptr_t<CompSExpression> option11 = dynamic_pointer_cast<CompSExpression>(nodemap[nakedPtr]);
    if (option11) {
        return option11->shared_from_this();
    }

    sptr_t<CompAttributeValue> option12 = dynamic_pointer_cast<CompAttributeValue>(nodemap[nakedPtr]);
    if (option12) {
        return option12->shared_from_this();
    }

    throw;
}

template<>
sptr_t<SExpression> share(SmtPtr nakedPtr) {
    if (dynamic_cast<SpecConstant*>(nakedPtr)) {
        return share<SpecConstant>(nakedPtr);
    }

    sptr_t<Symbol> option4 = dynamic_pointer_cast<Symbol>(nodemap[nakedPtr]);
    if (option4) {
        return option4->shared_from_this();
    }

    sptr_t<Keyword> option5 = dynamic_pointer_cast<Keyword>(nodemap[nakedPtr]);
    if (option5) {
        return option5->shared_from_this();
    }

    sptr_t<CompSExpression> option6 = dynamic_pointer_cast<CompSExpression>(nodemap[nakedPtr]);
    if (option6) {
        return option6->shared_from_this();
    }

    throw;
}

template<>
sptr_t<Identifier> share(SmtPtr nakedPtr) {
    sptr_t<SimpleIdentifier> option1 = dynamic_pointer_cast<SimpleIdentifier>(nodemap[nakedPtr]);
    if (option1) {
        return option1->shared_from_this();
    }

    sptr_t<QualifiedIdentifier> option2 = dynamic_pointer_cast<QualifiedIdentifier>(nodemap[nakedPtr]);
    if (option2) {
        return option2->shared_from_this();
    }

    throw;
}

template<>
sptr_t<Term> share(SmtPtr nakedPtr) {
    if (dynamic_cast<SpecConstant*>(nakedPtr)) {
        return share<SpecConstant>(nakedPtr);
    }

    if (dynamic_cast<Identifier*>(nakedPtr)) {
        return share<Identifier>(nakedPtr);
    }

    sptr_t<AnnotatedTerm> option6 = dynamic_pointer_cast<AnnotatedTerm>(nodemap[nakedPtr]);
    if (option6) {
        return option6->shared_from_this();
    }

    sptr_t<ExistsTerm> option7 = dynamic_pointer_cast<ExistsTerm>(nodemap[nakedPtr]);
    if (option7) {
        return option7->shared_from_this();
    }

    sptr_t<ForallTerm> option8 = dynamic_pointer_cast<ForallTerm>(nodemap[nakedPtr]);
    if (option8) {
        return option8->shared_from_this();
    }

    sptr_t<LetTerm> option9 = dynamic_pointer_cast<LetTerm>(nodemap[nakedPtr]);
    if (option9) {
        return option9->shared_from_this();
    }

    sptr_t<QualifiedTerm> option10 = dynamic_pointer_cast<QualifiedTerm>(nodemap[nakedPtr]);
    if (option10) {
        return option10->shared_from_this();
    }

    sptr_t<MatchTerm> option11 = dynamic_pointer_cast<MatchTerm>(nodemap[nakedPtr]);
    if (option11) {
        return option11->shared_from_this();
    }

    throw;
}

template<>
sptr_t<Index> share(SmtPtr nakedPtr) {
    sptr_t<NumeralLiteral> option1 = dynamic_pointer_cast<NumeralLiteral>(nodemap[nakedPtr]);
    if (option1) {
        return option1->shared_from_this();
    }

    sptr_t<Symbol> option2 = dynamic_pointer_cast<Symbol>(nodemap[nakedPtr]);
    if (option2) {
        return option2->shared_from_this();
    }

    throw;
}

//namespace smtlib {
//namespace ast {

class ParserInternalList {
private:
    vector<SmtPtr> v;
public:
    template<class T>
    sptr_v<T> unwrap() {
        sptr_v<T> result;
        for (unsigned long i = 0, n = v.size(); i < n; ++i) {
            sptr_t<T> ptr = share<T>(v[i]);
            result.push_back(ptr);
        }
        v.clear();
        return result;
    };

    inline void add(SmtPtr item) {
        v.push_back(item);
    }
};
//}
//}

SmtList smt_listCreate() {
    return new ParserInternalList();
}

void smt_listAdd(SmtList list, SmtPtr item) {
    list->add(item);
}

void smt_listDelete(SmtList list) {
    delete list;
}

void smt_print(SmtPtr ptr) {
    cout << ptr->toString();
}

void smt_setAst(SmtPrsr parser, SmtPtr ast) {
    if (parser && ast) {
        parser->setAst(nodemap[ast]);
        nodemap.clear();
    }
}

void smt_reportError(SmtPrsr parser, unsigned int rowLeft, unsigned int colLeft,
                     unsigned int rowRight, unsigned int colRight, const char* msg) {
    if (parser && msg) {
        parser->reportError(rowLeft, colLeft, rowRight, colRight, msg);
    }
}

void smt_setLocation(SmtPrsr parser, SmtPtr ptr, int rowLeft, int colLeft, int rowRight, int colRight) {
    ptr->setFilename(parser->getFilename());
    ptr->setRowLeft(rowLeft);
    ptr->setColLeft(colLeft);
    ptr->setRowRight(rowRight);
    ptr->setColRight(colRight);
}

int smt_bool_value(SmtPtr ptr) {
    sptr_t<BooleanValue> val = share<BooleanValue>(ptr);
    if (val) {
        return val->getValue();
    } else {
        return 2;
    }
}

// ast_attribute.h
SmtPtr smt_newAttribute1(SmtPtr keyword) {
    sptr_t<Attribute> ptr = make_shared<Attribute>(share<Keyword>(keyword));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newAttribute2(SmtPtr keyword, SmtPtr attr_value) {
    sptr_t<Attribute> ptr = make_shared<Attribute>(share<Keyword>(keyword),
                                                       share<AttributeValue>(attr_value));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newCompAttributeValue(SmtList values) {
    sptr_v<AttributeValue> v = values->unwrap<AttributeValue>();
    sptr_t<CompAttributeValue> ptr = make_shared<CompAttributeValue>(v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_basic.h
SmtPtr smt_newSymbol(char const* value) {
    sptr_t<Symbol> ptr = make_shared<Symbol>(value);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newKeyword(char const* value) {
    sptr_t<Keyword> ptr = make_shared<Keyword>(value);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newMetaSpecConstant(int value) {
    sptr_t<MetaSpecConstant> ptr = make_shared<MetaSpecConstant>(
            static_cast<MetaSpecConstant::Type>(value));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newBooleanValue(int value) {
    sptr_t<BooleanValue> ptr = make_shared<BooleanValue>((bool) value);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newPropLiteral(SmtPtr symbol, int negated) {
    sptr_t<PropLiteral> ptr = make_shared<PropLiteral>(share<Symbol>(symbol), (bool) negated);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_command.h
SmtPtr smt_newAssertCommand(SmtPtr term) {
    sptr_t<AssertCommand> ptr = make_shared<AssertCommand>(share<Term>(term));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newCheckSatCommand() {
    sptr_t<CheckSatCommand> ptr = make_shared<CheckSatCommand>();
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newCheckSatAssumCommand(SmtList assumptions) {
    sptr_v<PropLiteral> v = assumptions->unwrap<PropLiteral>();
    sptr_t<CheckSatAssumCommand> ptr = make_shared<CheckSatAssumCommand>(v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newDeclareConstCommand(SmtPtr symbol, SmtPtr sort) {
    sptr_t<DeclareConstCommand> ptr =
            make_shared<DeclareConstCommand>(share<Symbol>(symbol), share<Sort>(sort));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newDeclareDatatypeCommand(SmtPtr symbol, SmtPtr declaration) {
    sptr_t<DeclareDatatypeCommand> ptr =
            make_shared<DeclareDatatypeCommand>(share<Symbol>(symbol),
                                                share<DatatypeDeclaration>(declaration));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newDeclareDatatypesCommand(SmtList sorts, SmtList declarations) {
    sptr_v<SortDeclaration> v1 = sorts->unwrap<SortDeclaration>();
    sptr_v<DatatypeDeclaration> v2 = declarations->unwrap<DatatypeDeclaration>();
    sptr_t<DeclareDatatypesCommand> ptr = make_shared<DeclareDatatypesCommand>(v1, v2);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newDeclareFunCommand(SmtPtr symbol, SmtList params, SmtPtr sort) {
    sptr_v<Sort> v = params->unwrap<Sort>();
    sptr_t<DeclareFunCommand> ptr =
            make_shared<DeclareFunCommand>(share<Symbol>(symbol), v, share<Sort>(sort));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newDeclareSortCommand(SmtPtr symbol, SmtPtr arity) {
    sptr_t<DeclareSortCommand> ptr =
            make_shared<DeclareSortCommand>(share<Symbol>(symbol), share<NumeralLiteral>(arity));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newDefineFunCommand(SmtPtr definition) {
    sptr_t<DefineFunCommand> ptr =
            make_shared<DefineFunCommand>(share<FunctionDefinition>(definition));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newDefineFunRecCommand(SmtPtr definition) {
    sptr_t<DefineFunRecCommand> ptr = make_shared<DefineFunRecCommand>(
            share<FunctionDefinition>(definition));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newDefineFunsRecCommand(SmtList declarations, SmtList bodies) {
    sptr_v<FunctionDeclaration> v1 = declarations->unwrap<FunctionDeclaration>();
    sptr_v<Term> v2 = bodies->unwrap<Term>();
    sptr_t<DefineFunsRecCommand> ptr = make_shared<DefineFunsRecCommand>(v1, v2);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newDefineSortCommand(SmtPtr symbol, SmtList params, SmtPtr sort) {
    sptr_v<Symbol> v1 = params->unwrap<Symbol>();
    sptr_t<DefineSortCommand> ptr =
            make_shared<DefineSortCommand>(share<Symbol>(symbol), v1, share<Sort>(sort));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newEchoCommand(SmtPtr msg) {
    sptr_t<EchoCommand> ptr = make_shared<EchoCommand>(share<StringLiteral>(msg)->getValue());
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newExitCommand() {
    sptr_t<ExitCommand> ptr = make_shared<ExitCommand>();
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newGetAssertsCommand() {
    sptr_t<GetAssertsCommand> ptr = make_shared<GetAssertsCommand>();
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newGetAssignsCommand() {
    sptr_t<GetAssignsCommand> ptr = make_shared<GetAssignsCommand>();
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newGetInfoCommand(SmtPtr keyword) {
    sptr_t<GetInfoCommand> ptr = make_shared<GetInfoCommand>(share<Keyword>(keyword));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newGetModelCommand() {
    sptr_t<GetModelCommand> ptr = make_shared<GetModelCommand>();
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newGetOptionCommand(SmtPtr keyword) {
    sptr_t<GetOptionCommand> ptr = make_shared<GetOptionCommand>(share<Keyword>(keyword));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newGetProofCommand() {
    sptr_t<GetProofCommand> ptr = make_shared<GetProofCommand>();
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newGetUnsatAssumsCommand() {
    sptr_t<GetUnsatAssumsCommand> ptr = make_shared<GetUnsatAssumsCommand>();
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newGetUnsatCoreCommand() {
    sptr_t<GetUnsatCoreCommand> ptr = make_shared<GetUnsatCoreCommand>();
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newGetValueCommand(SmtList terms) {
    sptr_v<Term> v = terms->unwrap<Term>();
    sptr_t<GetValueCommand> ptr = make_shared<GetValueCommand>(v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newPopCommand(SmtPtr numeral) {
    sptr_t<PopCommand> ptr = make_shared<PopCommand>(share<NumeralLiteral>(numeral));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newPushCommand(SmtPtr numeral) {
    sptr_t<PushCommand> ptr = make_shared<PushCommand>(share<NumeralLiteral>(numeral));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newResetCommand() {
    sptr_t<ResetCommand> ptr = make_shared<ResetCommand>();
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newResetAssertsCommand() {
    sptr_t<ResetAssertsCommand> ptr = make_shared<ResetAssertsCommand>();
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newSetInfoCommand(SmtPtr info) {
    sptr_t<SetInfoCommand> ptr = make_shared<SetInfoCommand>(share<Attribute>(info));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newSetLogicCommand(SmtPtr logic) {
    sptr_t<SetLogicCommand> ptr = make_shared<SetLogicCommand>(share<Symbol>(logic));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newSetOptionCommand(SmtPtr option) {
    sptr_t<SetOptionCommand> ptr = make_shared<SetOptionCommand>(share<Attribute>(option));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

//smt_datatype.h
SmtPtr smt_newSortDeclaration(SmtPtr symbol, SmtPtr numeral) {
    sptr_t<SortDeclaration> ptr =
            make_shared<SortDeclaration>(share<Symbol>(symbol), share<NumeralLiteral>(numeral));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newSelectorDeclaration(SmtPtr symbol, SmtPtr sort) {
    sptr_t<SelectorDeclaration> ptr =
            make_shared<SelectorDeclaration>(share<Symbol>(symbol), share<Sort>(sort));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newConstructorDeclaration(SmtPtr symbol, SmtList selectors) {
    sptr_v<SelectorDeclaration> v = selectors->unwrap<SelectorDeclaration>();
    sptr_t<ConstructorDeclaration> ptr = make_shared<ConstructorDeclaration>(share<Symbol>(symbol), v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newSimpleDatatypeDeclaration(SmtList constructors) {
    sptr_v<ConstructorDeclaration> v = constructors->unwrap<ConstructorDeclaration>();
    sptr_t<SimpleDatatypeDeclaration> ptr = make_shared<SimpleDatatypeDeclaration>(v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newParametricDatatypeDeclaration(SmtList params, SmtList constructors) {
    sptr_v<Symbol> v1 = params->unwrap<Symbol>();
    sptr_v<ConstructorDeclaration> v2 = constructors->unwrap<ConstructorDeclaration>();
    sptr_t<ParametricDatatypeDeclaration> ptr = make_shared<ParametricDatatypeDeclaration>(v1, v2);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_fun.h
SmtPtr smt_newFunctionDeclaration(SmtPtr symbol, SmtList params, SmtPtr sort) {
    sptr_v<SortedVariable> v = params->unwrap<SortedVariable>();
    sptr_t<FunctionDeclaration> ptr =
            make_shared<FunctionDeclaration>(share<Symbol>(symbol), v, share<Sort>(sort));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newFunctionDefinition(SmtPtr signature, SmtPtr body) {
    sptr_t<FunctionDefinition> ptr = make_shared<FunctionDefinition>(
            share<FunctionDeclaration>(signature), share<Term>(body));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_identifier.h
SmtPtr smt_newSimpleIdentifier1(SmtPtr symbol) {
    sptr_t<SimpleIdentifier> ptr = make_shared<SimpleIdentifier>(share<Symbol>(symbol));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newSimpleIdentifier2(SmtPtr symbol, SmtList indices) {
    sptr_v<Index> v = indices->unwrap<Index>();
    sptr_t<SimpleIdentifier> ptr =
            make_shared<SimpleIdentifier>(share<Symbol>(symbol), v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newQualifiedIdentifier(SmtPtr identifier, SmtPtr sort) {
    sptr_t<QualifiedIdentifier> ptr =
            make_shared<QualifiedIdentifier>(share<SimpleIdentifier>(identifier), share<Sort>(sort));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_literal.h
SmtPtr smt_newNumeralLiteral(long value, unsigned int base) {
    sptr_t<NumeralLiteral> ptr = make_shared<NumeralLiteral>(value, base);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newDecimalLiteral(double value) {
    sptr_t<DecimalLiteral> ptr = make_shared<DecimalLiteral>(value);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newStringLiteral(char const* value) {
    sptr_t<StringLiteral> ptr = make_shared<StringLiteral>(value);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_logic.h
SmtPtr smt_newLogic(SmtPtr name, SmtList attributes) {
    sptr_v<Attribute> v = attributes->unwrap<Attribute>();
    sptr_t<Logic> ptr = make_shared<Logic>(share<Symbol>(name), v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_match.h
SmtPtr smt_newQualifiedConstructor(SmtPtr symbol, SmtPtr sort) {
    sptr_t<QualifiedConstructor> ptr =
            make_shared<QualifiedConstructor>(share<Symbol>(symbol), share<Sort>(sort));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newQualifiedPattern(SmtPtr constructor, SmtList symbols) {
    sptr_v<Symbol> v = symbols->unwrap<Symbol>();
    sptr_t<QualifiedPattern> ptr = make_shared<QualifiedPattern>(share<Constructor>(constructor), v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newMatchCase(SmtPtr pattern, SmtPtr term) {
    sptr_t<MatchCase> ptr =
            make_shared<MatchCase>(share<Pattern>(pattern), share<Term>(term));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_s_exp.h
SmtPtr smt_newCompSExpression(SmtList exprs) {
    sptr_v<SExpression> v = exprs->unwrap<SExpression>();
    sptr_t<CompSExpression> ptr = make_shared<CompSExpression>(v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_script.h
SmtPtr smt_newSmtScript(SmtList cmds) {
    sptr_v<Command> v = cmds->unwrap<Command>();
    sptr_t<Script> ptr = make_shared<Script>(v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_sort.h
SmtPtr smt_newSort1(SmtPtr identifier) {
    sptr_t<Sort> ptr = make_shared<Sort>(share<SimpleIdentifier>(identifier));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newSort2(SmtPtr identifier, SmtList params) {
    sptr_v<Sort> v = params->unwrap<Sort>();
    sptr_t<Sort> ptr = make_shared<Sort>(share<SimpleIdentifier>(identifier), v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_symbol_decl.h
SmtPtr smt_newSortSymbolDeclaration(SmtPtr identifier, SmtPtr arity, SmtList attributes) {
    sptr_v<Attribute> v = attributes->unwrap<Attribute>();
    sptr_t<SortSymbolDeclaration> ptr =
            make_shared<SortSymbolDeclaration>(share<SimpleIdentifier>(identifier),
                                               share<NumeralLiteral>(arity), v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newSpecConstFunDeclaration(SmtPtr constant, SmtPtr sort, SmtList attributes) {
    sptr_v<Attribute> v = attributes->unwrap<Attribute>();
    sptr_t<SpecConstFunDeclaration> ptr =
            make_shared<SpecConstFunDeclaration>(share<SpecConstant>(constant), share<Sort>(sort), v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newMetaSpecConstFunDeclaration(SmtPtr constant, SmtPtr sort, SmtList attributes) {
    sptr_v<Attribute> v = attributes->unwrap<Attribute>();
    sptr_t<MetaSpecConstFunDeclaration> ptr =
            make_shared<MetaSpecConstFunDeclaration>(share<MetaSpecConstant>(constant), share<Sort>(sort), v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newSimpleFunDeclaration(SmtPtr identifier, SmtList signature, SmtList attributes) {
    sptr_v<Sort> v1 = signature->unwrap<Sort>();
    sptr_v<Attribute> v2 = attributes->unwrap<Attribute>();
    sptr_t<SimpleFunDeclaration> ptr =
            make_shared<SimpleFunDeclaration>(share<SimpleIdentifier>(identifier), v1, v2);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newParametricFunDeclaration(SmtList params, SmtPtr identifier, SmtList signature, SmtList attributes) {
    sptr_v<Symbol> v1 = params->unwrap<Symbol>();
    sptr_v<Sort> v2 = signature->unwrap<Sort>();
    sptr_v<Attribute> v3 = attributes->unwrap<Attribute>();
    sptr_t<ParametricFunDeclaration> ptr =
            make_shared<ParametricFunDeclaration>(v1, share<SimpleIdentifier>(identifier), v2, v3);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_term.h
SmtPtr smt_newQualifiedTerm(SmtPtr identifier, SmtList terms) {
    sptr_v<Term> v = terms->unwrap<Term>();
    sptr_t<QualifiedTerm> ptr = make_shared<QualifiedTerm>(share<Identifier>(identifier), v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newLetTerm(SmtList bindings, SmtPtr term) {
    sptr_v<VarBinding> v = bindings->unwrap<VarBinding>();
    sptr_t<LetTerm> ptr = make_shared<LetTerm>(v, share<Term>(term));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newForallTerm(SmtList bindings, SmtPtr term) {
    sptr_v<SortedVariable> v = bindings->unwrap<SortedVariable>();
    sptr_t<ForallTerm> ptr = make_shared<ForallTerm>(v, share<Term>(term));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newExistsTerm(SmtList bindings, SmtPtr term) {
    sptr_v<SortedVariable> v = bindings->unwrap<SortedVariable>();
    sptr_t<ExistsTerm> ptr = make_shared<ExistsTerm>(v, share<Term>(term));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newMatchTerm(SmtPtr term, SmtList cases) {
    sptr_v<MatchCase> v = cases->unwrap<MatchCase>();
    sptr_t<MatchTerm> ptr = make_shared<MatchTerm>(share<Term>(term), v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newAnnotatedTerm(SmtPtr term, SmtList attrs) {
    sptr_v<Attribute> v = attrs->unwrap<Attribute>();
    sptr_t<AnnotatedTerm> ptr = make_shared<AnnotatedTerm>(share<Term>(term), v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_theory.h
SmtPtr smt_newTheory(SmtPtr name, SmtList attributes) {
    sptr_v<Attribute> v = attributes->unwrap<Attribute>();
    sptr_t<Theory> ptr =
            make_shared<Theory>(share<Symbol>(name), v);
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

// ast_var.h
SmtPtr smt_newSortedVariable(SmtPtr symbol, SmtPtr sort) {
    sptr_t<SortedVariable> ptr =
            make_shared<SortedVariable>(share<Symbol>(symbol), share<Sort>(sort));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}

SmtPtr smt_newVarBinding(SmtPtr symbol, SmtPtr term) {
    sptr_t<VarBinding> ptr =
            make_shared<VarBinding>(share<Symbol>(symbol), share<Term>(term));
    nodemap[ptr.get()] = ptr;
    return ptr.get();
}