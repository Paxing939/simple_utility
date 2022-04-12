#include <functional>
#include <iostream>
#include <fstream>
#include <string>

int get_batch_size(std::fstream &file, int batch_size) {
    int start = file.tellg();
    for (int i = 0; i < batch_size; ++i) {
        file.peek();
        if (!file.good()) {
            file.seekp(start);
            file.seekg(start);
            return i;
        }
        file.seekg(start + i + 1);
    }
    file.seekp(start);
    file.seekg(start);
    return batch_size;
}

std::string XOR(std::string line, const std::string &password, int& j) {
    for (int i = 0; i < line.size(); ++i, ++j) {
        if (j == password.size()) {
            j = 0;
        }
        line[i] ^= password[j];
    }
    return line;
}

void EncryptFile(const std::string &filename, const std::string &password,
                 const std::function<std::string(std::string, const std::string &)> &crypt_function = XOR) {
    std::fstream file(filename, std::ios::out | std::ios::in);
    if (!file.is_open()) {
        std::cout << "File is not open!" << std::endl;
        return;
    }

    int batch_size = password.size() < 10 ? 10 : static_cast<int>(password.size()), j = 0;
    bool first_time = true;
    std::string tmp_line;
    while (!file.eof()) {
        std::string line(batch_size, '\0');

        int actual = file.readsome(&line[0], batch_size);
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
            int actual = file.readsome(&tmp_line[0], a);
            tmp_line.resize(actual);
        }

        int tellg = file.tellg();
        int tellp = file.tellp();
        file.seekg(tellg - static_cast<int>(new_line.size()));
        file.seekp(tellp - static_cast<int>(new_line.size()));
        file << new_line;

        if (!file.peek()) {
            break;
        }
    }
    file.close();
}

int main() {
    EncryptFile("a.txt", "globant");
    return 0;
}
