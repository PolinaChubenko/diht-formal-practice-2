#include "LR.h"


LR::Situation::Situation(char term, std::string terms, size_t dot_pos, char predict)
        : rule(Rule(term, std::move(terms), dot_pos)), predict(predict) {}

LR::Situation::Situation(const std::string &rule, size_t dot_pos, char predict)
        : rule(Rule(rule, dot_pos)), predict(predict) {}

LR::Situation::Situation(const std::string& rule, char predict) : rule(rule), predict(predict) {}


bool operator<(const LR::Situation &situation1, const LR::Situation &situation2) {
    if (situation1.rule == situation2.rule) {
        return situation1.predict < situation2.predict;
    }
    return situation1.rule < situation2.rule;
}

bool operator==(const LR::Situation &situation1, const LR::Situation &situation2) {
    return situation1.rule == situation2.rule && situation1.predict == situation2.predict;
}

bool operator==(const LR::SetOfSituations &set1, const LR::SetOfSituations &set2) {
    return set1.situations == set2.situations;
}


/////////////////////////      LR(1)      /////////////////////////

void LR::closure(size_t set_number) {
    size_t prev_size = 0;
    SetOfSituations additional;
    while (automaton[set_number].situations.size() != prev_size) {
        prev_size = automaton[set_number].situations.size();
        for (auto &situation : automaton[set_number].situations) {
            // A -> p.Bq
            if (situation.rule.is_dot_valid()) {
                auto rules = grammar.get_rules();
                for (const auto &rule : rules) {
                    // B -> r
                    if (situation.rule.get_dot_term() != rule.get_term()) {
                        continue;
                    }
                    auto new_situation = Situation(rule.get_term(), rule.get_terms(), 0, 'a');
                    additional.situations.emplace(new_situation);
                }
            }
        }
        automaton[set_number].situations.merge(additional.situations);
    }
}

void LR::go_to(size_t set_number) {
    auto set = automaton[set_number];
    for (auto& situation : set.situations) {
        auto rule = situation.rule;
        if (rule.is_dot_valid()) {
            auto letter = rule.get_dot_term();
            auto search = automaton[set_number].next.find(letter);
            if (search == automaton[set_number].next.end()) {
                automaton[set_number].next[letter] = automaton.size();
                automaton.emplace_back(SetOfSituations());
                automaton.back().prev[letter].emplace(set_number);
            }
            auto new_el = Situation(rule.get_term(), rule.get_terms(), rule.get_dot_pos() + 1, 'a');
            auto to = automaton[set_number].next[letter];
            automaton[to].situations.emplace(new_el);
        }
    }
}


void LR::build_automaton() {
    automaton.resize(1);
    automaton[0].situations.emplace(Situation("$->S", '#'));
    size_t set_number = 0;
    while (set_number < automaton.size()) {
        closure(set_number);
        int same = find_equal_set(set_number);
        if (same != -1) {
            for (auto& prev : automaton[set_number].prev) {
                char letter = prev.first;
                std::set<size_t> vertices = prev.second;
                for (auto& vertex : vertices) {
                    automaton[vertex].next[letter] = same;
                    automaton[same].prev[letter].emplace(vertex);
                }
            }
            automaton.pop_back();
            continue;
        }
        go_to(set_number);
        ++set_number;
    }
}

size_t LR::find_equal_set(size_t index) {
    for (size_t i = 0; i < automaton.size(); ++i) {
        if (i == index) {
            continue;
        }
        if (automaton[index] == automaton[i]) {
            return i;
        }
    }
    return -1;
}




LR::LR(size_t n) : grammar(ContextFreeGrammar(n)) {}

LR::LR(ContextFreeGrammar grammar) : grammar(std::move(grammar)) {}

LR::LR(const std::vector<std::string> &vec_of_rules) : grammar(ContextFreeGrammar(vec_of_rules)) {}

void LR::set_grammar(const ContextFreeGrammar& new_grammar) {
    grammar = new_grammar;
}

std::istream &operator>>(std::istream &in, LR &lr) {
    in >> lr.grammar;
    lr.build_automaton();
    return in;
}

std::ostream &operator<<(std::ostream &out, const LR &lr) {
    size_t D_iter = 0;
    for (auto& set : lr.automaton) {
        out << "D_" << D_iter << ": [";
        size_t rules_iter = 0;
        for (auto& situation : set.situations) {
            out << situation.rule << ":" << situation.predict;
            ++rules_iter;
            if (rules_iter != set.situations.size()) {
                out << ", ";
            }
        }
        out << "]\nFROM: ";
        for (auto& prev : set.prev) {
            out << prev.first << "-";
            for (auto ver : prev.second) {
                out << ver << " ";
            }
        }
        out << "\nTO: ";
        for (auto& next : set.next) {
            out << next.first << "-" << next.second << " ";
        }
        out << "\n";
        ++D_iter;
    }
    return out;
}

bool LR::parse(const std::string &word) {
    if (grammar.is_definitely_not_in_grammar(word)) {
        return false;
    }
    return true;
}
