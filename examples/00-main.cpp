#include "ContextFreeGrammar.h"
#include <iostream>


int main() {
    std::cout << "Enter the number of rules in your context-free grammar: ";
    size_t n;
    std::cin >> n;
    auto grammar = ContextFreeGrammar(n);
    std::cout << "Rules:\n";
    std::cin >> grammar;
    std::cout << "This is the rules of your grammar:\n";
    std::cout << grammar << std::endl;
    std::cout << "This is the alphabet of your grammar:\n";
    auto alphabet = grammar.get_alphabet();
    for (auto terminal : alphabet) {
        std::cout << terminal << " ";
    }
    std::cout << std::endl;
    return 0;
}


