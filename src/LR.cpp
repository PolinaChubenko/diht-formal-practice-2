#include "LR.h"


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


/////////////////////////      LR(1)      /////////////////////////

void LR::init_table() {
    size_t states_amount = automaton.alignment.size();
    table['#'].resize(states_amount);
    for (auto& symbol : grammar.get_terminals()) {
        table[symbol].resize(states_amount);
    }
    for (auto& symbol : grammar.get_non_terminals()) {
        table[symbol].resize(states_amount);
    }
}

void LR::build_table() {
    for (int i = 0; i < automaton.sets.size(); ++i) {
        auto vertex = automaton.sets[i];
        if (vertex.state_number == -1) {
            continue;
        }
        int j = automaton.alignment[i];
        for (auto& to : vertex.next) {
            table[to.first][j] = Action('s', automaton.alignment[to.second]);
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
    if (grammar.is_epsilon_generative()) {
        throw std::invalid_argument("Grammar contains epsilon generative non-terminals");
    }
    automaton.set_grammar(grammar);
    init_table();
    build_table();
}

LR::LR(size_t n) : grammar(n) {}

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
    out << automaton;
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
