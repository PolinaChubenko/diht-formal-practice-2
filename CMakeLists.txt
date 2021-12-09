cmake_minimum_required(VERSION 3.8)
project(mipt-formal-practice-2 CXX)

include(ExternalProject)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${CMAKE_SOURCE_DIR}/cmake)

set(EXTERNAL_INSTALL_LOCATION ${mipt-formal-practice-2_SOURCE_DIR}/3rdparty)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${mipt-formal-practice-2_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${mipt-formal-practice-2_SOURCE_DIR}/bin)
set(Boost_USE_MULTITHREADED OFF)

SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage --coverage")
SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

ExternalProject_Add(
        googletest_repo
        GIT_REPOSITORY https://github.com/google/googletest.git
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION} -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
        GIT_TAG main
)

find_package(Boost COMPONENTS filesystem system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
include_directories(${CPR_INCLUDE_DIRS})
message("Boost libraries are located at ${Boost_LIBRARY_DIRS}")
message("Boost libraries are ${Boost_LIBRARIES}")


find_package(Python3 COMPONENTS Interpreter REQUIRED)

execute_process(COMMAND ${mipt-formal-practice-2_SOURCE_DIR}/install_deps.sh ${Python3_EXECUTABLE} WORKING_DIRECTORY ${mipt-formal-practice-2_SOURCE_DIR})

include(CodeCoverage)
add_custom_target(coverage_report)

function(run_coverage test_sources sources coverage_target target_name coverage_name)
    message(INFO test sources ${test_sources})
    set(test_name "${target_name}-test")
    message(INFO sources "${sources}")
    add_executable(${test_name} ${test_sources} ${sources})


    target_link_libraries(
            ${test_name} gtest_main gtest gmock Threads::Threads Boost::filesystem curl
    )

    setup_target_for_coverage_gcovr(
            ${coverage_target} ${target_name} ${test_name} ${coverage_name}
    )
    add_dependencies(${test_name} googletest_repo)

    add_dependencies(coverage_report ${coverage_target})
endfunction()


include_directories(include)

add_subdirectory(src/Early-library)
add_subdirectory(examples)

add_subdirectory(tests)


#COMMANDS
#cmake -DCMAKE_BUILD_TYPE=Debug ..
#make -j4
#make coverage_report

