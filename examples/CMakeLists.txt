add_executable(ContextFreeGrammar-bin 00-main.cpp)
target_link_libraries(ContextFreeGrammar-bin Parsers-library)

add_executable(Earley-bin 01-main.cpp)
target_link_libraries(Earley-bin Parsers-library)
