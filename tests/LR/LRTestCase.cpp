#include "LRTestCase.h"


TEST_F(LRTestCase, CheckingWord1) {
    auto grammar = ContextFreeGrammar({"S->CC", "C->cC", "C->d"});
    auto lr = LR(grammar);
    EXPECT_TRUE(lr.parse("cdd"));
    EXPECT_TRUE(lr.parse("cdcd"));
    EXPECT_FALSE(lr.parse("cddd"));
}

TEST_F(LRTestCase, CheckingWord2) {
    auto grammar = ContextFreeGrammar({"S->aB", "B->bc", "B->b"});
    auto lr = LR(grammar);
    EXPECT_TRUE(lr.parse("abc"));
    EXPECT_TRUE(lr.parse("ab"));
    EXPECT_FALSE(lr.parse("aabb"));
}

TEST_F(LRTestCase, CheckingWord3) {
    auto grammar = ContextFreeGrammar({"S->C", "C->cC", "C->d"});
    auto lr = LR(grammar);
    EXPECT_TRUE(lr.parse("cd"));
    EXPECT_TRUE(lr.parse("ccd"));
    EXPECT_FALSE(lr.parse("cdcd"));
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