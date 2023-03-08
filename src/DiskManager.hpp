#pragma once

// libraries
#include "ConsoleInput.hpp"
#include <string>
#include <vector>

// custom namespace
namespace DiskManager {

// prototypes
void create_empty_file(const std::string &filename);
bool load_words_from_disk(const ConsoleInput::console_args_t &args, std::vector<std::string> &vec);
bool save_csv_to_disk(const ConsoleInput::console_args_t &args, const std::string &s);

}  // namespace DiskManager
