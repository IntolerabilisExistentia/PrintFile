#include <iostream>
#include <fstream>
#include <cstring>

int numLines = 0;
bool tailMode = false;
char delimiter = '\n';
char* filename = nullptr;

bool StrLongerThan(char* str, const int n) {
    int length = 0;
    while (*str != '\0') {
        if (length > n) {
            return 1;
        }
        ++length;
        ++str;
    } 
    return 0;
}

void PrintHelp() {
    std::cout << "Usage: program_name [-l <num_lines> | --lines=<num_lines>] [-t | --tail] [-d <delimiter> | --delimiter=<delimiter>] <filename>\n";
}

bool ParseArguments(const int num, char* args[]) {
    for (int i = 1; i < num; ++i) {
        char* arg = args[i];
        if (std::strcmp(arg, "-l") == 0 && i + 1 < num) {
            numLines = std::strtol(args[++i], nullptr, 10);
            if (numLines <= 0) {
                std::cout << "Invalid number of lines.\n";
                return true;
            }
        } else if (std::strncmp(arg, "--lines=", 8) == 0) {
            numLines = std::strtol(arg + 8, nullptr, 10);
            if (numLines <= 0) {
                std::cout << "Invalid number of lines.\n" << std::endl;
                return true;
            }
        } else if (std::strcmp(arg, "--tail") == 0) {
            tailMode = true;
        } else if (std::strcmp(arg, "-t") == 0) {
            tailMode = true;
        } else if (std::strcmp(arg, "-d") == 0 && i + 1 < num) {
            if (StrLongerThan(args[++i], 1)) {
                std::cout << "Invalid delimiter.\n"; 
                return true;
            }
            delimiter = args[i][0];
        } else if (std::strncmp(arg, "--delimiter=", 12) == 0) {
            if (StrLongerThan(args[i], 13)) {
                std::cout << "Invalid delimiter.\n"; 
                return true;
            }
            delimiter = arg[12];
        } else if (filename == nullptr) {
            filename = arg;
        } else {
            PrintHelp();
            return true;
        }
    }
    
    if (!filename) {
        std::cout << "Please provide the name of the file.\n";
        return true;
    }
    return false;
}

bool PrintFile() {
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Failed to open the file: " << filename << '\n';
        return true;
    }
    if (tailMode && numLines != 0) {
        int totalLines = 0;
        while (true) {
            char c;
            if (!file.get(c)) {
                ++totalLines;
                break;
            }

            if (c == delimiter) {
                ++totalLines;
            }
        }
        file.clear(); 
        file.seekg(0); 

        int startIdx = std::max(0, totalLines - numLines);
        int currentLine = 0;
        char c;
        while (currentLine < startIdx && file.get(c)) {
            if (c == delimiter) {
                ++currentLine;
            }
        }
    } 
    int lines = 0;
    while (true) {
        char c;
        if (!file.get(c) || (numLines != 0 ? lines == numLines : false)) {
            if (!file.get(c)) {
                std::cout << '\n';
            }
            break;
        }

        if (c == delimiter) {
            std::cout << '\n';
            ++lines;
        } else {
            std::cout << c;
        }
    }

    file.close();

    return false;
}

int main(int argc, char* argv[]) {
    if (!ParseArguments(argc, argv)) {
        PrintFile();
    }
    return 0;
}
