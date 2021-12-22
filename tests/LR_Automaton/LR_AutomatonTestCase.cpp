#include "LR_AutomatonTestCase.h"


TEST_F(LR_AutomatonTestCase, CheckingClosure) {
    auto grammar = ContextFreeGrammar({"S->CC", "C->cC", "C->d"});
    mock_obj.set_grammar(grammar);
    mock_obj.add_situation_to_sets(0, "$->S", 0, "#");
    mock_obj.do_closure(0);

    std::stringstream output;
    output << mock_obj;
    std::string res = "V_0:[$->.S:#, C->.cC:cd, C->.d:cd, S->.CC:#]\n"
                      "FROM: \n"
                      "TO: \n";
    EXPECT_EQ(output.str(), res);
}