#pragma once

#include <gmock/gmock.h>
#include "LR.h"


class LRMock : public LR {
public:
    void set_automaton_grammar();
    void do_init_table();
    void do_build_table();
};
