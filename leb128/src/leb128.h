//
// Created by hp on 10/19/19.
//

#ifndef COMPRESSION_ALGORITHMS_LEB128_H
#define COMPRESSION_ALGORITHMS_LEB128_H


#include <type_traits>
#include <cinttypes>
#include <climits>



/**
 * @site https://en.wikipedia.org/wiki/LEB128
 */
namespace algorithm
{
    namespace leb128
    {
        template <typename T, typename OutputIterator, class = typename std::enable_if<std::is_integral<T>::value>::type>
        auto encode_unsigned(T value, OutputIterator out) -> void
        {
            uint8_t byte;
            do {
                byte = value;
                value >>= 7;
                byte &= ~0x80u;
                if (value != 0)
                    byte |= 0x80u;
                *out++ = byte;
            } while (value != 0);
        }

        template <typename T, typename InputIterator, class = typename std::enable_if<std::is_integral<T>::value>::type>
        auto decode_unsigned(T& value, InputIterator in) -> void
        {
            uint8_t shift = 0;
            uint8_t byte;
            value = 0;
            while (true)
            {
                byte = *in++;
                value |= (byte &~0x80u) << shift;
                if (!(byte >> 7u))
                    break;
                shift += 7;
            }
        }

        template <typename T, typename OutputIterator, class = typename std::enable_if<std::is_integral<T>::value>::type>
        auto encode_signed(T value, OutputIterator out) -> void
        {
            bool more = true;
            bool negative = (value < 0);
            uint8_t size = sizeof(T) * CHAR_BIT;
            uint8_t byte;
            bool signBit;
            while (more)
            {
                byte = value & ~0x80u;
                signBit = ((uint8_t)value &~0xBFu) >> 6u;
                value >>= 7;
                if (negative)
                    value |= (~0u << (size - 7u));
                if ((value == 0 && !signBit) || (value == -1 && signBit))
                    more = false;
                else
                    byte |= 0x80u;
                *out++ = byte;
            }
        }

        template <typename T, typename InputIterator, class = typename std::enable_if<std::is_arithmetic<T>::value>::type >
        auto decode_signed(T& value, InputIterator in) -> void
        {
            value = 0;
            uint8_t shift = 0;
            uint8_t size = sizeof(T) * CHAR_BIT;
            uint8_t byte;
            do {
                byte = *in++;
                value |= (byte &~0x80u) << shift;
                shift += 7;
            } while ((byte >> 7u));
            if ((shift < size) && (byte &~0xBFu)>>6u)
                value |= (~0u << shift);
        }
    }
}


#endif //COMPRESSION_ALGORITHMS_LEB128_H
