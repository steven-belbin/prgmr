cmake_minimum_required(VERSION 3.14)
project(prgmr LANGUAGES CXX)

add_library(prgmr INTERFACE)
target_include_directories(prgmr INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(prgmr INTERFACE cxx_std_17)

include(CTest)
if(BUILD_TESTING)
    add_executable(prgmr_tests tests/prgmr_tests.cpp)
    target_link_libraries(prgmr_tests PRIVATE prgmr)
    add_test(NAME prgmr_tests COMMAND prgmr_tests)
endif()
