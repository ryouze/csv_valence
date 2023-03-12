# csv_valence
Extract words from big CSV files.

I have to extract word frequencies and valence values from CSV files that are ~200MB for my MA essay.

Something like this:

|    Word     | Frequency |  LogFreq(Zipf)   |
| :---------: | :-------: | :--------------: |
| achievement |   3644    | 4.25743325884858 |
|  accurate   |   2904    | 4.15888186292094 |

This program can accomplish that without freezing constantly (unlike LibreOffice or Apple Numbers).

It checks if the word within the first column (can be customized) matches any of the words in `words.txt`, and if so, it grabs the entire line and saves it to `output.csv`.

The end result is a `.csv` file with only the words I need, instead of 2 million words.


---


# Example

```bash
./csv_valence
```

```
INFO: loaded '7' target words from 'words.txt'.
OK: found target word 'accomplish' (1/6).
OK: found target word 'account' (2/6).
OK: found target word 'accurate' (3/6).
OK: found target word 'accuse' (4/6).
OK: found target word 'achieve' (5/6).
OK: found target word 'achievement' (6/6).
INFO: CSV file was processed successfully, see 'output.csv'.
```


---


# Setup

### 1. Download the [csv-parser](https://github.com/AriaFallah/csv-parser) library (`parser.h`).

### 2. Rename it to `parser.hpp` and place it within `/src`.

```bash
[~/Github/csv_valence] $ mv parser.h parser.hpp
[~/Github/csv_valence] $ mv parser.hpp src/
[~/Github/csv_valence] $ cd src && ls
...  ProcessCSV.cpp   ProcessCSV.hpp   main.cpp         parser.hpp
[~/Github/csv_valence/src] $ cd ..
[~/Github/csv_valence] $
```

I believe that `.h` should be reserved for C headers, and `.cpp` for C++.

### 3. Generate Makefile.

```bash
mkdir build && cd build
cmake ..
```

### 4. Compile.

```bash
make
```


Alternatively, compile using all cores (MacOS):

```bash
make -j `sysctl -n hw.ncpu`
```

### 5. Create `words.txt`, and place a list of newline-separated words within it.

```
# This is a comment.

accomplish
according
account
accurate
accuse
achieve
achievement
```

Lines beginning with `#` will be ignored.

### 6. Place your CSV file, name it `data.csv`.

It should look like this:

```bash
[~/Github/csv_valence/build] $ ls
csv_valence      data.csv      words.txt
```

### 7. Run.

```bash
./csv_valence
```

If it couldn't find the words, you should specify the right column.

```bash
./csv_valence --column 0
```


### 8. Helper.

I have included a Bash script named `run_multiple.sh` that can process multiple files in sequential order.

The order of file-pairs in each array must be the same, i.e., `INPUT[0] + OUTPUT[0], INPUT[1] + OUTPUT[1], INPUT[2] + OUTPUT[2]`.

```bash
chmod +x run_multiple.sh
./run_multiple.sh
```

```
(0) run_multiple.sh: ./csv_valence --csv data_en_cleaned.csv --column 1 --words target_en.txt --output output_en.csv
(1) run_multiple.sh: ./csv_valence --csv wordbank_pl.csv --column 5 --words target_pl.txt --output output_pl.csv
```


---


# Commands

### 1. Get help.

```bash
./csv_valence --help
```

```
usage: ./csv_valence [-h | --help] [--words <filename>] [--csv <filename>] [--output <filename>] [--column <integer>] [-v | --verbose]

options:
  -h | --help                 print this help message
  --words <filename>          target list of newline-separated target words (default: "words.txt")
  --csv <filename>            big input CSV with all words (default: "data.csv")
  --output <filename>         small output CSV with target words only (default: "output.csv")
  --column <integer>          column in big input CSV that contains target words (default: "1")
  -v | --verbose              print extremely verbose messages
```

### 2. Specify input target words file.

Target list of newline-separated target words.

`./csv_valence --words "FILEPATH.TXT"`

Default: `words.txt`.

```bash
# path: current, relative or absolute
./csv_valence --words "english.txt"
./csv_valence --words "data/english.txt"
./csv_valence --words "/Users/hikari/data/english.txt"
```

### 3. Specify input CSV file.

Big input CSV with all words.

`./csv_valence --csv "FILEPATH.CSV"`

Default: `data.csv`.

```bash
# path: current, relative or absolute
./csv_valence --csv "vocab_EN_cleaned.csv"
./csv_valence --csv "data/vocab_EN_cleaned.csv"
./csv_valence --csv "/Users/hikari/data/vocab_EN_cleaned.csv"
```

### 4. Specify output CSV file.

Small output CSV with target words only.

`./csv_valence --output "FILEPATH.CSV"`

Default: `output.csv`.

```bash
# path: current, relative or absolute
./csv_valence --output "final.csv"
./csv_valence --output "result/final.csv"
./csv_valence --output "/Users/hikari/result/final.csv"
```

### 5. Specify the column that contains words.

Column in big input CSV that contains target words.

`./csv_valence --column "NUMBER"`

Default: `1`.

```bash
./csv_valence --column 0
./csv_valence --column 5
./csv_valence --column 22
```

### 6. Print verbose messages.

`./csv_valence -v`

```bash
./csv_valence -v
```

Note: this is very slow, because it prints ALL lines.
