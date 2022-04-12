#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

std::string XOR(std::string line, const std::string &password, int &j) {
    for (int i = 0; i < line.size(); ++i, ++j) {
        if (j == password.size()) {
            j = 0;
        }
        line[i] ^= password[j];
    }
    return line;
}

void EncryptFile(const std::string &filename, const std::string &password,
                 const std::function<std::string(std::string, const std::string &, int &)> &crypt_function = XOR) {
    std::fstream file(filename, std::ios::out | std::ios::in);
    if (!file.is_open()) {
        std::cout << "File is not open!" << std::endl;
        return;
    }

    int batch_size = password.size() < 10 ? 10 : static_cast<int>(password.size());
    int j = 0;
    int end_read = 0, begin_write = 0;
    bool first_time = true;
    std::string tmp_line;
    while (!file.eof()) {
        std::string line(batch_size, '\0');

        file.seekg(end_read);
        file.seekp(end_read);
        int actual = file.readsome(&line[0], batch_size);
        end_read += actual;
        line.resize(actual);

        if (!tmp_line.empty()) {
            line = tmp_line + line;
            tmp_line.resize(0);
        }
        std::string new_line = crypt_function(line, password, j);
        if (first_time) {
            new_line = "BLABLA" + new_line;
            first_time = false;
        }

        if (new_line.size() > line.size()) {
            int a = static_cast<int>(new_line.size()) - static_cast<int>(line.size());
            tmp_line.resize(a, '\0');
            file.seekg(end_read);
            file.seekp(end_read);
            int actual = file.readsome(&tmp_line[0], a);
            end_read += actual;
            tmp_line.resize(actual);
        }

        file.seekg(begin_write);
        file.seekp(begin_write);
        file.write(new_line.c_str(), new_line.size());
        begin_write += new_line.size();
        if (!file.peek()) {
            break;
        }
    }
    file.close();
}

void DecryptFile(const std::string &filename, const std::string &password,
                 const std::function<std::string(std::string, const std::string &, int &)> &crypt_function = XOR) {
    std::fstream file(filename, std::ios::out | std::ios::in);
    if (!file.is_open()) {
        std::cout << "File is not open!" << std::endl;
        return;
    }

    int batch_size = password.size() < 10 ? 10 : static_cast<int>(password.size());
    int j = 0, offset = 6;
    bool first_time = true;
    std::string tmp_line;
    int end_read = 6, begin_write = 0;
    while (!file.eof()) {
        std::string line(batch_size, '\0');

        file.seekg(end_read);
        file.seekp(end_read);
        int actual = file.readsome(&line[0], batch_size);
        end_read += actual;
        line.resize(actual);

        if (!tmp_line.empty()) {
            line = tmp_line + line;
            tmp_line.resize(0);
        }
        std::string new_line = crypt_function(line, password, j);
        if (new_line.size() > line.size()) {
            int a = static_cast<int>(new_line.size()) - static_cast<int>(line.size());
            tmp_line.resize(a, '\0');
            file.seekg(end_read);
            file.seekp(end_read);
            int actual = file.readsome(&tmp_line[0], a);
            end_read += actual;
            tmp_line.resize(actual);
        }

        file.seekg(begin_write);
        file.seekp(begin_write);
        file << new_line;
        begin_write += new_line.size();
        if (!file.peek()) {
            break;
        }
    }
    file.close();
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        std::cout << "Not enough arguments!" << std::endl;
    } else if (strcmp(argv[1], "encrypt") == 0) {
        EncryptFile(argv[2], argv[3]);
    } else if (strcmp(argv[1], "decrypt") == 0) {
        DecryptFile(argv[2], argv[3]);
    } else {
        std::cout << "Incorrect mode choice! Chose encrypt or decrypt" << std::endl;
    }
    return 0;
}
