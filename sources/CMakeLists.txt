
include_directories(    
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(
    LIB_BASE 
    STATIC
    base/string.cpp
    base/timer.cpp
    base/trace.cpp
)
set_target_properties(
    LIB_BASE
    PROPERTIES OUTPUT_NAME base
)

add_library(
    LIB_CSV_PARSER 
    STATIC
    parser/character_map.cpp
    parser/reader.cpp
    parser/row.cpp    
    parser/scanner.cpp 
)
target_link_libraries(
    LIB_CSV_PARSER
    LIB_BASE
)
set_target_properties(
    LIB_CSV_PARSER
    PROPERTIES OUTPUT_NAME csv-parser
)

add_executable(
    TEST_CSV_PARSER
    main.cpp    
)
target_link_libraries(
    TEST_CSV_PARSER
    LIB_CSV_PARSER
)
set_target_properties(
    TEST_CSV_PARSER
    PROPERTIES OUTPUT_NAME csv-parser
)
