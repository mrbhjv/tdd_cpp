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
        if (has_delimiter(numbers))
        {
            if (is_delimiter_before_value(numbers))
            {
                return add(get_value_after_delimiter(numbers));
            }
            else
            {
                return add(numbers.substr(0, numbers.find(','))) + add(get_value_after_delimiter(numbers));
            }
        }
        return std::stoi(numbers);
    }

private:
    std::string delimiter_ = ",";

    bool has_delimiter(std::string str)
    {
        return str.find(delimiter_) != std::string::npos;
    }
    bool is_delimiter_before_value(std::string str)
    {
        return str.find(delimiter_) == 0;
    }

    std::string get_value_after_delimiter(std::string str)
    {
        return str.substr(str.find(delimiter_) + 1);
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

TEST_F(StringCalculatorTestFixture, Should_ReturnNumberPlusZeroWhenNumberCommaEmpty)
{
    int result = sc.add("2,");
    ASSERT_EQ(2, result);
}

TEST_F(StringCalculatorTestFixture, Should_ReturnNumberPlusZeroWhenEmptyCommaNumber)
{
    int result = sc.add(",2");
    ASSERT_EQ(2, result);
}

TEST_F(StringCalculatorTestFixture, Should_ReturnZeroWhenEmptyCommaEmpty)
{
    int result = sc.add(",");
    ASSERT_EQ(0, result);
}

TEST_F(StringCalculatorTestFixture, Should_ReturnThreeWhenTwoCommaOne)
{
    int result = sc.add("1,2");
    ASSERT_EQ(3, result);
}
