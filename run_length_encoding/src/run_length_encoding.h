#include <algorithm>
#include <array>
#include <iterator>
#include <limits.h>
#include <tuple>

namespace algorithm
{
    namespace run_length_encoding
    {
        constexpr std::array<char, 10> digits = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

        template <typename OutputIterator>
        auto encode_run_length(std::size_t n, OutputIterator out) -> OutputIterator
        {
            auto base = digits.size();
            auto num_digits = [base](unsigned long n)
            {
                std::size_t d = 1;
                while ((n /= base))
                    ++d;
                return d;
            }(n);

            auto base_power = [base](unsigned long n)
            {
                auto res = decltype(base){1};
                for (auto i = decltype(n){1}; i < n; i++)
                    res *= base;
                return res;
            };

            for (auto i = decltype(num_digits){0}; i < num_digits; ++i)
                *out = digits[(n / base_power(num_digits - i)) % base];
            return out;
        }

        template <typename InputIterator>
        auto decode_run_length(InputIterator first, InputIterator last) -> std::pair<std::size_t, InputIterator>
        {
            std::size_t count{};
            while (first != last)
            {
                const auto p = std::find(digits.begin(), digits.end(), *first);
                if (p == digits.end())
                    break;
                count *= digits.size();
                count += std::distance(digits.begin(), p);
                ++first;
            }
            return std::make_pair(count, first);
        }

        template <typename InputIterator, typename OutputIterator>
        auto encode(InputIterator first, InputIterator last, OutputIterator out) -> OutputIterator
        {
            while (first != last)
            {
                const auto value = *first++;
                std::size_t count{1};
                while (first != last && *first == value)
                {
                    ++count;
                    ++first;
                }
                out = encode_run_length(count, out);
                *out++ = value;
            }
            return out;
        }

        template <typename InputIterator, typename OutputIterator>
        auto decode(InputIterator first, InputIterator last, OutputIterator out) -> OutputIterator
        {
            while (first != last)
            {
                std::size_t count = 1;
                if (std::find(digits.begin(), digits.end(), *first) != digits.end())
                    std::tie(count, first) = decode_run_length(first, last);
                out = std::fill_n(out, count, *first++);
            }
            return out;
        }

        template <typename Range, typename OutputIterator>
        auto encode(Range&& range, OutputIterator out) -> OutputIterator
        {
            return encode(std::begin(range), std::end(range), out);
        }

        template <typename Range, typename OutputIterator>
        auto decode(Range&& range, OutputIterator out) -> decltype(decode(std::begin(range), std::end(range), out))
        {
            return decode(std::begin(range), std::end(range), out);
        }
    }
}