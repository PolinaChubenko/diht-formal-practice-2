#pragma once

#include "ContextFreeGrammar.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>


class Earley {
protected:
    struct Situation {
        Rule rule;
        size_t word_pos = 0;
    public:
        Situation() = default;
        Situation(Rule, size_t);
        Situation(char, std::string, size_t, size_t);
        Situation(const std::string&, size_t, size_t);
        explicit Situation(const std::string& rule);
    };
    struct Comparator {
        bool operator() (const Situation&, const Situation&) const;
    };
private:
    ContextFreeGrammar grammar;
    std::vector<std::set<Situation, Comparator>> situations; // aka D
protected:
    void scan(size_t, const std::string&);
    bool complete(size_t);
    bool predict(size_t);
    friend std::istream& operator >> (std::istream&, Rule&);
    friend std::ostream& operator << (std::ostream&, const Rule&);
public:
    explicit Earley(size_t);
    explicit Earley(ContextFreeGrammar);
    explicit Earley(const std::vector<std::string> &vec_of_rules);
    friend std::istream& operator >> (std::istream&, Earley&);
    friend std::ostream& operator << (std::ostream&, const Earley&);
    bool Parse(const std::string&);
};

