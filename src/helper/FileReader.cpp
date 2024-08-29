#include "FileReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

vector<Person> FileReader::readPersonsFromFile(const string& filename) {
    vector<Person> persons;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo." << filename << endl;
        return persons;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 4) {
            Person person(tokens[0], tokens[1], tokens[2], tokens[3]);
            persons.push_back(person);
        } else {
            cerr << "Warning: Formato de línea incorrecto, saltando línea: " << line << endl;
        }
    }

    file.close();
    return persons;
}
