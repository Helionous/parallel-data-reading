#ifndef PARALLELFILEREADER_H
#define PARALLELFILEREADER_H

#include "Person.h"
#include <string>
#include <mutex>
#include <atomic>

using namespace std;

class ParallelFileReader {
    public:
        ParallelFileReader(const string& filename, const string& ruc, atomic<bool>& found, Person& result, mutex& resultMutex);
        void operator()(size_t start, size_t end);

    private:
        const string& filename;
        const string& ruc;
        atomic<bool>& found;
        Person& result;
        mutex& resultMutex;
};

#endif
