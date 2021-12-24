#pragma once

#include <gmock/gmock.h>
#include "LR.h"


class LRMock : public LR {
public:
    void set_automaton_grammar(const ContextFreeGrammar&);
    void do_init_table();
    void do_build_table();
    std::map<char, std::vector<Action>> get_table();
    Action get_action(char, int);
    Action get_action(char, int, std::string);
};
