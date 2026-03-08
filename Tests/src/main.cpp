#include <filesystem>
#include <gtest/gtest.h>
#include <sol/state.hpp>

int main(int argc, char** argv)
{
    ::testing::GTEST_FLAG(break_on_failure) = true;
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}