#include "ConsoleInput.hpp"
#include "ProcessCSV.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    ConsoleInput::console_args_t args;  // struct, because passing 5 variables is pain
    args.argc = argc;
    args.argv = argv;
    // default values that might be overwritten based on user input
    args.filename_words = "words.txt";    // file must exist
    args.filename_csv = "data.csv";       // file must exist
    args.filename_output = "output.csv";  // file created if doesn't exist
    args.word_column_idx = 1;             // column in CSV from which words are read
    args.verbose = false;                 // toggle, print a lot of information
    if (ConsoleInput::parse_args(args)) {
        if (!ProcessCSV::find_and_save(args)) {
            return -1;
        }
        std::cout << "INFO: CSV file was processed successfully, see '"
                  << args.filename_output << "'.\n";
    }
    return 0;
}
