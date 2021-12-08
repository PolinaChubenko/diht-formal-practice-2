project(Early-library)

set(source-files "${PROJECT_SOURCE_DIR}/Early_algorithm.cpp" CACHE INTERNAL "")

add_library(
        Early-library SHARED ${source-files} ${mipt-formal-practice-2_SOURCE_DIR}/include/Early_algorithm.h
)
