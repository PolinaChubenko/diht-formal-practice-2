#include "LRMock.h"

void LRMock::set_automaton_grammar(const ContextFreeGrammar& new_grammar) {
    grammar = new_grammar;
    automaton.set_grammar(new_grammar);
}

void LRMock::do_init_table() {
    init_table();
}

void LRMock::do_build_table() {
    build_table();
}

std::map<char, std::vector<LRMock::Action>> LRMock::get_table() {
    return table;
}

LRMock::Action LRMock::get_action(char method, int state) {
    auto action = Action(method, state);
    return action;
}

LRMock::Action LRMock::get_action(char method, int state, std::string str_rule) {
    auto action = Action(method, Rule(str_rule));
    action.state = state;
    return action;
}
