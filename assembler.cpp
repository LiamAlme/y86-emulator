#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_filename>\n";
        return 1;
    }

    const char* filename = argv[1];
    ifstream file(filename);
    if (!file) {
        cerr << "Failed to open file: " << filename << "\n";
        return 1;
    }

    string line;
    while (std::getline(file, line)) {
        cout << line << "\n";
    }

    return 0;
}