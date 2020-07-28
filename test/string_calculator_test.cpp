#include <string>

#include "gtest/gtest.h"

class StringCalculator
{
public:
    StringCalculator() = default;

    int add(std::string numbers)
    {
        if (numbers.empty())
        {
            return 0;
        }

        return std::stoi(numbers);
    }
};

class StringCalculatorTestFixture : public ::testing::Test
{
protected:
    StringCalculator sc{};
};

TEST_F(StringCalculatorTestFixture, Should_ReturnZeroWhenEmptyString)
{
    int result = sc.add("");
    ASSERT_EQ(0, result);
}

class StringCalculatorTestFixtureOneArgument : public ::testing::TestWithParam<std::string>
{
protected:
    StringCalculator sc{};
};

TEST_P(StringCalculatorTestFixtureOneArgument, When_NoComma_Should_ReturnSameValueAsInput)
{
    std::string expected = GetParam();
    int result = sc.add(expected);
    ASSERT_EQ(std::stoi(expected), result);
}

INSTANTIATE_TEST_SUITE_P(
    StringCalculatorSingleArgumentTests,
    StringCalculatorTestFixtureOneArgument,
    ::testing::Values("1", "105", "800"));

