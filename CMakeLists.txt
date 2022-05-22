cmake_minimum_required(VERSION 3.1)
project(ecc C)

set(CMAKE_DIR   "${CMAKE_CURRENT_LIST_DIR}/cmake"   CACHE PATH "The the path to the cmake directory")
option(ECC_USE_EXTERNAL_JSON "Use an external JSON library" OFF)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_DIR})
include(FindGMP)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_thread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lgmp")

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(PROJECT_VERSION 1.0)
set(DEFAULT_BUILD_TYPE "Release")
FILE(GLOB SOURCE_FILES "src/*.cpp")

if (NOT TARGET crypto_code)
    add_subdirectory(lib/crypto_code)
    link_directories(lib/crypto_code)
    MESSAGE(STATUS "Connected: crypto_code")
endif ()
if (NOT TARGET hash_code)
    add_subdirectory(lib/hash_code)
    link_directories(lib/hash_code)
    MESSAGE(STATUS "Connected: hash_code")
endif ()
if (NOT TARGET nlohmann_json)
    add_subdirectory(lib/json)
    link_directories(lib/json)
    MESSAGE(STATUS "Connected: nlohmann_json")
endif ()

include(GNUInstallDirs)
add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)
set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC gmp gmpxx)
target_link_libraries(${PROJECT_NAME} PUBLIC crypto_code)
target_link_libraries(${PROJECT_NAME} PUBLIC hash_code)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)
