set(TARGET_NAME "Earley")
set(COVERAGE_NAME "coverage")
set(COVERAGE_TARGET "coverage-Earley")
set(SOURCE_DIR_MAIN "${Earley_SOURCE_DIR}")
set(SOURCES ${source-files})

message(INFO ${SOURCES})

set(TEST_SOURCES EarleyTestCase.h EarleyTestCase.cpp EarleyMock.h EarleyMock.cpp)
run_coverage(
        "${TEST_SOURCES}"
        "${SOURCES}"
        "${COVERAGE_TARGET}"
        "${TARGET_NAME}"
        "${COVERAGE_NAME}"
)
