#include <Early_algorithm.h>
#include <iostream>

int main() {
    int n;
    std::cin >> n;
    std::vector<std::string> rules(n);
    for (auto& rule : rules) {
        std::cin >> rule;
    }
    auto gr = ContextFreeGrammar(rules);
    std::string word;
    std::cin >> word;
    std::cout << (gr.EarlyCheck(word) ? "Word is in grammar" : "Word is not in grammar") << "\n";
    return 0;
}

//    std::vector<std::string> rules = {"S->(S)S", "S->S(S)", "S-> "};
//    std::string word = "(()(()))()";
//    std::vector<std::string> rules = {"S->aSbS", "S-> "};
//    std::string word = "abab";
