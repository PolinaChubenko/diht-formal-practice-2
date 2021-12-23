set(TARGET_NAME "LR_Automaton")
set(COVERAGE_NAME "02-coverage")
set(COVERAGE_TARGET "coverage-LR_Automaton")
set(SOURCE_DIR_MAIN "${LR_Automaton_SOURCE_DIR}")
set(SOURCES ${source-files})

message(INFO ${SOURCES})

set(TEST_SOURCES LR_AutomatonTestCase.h LR_AutomatonTestCase.cpp LR_AutomatonMock.h LR_AutomatonMock.cpp)
run_coverage(
        "${TEST_SOURCES}"
        "${SOURCES}"
        "${COVERAGE_TARGET}"
        "${TARGET_NAME}"
        "${COVERAGE_NAME}"
)
