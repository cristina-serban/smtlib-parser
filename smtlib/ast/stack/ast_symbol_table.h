/**
 * \file ast_symbol_table.h
 * \brief Table of known sorts, functions and variables.
 * \author Cristina Serban <cristina.serban89@gmail.com>
 */

#ifndef SMTLIB_PARSER_SMT_SYMBOL_TABLE_H
#define SMTLIB_PARSER_SMT_SYMBOL_TABLE_H

#include "ast_symbol_util.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace smtlib {
    /** A table of known sorts, functions and variables */
    class SymbolTable {
    private:
        sptr_um2<std::string, SortInfo> sorts;
        umap<std::string, sptr_v<FunInfo>> funs;
        sptr_um2<std::string,VarInfo> vars;

    public:
        umap<std::string, sptr_t<SortInfo>>& getSorts();
        umap<std::string, sptr_v<FunInfo>>& getFuns();
        umap<std::string, sptr_t<VarInfo>>& getVars();

        sptr_t<SortInfo> getSortInfo(std::string name);
        sptr_v<FunInfo> getFunInfo(std::string name);
        sptr_t<VarInfo> getVarInfo(std::string name);

        bool add(sptr_t<SortInfo> info);
        bool add(sptr_t<FunInfo> info);
        bool add(sptr_t<VarInfo> info);

        void reset();
    };
}

#endif //SMTLIB_PARSER_SMT_SYMBOL_TABLE_H