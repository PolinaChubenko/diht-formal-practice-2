#include <iostream>
#include <string>
#include <set>
#include <utility>
#include <vector>
#include <map>


class ContextFreeGrammar {
protected:
    class Rule {
    public:
        char term{};
        std::string terms;
        int dot_pos = 0;
        int i = 0;
    private:
        void parse_rule(std::string rule) {
            auto found = rule.find("->");
            if (found == std::string::npos)
                throw std::invalid_argument("This is not a rule");
            if (found == 1) {
                term = rule[0];
                terms = rule.substr(found + 2);
            } else {
                throw std::invalid_argument("This rule is invalid");
            }
        }
    public:
        explicit Rule(const std::string& rule) {
            parse_rule(rule);
        }
        Rule(char term, std::string terms, int dot_pos, int i) :
                term(term), terms(std::move(terms)), dot_pos(dot_pos), i(i) {}
        Rule(const std::string& rule, int dot_pos, int i) : dot_pos(dot_pos), i(i) {
            parse_rule(rule);
        }
        bool operator==(const Rule& rule) const {
            return term == rule.term && terms == rule.terms && i == rule.i && dot_pos == rule.dot_pos;
        }
        friend std::ostream& operator << (std::ostream& out, const Rule& rule) {
            out << '[' << rule.term << "->";
            out << rule.terms.substr(0, rule.dot_pos) << "." << rule.terms.substr(rule.dot_pos);
            out << ':' << rule.i << "]\n";
            return out;
        }
    };
    struct Comp {
        bool operator() (const Rule& rule1, const Rule& rule2) const {
            if (rule1.term == rule2.term) {
                return rule1.terms < rule2.terms;
            }
            return rule1.term < rule2.term;
        }
    };
private:
    std::vector<Rule> rules;
    std::vector<std::set<Rule, Comp>> situations; // aka D
private:
    friend std::ostream& operator << (std::ostream& out, const ContextFreeGrammar& grammar) {
        int iter = 0;
        for (auto& set : grammar.situations) {
            out << "iter:" << iter << "\n";
            for (auto& rule : set) {
                out << rule;
            }
            iter++;
        }
        return out;
    }
    void scan(int j, const std::string& word) {
        if (j == 0) {
            return;
        }
        for (const auto& rule : situations[j - 1]) {
            // A -> P.aQ, i
            if (rule.dot_pos >= rule.terms.size()) {
                continue;
            }
            if (rule.terms[rule.dot_pos] == word[j - 1]) {
                // A -> Pa.Q, i
                auto new_rule = Rule(rule.term, rule.terms, rule.dot_pos + 1, rule.i);
                situations[j].insert(new_rule);
            }
        }
    }
    bool complete(int j, const std::string& word) {
        bool is_situations_changed = false;
        for (const auto& rule1 : situations[j]) {
            // B -> p., i
            if (rule1.dot_pos != rule1.terms.size()) {
                continue;
            }
            for (const auto& rule2 : situations[rule1.i]) {
                // A -> q.Br, j
                if (rule2.terms[rule2.dot_pos] != rule1.term) {
                    continue;
                }
                // A -> qB.r, j
                auto new_rule = Rule(rule2.term, rule2.terms, rule2.dot_pos + 1, rule2.i);
                auto result = situations[j].insert(new_rule);
                if (result.second) {
                    is_situations_changed = true;
                }
            }
        }
        return is_situations_changed;
    }
    bool predict(int j, const std::string& word) {
        bool is_situations_changed = false;
        for (const auto& rule1 : situations[j]) {
            // A -> p.Bq, i
            for (const auto& rule2 : rules) {
                // B -> r
                if (rule1.dot_pos >= rule1.terms.size()) {
                    continue;
                }
                if (rule1.terms[rule1.dot_pos] != rule2.term) {
                    continue;
                }
                // B -> .r, j
                auto new_rule = Rule(rule2.term, rule2.terms, 0, j);
                auto result = situations[j].insert(new_rule);
                if (result.second) {
                    is_situations_changed = true;
                }
            }
        }
        return is_situations_changed;
    }
public:
    explicit ContextFreeGrammar(const std::vector<std::string>& list_of_rules) {
        for (const auto& rule : list_of_rules) {
            rules.emplace_back(Rule(rule));
        }
    }
    bool EarlyCheck(const std::string& word) {
//        for (auto& rule : rules) {
//            std::cout << rule;
//        }
//        std::cout << "Early start\n\n";

        situations.resize(word.size() + 1);
        situations[0].insert(Rule("$->S"));
        for (int j = 0; j < word.size() + 1; ++j) {
            //std::cout << "SCAN:\n";
            scan(j, word);
            //std::cout << *this << std::endl;
            bool is_situations_changed = true;
            while(is_situations_changed) {
                //std::cout << "COMPLETE\n";
                bool is_change_in_complete = complete(j, word);
                //std::cout << *this << std::endl;
                //std::cout << "PREDICT\n";
                bool is_change_in_predict = predict(j, word);
                //std::cout << *this << std::endl;
                is_situations_changed = is_change_in_complete || is_change_in_predict;
            }
        }
        auto final_rule = Rule("$->S", 1, 0);
        const bool is_word_in_grammar = situations[word.size()].find(final_rule) != situations[word.size()].end();
        return is_word_in_grammar;
    }
};

int main() {
//    int n;
//    std::cin >> n;
//    std::vector<std::string> rules(n);
//    for (auto& rule : rules) {
//        std::cin >> rule;
//    }
    std::vector<std::string> rules = {"S->aSbS", "S->c"};
    std::string word = "acbc";
    auto gr = ContextFreeGrammar(rules);
//    std::string word;
//    std::cin >> word;
    std::cout << (gr.EarlyCheck(word) ? "Yes\n" : "No\n");
    return 0;
}
