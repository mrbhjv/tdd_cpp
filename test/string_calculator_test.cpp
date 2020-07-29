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
            return add(get_value_before_delimiter(numbers)) + add(get_value_after_delimiter(numbers));
        }
        return std::stoi(numbers);
    }

private:
    std::string delimiter_ = ",";

    bool has_delimiter(std::string str)
    {
        return str.find(delimiter_) != std::string::npos;
    }

    std::string get_value_after_delimiter(std::string str)
    {
        return str.substr(str.find(delimiter_) + 1);
    }

    std::string get_value_before_delimiter(std::string str)
    {
        return str.substr(0, str.find(','));
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

class StringCalculatorTestFixtureSingleValueNoComma : public ::testing::TestWithParam<std::string>
{
protected:
    StringCalculator sc{};
};

TEST_P(StringCalculatorTestFixtureSingleValueNoComma, When_NoComma_Should_ReturnSameValueAsInput)
{
    std::string expected = GetParam();
    int result = sc.add(expected);
    ASSERT_EQ(std::stoi(expected), result);
}

INSTANTIATE_TEST_SUITE_P(
    StringCalculatorSingleArgumentTests,
    StringCalculatorTestFixtureSingleValueNoComma,
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

TEST_F(StringCalculatorTestFixture, Should_ReturnZeroWhenMultipleCommasNoValues)
{
    int result = sc.add(",,");
    ASSERT_EQ(0, result);
}

class StringCalculatorTestFixtureSingleValueMultipleCommas : public ::testing::TestWithParam<std::pair<int, std::string>>
{
protected:
    StringCalculator sc{};
};

TEST_P(StringCalculatorTestFixtureSingleValueMultipleCommas, Should_ReturnValueWhenValueAndMultipleEmptyComma)
{
    auto param = GetParam();
    int result = sc.add(param.second);
    ASSERT_EQ(param.first, result);
}

INSTANTIATE_TEST_SUITE_P(
    StringCalculatorSingleArgumentTests,
    StringCalculatorTestFixtureSingleValueMultipleCommas,
    ::testing::Values(std::make_pair(1, "1,,"), std::make_pair(105, ",105,,"), std::make_pair(800, ",,800")));

class StringCalculatorTestFixtureMultipleValuesAndCommas : public ::testing::TestWithParam<std::pair<int, std::string>>
{
protected:
    StringCalculator sc{};
};

TEST_P(StringCalculatorTestFixtureMultipleValuesAndCommas, Should_ReturnValueWhenValueAndMultipleEmptyComma)
{
    auto param = GetParam();
    int result = sc.add(param.second);
    ASSERT_EQ(param.first, result);
}

INSTANTIATE_TEST_SUITE_P(
    StringCalculatorSingleArgumentTests,
    StringCalculatorTestFixtureMultipleValuesAndCommas,
    ::testing::Values(std::make_pair(3, "1,,2"), std::make_pair(127, "7,105,,5,10"), std::make_pair(-800, ",,800,-1600")));
