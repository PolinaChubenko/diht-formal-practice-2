set(TARGET_NAME "LR")
set(COVERAGE_NAME "03-coverage")
set(COVERAGE_TARGET "coverage-LR")
set(SOURCE_DIR_MAIN "${LR_SOURCE_DIR}")
set(SOURCES ${source-files})

message(INFO ${SOURCES})

set(TEST_SOURCES LRTestCase.h LRTestCase.cpp LRMock.h LRMock.cpp)
run_coverage(
        "${TEST_SOURCES}"
        "${SOURCES}"
        "${COVERAGE_TARGET}"
        "${TARGET_NAME}"
        "${COVERAGE_NAME}"
)
