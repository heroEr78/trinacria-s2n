file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "*.c" "*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS  "*.h" "*.hpp")

add_library(TrinacriaS2N ${SRC} ${HEADERS})
set_source_files_properties(${HEADERS} PROPERTIES HEADER_FILE_ONLY TRUE)

target_link_libraries(TrinacriaS2N PUBLIC sol2::sol2 lua)
target_include_directories(TrinacriaS2N PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)