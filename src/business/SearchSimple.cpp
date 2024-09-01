#include "SearchSimple.h"
#include "DetermineTime.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

SearchSimple::SearchSimple(const string& filePath) : filename(filePath) {}

Person SearchSimple::dataReading(const string& ruc) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        return Person();
    }

    constexpr size_t bufferSize = 4096; //4KB
    char buffer[bufferSize];
    string bufferContent;

    while (file.read(buffer, bufferSize) || file.gcount() > 0) {
        bufferContent.append(buffer, file.gcount());

        stringstream ss(bufferContent);
        string line;
        while (getline(ss, line)) {
            size_t pos = 0;
            vector<string> tokens;
            while ((pos = line.find('|')) != string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
            }
            tokens.push_back(line);

            if (tokens.size() >= 4 && tokens[0] == ruc) {
                file.close();
                return Person(tokens[0], tokens[1], tokens[2], tokens[3]);
            }
        }

        bufferContent.clear();
    }

    file.close();
    return Person();
}

pair<Person, long> SearchSimple::searchSimpleByRuc(const string& ruc) {
    long startTime = DetermineTime::getCurrentMillisecondsTime();
    Person result = dataReading(ruc);
    long elapsedTime = DetermineTime::getMillisecondsPassed(startTime);

    return {result, elapsedTime};
}
