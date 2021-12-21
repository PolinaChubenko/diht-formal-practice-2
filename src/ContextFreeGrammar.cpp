#include <ContextFreeGrammar.h>


//////////////////////      RULE      //////////////////////
Rule::Rule(const std::string &rule) {
    parse_rule(rule);
}

Rule::Rule(const std::string &rule, size_t dot_pos) : dot_pos(dot_pos) {
    parse_rule(rule);
}

Rule::Rule(char left, std::string right, size_t dot_pos) : left(left), right(std::move(right)), dot_pos(dot_pos) {}


void Rule::parse_rule(const std::string &rule) {
    auto found = rule.find("->");
    if (found != 1) {
        throw std::invalid_argument("There is an invalid rule");
    }
    left = rule[0];
    right = rule.substr(found + 2);
    if (right == " ") {
        right = "";
    }
}

bool operator==(const Rule &rule1, const Rule &rule2) {
    return rule1.left == rule2.left && rule1.right == rule2.right && rule1.dot_pos == rule2.dot_pos;
}

bool operator<(const Rule &rule1, const Rule &rule2) {
    if (rule1.left == rule2.left) {
        if (rule1.right == rule2.right) {
            return rule1.dot_pos < rule2.dot_pos;
        }
        return rule1.right < rule2.right;
    }
    return rule1.left < rule2.left;
}

std::istream &operator>>(std::istream &in, Rule &rule) {
    std::string str_rule;
    in >> str_rule;
    rule.parse_rule(str_rule);
    rule.dot_pos = 0;
    return in;
}

std::ostream &operator<<(std::ostream &out, const Rule &rule) {
    out << rule.left << "->";
    out << rule.right.substr(0, rule.dot_pos) << "." << rule.right.substr(rule.dot_pos);
    return out;
}

char Rule::get_left() const {
    return left;
}

std::string Rule::get_right() const {
    return right;
}

size_t Rule::get_dot_pos() const {
    return dot_pos;
}

char Rule::get_dot_symbol() const {
    return right[dot_pos];
}

bool Rule::is_dot_valid() const {
    return !(right.empty() || dot_pos >= right.size());
}

std::string Rule::str() const {
    std::string rule = " ->";
    rule[0] = left;
    rule.append(right);
    return rule;
}


//////////////////////      ContextFreeGrammar      //////////////////////
void ContextFreeGrammar::parse_alphabet() {
    for (const auto& rule : rules) {
        if (rule.get_left() == '$' || rule.get_left() == '#') {
            throw std::invalid_argument("Grammar has rules with forbidden symbols");
        }
        if (!std::isupper(rule.get_left())) {
            throw std::invalid_argument("Grammar has incorrect rule starting with terminal");
        }
        non_terminals.emplace(rule.get_left());
        auto terms = rule.get_right();
        for (const auto& symbol : terms) {
            if (symbol == '$' || symbol == '#') {
                throw std::invalid_argument("Grammar has rules with forbidden symbols");
            }
            if (!std::isupper(symbol)) {
                terminals.emplace(symbol);
            } else {
                non_terminals.emplace(symbol);
            }
        }
    }
}

ContextFreeGrammar::ContextFreeGrammar(size_t rules_amount) {
    set_rules_amount(rules_amount);
}

ContextFreeGrammar::ContextFreeGrammar(const std::vector<std::string> &vec_of_rules) {
    for (const auto& rule : vec_of_rules) {
        rules.emplace_back(Rule(rule));
    }
    parse_alphabet();
}

void ContextFreeGrammar::set_rules_amount(size_t rules_amount) {
    rules.resize(rules_amount);
}

std::istream &operator>>(std::istream &in, ContextFreeGrammar &grammar) {
    if (grammar.rules.empty()) {
        throw std::invalid_argument("Grammar has no rules");
    }
    for (auto & rule : grammar.rules) {
        in >> rule;
    }
    grammar.parse_alphabet();
    return in;
}

std::ostream &operator<<(std::ostream &out, const ContextFreeGrammar &grammar) {
    size_t rules_iter = 0;
    out << "[";
    for (auto& rule : grammar.rules) {
        out << rule;
        ++rules_iter;
        if (rules_iter != grammar.rules.size()) {
            out << ", ";
        }
    }
    out << "]";
    return out;
}

std::vector<Rule> ContextFreeGrammar::get_rules() const {
    return rules;
}

bool ContextFreeGrammar::is_definitely_not_in_grammar(const std::string &word) {
    for (const auto& letter : word) {
        if (terminals.find(letter) == terminals.end()) {
            return true;
        }
    }
    return false;
}

std::set<char> ContextFreeGrammar::get_terminals() {
    return terminals;
}

std::set<char> ContextFreeGrammar::get_non_terminals() {
    return non_terminals;
}
