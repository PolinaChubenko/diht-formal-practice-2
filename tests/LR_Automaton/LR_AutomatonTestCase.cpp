#include "LR_AutomatonTestCase.h"
#include <sstream>


TEST_F(LR_AutomatonTestCase, CheckingClosure) {
    auto grammar = ContextFreeGrammar({"S->CC", "C->cC", "C->d"});
    mock_obj.set_grammar(grammar);
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
    mock_obj.set_grammar(grammar);
    mock_obj.do_first();
    mock_obj.add_situation_to_sets(0, "$->S", 0, {'#'});
    mock_obj.do_closure(0);
    mock_obj.do_go_to(0);
//
//    std::stringstream output;
//    output << mock_obj;
//    std::string res = "V_0:[$->.S:#, C->.cC:cd, C->.d:cd, S->.CC:#]\n"
//                      "FROM: \n"
//                      "TO: C-4 S-1 c-2 d-3 \n"
//                      "V_1:[$->S.:#]\n"
//                      "FROM: S-{0} \n"
//                      "TO: \n"
//                      "V_2:[C->.cC:cd, C->c.C:cd, C->.d:cd]\n"
//                      "FROM: c-{0 2} \n"
//                      "TO: \n"
//                      "V_3:[C->d.:cd]\n"
//                      "FROM: d-{0 2} \n"
//                      "TO: \n"
//                      "V_4:[C->.cC:#, C->.d:#, S->C.C:#]\n"
//                      "FROM: C-{0} \n"
//                      "TO: \n ";
//    EXPECT_EQ(output.str(), res);
}