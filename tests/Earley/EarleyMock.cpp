#include "EarleyMock.h"


void EarleyMock::set_D_size(size_t size) {
    situations.clear();
    situations.resize(size);
}

void EarleyMock::add_situation_to_D(size_t j, std::string rule, size_t dot_pos, size_t word_pos) {
    situations[j].emplace(Situation(rule, dot_pos, word_pos));
}

void EarleyMock::do_scan(size_t j, const std::string &word) {
    scan(j, word);
}

bool EarleyMock::do_complete(size_t j) {
    return complete(j);
}

bool EarleyMock::do_predict(size_t j) {
    return predict(j);
}
