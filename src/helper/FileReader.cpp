#include "FileReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<Person> FileReader::readPersonsFromFile(const std::string& filename) {
    std::vector<Person> persons;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return persons;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 4) {
            Person person(tokens[0], tokens[1], tokens[2], tokens[3]);
            persons.push_back(person);
        } else {
            std::cerr << "Warning: Line format incorrect, skipping line: " << line << std::endl;
        }
    }

    file.close();
    return persons;
}
