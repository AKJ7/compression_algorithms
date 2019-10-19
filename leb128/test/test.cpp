//
// Created by hp on 10/19/19.
//

#include <vector>
#include <iostream>
#include <gtest/gtest.h>
#include "../src/leb128.h"


TEST(leb128_unsigned, works)
{
    using namespace algorithm::leb128;
    std::vector<uint8_t> byteVector;
    uint32_t value;
    for (uint32_t counter = 0; counter < 10000000; counter++)
    {
        byteVector.clear();
        encode_unsigned(counter, std::back_inserter(byteVector));
        decode_unsigned(value, byteVector.begin());
        assert(value == counter);
    }
}

TEST(leb_signed, works)
{
    using namespace algorithm::leb128;
    std::vector<uint8_t > byteVector;
    int32_t value;
    for (int32_t counter = -5000000; counter < 5000000; counter++)
    {
        byteVector.clear();
        encode_signed(counter, std::back_inserter(byteVector));
        decode_signed(value, byteVector.begin());
        assert(value == counter);
    }
}