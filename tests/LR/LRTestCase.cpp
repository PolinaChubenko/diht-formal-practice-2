#include "LRTestCase.h"


TEST_F(LRTestCase, CheckingWord1) {
//    EXPECT_TRUE(1);
    auto grammar = ContextFreeGrammar({"S->CC", "C->cC", "C->d"});
    auto lr = LR(grammar);
    EXPECT_TRUE(lr.parse("cdd"));
    EXPECT_TRUE(lr.parse("cdcd"));
    EXPECT_FALSE(lr.parse("cddd"));
}