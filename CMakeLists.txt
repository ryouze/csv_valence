# Set the minimum version of CMake that can be used.
# To find your current version, run "cmake --version".
cmake_minimum_required(VERSION 3.25)


# Set the project name.
project(csv_valence)


# Set C++20 standard.
set(CMAKE_CXX_STANDARD 20)


# Enable warnings (unused variables and other stupidity).
# "-Werror" turns all warnings into errors, which prevents "make && ./output" from running.
add_compile_options(-Wall -Wextra -Werror -Wpedantic)


# Set directory containing ".cpp" files.
# Note: "if you #include all of your .hpp files in .cpp files, there's no need to explicitly put them in add_executable; you just need to specify include directory with include_directories".
set(SRC_DIR
    "src"
)


# Listing files explicitly is faster and less error-prone than globbing.
set(FILES
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/ConsoleInput.cpp"
    "${SRC_DIR}/DiskManager.cpp"
    "${SRC_DIR}/ProcessCSV.cpp"
    "${SRC_DIR}/parser.hpp"
)


# Set an output executable, created using the ".cpp" files listed above.
add_executable(${PROJECT_NAME}
    ${FILES}
)
