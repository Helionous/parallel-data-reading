#include "FileReaderRam.h"
#include "SearchRamMemory.h"
#include "DetermineTime.h"
#include "Person.h"
#include <fstream>
#include <iostream>
#include <thread>

using namespace std;

vector<Person> SearchRamMemory::persons;

SearchRamMemory::SearchRamMemory() = default;

void SearchRamMemory::loadData() {
    string filename = "/home/lionos/Documents/padron_reducido_ruc.txt";

    ifstream file(filename, ios::ate | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
    }

    size_t fileSize = file.tellg();
    file.close();

    size_t numThreads = 8;
    size_t chunkSize = fileSize / numThreads;

    vector<thread> threads;

    for (size_t i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? fileSize : start + chunkSize;

        threads.emplace_back(&FileReaderRam::operator(), FileReaderRam(filename, start, end, dataMutex), ref(persons));
    }

    for (auto& t : threads) {
        t.join();
    }
}

pair<Person, long> SearchRamMemory::SearchByRuc(const string& ruc)
{
    Person result;
    bool found = false;

    long startTime = DetermineTime::getCurrentMillisecondsTime();

    size_t numThreads = 8;
    size_t chunkSize = persons.size() / numThreads;
    vector<thread> threads;
    vector<Person> results(numThreads);
    vector<bool> foundFlags(numThreads, false);

    auto searchChunk = [&ruc, &results, &foundFlags](size_t start, size_t end, size_t index) {
        for (size_t i = start; i < end; ++i) {
            if (persons[i].getRuc() == ruc) {
                results[index] = persons[i];
                foundFlags[index] = true;
                break;
            }
        }
    };

    for (size_t i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? persons.size() : start + chunkSize;

        threads.emplace_back(searchChunk, start, end, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    for (size_t i = 0; i < numThreads; ++i) {
        if (foundFlags[i]) {
            result = results[i];
            found = true;
            break;
        }
    }

    long long elapsedTime = DetermineTime::getMillisecondsPassed(startTime);
    return {result, elapsedTime};
}
