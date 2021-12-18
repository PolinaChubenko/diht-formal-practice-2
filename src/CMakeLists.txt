project(Parsers-library)

set(header-files
        "${mipt-formal-practice-2_SOURCE_DIR}/include/ContextFreeGrammar.h"
        "${mipt-formal-practice-2_SOURCE_DIR}/include/Earley.h"
        "${mipt-formal-practice-2_SOURCE_DIR}/include/LR.h")
set(source-files
        "${PROJECT_SOURCE_DIR}/ContextFreeGrammar.cpp"
        "${PROJECT_SOURCE_DIR}/Earley.cpp"
        "${PROJECT_SOURCE_DIR}/LR.cpp"
        CACHE INTERNAL "")


add_library(
        Parsers-library SHARED ${source-files} ${header-files}
)