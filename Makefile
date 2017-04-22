INC=-I./smtlib -I.

smtlib-parser:
	g++ -g -c -std=c++11 $(INC) exec/execution.cpp -o execution.o
	g++ -g -c -std=c++11 $(INC) exec/execution_settings.cpp -o execution_settings.o
	g++ -g -c -std=c++11 $(INC) smtlib/parser/smtlib-flex-lexer.l.c -o smtlib-flex-lexer.l.o
	g++ -g -c -std=c++11 $(INC) smtlib/parser/smtlib-bison-parser.y.c -o smtlib-bison-parser.y.o
	g++ -g -c -std=c++11 $(INC) smtlib/parser/smtlib-glue.cpp -o smtlib-glue.o
	g++ -g -c -std=c++11 $(INC) smtlib/parser/smtlib_parser.cpp -o smtlib_parser.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_attribute.cpp -o ast_attribute.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_basic.cpp -o ast_basic.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_command.cpp -o ast_command.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_datatype.cpp -o ast_datatype.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_fun.cpp -o ast_fun.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_identifier.cpp -o ast_identifier.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_literal.cpp -o ast_literal.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_logic.cpp -o ast_logic.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_match.cpp -o ast_match.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_script.cpp -o ast_script.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_sexp.cpp -o ast_sexp.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_sort.cpp -o ast_sort.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_symbol_decl.cpp -o ast_symbol_decl.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_term.cpp -o ast_term.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_theory.cpp -o ast_theory.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/ast_var.cpp -o ast_var.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/stack/ast_symbol_stack.cpp -o ast_symbol_stack.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/stack/ast_symbol_table.cpp -o ast_symbol_table.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/stack/ast_symbol_util.cpp -o ast_symbol_util.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/visitor/ast_visitor.cpp -o ast_visitor.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/visitor/ast_syntax_checker.cpp -o ast_syntax_checker.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/visitor/ast_sortedness_checker.cpp -o ast_sortedness_checker.o
	g++ -g -c -std=c++11 $(INC) smtlib/ast/visitor/ast_term_sorter.cpp -o ast_term_sorter.o
	g++ -g -c -std=c++11 $(INC) util/configuration.cpp -o configuration.o
	g++ -g -c -std=c++11 $(INC) util/error_messages.cpp -o error_messages.o
	g++ -g -c -std=c++11 $(INC) util/global_values.cpp -o global_values.o
	g++ -g -c -std=c++11 $(INC) util/logger.cpp -o logger.o
	g++ -g -c -std=c++11 $(INC) main.cpp -o main.o
	g++ -g -o smtlib-parser smtlib-flex-lexer.l.o smtlib-bison-parser.y.o smtlib-glue.o \
	ast_attribute.o ast_basic.o ast_command.o ast_datatype.o ast_fun.o \
	ast_identifier.o ast_literal.o ast_logic.o ast_match.o ast_script.o \
	ast_sexp.o ast_sort.o ast_symbol_decl.o ast_term.o ast_theory.o \
	ast_var.o smtlib_parser.o ast_symbol_stack.o ast_symbol_table.o \
	ast_symbol_util.o error_messages.o logger.o global_values.o ast_visitor.o \
	ast_syntax_checker.o ast_sortedness_checker.o ast_term_sorter.o \
	execution.o execution_settings.o configuration.o main.o -lfl
	rm -f *.o
clean:
	rm -f *.o
	rm -f smtlib-parser