#include <functional>
#include <iostream>
#include <fstream>
#include <string>

std::string XOR(std::string line, const std::string &password, int& j) {
    for (int i = 6; i < line.size(); ++i, ++j) {
        if (j == password.size()) {
            j = 0;
        }
        line[i] ^= password[j];
    }
    return line;
}

int main() {
    char a[] = {'\001', 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf,
                0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf,
                0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf,};
    std::string ab(24, '\0');
    std::fstream out("a.txt");
    out.readsome(&ab[0], 24);
    int j = 0;
    ab = XOR(ab, "globant", j);
    return 0;
}
