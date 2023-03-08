#include "ProcessCSV.hpp"
#include "ConsoleInput.hpp"
#include "DiskManager.hpp"
#include "parser.hpp"  // Author: https://github.com/AriaFallah/csv-parser
#include <fstream>
#include <iostream>
#include <vector>

using namespace aria::csv;

class WordsHelper {
  private:
    // types
    using vec_temp_t = std::vector<std::string>;
    using vec_csv_t = std::vector<std::vector<std::string>>;

    // variables
    ConsoleInput::console_args_t args;
    std::string result;

    bool words_to_vec(vec_temp_t &v) const
    {
        /*
         * Load a list of target words from disk and save them to a vector.
         *
         * Return True if succeeded, False if failed.
         */
        // invalid path or wrong permissions
        if (!DiskManager::load_words_from_disk(this->args, v)) {
            std::cerr << "ERROR: cannot open words file: '" << this->args.filename_words
                      << "'.\n";
            return false;
        }
        std::cout << "INFO: loaded '" << v.size()
                  << "' target words from '" << this->args.filename_words << "'.\n";
        if (this->args.verbose) {
            std::cout << "DEBUG: here are all the words:\n";
            for (const auto &word : v) {
                std::cout << "'" << word << "'\n";
            }
        }
        return true;
    }

    bool process_csv(vec_csv_t &v) const
    {
        /*
         * Find a list of target words inside CSV and return them as a vector of vectors.
         *
         * Return True if succeeded, False if failed.
         */
        vec_temp_t vec_target_words;
        if (!words_to_vec(vec_target_words)) {
            std::cerr << "ERROR: failed to extract target words file: '" << this->args.filename_words
                      << "'.\n";
            return false;
        }
        // open CSV using external library
        std::ifstream file(this->args.filename_csv);
        CsvParser parser(file);
        vec_temp_t vec_temp;
        // append header (doing this separately to prevent bool checks for the following rows)
        for (const auto &row : parser) {
            // unsigned i = 0;
            for (const auto &column : row) {
                if (args.verbose) {
                    auto i = &column - &row[0];  // get iterator
                    std::cout << "Header column no. " << i << " = '" << column << "'\n";
                }
                vec_temp.push_back(column);
            }
            // break after 1 row
            break;
        }
        v.push_back(vec_temp);
        // search for rows
        vec_temp_t::size_type word_amount_found = 0;
        const vec_temp_t::size_type word_amount_total = vec_target_words.size();  // total amount of words
        for (const auto &row : parser) {
            // if no more target words, stop; prevents looping over entire csv
            if (vec_target_words.empty()) {
                break;
            }
            // dynamically check all target words against the column at target idx
            // they will be removed when found; when no words are left, break out of the loop
            vec_temp_t::iterator itr = vec_target_words.begin();
            while (itr != vec_target_words.end()) {
                if (args.verbose) {
                    // this is really wordy, but ensures that the user knows what's going on
                    std::cout << "DEBUG: checking if target word '" << *itr
                              << "' is equal to the word at column no. '"
                              << args.word_column_idx << "', which, based on the index, is equal to '"
                              << row.at(args.word_column_idx) << "'.\n";
                }
                // if matches the word at that specific column (e.g., column 1 is "ability")
                if (*itr == row.at(args.word_column_idx)) {
                    ++word_amount_found;
                    // append entire row
                    if (args.verbose) {
                        std::cout << "OK: found target word '" << *itr
                                  << "' that is equal to the word at column no. '"
                                  << args.word_column_idx << "', which, based on the index, is equal to '"
                                  << row.at(args.word_column_idx)
                                  << "' ("
                                  << word_amount_found << "/" << word_amount_total
                                  << ").\n";
                    }
                    else {
                        std::cout << "OK: found target word '" << *itr
                                  << "' ("
                                  << word_amount_found << "/" << word_amount_total
                                  << ").\n";
                    }
                    // if found, remove from global target word pool (so it doesn't check the same word again)
                    itr = vec_target_words.erase(itr);
                    vec_temp.clear();  // otherwise it all keeps piling up
                    for (const auto &column : row) {
                        vec_temp.push_back(column);
                    }
                    // push entire row, then do not check next word
                    v.push_back(vec_temp);
                    break;
                }
                else {
                    ++itr;
                }
            }
        }
        if (word_amount_found == 0) {
            std::cerr << "ERROR: couldn't find any matches for the words provided in '"
                      << args.filename_words
                      << "'.\nERROR: here is a list of words that were checked:\n";
            for (const auto &word : vec_target_words) {
                auto i = (&word - &vec_target_words[0]) + 1;
                std::cout << i << ". '" << word << "'\n";
            }
            return false;
        }
        if (word_amount_found != word_amount_total) {
            std::cerr << "INFO: couldn't find matches for the following words:\n";
            for (const auto &word : vec_target_words) {
                auto i = (&word - &vec_target_words[0]) + 1;
                std::cout << i << ". '" << word << "'\n";
            }
        }
        v.shrink_to_fit();
        return true;
    }

  public:
    WordsHelper(const ConsoleInput::console_args_t &args)
    /*
     * Class constructor.
     */
    {
        this->args = args;
    }

    bool csv_to_string(std::string &s)
    {
        // if not already processed, extract csv and turn into string
        if (this->result.empty()) {
            vec_csv_t vec_csv;
            if (!this->process_csv(vec_csv)) {
                std::cerr << "ERROR: failed to turn a vector into a comma-separated string.\n";
                return false;
            }
            // convert vector into CSV string
            for (const auto &v : vec_csv) {
                vec_temp_t::size_type len = v.size();
                for (vec_temp_t::size_type i = 0; i < len; ++i) {
                    this->result += v.at(i);
                    // add comma unless last item
                    if (i + 1 < len) {
                        this->result += ',';
                    }
                }
                this->result += '\n';
            }
        }
        s = this->result;
        return true;
    }
};

namespace ProcessCSV {

bool find_and_save(const ConsoleInput::console_args_t &args)
{
    WordsHelper words_helper(args);
    std::string str_to_save;
    if (!words_helper.csv_to_string(str_to_save)) {
        std::cerr << "ERROR: failed to extract the final string that "
                  << "would be saved to '" << args.filename_output << "'.\n";
        return false;
    }
    if (args.verbose) {
        std::cout << "RESULT: the following string (" << str_to_save.length()
                  << " characters) will be saved to '" << args.filename_output << "':\n"
                  << str_to_save;  // it includes trailing comma
    }
    const bool final_result = DiskManager::save_csv_to_disk(args, str_to_save);
    if (args.verbose) {
        std::cout << "INFO: save to disk status: " << (final_result ? "success" : "failure") << ".\n";
    }
    return true;
}

}  // namespace ProcessCSV
