#include "LR_AutomatonMock.h"

void LR_AutomatonMock::set_grammar(const ContextFreeGrammar &new_grammar) {
    grammar = new_grammar;
}

void LR_AutomatonMock::add_situation_to_sets(size_t set_number, std::string rule, size_t dot_pos, std::string predict) {
    auto situation = Situation(rule, dot_pos);
    for (auto& letter : predict) {
        situation.predict.insert(letter);
    }
    sets.resize(set_number);
    sets[set_number].state_number = set_number;
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
}