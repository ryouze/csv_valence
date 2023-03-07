#include "DiskManager.hpp"
#include "ConsoleInput.hpp"
#include <fstream>
#include <iostream>
#include <vector>

namespace DiskManager {

bool load_words_from_disk(const ConsoleInput::console_args_t &args, std::vector<std::string> &vec)
/*
 * Read txt file from disk and append it to the "vec" vector.
 */
{
    std::string line;
    std::ifstream file_str(args.filename_words);  // filehandle is closed automatically
    if (!file_str) {
        std::cerr << "ERROR: could not open ifstream for '" << args.filename_words
                  << "', possibly due to missing permissions to the filesystem.\n";
        return false;
    }
    // while exists, append to string with newline
    while (std::getline(file_str, line)) {
        // ignore comments (lines beginning with "#")
        if (line[0] == '#') {
            if (args.verbose) {
                std::cout << "DEBUG: skipping commented-out line: '" << line << "'\n";
            }
            continue;
        }
        if (line.empty()) {
            if (args.verbose) {
                std::cout << "DEBUG: skipping empty line: '" << line << "'\n";
            }
            continue;
        }
        vec.push_back(line);
    }
    // file exists, but contains nothing
    if (vec.empty()) {  // filename, text to modify
        std::cerr << "ERROR: the words file is empty: '" << args.filename_words
                  << "'.\n";
        return false;
    }
    vec.shrink_to_fit();
    return true;
}

bool save_csv_to_disk(const ConsoleInput::console_args_t &args, const std::string &s)
/*
 * Save "s" string to disk as a txt file.
 */
{
    if (s.empty()) {  // filename, text to modify
        std::cerr << "ERROR: the string to be saved is empty: '" << s
                  << "'.\n";
        return false;
    }
    std::ofstream file_str(args.filename_output);
    if (!file_str) {
        std::cerr << "ERROR: could not open ofstream for '" << args.filename_output << "'.\n";
        return false;
    }
    file_str << s;
    return true;
}
}  // namespace DiskManager
