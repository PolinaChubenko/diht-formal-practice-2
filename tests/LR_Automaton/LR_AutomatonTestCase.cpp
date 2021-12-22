#include "LR_AutomatonTestCase.h"
#include <sstream>


TEST_F(LR_AutomatonTestCase, GettingFromStreamSetOfSituations) {
    auto grammar = ContextFreeGrammar({"S->C", "C->cC", "C->d"});
    mock_obj.set_grammar_only(grammar);

    mock_obj.add_situation_to_sets(0, "$->S", 0, {'#'});
    mock_obj.add_situation_to_sets(0, "S->C", 0, {'#'});
    mock_obj.add_situation_to_sets(0, "C->cC", 0, {'c', 'd'});
    mock_obj.add_situation_to_sets(0, "C->d", 0, {'c', 'd'});

    std::stringstream output;
    mock_obj.print_set(output, 0);
    std::string res = "[$->.S:#, C->.cC:cd, C->.d:cd, S->.C:#]";
    EXPECT_EQ(output.str(), res);
}

TEST_F(LR_AutomatonTestCase, CheckingFirst1) {
    auto grammar = ContextFreeGrammar({"S->CC", "C->cC", "C->d"});
    mock_obj.set_grammar_only(grammar);
    mock_obj.do_first();

    std::stringstream output;
    mock_obj.print_first(output);
    std::string res = "C: c d \n"
                      "S: c d \n"
                      "c: c \n"
                      "d: d \n";
    EXPECT_EQ(output.str(), res);
}

TEST_F(LR_AutomatonTestCase, CheckingFirst2) {
    auto grammar = ContextFreeGrammar({"S->CB", "C->cC", "C->d", "B->dC"});
    mock_obj.set_grammar_only(grammar);
    mock_obj.do_first();

    std::stringstream output;
    mock_obj.print_first(output);
    std::string res = "B: d \n"
                      "C: c d \n"
                      "S: c d \n"
                      "c: c \n"
                      "d: d \n";
    EXPECT_EQ(output.str(), res);
}

TEST_F(LR_AutomatonTestCase, CheckingClosure) {
    auto grammar = ContextFreeGrammar({"S->CC", "C->cC", "C->d"});
    mock_obj.set_grammar_only(grammar);
    mock_obj.do_first();
    mock_obj.add_situation_to_sets(0, "$->S", 0, {'#'});
    mock_obj.do_closure(0);

    std::stringstream output;
    output << mock_obj;
    std::string res = "V_0:[$->.S:#, C->.cC:cd, C->.d:cd, S->.CC:#]\n"
                      "FROM: \n"
                      "TO: \n";
    EXPECT_EQ(output.str(), res);
}

TEST_F(LR_AutomatonTestCase, CheckingGoTo) {
    auto grammar = ContextFreeGrammar({"S->CC", "C->cC", "C->d"});
    mock_obj.set_grammar_only(grammar);
    mock_obj.do_first();
    mock_obj.add_situation_to_sets(0, "$->S", 0, {'#'});
    mock_obj.do_closure(0);
    mock_obj.do_go_to(0);

    std::stringstream output;
    output << mock_obj;
    std::string res = "V_0:[$->.S:#, C->.cC:cd, C->.d:cd, S->.CC:#]\n"
                      "FROM: \n"
                      "TO: C-4 S-1 c-2 d-3 \n"
                      "V_1:[$->S.:#]\n"
                      "FROM: S-{0} \n"
                      "TO: \n"
                      "V_2:[C->c.C:cd]\n"
                      "FROM: c-{0} \n"
                      "TO: \n"
                      "V_3:[C->d.:cd]\n"
                      "FROM: d-{0} \n"
                      "TO: \n"
                      "V_4:[S->C.C:#]\n"
                      "FROM: C-{0} \n"
                      "TO: \n";
    EXPECT_EQ(output.str(), res);
}

TEST_F(LR_AutomatonTestCase, CheckingFindingEqualSets) {
    auto grammar = ContextFreeGrammar({"S->CC", "C->cC", "C->d"});
    mock_obj.set_grammar_only(grammar);
    mock_obj.do_first();
    mock_obj.add_situation_to_sets(0, "$->S", 0, {'#'});
    mock_obj.do_closure(0);
    mock_obj.do_go_to(0);
    mock_obj.do_closure(2);
    mock_obj.do_go_to(2);

    std::stringstream output;
    output << mock_obj;
    std::string res = "V_0:[$->.S:#, C->.cC:cd, C->.d:cd, S->.CC:#]\n"
                      "FROM: \n"
                      "TO: C-4 S-1 c-2 d-3 \n"
                      "V_1:[$->S.:#]\n"
                      "FROM: S-{0} \n"
                      "TO: \nV_2:[C->.cC:cd, C->c.C:cd, C->.d:cd]\n"
                      "FROM: c-{0} \n"
                      "TO: C-6 c-5 d-7 \n"
                      "V_3:[C->d.:cd]\n"
                      "FROM: d-{0} \n"
                      "TO: \n"
                      "V_4:[S->C.C:#]\n"
                      "FROM: C-{0} \n"
                      "TO: \n"
                      "V_5:[C->c.C:cd]\n"
                      "FROM: c-{2} \n"
                      "TO: \n"
                      "V_6:[C->cC.:cd]\n"
                      "FROM: C-{2} \n"
                      "TO: \n"
                      "V_7:[C->d.:cd]\n"
                      "FROM: d-{2} \n"
                      "TO: \n";
    EXPECT_EQ(output.str(), res);

    mock_obj.do_closure(5);
    mock_obj.do_closure(6);
    mock_obj.do_closure(7);

    EXPECT_EQ(mock_obj.do_find_equal_set(5), 2);
    EXPECT_EQ(mock_obj.do_find_equal_set(6), -1);
    EXPECT_EQ(mock_obj.do_find_equal_set(7), 3);
}

TEST_F(LR_AutomatonTestCase, CheckingBuildingWithCycle) {
    auto grammar = ContextFreeGrammar({"S->C", "C->cC", "C->d"});
    mock_obj.set_grammar(grammar);

    std::stringstream output;
    output << mock_obj;
    std::string res = "V_0:[$->.S:#, C->.cC:#, C->.d:#, S->.C:#]\n"
                      "FROM: \n"
                      "TO: C-4 S-1 c-2 d-3 \n"
                      "V_1:[$->S.:#]\n"
                      "FROM: S-{0} \n"
                      "TO: \n"
                      "V_2:[C->.cC:#, C->c.C:#, C->.d:#]\n"
                      "FROM: c-{0 2} \n"
                      "TO: C-5 c-2 d-3 \n"
                      "V_3:[C->d.:#]\n"
                      "FROM: d-{0 2} \n"
                      "TO: \n"
                      "V_4:[S->C.:#]\n"
                      "FROM: C-{0} \n"
                      "TO: \n"
                      "V_5:[C->cC.:#]\n"
                      "FROM: C-{2} \n"
                      "TO: \n";
    EXPECT_EQ(output.str(), res);
}

