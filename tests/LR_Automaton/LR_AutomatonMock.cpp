#include "LR_AutomatonMock.h"

void LR_AutomatonMock::set_grammar(const ContextFreeGrammar &new_grammar) {
    grammar = new_grammar;
}

void LR_AutomatonMock::add_situation_to_sets(size_t set_number, std::string rule, size_t dot_pos, std::set<char> predict) {
    auto situation = Situation(rule, dot_pos, predict);
    sets.resize(set_number + 1);
    sets[set_number].state_number = set_number;
    alignment[set_number] = set_number;
    sets[set_number].situations.insert(situation);
}

void LR_AutomatonMock::do_first() {
    init_first();
}

void LR_AutomatonMock::do_closure(size_t set_number) {
    closure(set_number);
}

void LR_AutomatonMock::do_go_to(size_t set_number) {
    go_to(set_number);
    for (int i = set_number; i < sets.size(); ++i) {
        sets[i].state_number = i;
        alignment[i] = i;
    }
}