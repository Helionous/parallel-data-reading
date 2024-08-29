#include "ParallelFileReader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

ParallelFileReader::ParallelFileReader(const string& filename, const string& ruc, atomic<bool>& found, Person& result, mutex& resultMutex)
    : filename(filename), ruc(ruc), found(found), result(result), resultMutex(resultMutex) {}

void ParallelFileReader::operator()(size_t start, size_t end) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        return;
    }

    file.seekg(start);

    string line;
    while (getline(file, line) && file.tellg() <= static_cast<streampos>(end) && !found) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 4 && tokens[0] == ruc) {
            lock_guard<mutex> lock(resultMutex);
            if (!found) {
                result = Person(tokens[0], tokens[1], tokens[2], tokens[3]);
                found = true;
            }
            break; // Salir temprano si se encuentra
        }
    }

    file.close();
}
