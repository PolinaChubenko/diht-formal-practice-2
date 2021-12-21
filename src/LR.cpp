#include "LR.h"



LR::Situation::Situation(char term, std::string terms, size_t dot_pos, std::set<char> predict)
        : rule(Rule(term, std::move(terms), dot_pos)), predict(std::move(predict)) {}

LR::Situation::Situation(const std::string &rule, size_t dot_pos, std::set<char> predict)
        : rule(Rule(rule, dot_pos)), predict(std::move(predict)) {}

LR::Situation::Situation(const std::string& rule, char symbol) : rule(rule) {
    predict.insert(symbol);
}

LR::SetOfSituations::SetOfSituations(int state_number) : state_number(state_number) {}

LR::Action::Action(char method) : method(method) {}

LR::Action::Action(char method, int state) : method(method), state(state) {}

LR::Action::Action(char method, Rule rule) : method(method), rule(std::move(rule)) {}

std::string LR::Action::str() const {
    std::string str_out;
    if (method == 'n') {
        str_out = " ";
    } else if (method == 's') {
        str_out = std::string(1, method);
        str_out.append(":");
        str_out.append(std::to_string(state));
    } else if (method == 'r') {
        str_out = std::string(1, method);
        str_out.append(":");
        str_out.append(rule.str());
    } else {
        str_out = std::string(1, method);
    }
    return str_out;
}

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

std::set<char> LR::get_first(const Situation& situation) {
    auto rule = situation.rule;
    if (rule.get_dot_pos() + 1 == rule.get_right().size()) {
        return situation.predict;
    }
    auto next = rule.get_right()[rule.get_dot_pos() + 1];
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
                    if (situation.rule.get_dot_symbol() != rule.get_left()) {
                        continue;
                    }
                    auto predict = get_first(situation);
                    auto new_situation = Situation(rule.get_left(), rule.get_right(), 0, predict);
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
            auto letter = rule.get_dot_symbol();
            auto search = automaton[set_number].next.find(letter);
            if (search == automaton[set_number].next.end()) {
                automaton[set_number].next[letter] = automaton.size();
                automaton.emplace_back(SetOfSituations());
                automaton.back().prev[letter].emplace(set_number);
            }
            size_t new_dot_pos = rule.get_dot_pos() + 1;
            auto new_situation = Situation(rule.get_left(), rule.get_right(), new_dot_pos, situation.predict);
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

void LR::init_table() {
    size_t automaton_size = alignment.size();
    table['#'].resize(automaton_size);
    for (auto& symbol : grammar.get_terminals()) {
        table[symbol].resize(automaton_size);
    }
    for (auto& symbol : grammar.get_non_terminals()) {
        table[symbol].resize(automaton_size);
    }
}

void LR::build_table() {
    for (int i = 0; i < automaton.size(); ++i) {
        auto vertex = automaton[i];
        if (vertex.state_number == -1) {
            continue;
        }
        int j = alignment[i];
        for (auto& to : vertex.next) {
            table[to.first][j] = Action('s', alignment[to.second]);
        }
        for (auto& situation : vertex.situations) {
            if (!situation.rule.is_dot_valid()) {
                if (situation.rule.get_left() == '$') {
                    for (auto& letter : situation.predict) {
                        table[letter][j] = Action('a');
                    }
                } else {
                    for (auto& letter : situation.predict) {
                        table[letter][j] = Action('r', situation.rule);
                    }
                }
            }
        }
    }
}


void LR::preprocessing() {
    init_first();
    build_automaton();
    init_table();
    build_table();
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

void LR::get_table(std::ostream &out) const {
    int cell_width = 11;
    bool is_start = true;
    for (auto& cell : table) {
        auto letter = cell.first;
        auto actions = cell.second;
        if (is_start) {
            out << std::setw(cell_width) << "states";
            for (int i = 0; i < actions.size(); ++i) {
                out << std::setw(cell_width) << i;
            }
            out << std::endl;
            is_start = false;
        }
        out << std::setw(cell_width) << letter;
        for (auto& action : actions) {
            out << std::setw(cell_width) << action.str();
        }
        out << "\n";
    }
}

std::istream &operator>>(std::istream &in, LR &lr) {
    in >> lr.grammar;
    lr.preprocessing();
    return in;
}

std::ostream &operator<<(std::ostream &out, const LR &lr) {
    lr.get_table(out);
    return out;
}

bool LR::parse(const std::string &word) {
    if (grammar.is_definitely_not_in_grammar(word)) {
        return false;
    }
    auto input = word + "#";
    std::stack<std::string> path;
    path.push("0");
    for (auto& letter : input) {
        bool is_letter_processed = false;
        while (!is_letter_processed) {
            auto state = std::stoi(path.top());
            auto action = table[letter][state];
            if (action.method == 'n') {
                return false;
            } else if (action.method == 'a') {
                break;
            } else if (action.method == 's') {
                path.push(std::string(1, letter));
                path.push(std::to_string(action.state));
                is_letter_processed = true;
            } else if (action.method == 'r') {
                int rule_size = action.rule.get_right().size();
                for (int i = 0; i < 2 * rule_size; ++i) {
                    path.pop();
                }
                state = std::stoi(path.top());
                path.push(std::string(1, action.rule.get_left()));
                action = table[action.rule.get_left()][state];
                path.push(std::to_string(action.state));
            }
        }
    }
    return true;
}
