add_subdirectory(source)

add_executable(${PROJECT_NAME} ${advent_of_code_SourceFiles})

enable_static_analyzers(${PROJECT_NAME} ON)

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" 
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/advent_of_code"
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23)
target_compile_options(${PROJECT_NAME} PRIVATE 
    -Werror
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wimplicit-fallthrough
    -fsanitize=address,leak,undefined
)
target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address,leak,undefined)
target_link_libraries(${PROJECT_NAME} PRIVATE ${advent_of_code_ExternalLibraries})
