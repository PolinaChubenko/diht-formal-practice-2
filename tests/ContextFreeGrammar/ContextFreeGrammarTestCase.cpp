#include "ContextFreeGrammarTestCase.h"
#include "ContextFreeGrammar.h"
#include <sstream>


TEST_F(ContextFreeGrammarTestCase, RulesBasic) {
    auto rule1 = Rule("S->aSbS");
    auto rule2 = Rule('S', "aSbS", 0);
    auto rule3 = Rule("S->aSbS", 0);
    EXPECT_EQ(rule1, rule2);
    EXPECT_EQ(rule2, rule3);
}

TEST_F(ContextFreeGrammarTestCase, ParseRuleExceptions) {
    auto rule = Rule();
    EXPECT_THROW(rule.parse_rule("S-aSbS"), std::invalid_argument);
    EXPECT_THROW(rule.parse_rule("Sa->SbS"), std::invalid_argument);
    EXPECT_THROW(rule.parse_rule("SaSbS"), std::invalid_argument);
    EXPECT_NO_THROW(rule.parse_rule("S->aSbS"));
    EXPECT_NO_THROW(rule.parse_rule("S-> "));
}

TEST_F(ContextFreeGrammarTestCase, GetMethods) {
    auto rule = Rule('S', "(S)S", 1);
    EXPECT_EQ(rule.get_term(), 'S');
    EXPECT_EQ(rule.get_terms(), "(S)S");
    EXPECT_EQ(rule.get_dot_pos(), 1);
    EXPECT_EQ(rule.get_dot_term(), 'S');
}

TEST_F(ContextFreeGrammarTestCase, DotMethod) {
    auto rule = Rule('S', "(S)S", 1);
    EXPECT_TRUE(rule.is_dot_valid());

    rule = Rule('S', "(S)S", 4);
    EXPECT_FALSE(rule.is_dot_valid());

    rule = Rule('S', "", 0);
    EXPECT_FALSE(rule.is_dot_valid());
}

TEST_F(ContextFreeGrammarTestCase, LessOperator) {
    auto rule1 = Rule("S->aS", 0);
    auto rule2 = Rule("S->aS", 1);
    auto rule3 = Rule("U->b", 1);
    auto rule4 = Rule("U->bc", 1);

    EXPECT_LT(rule1, rule2);
    EXPECT_LT(rule2, rule3);
    EXPECT_LT(rule3, rule4);
}

TEST_F(ContextFreeGrammarTestCase, GettingRuleFromStream) {
    std::stringstream input;
    input << "S->aSSb\n";

    Rule rule;
    input >> rule;
    EXPECT_EQ(rule.get_term(), 'S');
    EXPECT_EQ(rule.get_terms(), "aSSb");
    EXPECT_EQ(rule.get_dot_pos(), 0);

    std::stringstream output;
    output << rule;
    EXPECT_EQ(output.str(), "S->.aSSb");
}



TEST_F(ContextFreeGrammarTestCase, GrammarBasic) {
    std::vector<std::string> rules = {"S->(S)S", "S->S(S)", "S-> "};
    std::vector<Rule> vec_rules = {Rule("S->(S)S"), Rule("S->S(S)"), Rule("S-> ")};
    std::set<char> alphabet = {'(', ')'};
    auto grammar = ContextFreeGrammar(rules);
    EXPECT_EQ(grammar.get_rules(), vec_rules);
    EXPECT_EQ(grammar.get_alphabet(), alphabet);
}

TEST_F(ContextFreeGrammarTestCase, AlphabetExceptions) {
    std::vector<std::string> rules = {"$->aSbS", "S->c"};
    EXPECT_THROW(auto rule = ContextFreeGrammar(rules), std::invalid_argument);

    rules = {"S->a#bS", "S->c"};
    EXPECT_THROW(auto rule = ContextFreeGrammar(rules), std::invalid_argument);

    rules = {"S->a$bS", "S->c"};
    EXPECT_THROW(auto rule = ContextFreeGrammar(rules), std::invalid_argument);

    rules = {"#->aSbS", "S->c"};
    EXPECT_THROW(auto rule = ContextFreeGrammar(rules), std::invalid_argument);

    rules = {"S->aSbS", "S-> ", "S->c"};
    EXPECT_NO_THROW(auto rule = ContextFreeGrammar(rules));
}

TEST_F(ContextFreeGrammarTestCase, GettingGrammarFromStream) {
    std::stringstream input;
    input << "S->aSbS\nS-> \n";
    std::vector<Rule> rules = {Rule("S->aSbS"), Rule("S-> ")};
    std::set<char> alphabet = {'a', 'b'};

    auto grammar = ContextFreeGrammar(2);
    input >> grammar;
    EXPECT_EQ(grammar.get_rules(), rules);
    EXPECT_EQ(grammar.get_alphabet(), alphabet);

    std::stringstream output;
    output << grammar;
    EXPECT_EQ(output.str(), "[S->.aSbS, S->.]");
}

TEST_F(ContextFreeGrammarTestCase, GettingGrammarFromStreamException) {
    std::stringstream input;
    input << "S->aSbS\nS-> \n";
    auto grammar = ContextFreeGrammar({});
    EXPECT_THROW(input >> grammar, std::invalid_argument);
}

TEST_F(ContextFreeGrammarTestCase, WordInGrammarPrediction) {
    std::stringstream input;
    input << "S->aSbS\nS-> \n";
    auto grammar = ContextFreeGrammar(2);
    input >> grammar;
    EXPECT_TRUE(grammar.is_definitely_not_in_grammar("abc"));
    EXPECT_FALSE(grammar.is_definitely_not_in_grammar("abab"));
}
