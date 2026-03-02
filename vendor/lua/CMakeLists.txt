file(GLOB_RECURSE SRC "*.c" "*.cpp")
file(GLOB_RECURSE HEADERS "*.h" "*.hpp")

add_library(lua ${SRC} ${HEADERS})
set_source_files_properties(${HEADERS} PROPERTIES HEADER_FILE_ONLY TRUE)
target_include_directories(lua PUBLIC 
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/lua>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

