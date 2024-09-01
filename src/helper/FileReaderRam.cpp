#include "FileReaderRam.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

FileReaderRam::FileReaderRam(const string& filename, size_t start, size_t end, mutex& dataMutex)
    : filename(filename), start(start), end(end), dataMutex(dataMutex) {}

void FileReaderRam::operator()(vector<Person>& data) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        return;
    }

    file.seekg(start);
    string line;
    while (getline(file, line) && file.tellg() <= static_cast<streampos>(end)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 4) {
            lock_guard<mutex> lock(dataMutex);
            data.emplace_back(tokens[0], tokens[1], tokens[2], tokens[3]);
        }
    }

    file.close();
}
