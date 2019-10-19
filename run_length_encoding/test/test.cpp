#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "../src/run_length_encoding.h"


TEST(run_length_encoding, works)
{
    using namespace algorithm::run_length_encoding;
    std::string test_string {"WWWWWWWWWWWWBWWWWWWWWWWWWBBBWWWWWWWWWWWWWWWWWWWWWWWWBWWWWWWWWWWWWWW"};
    std::cout << "Input: \"" << test_string << "\"\n";
    std::cout << "Output: \"";
    encode(test_string, std::ostream_iterator<char>{std::cout});
    std::cout << "\"\n";

    std::string encoded_str{};
    std::string decoded_str{};
    encode(test_string, std::back_inserter(encoded_str));
    decode(encoded_str, std::back_inserter(decoded_str));

    std::cout << std::boolalpha << "Round trip works: " << (test_string == decoded_str) << '\n';
    ASSERT_TRUE(test_string == decoded_str);
}