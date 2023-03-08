#!/bin/bash

# path to compiled program
BINARY="./build/csv_valence"

# -v | --verbose : print extremely verbose messages
verbose=false

# --csv <filename> : big input CSV with all words (default: "data.csv")
INPUT_CSV=(
    "build/data.csv"
    # "my_data1.csv"
    # "my_data2.csv"
)

# --column <integer> : column in big input CSV that contains target words (default: "1")
INPUT_CSV_COLUMN=(
    "1"
    # "0"
    # "5"
)

# --words <filename> : target list of newline-separated target words (default: "words.txt")
INPUT_TARGET_WORDS=(
    "build/words.txt"
    # "my_words1.txt"
    # "my_words2.txt"
)

# --output <filename> : small output CSV with target words only (default: "output.csv")
OUTPUT_CSV=(
    "build/output.csv"
    # "my_output1.csv"
    # "my_output2.csv"
)

for ((i=0; i<${#INPUT_CSV[@]}; ++i)); do
    full_command="${BINARY} --csv ${INPUT_CSV[i]} --column ${INPUT_CSV_COLUMN[i]} --words ${INPUT_TARGET_WORDS[i]} --output ${OUTPUT_CSV[i]}"
    if [ "$verbose" = true ]; then
        full_command+=" --verbose"
    fi
    echo "(${i}) run_multiple.sh: ${full_command}"
    eval "${full_command}"
done
