#pragma once

#include "ContextFreeGrammar.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>



class LR {
protected:
    struct Situation {
        Rule rule;
        char predict{};
    public:
        Situation() = default;
        Situation(char, std::string, size_t, char);
        Situation(const std::string&, size_t, char);
        Situation(const std::string& rule, char);
    };
    struct SetOfSituations {
        std::set<Situation> situations{};
        std::map<char, std::set<size_t>> prev{};
        std::map<char, size_t> next{};
    public:
        SetOfSituations() = default;
    };
protected:
    ContextFreeGrammar grammar;
    std::vector<SetOfSituations> automaton;
protected:
    friend bool operator<(const LR::Situation &, const LR::Situation &);
    friend bool operator==(const LR::Situation &, const LR::Situation &);
    friend bool operator==(const SetOfSituations &, const SetOfSituations &);
    friend std::ostream& operator << (std::ostream&, const SetOfSituations&);
    void closure(size_t);
    void go_to(size_t);
    void build_automaton();
    size_t find_equal_set(size_t);
public:
    LR() = default;
    explicit LR(size_t);
    explicit LR(ContextFreeGrammar);
    explicit LR(const std::vector<std::string>&);
    void set_grammar(const ContextFreeGrammar&);
    void get_automaton(std::ostream&) const;
    void get_table(std::ostream&) const;
    friend std::istream& operator >> (std::istream&, LR&);
    friend std::ostream& operator << (std::ostream&, const LR&);
    bool parse(const std::string&);
};

