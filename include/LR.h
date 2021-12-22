#pragma once

#include "ContextFreeGrammar.h"
#include "LR_Automaton.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <iomanip>
#include <stack>



class LR {
protected:
    struct Action {
        char method = 'n';
        int state = -1;
        Rule rule{};
    public:
        Action() = default;
        explicit Action(char);
        Action(char, int);
        Action(char, Rule);
        [[nodiscard]] std::string str() const;
    };
protected:
    ContextFreeGrammar grammar;
    LR_Automaton automaton;
    std::map<char, std::vector<Action>> table;
protected:
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

