cmake_minimum_required (VERSION 4.0)
project ("TrinacriaS2N")

include(CTest)
enable_testing()

include(GoogleTest)
include(GNUInstallDirs)

set_property(GLOBAL PROPERTY CTEST_TARGETS_FOLDER "CMakeTargets/CTestDashboardTargets")
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin-int")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin-int")

if(MSVC)
    add_compile_options(/Zi-)
    add_compile_options(/ZI)
    set(CMAKE_VS_STARTUP_PROJECT "TrinacriaS2NTests")
endif()

set(SOL2_ENABLE_INSTALL ON)

add_custom_target(REGEN
    COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_SOURCE_DIR}/CMakeLists.txt"
    COMMAND ${CMAKE_COMMAND} -E remove -f 
        "${CMAKE_BINARY_DIR}/CMakeFiles/generate.stamp"
        "${CMAKE_BINARY_DIR}/CMakeFiles/cmake.check_cache"
        "${CMAKE_BINARY_DIR}/CMakeFiles/generate.stamp.depend"
    COMMAND ${CMAKE_COMMAND} 
        -S "${CMAKE_SOURCE_DIR}" 
        -B "${CMAKE_BINARY_DIR}"
        -G "${CMAKE_GENERATOR}"
    COMMENT "Force regenerating all CMake build files..."
    VERBATIM
)

add_compile_definitions(
    $<$<CONFIG:Debug>:TRINACRIA_DEBUG=1>
    $<$<CONFIG:Release>:TRINACRIA_DEBUG=0>
)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/vendor/lua")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/vendor/googletest")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/vendor/sol2")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/TrinacriaS2N")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Tests")

set_target_properties(REGEN PROPERTIES FOLDER "CMakeTargets")
set_target_properties(sol2 PROPERTIES FOLDER "Dependencies")
set_target_properties(lua PROPERTIES FOLDER "Dependencies")
set_target_properties(gtest PROPERTIES FOLDER "Dependencies/gtest")
set_target_properties(gtest_main PROPERTIES FOLDER "Dependencies/gtest")
set_target_properties(gmock_main PROPERTIES FOLDER "Dependencies/gtest")
set_target_properties(gmock PROPERTIES FOLDER "Dependencies/gtest")
