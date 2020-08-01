#include <string>

#include "gtest/gtest.h"

class StringCalculator
{
public:
    StringCalculator() = default;

    int add(std::string input)
    {
        if (input.empty())
        {
            return 0;
        }
        if (has_override_delimiter_seq(input))
        {
            delimiter_ = get_new_delimiter(input);
            input = remove_override_delimiter_seq(input);
        }
        if (has_any_default_delimiter(input))
        {
            validate_delimiter(input);
            return add(get_value_before_delimiter(input)) + add(get_value_after_delimiter(input));
        }
        return std::stoi(input);
    }

private:
    std::string delimiter_override_seq_ = "//";
    std::string comma_delimiter_ = ",";
    std::string newline_delimiter_ = "\n";
    std::string default_delimiters_ = comma_delimiter_ + newline_delimiter_;
    std::string delimiter_ = default_delimiters_;

    bool has_override_delimiter_seq(const std::string& str)
    {
        return str.find(delimiter_override_seq_) != std::string::npos;
    }

    std::string get_new_delimiter(const std::string& str)
    {
        return str.substr(str.find(delimiter_override_seq_) + delimiter_override_seq_.length(), 1);
    }

    std::string remove_override_delimiter_seq(const std::string& str)
    {
        return str.substr(3);
    }

    bool has_any_default_delimiter(const std::string& str)
    {
        return str.find_first_of(delimiter_) != std::string::npos;
    }

    void validate_delimiter(const std::string& str)
    {
        if (has_illegal_delimiters(str))
        {
            throw std::invalid_argument(", and newline can't be together");
        }
    }

    bool has_illegal_delimiters(const std::string& str)
    {
        return str.find(comma_delimiter_ + newline_delimiter_) != std::string::npos ||
               str.find(newline_delimiter_ + comma_delimiter_) != std::string::npos;
    }

    std::string get_value_after_delimiter(const std::string& str)
    {
        return str.substr(str.find_first_of(delimiter_) + 1);
    }

    std::string get_value_before_delimiter(const std::string& str)
    {
        return str.substr(0, str.find_first_of(delimiter_));
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

TEST_F(StringCalculatorTestFixture, Should_WorkWithNewLineAndCommaAsDelimiter)
{
    auto result = sc.add("1\n2,3");
    ASSERT_EQ(6, result);
    result = sc.add("1,2\n3");
    ASSERT_EQ(6, result);
}

TEST_F(StringCalculatorTestFixture, Should_NotWorkWithCommaAndNewLineAfterEachOther)
{
    ASSERT_THROW(sc.add("1,\n"), std::invalid_argument);
    ASSERT_THROW(sc.add("1\n,"), std::invalid_argument);
}

TEST_F(StringCalculatorTestFixture, Should_ChangeDefaultDelimiterToSemiCollon)
{
    auto result = sc.add("//;\n1;1");
    ASSERT_EQ(2, result);
}

TEST_F(StringCalculatorTestFixture, Should_ChangeDefaultDelimiterToHyphen)
{
    auto result = sc.add("//_\n1_1");
    ASSERT_EQ(2, result);
}

TEST_F(StringCalculatorTestFixture, Should_ChangeDefaultDelimiterWithoutNewlineAfterDelimiter)
{
    auto result = sc.add("//_1_1_1_");
    ASSERT_EQ(3, result);
}