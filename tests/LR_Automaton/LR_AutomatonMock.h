#pragma once

#include <gmock/gmock.h>
#include "LR_Automaton.h"


class LR_AutomatonMock : public LR_Automaton {
public:
    void set_grammar_only(const ContextFreeGrammar&);
    void add_situation_to_sets(size_t, std::string, size_t, std::set<char>);
    void do_first();
    void do_closure(size_t);
    void do_go_to(size_t);
    void print_first(std::ostream&);
    void print_set(std::ostream&, size_t);
    int do_find_equal_set(size_t);
};

