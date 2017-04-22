#include "ast_visitor.h"

#include "ast/ast_attribute.h"
#include "ast/ast_command.h"
#include "ast/ast_logic.h"
#include "ast/ast_theory.h"
#include "ast/ast_script.h"
#include "ast/ast_sexp.h"
#include "ast/ast_symbol_decl.h"
#include "ast/ast_term.h"

using namespace smtlib::ast;

void AstVisitor0::visit0(sptr_t<AstNode> node) {
    if (node == NULL) {
        return;
    }
    node->accept(this);
}

void DummyAstVisitor0::visit(sptr_t<Attribute> node) {
    visit0(node->getKeyword());
    visit0(node->getValue());
}

void DummyAstVisitor0::visit(sptr_t<CompAttributeValue> node) {
    visit0(node->getValues());
}

void DummyAstVisitor0::visit(sptr_t<Symbol> node) { }

void DummyAstVisitor0::visit(sptr_t<Keyword> node) { }

void DummyAstVisitor0::visit(sptr_t<MetaSpecConstant> node) { }

void DummyAstVisitor0::visit(sptr_t<BooleanValue> node) { }

void DummyAstVisitor0::visit(sptr_t<PropLiteral> node) { }

void DummyAstVisitor0::visit(sptr_t<AssertCommand> node) {
    visit0(node->getTerm());
}

void DummyAstVisitor0::visit(sptr_t<CheckSatCommand> node) { }

void DummyAstVisitor0::visit(sptr_t<CheckSatAssumCommand> node) {
    visit0(node->getAssumptions());
}

void DummyAstVisitor0::visit(sptr_t<DeclareConstCommand> node) {
    visit0(node->getSymbol());
    visit0(node->getSort());
}

void DummyAstVisitor0::visit(sptr_t<DeclareDatatypeCommand> node) {
    visit0(node->getSymbol());
    visit0(node->getDeclaration());
}

void DummyAstVisitor0::visit(sptr_t<DeclareDatatypesCommand> node) {
    visit0(node->getDeclarations());
}

void DummyAstVisitor0::visit(sptr_t<DeclareFunCommand> node) {
    visit0(node->getSymbol());
    visit0(node->getParams());
    visit0(node->getSort());
}

void DummyAstVisitor0::visit(sptr_t<DeclareSortCommand> node) {
    visit0(node->getSymbol());
}

void DummyAstVisitor0::visit(sptr_t<DefineFunCommand> node) {
    visit0(node->getDefinition());
}

void DummyAstVisitor0::visit(sptr_t<DefineFunRecCommand> node) {
    visit0(node->getDefinition());
}

void DummyAstVisitor0::visit(sptr_t<DefineFunsRecCommand> node) {
    visit0(node->getDeclarations());
    visit0(node->getBodies());
}

void DummyAstVisitor0::visit(sptr_t<DefineSortCommand> node) {
    visit0(node->getSymbol());
    visit0(node->getParams());
    visit0(node->getSort());
}

void DummyAstVisitor0::visit(sptr_t<EchoCommand> node) { }

void DummyAstVisitor0::visit(sptr_t<ExitCommand> node) { }

void DummyAstVisitor0::visit(sptr_t<GetAssertsCommand> node) { }

void DummyAstVisitor0::visit(sptr_t<GetAssignsCommand> node) { }

void DummyAstVisitor0::visit(sptr_t<GetInfoCommand> node) {
    visit0(node->getFlag());
}

void DummyAstVisitor0::visit(sptr_t<GetModelCommand> node) { }

void DummyAstVisitor0::visit(sptr_t<GetOptionCommand> node) {
    visit0(node->getOption());
}

void DummyAstVisitor0::visit(sptr_t<GetProofCommand> node) { }

void DummyAstVisitor0::visit(sptr_t<GetUnsatAssumsCommand> node) { }

void DummyAstVisitor0::visit(sptr_t<GetUnsatCoreCommand> node) { }

void DummyAstVisitor0::visit(sptr_t<GetValueCommand> node) {
    visit0(node->getTerms());
}

void DummyAstVisitor0::visit(sptr_t<PopCommand> node) {
    visit0(node->getNumeral());
}

void DummyAstVisitor0::visit(sptr_t<PushCommand> node) {
    visit0(node->getNumeral());
}

void DummyAstVisitor0::visit(sptr_t<ResetCommand> node) { }

void DummyAstVisitor0::visit(sptr_t<ResetAssertsCommand> node) { }

void DummyAstVisitor0::visit(sptr_t<SetInfoCommand> node) {
    visit0(node->getInfo());
}

void DummyAstVisitor0::visit(sptr_t<SetLogicCommand> node) {
    visit0(node->getLogic());
}

void DummyAstVisitor0::visit(sptr_t<SetOptionCommand> node) {
    visit0(node->getOption());
}

void DummyAstVisitor0::visit(sptr_t<FunctionDeclaration> node) {
    visit0(node->getSymbol());
    visit0(node->getParams());
    visit0(node->getSort());
}

void DummyAstVisitor0::visit(sptr_t<FunctionDefinition> node) {
    visit0(node->getSignature());
    visit0(node->getBody());
}

void DummyAstVisitor0::visit(sptr_t<SimpleIdentifier> node) {
    visit0(node->getSymbol());
}

void DummyAstVisitor0::visit(sptr_t<QualifiedIdentifier> node) {
    visit0(node->getIdentifier());
    visit0(node->getSort());
}

void DummyAstVisitor0::visit(sptr_t<DecimalLiteral> node) { }

void DummyAstVisitor0::visit(sptr_t<NumeralLiteral> node) { }

void DummyAstVisitor0::visit(sptr_t<StringLiteral> node) { }

void DummyAstVisitor0::visit(sptr_t<Logic> node) {
    visit0(node->getName());
    visit0(node->getAttributes());
}

void DummyAstVisitor0::visit(sptr_t<Theory> node) {
    visit0(node->getName());
    visit0(node->getAttributes());
}

void DummyAstVisitor0::visit(sptr_t<Script> node) {
    visit0(node->getCommands());
}

void DummyAstVisitor0::visit(sptr_t<Sort> node) {
    visit0(node->getIdentifier());
    visit0(node->getArgs());
}

void DummyAstVisitor0::visit(sptr_t<CompSExpression> node) {
    visit0(node->getExpressions());
}

void DummyAstVisitor0::visit(sptr_t<SortSymbolDeclaration> node) {
    visit0(node->getIdentifier());
    visit0(node->getArity());
    visit0(node->getAttributes());
}

void DummyAstVisitor0::visit(sptr_t<SortDeclaration> node) {
    visit0(node->getSymbol());
    visit0(node->getArity());
}

void DummyAstVisitor0::visit(sptr_t<SelectorDeclaration> node) {
    visit0(node->getSymbol());
    visit0(node->getSort());
}

void DummyAstVisitor0::visit(sptr_t<ConstructorDeclaration> node) {
    visit0(node->getSymbol());
    visit0(node->getSelectors());
}

void DummyAstVisitor0::visit(sptr_t<SimpleDatatypeDeclaration> node) {
    visit0(node->getConstructors());
}

void DummyAstVisitor0::visit(sptr_t<ParametricDatatypeDeclaration> node) {
    visit0(node->getConstructors());
    visit0(node->getParams());
}

void DummyAstVisitor0::visit(sptr_t<QualifiedConstructor> node) {
    visit0(node->getSymbol());
    visit0(node->getSort());
}

void DummyAstVisitor0::visit(sptr_t<QualifiedPattern> node) {
    visit0(node->getConstructor());
    visit0(node->getSymbols());
}

void DummyAstVisitor0::visit(sptr_t<MatchCase> node) {
    visit0(node->getPattern());
    visit0(node->getTerm());
}

void DummyAstVisitor0::visit(sptr_t<SpecConstFunDeclaration> node) {
    visit0(node->getConstant());
    visit0(node->getSort());
    visit0(node->getAttributes());
}

void DummyAstVisitor0::visit(sptr_t<MetaSpecConstFunDeclaration> node) {
    visit0(node->getConstant());
    visit0(node->getSort());
    visit0(node->getAttributes());
}

void DummyAstVisitor0::visit(sptr_t<SimpleFunDeclaration> node) {
    visit0(node->getIdentifier());
    visit0(node->getSignature());
    visit0(node->getAttributes());
}

void DummyAstVisitor0::visit(sptr_t<ParametricFunDeclaration> node) {
    visit0(node->getParams());
    visit0(node->getIdentifier());
    visit0(node->getSignature());
    visit0(node->getAttributes());
}

void DummyAstVisitor0::visit(sptr_t<QualifiedTerm> node) {
    visit0(node->getIdentifier());
    visit0(node->getTerms());
}

void DummyAstVisitor0::visit(sptr_t<LetTerm> node) {
    visit0(node->getBindings());
    visit0(node->getTerm());
}

void DummyAstVisitor0::visit(sptr_t<ForallTerm> node) {
    visit0(node->getBindings());
    visit0(node->getTerm());
}

void DummyAstVisitor0::visit(sptr_t<ExistsTerm> node) {
    visit0(node->getBindings());
    visit0(node->getTerm());
}

void DummyAstVisitor0::visit(sptr_t<MatchTerm> node) {
    visit0(node->getTerm());
    visit0(node->getCases());
}

void DummyAstVisitor0::visit(sptr_t<AnnotatedTerm> node) {
    visit0(node->getTerm());
    visit0(node->getAttributes());
}

void DummyAstVisitor0::visit(sptr_t<SortedVariable> node) {
    visit0(node->getSymbol());
    visit0(node->getSort());
}

void DummyAstVisitor0::visit(sptr_t<VarBinding> node) {
    visit0(node->getSymbol());
    visit0(node->getTerm());
}