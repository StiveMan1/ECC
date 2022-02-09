cmake_minimum_required(VERSION 3.1)
project(ecc C)

set(CMAKE_C_STANDARD 99)
set(PROJECT_VERSION 1.0)
set(DEFAULT_BUILD_TYPE "Release")

set(CMAKE_DIR   "${CMAKE_CURRENT_LIST_DIR}/cmake"   CACHE PATH "The the path to the cmake directory")
set(SOURCE_FILES
        src/ecc_point.c
        src/ecc_curve.c
        src/ecc_signature.c
        src/ecc_key.c
        src/random_by_password.c)
option(ECC_USE_EXTERNAL_JSON "Use an external JSON library" OFF)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_DIR})

include(FindGMP)
include(FindNlohmannJson)
include(GNUInstallDirs)

add_subdirectory(lib/crypto_code)
add_subdirectory(lib/hash_code)
add_subdirectory(lib/big_int)

link_directories(lib/crypto_code)
link_directories(lib/hash_code)
link_directories(lib/big_int)


add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src
        PRIVATE lib)
set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1)
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})


target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)
target_link_libraries(${PROJECT_NAME} PUBLIC big_int)
target_link_libraries(${PROJECT_NAME} PUBLIC crypto_code)
target_link_libraries(${PROJECT_NAME} PUBLIC hash_code)