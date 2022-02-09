if( TARGET nlohmann_json )
    return()
endif()

if(NOT NLOHMANN_JSON_ROOT)
    set(NLOHMANN_JSON_ROOT "${PROJECT_SOURCE_DIR}/lib/json")
endif()

add_library( nlohmann_json INTERFACE )
target_include_directories(
        nlohmann_json
        INTERFACE
        ${NLOHMANN_JSON_ROOT}/include
)
