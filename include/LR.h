#pragma once

#include "ContextFreeGrammar.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <iomanip>
#include <stack>



class LR {
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
    struct Action {
        char method = 'n';
        int state = -1;
        Rule rule{};
    public:
        Action() = default;
        explicit Action(char);
        Action(char, int);
        Action(char, Rule);
    };
protected:
    ContextFreeGrammar grammar;
    std::map<char, std::set<char>> first;
    std::vector<SetOfSituations> automaton;
    std::map<int, int> alignment;
    std::map<char, std::vector<Action>> table;
protected:
    friend bool operator<(const LR::Situation &, const LR::Situation &);
    friend bool operator==(const LR::Situation &, const LR::Situation &);
    friend bool operator==(const SetOfSituations &, const SetOfSituations &);
    friend std::ostream& operator << (std::ostream&, const SetOfSituations&);
    void init_first();
    std::set<char> get_first(const Situation&);
    void closure(size_t);
    void go_to(size_t);
    void build_automaton();
    size_t find_equal_set(size_t);
    void init_table();
    void build_table();
public:
    void preprocessing();
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

