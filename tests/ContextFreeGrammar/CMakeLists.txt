set(TARGET_NAME "ContextFreeGrammar")
set(COVERAGE_NAME "00-coverage")
set(COVERAGE_TARGET "coverage-ContextFreeGrammar")
set(SOURCE_DIR_MAIN "${ContextFreeGrammar_SOURCE_DIR}")
set(SOURCES ${source-files})

message(INFO ${SOURCES})

set(TEST_SOURCES ContextFreeGrammarTestCase.h ContextFreeGrammarTestCase.cpp)
run_coverage(
        "${TEST_SOURCES}"
        "${SOURCES}"
        "${COVERAGE_TARGET}"
        "${TARGET_NAME}"
        "${COVERAGE_NAME}"
)
