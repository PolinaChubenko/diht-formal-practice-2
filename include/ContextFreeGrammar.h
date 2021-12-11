#pragma once

#include <iostream>
#include <string>
#include <set>
#include <vector>


class Rule {
private:
    char term{};
    std::string terms;
    size_t dot_pos = 0;
public:
    Rule() = default;
    explicit Rule(const std::string&);
    Rule(const std::string&, size_t);
    Rule(char, std::string , size_t);
    void parse_rule(const std::string&);
    friend bool operator==(const Rule &, const Rule &);
    friend bool operator<(const Rule &, const Rule &);
    friend std::istream &operator>>(std::istream &, Rule&);
    friend std::ostream &operator<<(std::ostream &, const Rule&);

    [[nodiscard]] char get_term() const;
    [[nodiscard]] std::string get_terms() const;
    [[nodiscard]] size_t get_dot_pos() const;
    [[nodiscard]] char get_dot_term() const;
    [[nodiscard]] bool is_dot_valid() const;
};



class ContextFreeGrammar {
private:
    std::set<char> alphabet;
    std::vector<Rule> rules;
protected:
    void parse_alphabet();
public:
    explicit ContextFreeGrammar(size_t);
    explicit ContextFreeGrammar(const std::vector<std::string>&);
    friend std::istream &operator>>(std::istream&, ContextFreeGrammar&);
    friend std::ostream &operator<<(std::ostream&, const ContextFreeGrammar&);
    [[nodiscard]] std::vector<Rule> get_rules() const;
    bool is_definitely_not_in_grammar(const std::string&);
    std::set<char> get_alphabet();
};

