#ifndef FILEREADERRAM_H
#define FILEREADERRAM_H

#include "Person.h"
#include <string>
#include <mutex>
#include <vector>

using namespace std;

class FileReaderRam {
    public:
        FileReaderRam(const string& filename, size_t start, size_t end, mutex& dataMutex);

        void operator()(vector<Person>& data);

    private:
        const string filename;
        const size_t start;
        const size_t end;
        mutex& dataMutex;
    };

#endif
