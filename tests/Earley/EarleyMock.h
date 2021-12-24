#pragma once

#include <gmock/gmock.h>
#include "Earley.h"


class EarleyMock : public Earley {
public:
    void set_D_size(size_t size);
    void add_situation_to_D(size_t, std::string, size_t, size_t);
    void do_scan(size_t, const std::string&);
    bool do_complete(size_t);
    bool do_predict(size_t);
};


