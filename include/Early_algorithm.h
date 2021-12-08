#pragma once

#include <iostream>
#include <string>
#include <set>
#include <utility>
#include <vector>

class ContextFreeGrammar {
protected:
    class Rule {
    public:
        char term{};
        std::string terms;
        int dot_pos = 0;
        int word_pos = 0;
    private:
        void parse_rule(std::string);
    public:
        explicit Rule(const std::string&);
        Rule(char term, std::string terms, int dot_pos, int word_pos) :
                term(term), terms(std::move(terms)), dot_pos(dot_pos), word_pos(word_pos) {}
        Rule(const std::string&, int, int);
        bool operator==(const Rule&) const;
        friend std::ostream& operator << (std::ostream&, const Rule&);
    };
    struct Comparator {
        bool operator() (const Rule&, const Rule&) const;
    };
private:
    std::vector<Rule> rules;
    std::vector<std::set<Rule, Comparator>> situations; // aka D
private:
    void scan(int, const std::string&);
    bool complete(int, const std::string&);
    bool predict(int, const std::string&);
    friend std::ostream &operator<<(std::ostream &, const Rule&);
public:
    explicit ContextFreeGrammar(const std::vector<std::string>&);
    friend std::ostream& operator << (std::ostream&, const ContextFreeGrammar&);
    bool EarlyCheck(const std::string&);
};
