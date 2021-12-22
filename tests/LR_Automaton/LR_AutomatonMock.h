#pragma once

#include <gmock/gmock.h>
#include "LR_Automaton.h"


class LR_AutomatonMock : public LR_Automaton {
public:
    void set_grammar(const ContextFreeGrammar&);
    void add_situation_to_sets(size_t, std::string, size_t, std::string);
    void do_first();
    void do_closure(size_t);
    void do_go_to(size_t);
};

