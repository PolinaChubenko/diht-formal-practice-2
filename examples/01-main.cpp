#include "ContextFreeGrammar.h"
#include <Earley.h>
#include <iostream>


int main() {
    std::cout << "Enter the number of rules in your context-free grammar: ";
    size_t n;
    std::cin >> n;
    auto grammar = ContextFreeGrammar(n);
    std::cout << "Rules:\n";
    std::cin >> grammar;
    auto earley = Earley(grammar);

    std::cout << "Enter the number of words you want to check for belonging to your grammar: ";
    std::cin >> n;
    std::string word;
    for (int i = 0; i < n; ++i) {
        std::cout << "Word: ";
        std::cin >> word;
        std::cout << (earley.Parse(word) ? "In grammar" : "Not in grammar") << std::endl;
    }
    return 0;
}

//    std::vector<std::string> rules = {"S->(S)S", "S->S(S)", "S-> "};
//    std::string word = "(()(()))()";
//    std::vector<std::string> rules = {"S->aSbS", "S-> "};
//    std::string word = "abab";
