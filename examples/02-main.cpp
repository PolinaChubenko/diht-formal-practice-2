#include "ContextFreeGrammar.h"
#include <LR.h>
#include <iostream>


int main() {
    std::cout << "Enter the number of rules in your context-free grammar: ";
    size_t n;
    std::cin >> n;
    auto grammar = ContextFreeGrammar(n);
    std::cout << "Rules:\n";
    std::cin >> grammar;
    auto lr = LR(grammar);

    std::cout << "Enter the number of words you want to check for belonging to your grammar: ";
    std::cin >> n;
    std::string word;
    for (int i = 0; i < n; ++i) {
        std::cout << "Word: ";
        std::cin >> word;
        std::cout << (lr.parse(word) ? "In grammar" : "Not in grammar") << std::endl;
    }
    return 0;
}
