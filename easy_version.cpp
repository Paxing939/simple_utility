#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>

std::string XOR(std::string line, const std::string &password, int &j) {
    for (int i = 0; i < line.size(); ++i, ++j) {
        if (j == password.size()) {
            j = 0;
        }
        line[i] ^= password[j];
    }
    return line;
}

void EncryptDecryptFile(char mode, const std::string &filename, const std::string &password, const std::string &prefix,
                 const std::function<std::string(std::string, const std::string &, int &)> &crypt_function = XOR) {
    std::ifstream input_file(filename, std::ios::binary);
    if (!input_file.is_open()) {
        std::cout << "File is not open!" << std::endl;
        return;
    }

    int j = 0;
    std::vector<std::string> lines;
    if (mode == 0) {
        lines.push_back(prefix);
    } else if (mode == 1) {
        input_file.seekg(prefix.size());
    }
    while(!input_file.eof()) {
        std::string line;
        std::getline(input_file, line);
        if (!line.empty()) {
            if (!input_file.eof())
                line += '\n';
            line = XOR(move(line), password, j);
            lines.push_back(move(line));
        }
    }
    input_file.close();

    std::ofstream output_file(filename, std::ios::binary);
    for(const auto &line : lines) {
        output_file << line;
    }
    output_file.close();
}

int main(int argc, char *argv[]) {
//    if (argc < 5) {
//        std::cout << "Not enough arguments!" << std::endl;
//    } else if (strcmp(argv[1], "encrypt") == 0) {
//        EncryptDecryptFile(0, argv[2], argv[3], "BLABLA");
//    } else if (strcmp(argv[1], "decrypt") == 0) {
//        EncryptDecryptFile(1, argv[2], argv[3], "BLABLA");
//    } else {
//        std::cout << "Incorrect mode choice! Chose encrypt or decrypt" << std::endl;
//    }
    EncryptDecryptFile(0, "a.txt", "globant", "BLABLA");
    EncryptDecryptFile(1, "a.txt", "globant", "BLABLA");
    return 0;
}
