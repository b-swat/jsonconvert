cmake_minimum_required(VERSION 3.19)
project(json_convert)

set(CMAKE_CXX_STANDARD 11)

#include(FetchContent)
#
#set (BUILD_SHARED_LIBS OFF CACHE INTERNAL "Turn off shared")
#set (JSONCPP_WITH_TESTS OFF CACHE INTERNAL "Turn off jsontest")
#
#FetchContent_Declare(json
#        GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp
#        GIT_TAG 1.9.4
#        )
#FetchContent_MakeAvailable(json)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake" TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES
        jsoncpp/1.9.4
        GENERATORS cmake)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
        SETTINGS ${settings} compiler.cppstd=11
        BUILD missing)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME})

target_compile_options(${PROJECT_NAME}
    PRIVATE -std=c++11 -O3 -Wall -pedantic -Wextra)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC ASIO_STANDALONE)

target_include_directories(${PROJECT_NAME}
    PUBLIC   ${CONAN_INCLUDE_DIRS})

target_sources(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/main.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE pthread gcc stdc++ ${CONAN_LIBS})
