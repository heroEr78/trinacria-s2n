file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "*.cpp" "*.c")
file(GLOB_RECURSE RESOURCES CONFIGURE_DEPENDS "resources/*")

add_executable(TrinacriaS2NTests ${SRC} ${RESOURCES})
add_custom_command(TARGET TrinacriaS2NTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_LIST_DIR}/resources
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/resources
    VERBATIM
)

target_link_libraries(TrinacriaS2NTests PUBLIC GTest::gtest_main TrinacriaS2N sol2::sol2)
set_source_files_properties(${RESOURCES} PROPERTIES HEADER_FILE_ONLY TRUE)
gtest_add_tests(TARGET TrinacriaS2NTests WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} CONFIGURATION $<CONFIG>)

set_target_properties(TrinacriaS2NTests PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
