#include <Early_algorithm.h>
#include <iostream>


int main() {
    std::cout << "Enter the number of rules in your context-free Grammar: ";
    int n;
    std::cin >> n;
    auto grammar = ContextFreeGrammar(n);
    std::cout << "Rules:\n";
    std::cin >> grammar;

    std::cout << "Enter the number of words you want to check for belonging to grammar: ";
    std::cin >> n;
    std::string word;
    for (int i = 0; i < n; ++i) {
        std::cout << "Word: ";
        std::cin >> word;
        std::cout << (grammar.EarlyParse(word) ? "In grammar" : "Not in grammar") << std::endl;
    }
    return 0;
}

//    std::vector<std::string> rules = {"S->(S)S", "S->S(S)", "S-> "};
//    std::string word = "(()(()))()";
//    std::vector<std::string> rules = {"S->aSbS", "S-> "};
//    std::string word = "abab";
