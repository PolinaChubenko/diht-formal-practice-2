#include "LRMock.h"

void LRMock::set_automaton_grammar() {
    automaton.set_grammar(grammar);
}

void LRMock::do_init_table() {
    init_table();
}

void LRMock::do_build_table() {
    build_table();
}