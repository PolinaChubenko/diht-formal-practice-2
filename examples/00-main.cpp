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
    std::cout << grammar;
    return 0;
}


