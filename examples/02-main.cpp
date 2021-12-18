#include "ContextFreeGrammar.h"
#include <LR.h>
#include <iostream>


int main() {
    std::cout << "Enter the number of rules in your context-free grammar: ";
    size_t n;
    std::cin >> n;
    auto lr = LR(n);
    std::cin >> lr;
    std::cout << lr;
    return 0;
}
