set(TARGET_NAME "Early-library")
set(COVERAGE_NAME "coverage")
set(COVERAGE_TARGET "coverage-Early-library")
set(SOURCE_DIR_MAIN "${Early-library_SOURCE_DIR}")
set(SOURCES ${source-files})

message(INFO ${SOURCES})

set(TEST_SOURCES EarlyTestCase.h EarlyTestCase.cpp)
run_coverage(
        "${TEST_SOURCES}"
        "${SOURCES}"
        "${COVERAGE_TARGET}"
        "${TARGET_NAME}"
        "${COVERAGE_NAME}"
)
