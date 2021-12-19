#include <ContextFreeGrammar.h>


//////////////////////      RULE      //////////////////////
Rule::Rule(const std::string &rule) {
    parse_rule(rule);
}

Rule::Rule(const std::string &rule, size_t dot_pos) : dot_pos(dot_pos) {
    parse_rule(rule);
}

Rule::Rule(char term, std::string terms, size_t dot_pos) : term(term), terms(std::move(terms)), dot_pos(dot_pos) {}


void Rule::parse_rule(const std::string &rule) {
    auto found = rule.find("->");
    if (found != 1) {
        throw std::invalid_argument("There is an invalid rule");
    }
    term = rule[0];
    terms = rule.substr(found + 2);
    if (terms == " ") {
        terms = "";
    }
}

bool operator==(const Rule &rule1, const Rule &rule2) {
    return rule1.term == rule2.term && rule1.terms == rule2.terms && rule1.dot_pos == rule2.dot_pos;
}

bool operator<(const Rule &rule1, const Rule &rule2) {
    if (rule1.term == rule2.term) {
        if (rule1.terms == rule2.terms) {
            return rule1.dot_pos < rule2.dot_pos;
        }
        return rule1.terms < rule2.terms;
    }
    return rule1.term < rule2.term;
}

std::istream &operator>>(std::istream &in, Rule &rule) {
    std::string str_rule;
    in >> str_rule;
    rule.parse_rule(str_rule);
    rule.dot_pos = 0;
    return in;
}

std::ostream &operator<<(std::ostream &out, const Rule &rule) {
    out << rule.term << "->";
    out << rule.terms.substr(0, rule.dot_pos) << "." << rule.terms.substr(rule.dot_pos);
    return out;
}

char Rule::get_term() const {
    return term;
}

std::string Rule::get_terms() const {
    return terms;
}

size_t Rule::get_dot_pos() const {
    return dot_pos;
}

char Rule::get_dot_term() const {
    return terms[dot_pos];
}

bool Rule::is_dot_valid() const {
    return !(terms.empty() || dot_pos >= terms.size());
}




//////////////////////      ContextFreeGrammar      //////////////////////
void ContextFreeGrammar::parse_alphabet() {
    for (const auto& rule : rules) {
        if (rule.get_term() == '$' || rule.get_term() == '#') {
            throw std::invalid_argument("Grammar has rules with forbidden symbols");
        }
        if (!std::isupper(rule.get_term())) {
            throw std::invalid_argument("Grammar has incorrect rule starting with terminal");
        }
        auto terms = rule.get_terms();
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
