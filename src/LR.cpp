#include "LR.h"



LR::Situation::Situation(char term, std::string terms, size_t dot_pos, std::set<char> predict)
        : rule(Rule(term, std::move(terms), dot_pos)), predict(std::move(predict)) {}

LR::Situation::Situation(const std::string &rule, size_t dot_pos, std::set<char> predict)
        : rule(Rule(rule, dot_pos)), predict(std::move(predict)) {}

LR::Situation::Situation(const std::string& rule, char symbol) : rule(rule) {
    predict.insert(symbol);
}

LR::SetOfSituations::SetOfSituations(int state_number) : state_number(state_number) {}

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

std::ostream &operator<<(std::ostream &out, const LR::SetOfSituations &set) {
    out << "[";
    size_t rules_iter = 0;
    for (auto& situation : set.situations) {
        out << situation.rule << ":";
        for (auto& predict : situation.predict ) {
            out << predict;
        }
        ++rules_iter;
        if (rules_iter != set.situations.size()) {
            out << ", ";
        }
    }
    out << "]";
    return out;
}


/////////////////////////      LR(1)      /////////////////////////

void LR::init_first() {
    first.clear();
    for (auto& symbol : grammar.get_terminals()) {
        first[symbol].insert(symbol);
    }
    bool is_changed = true;
    while (is_changed) {
        is_changed = false;
        for (auto& rule : grammar.get_rules()) {
            size_t prev_size = first[rule.get_term()].size();
            auto it_begin = first[rule.get_terms()[0]].begin();
            auto it_end = first[rule.get_terms()[0]].end();
            first[rule.get_term()].insert(it_begin, it_end);
            if (prev_size != first[rule.get_term()].size()) {
                is_changed = true;
            }
        }
    }
}

std::set<char> LR::get_first(const Situation& situation) {
    auto rule = situation.rule;
    if (rule.get_dot_pos() + 1 == rule.get_terms().size()) {
        return situation.predict;
    }
    auto next = rule.get_terms()[rule.get_dot_pos() + 1];
    return first[next];
}


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
                    auto predict = get_first(situation);
                    auto new_situation = Situation(rule.get_term(), rule.get_terms(), 0, predict);
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
            size_t new_dot_pos = rule.get_dot_pos() + 1;
            auto new_situation = Situation(rule.get_term(), rule.get_terms(), new_dot_pos, situation.predict);
            auto to = automaton[set_number].next[letter];
            automaton[to].situations.emplace(new_situation);
        }
    }
}


void LR::build_automaton() {
    automaton.resize(1);
    automaton[0].situations.emplace(Situation("$->S", '#'));
    size_t state = 0;
    for (size_t set_number = 0; set_number < automaton.size(); ++set_number) {
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
            automaton[set_number] = SetOfSituations(-1);
            continue;
        }
        go_to(set_number);
        automaton[set_number].state_number = state;
        alignment[set_number] = state++;
    }
}

size_t LR::find_equal_set(size_t index) {
    for (size_t i = 0; i < index; ++i) {
        if (automaton[index] == automaton[i]) {
            return i;
        }
    }
    return -1;
}


void LR::preprocessing() {
    init_first();
    build_automaton();
}

LR::LR(size_t n) : grammar(ContextFreeGrammar(n)) {}

LR::LR(ContextFreeGrammar grammar) : grammar(std::move(grammar)) {
    preprocessing();
}

LR::LR(const std::vector<std::string> &vec_of_rules) : grammar(ContextFreeGrammar(vec_of_rules)) {
    preprocessing();
}

void LR::set_grammar(const ContextFreeGrammar& new_grammar) {
    grammar = new_grammar;
    preprocessing();
}

void LR::get_automaton(std::ostream &out) const {
    for (auto& set : automaton) {
        if (set.state_number != -1) {
            out << "V_" << set.state_number << ":" << set;
            out << "\nFROM: ";
            for (auto& prev : set.prev) {
                out << prev.first << "-{";
                size_t ver_iter = 0;
                for (auto ver : prev.second) {
                    out << alignment.at(ver);
                    ++ver_iter;
                    if (ver_iter != prev.second.size()) {
                        out << " ";
                    }
                }
                out << "} ";
            }
            out << "\nTO: ";
            for (auto& next : set.next) {
                out << next.first << "-" << alignment.at(next.second) << " ";
            }
            out << "\n";
        }
    }
}

void LR::get_table(std::ostream &) const {

}

std::istream &operator>>(std::istream &in, LR &lr) {
    in >> lr.grammar;
    lr.preprocessing();
    return in;
}

std::ostream &operator<<(std::ostream &out, const LR &lr) {
    lr.get_automaton(out);
    return out;
}

bool LR::parse(const std::string &word) {
    if (grammar.is_definitely_not_in_grammar(word)) {
        return false;
    }
    return true;
}
