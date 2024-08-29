#ifndef PERSONFILEREADER_H
#define PERSONFILEREADER_H

#include <vector>
#include <string>
#include "Person.h"

class FileReader {
    public:
    static std::vector<Person> readPersonsFromFile(const std::string& filename);
};

#endif
