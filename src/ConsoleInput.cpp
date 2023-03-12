#include "ConsoleInput.hpp"
#include "DiskManager.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class ArgsHelper {
  private:
    // types
    using vec_opts_t = std::vector<std::string>;

    // variables
    vec_opts_t args_vec;
    vec_opts_t::iterator vec_begin;
    vec_opts_t::iterator vec_end;
    inline static const std::string empty_string = "";

  public:
    ArgsHelper(const int &argc, char *argv[])
    /*
     * Class constructor.
     */
    {
        for (int i = 1; i < argc; ++i) {
            this->args_vec.push_back(std::string(argv[i]));
        }
        // shrink capacity to actual size
        this->args_vec.shrink_to_fit();
        // get begin and end iterators
        this->vec_begin = this->args_vec.begin();
        this->vec_end = this->args_vec.end();
    }

    bool check_if_exists(const std::string &name) const
    /*
     * Check if a single argument exists.
     * E.g., ./main.o -h
     *
     * Usage:
     * if (args_helper.check_if_exists("-h")) {
     *     // do things
     * }
     */
    {
        return (std::find(this->vec_begin, this->vec_end, name) != this->vec_end);
    }

    bool check_if_exists(const std::string &name1, const std::string &name2) const
    /*
     * Check if two arguments exists.
     * E.g., ./main.o -h || ./main.o --help
     *
     * Usage:
     * if (args_helper.check_if_exists("-h", "--help")) {
     *     // do things
     * }
     */
    {
        return (this->check_if_exists(name1) || this->check_if_exists(name2));
    }

    const std::string &get_keyword_pair(const std::string &name) const
    /*
     * Get keyword argument by name.
     * E.g., ./main.o --csv "file.csv"
     *
     * Usage:
     *     const std::string &filename = args_helper.get_keyword_pair("-c", "--csv");
     *     if (!filename.empty()) {
     *         // do things
     *     }
     */
    {
        // get iterator to the first element is equal to name.
        vec_opts_t::const_iterator itr = std::find(this->vec_begin, this->vec_end, name);
        // get element AFTER that iterator (+1)
        if (itr != vec_end && ++itr != vec_end) {
            return *itr;
        }
        std::cerr << "ERROR: argument '" << name << "' doesn't have a value.\n"
                  << "Please enter '" << name << " \"VALUE\"'.\n";
        return this->empty_string;
    }
};

bool string_to_long_long(const std::string &s, long long &i)
{
    /*
     * Convert string "s" to long long "i".
     *
     * Return True if succeeded, False if failed.
     */
    bool success = false;
    // convert to int
    try {
        i = std::stol(s);
        success = true;
    }
    catch (std::invalid_argument const &e) {
        std::cerr << "ERROR: invalid number (tried to convert '"
                  << s << "' to long long).\n";
    }
    catch (std::out_of_range const &e) {
        std::cerr << "ERROR: out of integer range (tried to convert '"
                  << s << "' to long long).\n";
    }
    return success;
}

namespace ConsoleInput {
void print_help(const char *program_name, const console_args_t &args)
{
    /*
     * Print help message to console, to be called using "-h" or "--help".
     */
    std::cout << "usage: "
              << program_name
              << " [-h | --help] [--words <filename>] [--csv <filename>] [--output <filename>] [--column <integer>] [-v | --verbose]\n\n"
                 "options:\n"
                 "  -h | --help                 print this help message\n"
                 "  --words <filename>          target list of newline-separated target words (default: \""
              << args.filename_words << "\")\n"
              << "  --csv <filename>            big input CSV with all words (default: \""
              << args.filename_csv << "\")\n"
              << "  --output <filename>         small output CSV with target words only (default: \""
              << args.filename_output << "\")\n"
              << "  --column <integer>          column in big input CSV that contains target words (default: \""
              << args.word_column_idx << "\")\n"
              << "  -v | --verbose              print extremely verbose messages\n";
}

bool parse_args(console_args_t &args)
{
    /*
     * Read filenames from commandline arguments, then check if they exist.
     *
     * Return True if everything worked as expected, False if failed.
     */
    const ArgsHelper args_helper(args.argc, args.argv);
    if (args_helper.check_if_exists("-h", "--help")) {
        print_help(args.argv[0], args);
        return false;
    }
    // --- words.txt ---
    // check for "--words", but use "words.txt" as fallback
    if (args_helper.check_if_exists("--words")) {
        args.filename_words = args_helper.get_keyword_pair("--words");
        if (args.filename_words.empty()) {
            std::cerr << "ERROR: please provide path to the words file "
                         "(i.e., a file containing individual words, with each "
                         "of them placed on a newline).\n";
            return false;
        }
    }
    // file doesn't exist
    if (!std::filesystem::exists(args.filename_words)) {
        std::cerr << "FATAL ERROR: words file doesn't exist: '" << args.filename_words
                  << "'. Please create a list of newline-separated words.\n";
        exit(EXIT_FAILURE);
    }
    // --- data.csv ---
    // check for "--csv", but use "data.csv" as fallback
    if (args_helper.check_if_exists("--csv")) {
        args.filename_csv = args_helper.get_keyword_pair("--csv");
        if (args.filename_csv.empty()) {
            std::cerr << "ERROR: please provide a path to the input CSV file "
                         "(e.g., 'data.csv').\n";
            return false;
        }
    }
    if (!std::filesystem::exists(args.filename_csv)) {
        std::cerr << "FATAL ERROR: big CSV file doesn't exist: '" << args.filename_csv
                  << "'. Please provide a path to the input CSV file that contains "
                     "words and their data\n ";
        exit(EXIT_FAILURE);
    }
    // --- output.csv ---
    // check for "--output", but use "output.csv" as fallback
    if (args_helper.check_if_exists("--output")) {
        args.filename_output = args_helper.get_keyword_pair("--output");
        if (args.filename_output.empty()) {
            std::cerr << "ERROR: please provide a path to the output CSV file "
                         "(e.g., 'output.csv').\n";
            return false;
        }
    }
    if (!std::filesystem::exists(args.filename_output)) {
        std::cout << "INFO: output CSV file doesn't exist, creating now: '"
                  << args.filename_output
                  << "'.\n";
        DiskManager::create_empty_file(args.filename_output);
    }
    if (args_helper.check_if_exists("--column")) {
        std::string temp = args_helper.get_keyword_pair("--column");
        if (temp.empty()) {
            std::cerr << "ERROR: no integer was provided to a column (default: \""
                      << args.word_column_idx
                      << "\"). Please provide a number.\n";
            return false;
        }
        // convert string to long long
        long long temp_num;
        if (!string_to_long_long(temp, temp_num)) {
            std::cerr << "ERROR: please provide a valid integer to a column "
                         "(e.g., \"1\").\n";
            return false;
        }
        if (temp_num < 0) {
            std::cerr << "ERROR: please provide column integers equal to 0 or above "
                         "(e.g., \"1\").\n";
            return false;
        }
        // cast long long to unsigned long
        args.word_column_idx = static_cast<std::size_t>(temp_num);
    }
    if (args_helper.check_if_exists("-v", "--verbose")) {
        args.verbose = true;
    }
    return true;
}
}  // namespace ConsoleInput
