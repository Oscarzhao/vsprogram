#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "Turtle.h"

using ::testing::AtLeast;  //# 1

int main(int argc, char ** argv)
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock(&argc, argv);
}