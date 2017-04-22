/**
 * \file smt_command.h
 * \brief SMT-LIB commands that appear in a query file.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_AST_COMMAND_H
#define SMTLIB_PARSER_AST_COMMAND_H

#include "ast_abstract.h"
#include "ast_attribute.h"
#include "ast_basic.h"
#include "ast_datatype.h"
#include "ast_interfaces.h"
#include "ast_fun.h"
#include "ast_literal.h"
#include "ast_sort.h"

#include <memory>
#include <vector>

namespace smtlib {
    namespace ast {
        /* ===================================== Command ====================================== */
        /** Abstract root of the hierarchy of commands */
        class Command : public AstNode { };

        /* ================================== AssertCommand =================================== */
        /**
         * An 'assert' command containing a term.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class AssertCommand : public Command,
                              public std::enable_shared_from_this<AssertCommand> {
        private:
            sptr_t<Term> term;

        public:
            /**
             * \param term  Asserted term
             */
            inline AssertCommand(sptr_t<Term> term) : term(term) { }

            inline sptr_t<Term> getTerm() { return term; }

            inline void setTerm(sptr_t<Term> term) { this->term = term; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================= CheckSatCommand ================================== */
        /**
         * A 'check-sat' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class CheckSatCommand : public Command,
                                public std::enable_shared_from_this<CheckSatCommand> {
        public:
            inline CheckSatCommand() { }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =============================== CheckSatAssumCommand =============================== */
        /**
         * A 'check-sat-assuming' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class CheckSatAssumCommand : public Command,
                                     public std::enable_shared_from_this<CheckSatAssumCommand> {
        private:
            sptr_v<PropLiteral> assumptions;

        public:
            /**
             * \param assumptions   List of assumptions
             */
            CheckSatAssumCommand(sptr_v<PropLiteral>& assumptions);

            inline sptr_v<PropLiteral>& getAssumptions() { return assumptions; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =============================== DeclareConstCommand ================================ */
        /**
         * A 'declare-const' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class DeclareConstCommand : public Command,
                                    public std::enable_shared_from_this<DeclareConstCommand> {
        private:
            sptr_t<Symbol> symbol;
            sptr_t<Sort> sort;
        public:
            /**
             * \param name  Name of the constant
             * \param sort  Sort of the constant
             */
            inline DeclareConstCommand(sptr_t<Symbol> symbol, sptr_t<Sort> sort)
                    : symbol(symbol), sort(sort) { }

            inline sptr_t<Symbol> getSymbol() { return symbol; }

            inline void setSymbol(sptr_t<Symbol> symbol) { this->symbol = symbol; }

            inline sptr_t<Sort> getSort() { return sort; }

            inline void setSort(sptr_t<Sort> sort) { this->sort = sort; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ============================== DeclareDatatypeCommand ============================== */
        /**
         * A 'declare-datatype' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class DeclareDatatypeCommand : public Command,
                                       public std::enable_shared_from_this<DeclareDatatypeCommand> {
        private:
            sptr_t<Symbol> symbol;
            sptr_t<DatatypeDeclaration> declaration;
        public:
            /**
             * \param declaration   Datatype declaration
             */
            inline DeclareDatatypeCommand(sptr_t<Symbol> symbol,
                                          sptr_t<DatatypeDeclaration> declaration)
                    : symbol(symbol), declaration(declaration) { }

            inline sptr_t<Symbol> getSymbol() { return symbol; }

            inline void setSymbol(sptr_t<Symbol> symbol) { this->symbol = symbol; }

            inline sptr_t<DatatypeDeclaration> getDeclaration() { return  declaration; }

            inline void setDeclaration(sptr_t<DatatypeDeclaration> declaration) {
                this->declaration = declaration;
            }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ============================= DeclareDatatypesCommand ============================== */
        /**
         * A 'declare-datatypes' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class DeclareDatatypesCommand : public Command,
                                        public std::enable_shared_from_this<DeclareDatatypesCommand> {
        private:
            sptr_v<SortDeclaration> sorts;
            sptr_v<DatatypeDeclaration> declarations;
        public:
            /**
             * \param sorts         Names and arities of the new datatypes
             * \param declarations  Declarations of the new datatypes
             */
            DeclareDatatypesCommand(sptr_v<SortDeclaration>& sorts,
                                    sptr_v<DatatypeDeclaration>& declarations);

            inline sptr_v<SortDeclaration>& getSorts() { return sorts; }

            inline sptr_v<DatatypeDeclaration>& getDeclarations() { return declarations; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================ DeclareFunCommand ================================= */
        /**
         * A 'declare-fun' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class DeclareFunCommand : public Command,
                                  public std::enable_shared_from_this<DeclareFunCommand> {
        private:
            sptr_t<Symbol> symbol;
            sptr_v<Sort> params;
            sptr_t<Sort> sort;
        public:
            /**
             * \param name      Name of the function
             * \param params    Sorts of the parameters
             * \param sort      Sort of the return value
             */
            DeclareFunCommand(sptr_t<Symbol> symbol,
                              sptr_v<Sort>& params,
                              sptr_t<Sort> sort);

            inline sptr_t<Symbol> getSymbol() { return symbol; }

            inline void setSymbol(sptr_t<Symbol> symbol) { this->symbol = symbol; }

            inline sptr_v<Sort>& getParams() { return params; }

            inline sptr_t<Sort> getSort() { return sort; }

            inline void setSort(sptr_t<Sort> sort) { this->sort = sort; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================ DeclareSortCommand ================================ */
        /**
         * A 'declare-sort' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class DeclareSortCommand : public Command,
                                   public std::enable_shared_from_this<DeclareSortCommand> {
        private:
            sptr_t<Symbol> symbol;
            sptr_t<NumeralLiteral> arity;
        public:
            /**
             * \param name      Name of the sort
             * \param arity     Arity of the sort
             */
            inline DeclareSortCommand(sptr_t<Symbol> symbol,
                                      sptr_t<NumeralLiteral> arity)
                    : symbol(symbol), arity(arity) { }

            inline sptr_t<Symbol> getSymbol() { return symbol; }

            inline void setSymbol(sptr_t<Symbol> symbol) { this->symbol = symbol; }

            inline sptr_t<NumeralLiteral> getArity() { return arity; }

            inline void setArity(sptr_t<NumeralLiteral> arity) { this->arity = arity; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================= DefineFunCommand ================================= */
        /**
         * A 'define-fun' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class DefineFunCommand : public Command,
                                 public std::enable_shared_from_this<DefineFunCommand> {
        private:
            sptr_t<FunctionDefinition> definition;
        public:
            /**
             * \param definition    Function definition
             */
            inline DefineFunCommand(sptr_t<FunctionDefinition> definition)
                    : definition(definition) { }

            /**
             * \param signature    Function signature
             * \param body         Function body
             */
            inline DefineFunCommand(sptr_t<FunctionDeclaration> signature,
                                    sptr_t<Term> body)
                    : definition(std::make_shared<FunctionDefinition>(signature, body)) { }

            /**
             * \param symbol    Name of the function
             * \param params    List of parameters
             * \param type      Sort of the return value
             * \param body      Function body
             */
            DefineFunCommand(sptr_t<Symbol> symbol,
                             sptr_v<SortedVariable>& params,
                             sptr_t<Sort> sort,
                             sptr_t<Term> body);

            inline sptr_t<FunctionDefinition> getDefinition() { return definition; }

            inline void setDefinition(sptr_t<FunctionDefinition> definition) { this->definition = definition; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================ DefineFunRecCommand =============================== */
        /**
         * A 'define-fun-rec' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class DefineFunRecCommand : public Command,
                                    public std::enable_shared_from_this<DefineFunRecCommand> {
        private:
            sptr_t<FunctionDefinition> definition;
        public:
            /**
             * \param definition    Function definition
             */
            inline DefineFunRecCommand(sptr_t<FunctionDefinition> definition)
                    : definition(definition) { }

            /**
             * \param signature    Function signature
             * \param body         Function body
             */
            inline DefineFunRecCommand(sptr_t<FunctionDeclaration> signature, sptr_t<Term> body) {
                definition = std::make_shared<FunctionDefinition>(signature, body);
            }

            /**
             * \param symbol    Name of the function
             * \param params    List of parameters
             * \param type      Sort of the return value
             * \param body      Function body
             */
            DefineFunRecCommand(sptr_t<Symbol> symbol,
                                sptr_v<SortedVariable>& params,
                                sptr_t<Sort> sort,
                                sptr_t<Term> body);

            inline sptr_t<FunctionDefinition> getDefinition() { return definition; }

            inline void setDefinition(sptr_t<FunctionDefinition> definition) { this->definition = definition; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =============================== DefineFunsRecCommand =============================== */
        /**
         * A 'define-funs-rec' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class DefineFunsRecCommand : public Command,
                                     public std::enable_shared_from_this<DefineFunsRecCommand> {
        private:
            sptr_v<FunctionDeclaration> declarations;
            sptr_v<Term> bodies;
        public:
            /**
             * \param declarations    Function declarations
             * \param bodies          Function bodies
             */
            DefineFunsRecCommand(sptr_v<FunctionDeclaration>& declarations, sptr_v<Term>& bodies);

            inline sptr_v<FunctionDeclaration>& getDeclarations() { return declarations; }

            inline sptr_v<Term>& getBodies() { return bodies; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================ DefineSortCommand ================================= */
        /**
         * A 'define-sort' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class DefineSortCommand : public Command,
                                  public std::enable_shared_from_this<DefineSortCommand> {
        private:
            sptr_t<Symbol> symbol;
            sptr_v<Symbol> params;
            sptr_t<Sort> sort;
        public:
            /**
             * \param symbol    Name of the sort
             * \param params    Sort parameters
             * \param sort      Definition of the new sort
             */
            DefineSortCommand(sptr_t<Symbol> symbol, sptr_v<Symbol>& params, sptr_t<Sort> sort);

            inline sptr_t<Symbol> getSymbol() { return symbol; }

            inline void setSymbol(sptr_t<Symbol> symbol) { this->symbol = symbol; }

            inline sptr_v<Symbol>& getParams() { return params; }

            inline sptr_t<Sort> getSort() { return sort; }

            inline void setSort(sptr_t<Sort> sort) { this->sort = sort; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =================================== EchoCommand ==================================== */
        /**
         * An 'echo' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class EchoCommand : public Command,
                            public std::enable_shared_from_this<EchoCommand> {
        private:
            std::string message;
        public:
            /**
             * \param   Message to print
             */
            inline EchoCommand(std::string message) : message(message) { }

            inline std::string &getMessage() { return message; }

            inline void setMessage(std::string message) { this->message = message; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =================================== ExitCommand ==================================== */
        /**
         * An 'exit' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class ExitCommand : public Command,
                            public std::enable_shared_from_this<ExitCommand> {
        public:
            inline ExitCommand() { }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================ GetAssertsCommand ================================= */
        /**
         * A 'get-assertions' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class GetAssertsCommand : public Command,
                                  public std::enable_shared_from_this<GetAssertsCommand> {
        public:
            inline GetAssertsCommand() { }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================ GetAssignsCommand ================================= */
        /**
         * A 'get-assignments' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class GetAssignsCommand : public Command,
                                  public std::enable_shared_from_this<GetAssignsCommand> {
        public:
            inline GetAssignsCommand() { }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================== GetInfoCommand ================================== */
        /**
         * A 'get-info' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class GetInfoCommand : public Command,
                               public std::enable_shared_from_this<GetInfoCommand> {
        private:
            sptr_t<Keyword> flag;
        public:
            /**
             * \param flag  Flag name
             */
            inline GetInfoCommand(sptr_t<Keyword> flag) : flag(flag) { }

            inline sptr_t<Keyword> getFlag() { return flag; }

            inline void setFlag(sptr_t<Keyword> flag) { this->flag = flag; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================= GetModelCommand ================================== */
        /**
         * A 'get-model' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class GetModelCommand : public Command,
                                public std::enable_shared_from_this<GetModelCommand> {
        public:
            inline GetModelCommand() { }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================= GetOptionCommand ================================= */
        /**
         * A 'get-option' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class GetOptionCommand : public Command,
                                 public std::enable_shared_from_this<GetOptionCommand> {
        private:
            sptr_t<Keyword> option;
        public:
            /**
             * \param option    Option name
             */
            inline GetOptionCommand(sptr_t<Keyword> option) : option(option) { }

            inline sptr_t<Keyword> getOption() { return option; }

            inline void setOption(sptr_t<Keyword> option) { this->option = option; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================= GetProofCommand ================================== */
        /**
         * A 'get-proof' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class GetProofCommand : public Command,
                                public std::enable_shared_from_this<GetProofCommand> {
        public:
            inline GetProofCommand() { }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ============================== GetUnsatAssumsCommand =============================== */
        /**
         * A 'get-unsat-assumptions' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class GetUnsatAssumsCommand : public Command,
                                      public std::enable_shared_from_this<GetUnsatAssumsCommand> {
        public:
            inline GetUnsatAssumsCommand() { }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =============================== GetUnsatCoreCommand ================================ */
        /**
         * A 'get-unsat-core' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class GetUnsatCoreCommand : public Command,
                                    public std::enable_shared_from_this<GetUnsatCoreCommand> {
        public:
            inline GetUnsatCoreCommand() { }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================= GetValueCommand ================================== */
        /**
         * A 'get-value' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class GetValueCommand : public Command,
                                public std::enable_shared_from_this<GetValueCommand> {
        private:
            sptr_v<Term> terms;
        public:
            /**
             * \param terms Terms to evaluate
             */
            GetValueCommand(sptr_v<Term>& terms);

            inline sptr_v<Term>& getTerms() { return terms; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ==================================== PopCommand ==================================== */
        /**
         * A 'pop' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class PopCommand : public Command,
                           public std::enable_shared_from_this<PopCommand> {
        private:
            sptr_t<NumeralLiteral> numeral;
        public:
            /**
             * \param numeral   Number of levels to pop
             */
            inline PopCommand(sptr_t<NumeralLiteral> numeral) : numeral(numeral) { }

            inline sptr_t<NumeralLiteral> getNumeral() { return numeral; }

            inline void setNumeral(sptr_t<NumeralLiteral> numeral) { this->numeral = numeral; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =================================== PushCommand ==================================== */
        /**
         * A 'push' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class PushCommand : public Command,
                            public std::enable_shared_from_this<PushCommand> {
        private:
            sptr_t<NumeralLiteral> numeral;
        public:
            /**
             * \param numeral   Number of levels to push
             */
            inline PushCommand(sptr_t<NumeralLiteral> numeral) : numeral(numeral) { }

            inline sptr_t<NumeralLiteral> getNumeral() { return numeral; }

            inline void setNumeral(sptr_t<NumeralLiteral> numeral) { this->numeral = numeral; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =================================== ResetCommand =================================== */
        /**
         * A 'reset' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class ResetCommand : public Command,
                             public std::enable_shared_from_this<ResetCommand> {
        public:
            inline ResetCommand() { }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* =============================== ResetAssertsCommand ================================ */
        /**
         * A 'reset-assertions' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class ResetAssertsCommand : public Command,
                                    public std::enable_shared_from_this<ResetAssertsCommand> {
        public:
            inline ResetAssertsCommand() { }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================== SetInfoCommand ================================== */
        /**
         * A 'set-info' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class SetInfoCommand : public Command,
                               public std::enable_shared_from_this<SetInfoCommand> {
        private:
            sptr_t<Attribute> info;
        public:
            /**
             * \param info    Info to set
             */
            inline SetInfoCommand(sptr_t<Attribute> info) : info(info) { }

            inline sptr_t<Attribute> getInfo() { return info; }

            inline void setInfo(sptr_t<Attribute> info) { this->info = info; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================= SetLogicCommand ================================== */
        /**
         * A 'set-logic' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class SetLogicCommand : public Command,
                                public std::enable_shared_from_this<SetLogicCommand> {
        private:
            sptr_t<Symbol> logic;
        public:
            /**
             * \param name  Name of the logic to set
             */
            inline SetLogicCommand(sptr_t<Symbol> logic) : logic(logic) { }

            inline sptr_t<Symbol> getLogic() { return logic; }

            inline void setLogic(sptr_t<Symbol> logic) { this->logic = logic; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };

        /* ================================= SetOptionCommand ================================= */
        /**
         * A 'set-option' command.
         * Node of the SMT-LIB abstract syntax tree.
         */
        class SetOptionCommand : public Command,
                                 public std::enable_shared_from_this<SetOptionCommand> {
        private:
            sptr_t<Attribute> option;
        public:
            /**
             * \param option    Option to set
             */
            inline SetOptionCommand(sptr_t<Attribute> option) : option(option) { }

            inline sptr_t<Attribute> getOption() { return option; }

            inline void setOption(sptr_t<Attribute> option) { this->option = option; }

            virtual void accept(AstVisitor0* visitor);

            virtual std::string toString();
        };
    }
}

#endif //SMTLIB_PARSER_AST_COMMAND_H