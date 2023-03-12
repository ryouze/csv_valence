#pragma once

// libraries
#include <string>

// custom namespace
namespace ConsoleInput {

// types
struct console_args_t {
    int argc;
    char **argv;
    std::string filename_words;
    std::string filename_csv;
    std::string filename_output;
    std::size_t word_column_idx;
    bool verbose;
};

// prototypes
bool parse_args(console_args_t &args);

}  // namespace ConsoleInput
