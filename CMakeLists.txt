cmake_minimum_required(VERSION 2.8.4)
project(smtlib-parser)

set(LIB_FLEX fl)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${CMAKE_SOURCE_DIR} "smtlib")
set(SOURCE_FILES
        main.cpp
        exec/execution.h
        exec/execution.cpp
        exec/execution_settings.h
        exec/execution_settings.cpp
        smtlib/ast/ast_abstract.h
        smtlib/ast/ast_attribute.h
        smtlib/ast/ast_attribute.cpp
        smtlib/ast/ast_basic.h
        smtlib/ast/ast_basic.cpp
        smtlib/ast/ast_classes.h
        smtlib/ast/ast_command.cpp
        smtlib/ast/ast_command.h
        smtlib/ast/ast_datatype.h
        smtlib/ast/ast_datatype.cpp
        smtlib/ast/ast_fun.cpp
        smtlib/ast/ast_fun.h
        smtlib/ast/ast_logic.cpp
        smtlib/ast/ast_logic.h
        smtlib/ast/ast_identifier.cpp
        smtlib/ast/ast_identifier.h
        smtlib/ast/ast_interfaces.h
        smtlib/ast/ast_literal.cpp
        smtlib/ast/ast_literal.h
        smtlib/ast/ast_match.h
        smtlib/ast/ast_match.cpp
        smtlib/ast/ast_script.cpp
        smtlib/ast/ast_script.h
        smtlib/ast/ast_sexp.cpp
        smtlib/ast/ast_sexp.h
        smtlib/ast/ast_sort.cpp
        smtlib/ast/ast_sort.h
        smtlib/ast/ast_symbol_decl.cpp
        smtlib/ast/ast_symbol_decl.h
        smtlib/ast/ast_term.cpp
        smtlib/ast/ast_term.h
        smtlib/ast/ast_theory.h
        smtlib/ast/ast_theory.cpp
        smtlib/ast/ast_var.cpp
        smtlib/ast/ast_var.h
        smtlib/ast/stack/ast_symbol_table.h
        smtlib/ast/stack/ast_symbol_table.cpp
        smtlib/ast/stack/ast_symbol_stack.h
        smtlib/ast/stack/ast_symbol_stack.cpp
        smtlib/ast/stack/ast_symbol_util.h
        smtlib/ast/stack/ast_symbol_util.cpp
        smtlib/ast/visitor/ast_syntax_checker.h
        smtlib/ast/visitor/ast_syntax_checker.cpp
        smtlib/ast/visitor/ast_sortedness_checker.h
        smtlib/ast/visitor/ast_sortedness_checker.cpp
        smtlib/ast/visitor/ast_term_sorter.h
        smtlib/ast/visitor/ast_term_sorter.cpp
        smtlib/ast/visitor/ast_visitor.h
        smtlib/ast/visitor/ast_visitor.cpp
        smtlib/ast/visitor/ast_visitor_extra.h
        smtlib/parser/smtlib-glue.h
        smtlib/parser/smtlib-glue.cpp
        smtlib/parser/smtlib-bison-parser.y.h
        smtlib/parser/smtlib-bison-parser.y.c
        smtlib/parser/smtlib-flex-lexer.l.c
        smtlib/parser/smtlib_parser.h
        smtlib/parser/smtlib_parser.cpp
        util/global_values.h
        util/global_values.cpp
        util/configuration.h
        util/configuration.cpp
        util/error_messages.h
        util/error_messages.cpp
        util/logger.h
        util/logger.cpp)

add_executable(smtlib-parser ${SOURCE_FILES})
target_link_libraries(smtlib-parser ${LIB_FLEX})