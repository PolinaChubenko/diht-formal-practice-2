#include "EarleyTestCase.h"
#include "Earley.h"


TEST_F(EarleyTestCase, TestingWords_1) {
    std::vector<std::string> rules = {"S->(S)S", "S->S(S)", "S-> "};
    Earley earley;
    earley.set_grammar(ContextFreeGrammar(rules));
    EXPECT_TRUE(earley.parse("()(()(()))"));
    EXPECT_FALSE(earley.parse(")("));
    EXPECT_FALSE(earley.parse(")a("));
}

TEST_F(EarleyTestCase, TestingWords_2) {
    std::vector<std::string> rules = {"S->S+S", "S->S*S", "S->(S)", "S->a"};
    auto earley = Earley(rules);
    EXPECT_TRUE(earley.parse("(a+a)*a"));
    EXPECT_TRUE(earley.parse("a+(a*a)"));
    EXPECT_TRUE(earley.parse("a*a*a"));
    EXPECT_FALSE(earley.parse("a*b"));
}

TEST_F(EarleyTestCase, TestingWords_3) {
    std::vector<std::string> rules = {"S->A+S", "S->A", "A->B*A", "A->B", "B->(S)", "B->a"};
    auto earley = Earley(rules);
    EXPECT_TRUE(earley.parse("((a+a)*a)"));
    EXPECT_TRUE(earley.parse("(a+a*a+a)"));
    EXPECT_FALSE(earley.parse("(a+a**a+a)"));
    EXPECT_FALSE(earley.parse("(a+(a*)a*a+a)"));
}

TEST_F(EarleyTestCase, TestingWords_4) {
    std::vector<std::string> rules = {"S->for_x_in_A:_S", "A->dogs", "A->cats", "S->print(B)", "B->0"};
    auto earley = Earley(rules);
    EXPECT_TRUE(earley.parse("for_x_in_dogs:_print(0)"));
}

TEST_F(EarleyTestCase, TestingWords_5) {
    std::vector<std::string> rules = {"S->for_X_in_A:_S", "X->x", "X->y", "A->dogs", "A->cats",
                                      "S->print(B)", "B->0", "B->1"};
    auto earley = Earley(rules);
    EXPECT_TRUE(earley.parse("for_x_in_dogs:_for_y_in_cats:_print(1)"));
    EXPECT_FALSE(earley.parse("for_x_in_dogs:_for_1_in_cats:_print(1)"));
}

TEST_F(EarleyTestCase, GettingFromStream) {
    auto earley = Earley(3);
    std::stringstream input;
    input << "S->(S)S\nS->S(S)\nS-> \n";
    input >> earley;
    EXPECT_TRUE(earley.parse("()(()(()))"));
    EXPECT_FALSE(earley.parse(")("));
    EXPECT_FALSE(earley.parse(")a("));
}

TEST_F(EarleyTestCase, GettingOutStream) {
    auto grammar = ContextFreeGrammar({"S->aS", "S-> "});
    auto earley = Earley(grammar);
    std::stringstream output;
    earley.parse("a");
    output << earley;
    std::string res = "D_0: [$->.S:0, $->S.:0, S->.:0, S->.aS:0]\n"
                      "D_1: [$->S.:0, S->.:1, S->.aS:1, S->a.S:0, S->aS.:0]\n";
    EXPECT_EQ(output.str(), res);
}

TEST_F(EarleyTestCase, CheckingScan) {
    auto grammar = ContextFreeGrammar({"S->SaSb", "S-> "});
    mock_obj.set_grammar(grammar);
    mock_obj.set_D_size(3); //ab
    mock_obj.add_situation_to_D(0, "$->S", 0, 0);
    mock_obj.add_situation_to_D(0, "$->S", 1, 0);
    mock_obj.add_situation_to_D(0, "S->", 0, 0);
    mock_obj.add_situation_to_D(0, "S->SaSb", 0, 0);
    mock_obj.add_situation_to_D(0, "S->SaSb", 1, 0); // for scan
    // D_0: [$->.S:0, $->S.:0, S->.:0, S->.SaSb:0, S->S.aSb:0]

    mock_obj.do_scan(1, "ab");

    std::stringstream output;
    output << mock_obj;
    std::string res = "D_0: [$->.S:0, $->S.:0, S->.:0, S->.SaSb:0, S->S.aSb:0]\n"
                      "D_1: [S->Sa.Sb:0]\n"
                      "D_2: []\n";
    EXPECT_EQ(output.str(), res);
}

TEST_F(EarleyTestCase, CheckingComplete) {
    auto grammar = ContextFreeGrammar({"S->SSb", "S->c"});
    mock_obj.set_grammar(grammar);
    mock_obj.set_D_size(4); //ccb
    mock_obj.add_situation_to_D(0, "$->S", 0, 0);
    mock_obj.add_situation_to_D(0, "S->SSb", 0, 0);
    mock_obj.add_situation_to_D(0, "S->c", 0, 0);
    mock_obj.add_situation_to_D(1, "S->c", 1, 0); // for complete
    // D_0: [$->.S:0, S->.SSb:0, S->.c:0]
    // D_1: [S->c.:0]

    mock_obj.do_complete(1);

    std::stringstream output;
    output << mock_obj;
    std::string res = "D_0: [$->.S:0, S->.SSb:0, S->.c:0]\n"
                      "D_1: [$->S.:0, S->S.Sb:0, S->c.:0]\n"
                      "D_2: []\n"
                      "D_3: []\n";
    EXPECT_EQ(output.str(), res);
}

TEST_F(EarleyTestCase, CheckingPredict) {
    auto grammar = ContextFreeGrammar({"S->aS", "S->b"});
    mock_obj.set_grammar(grammar);
    mock_obj.set_D_size(3); //ab
    mock_obj.add_situation_to_D(0, "$->S", 0, 0); // for predict
    // D_0: [$->.S:0]

    mock_obj.do_predict(0);

    std::stringstream output1;
    output1 << mock_obj;
    std::string res1 = "D_0: [$->.S:0, S->.aS:0, S->.b:0]\n"
                       "D_1: []\n"
                       "D_2: []\n";
    EXPECT_EQ(output1.str(), res1);

    mock_obj.add_situation_to_D(1, "S->aS", 1, 0); // for predict
    // D_0: [$->.S:0, S->.aS:0, S->.b:0]
    // D_1: [S->a.S:0]
    // D_2: []

    mock_obj.do_predict(1);

    std::stringstream output2;
    output2 << mock_obj;
    std::string res2 = "D_0: [$->.S:0, S->.aS:0, S->.b:0]\n"
                       "D_1: [S->.aS:1, S->a.S:0, S->.b:1]\n"
                       "D_2: []\n";
    EXPECT_EQ(output2.str(), res2);
}
