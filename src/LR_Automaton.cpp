#include "LR_Automaton.h"


Automaton::Situation::Situation(char term, std::string terms, size_t dot_pos, std::set<char> predict)
        : rule(Rule(term, std::move(terms), dot_pos)), predict(std::move(predict)) {}

Automaton::Situation::Situation(const std::string &rule, size_t dot_pos, std::set<char> predict)
        : rule(Rule(rule, dot_pos)), predict(std::move(predict)) {}

Automaton::Situation::Situation(const std::string& rule, char symbol) : rule(rule) {
    predict.insert(symbol);
}

Automaton::SetOfSituations::SetOfSituations(int state_number) : state_number(state_number) {}

bool operator<(const Automaton::Situation &situation1, const Automaton::Situation &situation2) {
    if (situation1.rule == situation2.rule) {
        return situation1.predict < situation2.predict;
    }
    return situation1.rule < situation2.rule;
}

bool operator==(const Automaton::Situation &situation1, const Automaton::Situation &situation2) {
    return situation1.rule == situation2.rule && situation1.predict == situation2.predict;
}

bool operator==(const Automaton::SetOfSituations &set1, const Automaton::SetOfSituations &set2) {
    return set1.situations == set2.situations;
}

std::ostream &operator<<(std::ostream &out, const Automaton::SetOfSituations &set) {
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


/////////////////////////      LR_AUTOMATON      /////////////////////////

void Automaton::init_first() {
    first.clear();
    for (auto& symbol : grammar.get_terminals()) {
        first[symbol].insert(symbol);
    }
    bool is_changed = true;
    while (is_changed) {
        is_changed = false;
        for (auto& rule : grammar.get_rules()) {
            size_t prev_size = first[rule.get_left()].size();
            auto it_begin = first[rule.get_right()[0]].begin();
            auto it_end = first[rule.get_right()[0]].end();
            first[rule.get_left()].insert(it_begin, it_end);
            if (prev_size != first[rule.get_left()].size()) {
                is_changed = true;
            }
        }
    }
}

std::set<char> Automaton::get_first(const Situation& situation) {
    auto rule = situation.rule;
    if (rule.get_dot_pos() + 1 == rule.get_right().size()) {
        return situation.predict;
    }
    auto next = rule.get_right()[rule.get_dot_pos() + 1];
    return first[next];
}


void Automaton::closure(size_t set_number) {
    size_t prev_size = 0;
    SetOfSituations additional;
    while (sets[set_number].situations.size() != prev_size) {
        prev_size = sets[set_number].situations.size();
        for (auto &situation : sets[set_number].situations) {
            // A -> p.Bq
            if (situation.rule.is_dot_valid()) {
                auto rules = grammar.get_rules();
                for (const auto &rule : rules) {
                    // B -> r
                    if (situation.rule.get_dot_symbol() != rule.get_left()) {
                        continue;
                    }
                    auto predict = get_first(situation);
                    auto new_situation = Situation(rule.get_left(), rule.get_right(), 0, predict);
                    additional.situations.emplace(new_situation);
                }
            }
        }
        sets[set_number].situations.merge(additional.situations);
    }
}

void Automaton::go_to(size_t set_number) {
    auto set = sets[set_number];
    for (auto& situation : set.situations) {
        auto rule = situation.rule;
        if (rule.is_dot_valid()) {
            auto letter = rule.get_dot_symbol();
            auto search = sets[set_number].next.find(letter);
            if (search == sets[set_number].next.end()) {
                sets[set_number].next[letter] = sets.size();
                sets.emplace_back(SetOfSituations());
                sets.back().prev[letter].emplace(set_number);
            }
            size_t new_dot_pos = rule.get_dot_pos() + 1;
            auto new_situation = Situation(rule.get_left(), rule.get_right(), new_dot_pos, situation.predict);
            auto to = sets[set_number].next[letter];
            sets[to].situations.emplace(new_situation);
        }
    }
}


void Automaton::build_automaton() {
    sets.resize(1);
    sets[0].situations.emplace(Situation("$->S", '#'));
    size_t state = 0;
    for (size_t set_number = 0; set_number < sets.size(); ++set_number) {
        closure(set_number);
        int same = find_equal_set(set_number);
        if (same != -1) {
            for (auto& prev : sets[set_number].prev) {
                char letter = prev.first;
                std::set<size_t> vertices = prev.second;
                for (auto& vertex : vertices) {
                    sets[vertex].next[letter] = same;
                    sets[same].prev[letter].emplace(vertex);
                }
            }
            sets[set_number] = SetOfSituations(-1);
            continue;
        }
        go_to(set_number);
        sets[set_number].state_number = state;
        alignment[set_number] = state++;
    }
}

size_t Automaton::find_equal_set(size_t index) {
    for (size_t i = 0; i < index; ++i) {
        if (sets[index] == sets[i]) {
            return i;
        }
    }
    return -1;
}

void Automaton::set_grammar(const ContextFreeGrammar &new_grammar) {
    grammar = new_grammar;
    init_first();
    build_automaton();
}

std::ostream &operator<<(std::ostream &out, const Automaton &automaton) {
    for (auto& set : automaton.sets) {
        if (set.state_number != -1) {
            out << "V_" << set.state_number << ":" << set;
            out << "\nFROM: ";
            for (auto& prev : set.prev) {
                out << prev.first << "-{";
                size_t ver_iter = 0;
                for (auto ver : prev.second) {
                    out << automaton.alignment.at(ver);
                    ++ver_iter;
                    if (ver_iter != prev.second.size()) {
                        out << " ";
                    }
                }
                out << "} ";
            }
            out << "\nTO: ";
            for (auto& next : set.next) {
                out << next.first << "-" << automaton.alignment.at(next.second) << " ";
            }
            out << "\n";
        }
    }
    return out;
}


