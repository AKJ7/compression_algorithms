set(algorithm_name "Run-length encoding")

title_message(${algorithm_name})

package_add_test(run_length_encoding src/run_length_encoding.h tests/test.cpp)