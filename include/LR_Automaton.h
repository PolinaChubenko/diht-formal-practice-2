#pragma once

#include "ContextFreeGrammar.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <iomanip>
#include <stack>


class LR_Automaton {
protected:
    struct Situation {
        Rule rule;
        std::set<char> predict;
    public:
        Situation() = default;
        Situation(char, std::string, size_t, std::set<char>);
        Situation(const std::string&, size_t, std::set<char>);
        Situation(const std::string& rule, char);
    };
    struct SetOfSituations {
        std::set<Situation> situations{};
        std::map<char, std::set<size_t>> prev{};
        std::map<char, size_t> next{};
        int state_number = -1;
    public:
        SetOfSituations() = default;
        explicit SetOfSituations(int);
    };
protected:
    ContextFreeGrammar grammar;
    std::map<char, std::set<char>> first;
    std::vector<SetOfSituations> sets;
    std::map<int, int> alignment;
protected:
    friend bool operator<(const Situation &, const Situation &);
    friend bool operator==(const Situation &, const Situation &);
    friend bool operator==(const SetOfSituations &, const SetOfSituations &);
    friend std::ostream& operator << (std::ostream&, const SetOfSituations&);
    void init_first();
    std::set<char> get_first(const Situation&);
    void closure(size_t);
    void go_to(size_t);
    void build_automaton();
    size_t find_equal_set(size_t);
    friend class LR;
public:
    LR_Automaton() = default;
    void set_grammar(const ContextFreeGrammar&);
    friend std::ostream& operator << (std::ostream&, const LR_Automaton&);
};

