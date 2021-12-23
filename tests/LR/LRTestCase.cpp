#include "LRTestCase.h"


TEST_F(LRTestCase, CheckingWord1) {
    auto grammar = ContextFreeGrammar({"S->CC", "C->cC", "C->d"});
    auto lr = LR(grammar);
    EXPECT_TRUE(lr.parse("cdd"));
    EXPECT_TRUE(lr.parse("cdcd"));
    EXPECT_FALSE(lr.parse("cddd"));
    EXPECT_FALSE(lr.parse("(A)A"));
}

TEST_F(LRTestCase, CheckingWord2) {
    std::vector<std::string> rules = {"S->aB", "B->bc", "B->b"};
    auto lr = LR(rules);
    EXPECT_TRUE(lr.parse("abc"));
    EXPECT_TRUE(lr.parse("ab"));
    EXPECT_FALSE(lr.parse("aabb"));
}

TEST_F(LRTestCase, CheckingWord3) {
    auto grammar = ContextFreeGrammar({"S->C", "C->cC", "C->d"});
    mock_obj.set_grammar(grammar);
    EXPECT_TRUE(mock_obj.parse("cd"));
    EXPECT_TRUE(mock_obj.parse("ccd"));
    EXPECT_FALSE(mock_obj.parse("cdcd"));
}

TEST_F(LRTestCase, ExceptionThrow) {
    auto grammar1 = ContextFreeGrammar({"S->(S)S", "S->S(S)", "S-> "});
    auto grammar2 = ContextFreeGrammar({"S->aB", "B->bc"});

    LR lr;
    EXPECT_THROW(lr.set_grammar(grammar1), std::invalid_argument);
    EXPECT_NO_THROW(lr.set_grammar(grammar2));
}

TEST_F(LRTestCase, GettingFromStream) {
    auto lr = LR(3);
    std::stringstream input;
    input << "S->CC\nC->cC\nC->d\n";
    input >> lr;
    EXPECT_TRUE(lr.parse("cdcd"));
    EXPECT_FALSE(lr.parse("cdcdcd"));
    EXPECT_FALSE(lr.parse("ccc"));
}

TEST_F(LRTestCase, GettingTableFromStream1) {
    auto grammar = ContextFreeGrammar({"S->C", "C->cC", "C->d"});
    auto lr = LR(grammar);

    std::stringstream output;
    output << lr;
    std::string res = "     states          0          1          2          3          4          5\n"
                      "          #                     a                r:C->d     r:S->C    r:C->cC\n"
                      "          C        s:4                   s:5                                 \n"
                      "          S        s:1                                                       \n"
                      "          c        s:2                   s:2                                 \n"
                      "          d        s:3                   s:3                                 \n";
    EXPECT_EQ(output.str(), res);
}

TEST_F(LRTestCase, GettingTableFromStream2) {
    auto grammar = ContextFreeGrammar({"S->CC", "C->cC", "C->d"});
    auto lr = LR(grammar);

    std::stringstream output;
    output << lr;
    std::string res = "     states          0          1          2          3          4          5          6          7          8          9\n"
                      "          #                     a                                                            r:C->d    r:S->CC    r:C->cC\n"
                      "          C        s:4                   s:5                   s:8                   s:9                                 \n"
                      "          S        s:1                                                                                                   \n"
                      "          c        s:2                   s:2     r:C->d        s:6    r:C->cC        s:6                                 \n"
                      "          d        s:3                   s:3     r:C->d        s:7    r:C->cC        s:7                                 \n";
    EXPECT_EQ(output.str(), res);
}

TEST_F(LRTestCase, GettingEvrythingFromStream) {
    auto grammar = ContextFreeGrammar({"S->aB", "B->b", "B->bc"});
    auto lr = LR(grammar);

    std::stringstream output1;
    output1 << lr;
    std::string res = "     states          0          1          2          3          4          5\n"
                      "          #                     a                r:B->b    r:S->aB    r:B->bc\n"
                      "          B                              s:4                                 \n"
                      "          S        s:1                                                       \n"
                      "          a        s:2                                                       \n"
                      "          b                              s:3                                 \n"
                      "          c                                         s:5                      \n";
    EXPECT_EQ(output1.str(), res);

    std::stringstream output2;
    lr.get_automaton(output2);
    res = "V_0:[$->.S:#, S->.aB:#]\n"
          "FROM: \n"
          "TO: S-1 a-2 \n"
          "V_1:[$->S.:#]\n"
          "FROM: S-{0} \n"
          "TO: \n"
          "V_2:[B->.b:#, B->.bc:#, S->a.B:#]\n"
          "FROM: a-{0} \n"
          "TO: B-4 b-3 \n"
          "V_3:[B->b.:#, B->b.c:#]\n"
          "FROM: b-{2} \n"
          "TO: c-5 \n"
          "V_4:[S->aB.:#]\n"
          "FROM: B-{2} \n"
          "TO: \n"
          "V_5:[B->bc.:#]\n"
          "FROM: c-{3} \n"
          "TO: \n";
    EXPECT_EQ(output2.str(), res);
}

TEST_F(LRTestCase, CheckingTableInit) {
    auto grammar = ContextFreeGrammar({"S->CC", "C->cC", "C->d"});
    mock_obj.set_automaton_grammar(grammar);
    mock_obj.do_init_table();
    auto table = mock_obj.get_table();

    EXPECT_EQ(table.size(), 5);
    EXPECT_NO_THROW(table.at('#'));
    EXPECT_NO_THROW(table.at('S'));
    EXPECT_NO_THROW(table.at('C'));
    EXPECT_NO_THROW(table.at('c'));
    EXPECT_NO_THROW(table.at('d'));
    EXPECT_THROW(table.at('b'), std::out_of_range);
    EXPECT_THROW(table.at('$'), std::out_of_range);

    for (auto& line : table) {
        auto actions = line.second;
        EXPECT_EQ(actions.size(), 10);
    }

}

TEST_F(LRTestCase, CheckingTable) {
    auto grammar = ContextFreeGrammar({"S->CC", "C->cC", "C->d"});
    mock_obj.set_automaton_grammar(grammar);
    mock_obj.do_init_table();
    mock_obj.do_build_table();
    auto table = mock_obj.get_table();

    EXPECT_EQ(table.size(), 5);
    for (auto& line : table) {
        auto actions = line.second;
        EXPECT_EQ(actions.size(), 10);
    }

    auto actions = table.at('c');
    EXPECT_EQ(actions[0].str(), "s:2");
    EXPECT_EQ(actions[3].str(), "r:C->d");
    EXPECT_EQ(actions[2], mock_obj.get_action('s', 2));

    actions = table.at('#');
    EXPECT_EQ(actions[1].str(), "a");
    EXPECT_EQ(actions[7].str(), "r:C->d");
    EXPECT_EQ(actions[8], mock_obj.get_action('r', -1, "S->CC"));
    EXPECT_EQ(actions[9], mock_obj.get_action('r', -1, "C->cC"));
}