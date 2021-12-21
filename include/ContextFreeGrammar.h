#pragma once

#include <iostream>
#include <string>
#include <set>
#include <vector>


class Rule {
private:
    char left{};
    std::string right;
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

    [[nodiscard]] char get_left() const;
    [[nodiscard]] std::string get_right() const;
    [[nodiscard]] size_t get_dot_pos() const;
    [[nodiscard]] char get_dot_symbol() const;
    [[nodiscard]] bool is_dot_valid() const;
    [[nodiscard]] std::string str() const;
};



class ContextFreeGrammar {
private:
    std::set<char> terminals{};
    std::set<char> non_terminals{};
    std::vector<Rule> rules{};
protected:
    void parse_alphabet();
public:
    ContextFreeGrammar() = default;
    explicit ContextFreeGrammar(size_t);
    explicit ContextFreeGrammar(const std::vector<std::string>&);
    void set_rules_amount(size_t);
    friend std::istream &operator>>(std::istream&, ContextFreeGrammar&);
    friend std::ostream &operator<<(std::ostream&, const ContextFreeGrammar&);
    [[nodiscard]] std::vector<Rule> get_rules() const;
    bool is_definitely_not_in_grammar(const std::string&);
    [[nodiscard]] std::set<char> get_terminals();
    [[nodiscard]] std::set<char> get_non_terminals();
};

