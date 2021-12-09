#pragma once

#include <iostream>
#include <string>
#include <set>
#include <vector>

class ContextFreeGrammar {
protected:
    class Rule {
    public:
        char term{};
        std::string terms;
        int dot_pos = 0;
        int word_pos = 0;
    public:
        void parse_rule(std::string);
    public:
        Rule() = default;
        explicit Rule(const std::string&);
        Rule(char, const std::string&, int, int);
        Rule(const std::string&, int, int);
        bool operator==(const Rule&) const;
    };
    struct Comparator {
        bool operator() (const Rule&, const Rule&) const;
    };
private:
    std::vector<Rule> rules;
    std::vector<std::set<Rule, Comparator>> situations; // aka D
protected:
    void scan(int, const std::string&);
    bool complete(int, const std::string&);
    bool predict(int, const std::string&);
    friend std::istream& operator >> (std::istream&, Rule&);
    friend std::ostream& operator << (std::ostream&, const Rule&);
public:
    explicit ContextFreeGrammar(int);
    explicit ContextFreeGrammar(const std::vector<std::string>&);
    friend std::istream& operator >> (std::istream&, ContextFreeGrammar&);
    friend std::ostream& operator << (std::ostream&, const ContextFreeGrammar&);
    bool EarlyParse(const std::string&);
};
