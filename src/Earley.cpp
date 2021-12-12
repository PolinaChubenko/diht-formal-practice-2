#include "Earley.h"


Earley::Situation::Situation(char term, std::string terms, size_t dot_pos, size_t word_pos)
        : rule(Rule(term, std::move(terms), dot_pos)), word_pos(word_pos) {}

Earley::Situation::Situation(const std::string &rule, size_t dot_pos, size_t word_pos)
        : rule(Rule(rule, dot_pos)), word_pos(word_pos) {}

Earley::Situation::Situation(const std::string &rule) : rule(rule) {}

bool Earley::Comparator::operator()(const Earley::Situation &situation1, const Earley::Situation &situation2) const {
    if (situation1.rule == situation2.rule) {
        return situation1.word_pos < situation2.word_pos;
    }
    return situation1.rule < situation2.rule;
}


//////////////////////      Earley      //////////////////////

void Earley::scan(size_t j, const std::string &word) {
    if (j == 0) {
        return;
    }
    for (const auto& situation : situations[j - 1]) {
        // A -> P.aQ, i
        if (!situation.rule.is_dot_valid()) {
            continue;
        }
        if (situation.rule.get_dot_term() == word[j - 1]) {
            // A -> Pa.Q, i
            auto new_rule = Situation(situation.rule.get_term(), situation.rule.get_terms(),
                                      situation.rule.get_dot_pos() + 1, situation.word_pos);
            situations[j].insert(new_rule);
        }
    }
}

bool Earley::complete(size_t j) {
    bool is_situations_changed = false;
    for (const auto& situation1 : situations[j]) {
        // B -> p., i
        if (situation1.rule.get_dot_pos() != situation1.rule.get_terms().size()) {
            continue;
        }
        for (const auto& situation2 : situations[situation1.word_pos]) {
            // A -> q.Br, j
            if (!situation2.rule.is_dot_valid()) {
                continue;
            }
            if (situation2.rule.get_dot_term() != situation1.rule.get_term()) {
                continue;
            }
            // A -> qB.r, j
            auto new_rule = Situation(situation2.rule.get_term(), situation2.rule.get_terms(),
                                      situation2.rule.get_dot_pos() + 1, situation2.word_pos);
            auto result = situations[j].insert(new_rule);
            if (result.second) {
                is_situations_changed = true;
            }
        }
    }
    return is_situations_changed;
}

bool Earley::predict(size_t j) {
    bool is_situations_changed = false;
    for (const auto& situation : situations[j]) {
        // A -> p.Bq, i
        if (!situation.rule.is_dot_valid()) {
            continue;
        }
        auto rules = grammar.get_rules();
        for (const auto& rule : rules) {
            // B -> r
            if (situation.rule.get_dot_term() != rule.get_term()) {
                continue;
            }
            // B -> .r, j
            auto new_rule = Situation(rule.get_term(), rule.get_terms(), 0, j);
            auto result = situations[j].insert(new_rule);
            if (result.second) {
                is_situations_changed = true;
            }
        }
    }
    return is_situations_changed;
}

Earley::Earley(size_t n) : grammar(ContextFreeGrammar(n)) {}

Earley::Earley(ContextFreeGrammar grammar) : grammar(std::move(grammar)) {}

Earley::Earley(const std::vector<std::string> &vec_of_rules) : grammar(ContextFreeGrammar(vec_of_rules)) {}

void Earley::set_grammar(const ContextFreeGrammar& new_grammar) {
    grammar = new_grammar;
}

std::istream &operator>>(std::istream &in, Earley &earley) {
    in >> earley.grammar;
    return in;
}

std::ostream &operator<<(std::ostream &out, const Earley &earley) {
    size_t D_iter = 0;
    for (auto& set : earley.situations) {
        out << "D_" << D_iter << ": [";
        size_t rules_iter = 0;
        for (auto& situation : set) {
            out << situation.rule << ":" << situation.word_pos;
            ++rules_iter;
            if (rules_iter != set.size()) {
                out << ", ";
            }
        }
        out << "]\n";
        ++D_iter;
    }
    return out;
}

bool Earley::parse(const std::string &word) {
    if (grammar.is_definitely_not_in_grammar(word)) {
        return false;
    }
    situations.clear();
    situations.resize(word.size() + 1);
    situations[0].insert(Situation("$->S"));
    for (size_t j = 0; j < word.size() + 1; ++j) {
        scan(j, word);
        bool is_situations_changed = true;
        while(is_situations_changed) {
            bool is_change_in_complete = complete(j);
            bool is_change_in_predict = predict(j);
            is_situations_changed = is_change_in_complete || is_change_in_predict;
        }
    }
    auto final_rule = Situation("$->S", 1, 0);
    const bool is_word_in_grammar = situations[word.size()].find(final_rule) != situations[word.size()].end();
    return is_word_in_grammar;
}



