#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <fstream>
#include <iostream>

#include "SearchParallelism.h"
#include "DetermineTime.h"
#include "Person.h"
#include "ParallelFileReader.h"
#include "GlobalVar.h"

using namespace std;

SearchParallelism::SearchParallelism(const string& filePath) : filename(filePath) {}

pair<Person, long> SearchParallelism::performParallelSearch(const string& ruc) {
    long startTime = DetermineTime::getCurrentMillisecondsTime();
    ifstream file(filename, ios::ate | ios::binary);

    Person result;
    atomic<bool> found(false);
    mutex resultMutex;

    size_t numThreads = numberOfThreads;
    vector<thread> threads;
    size_t fileSize;

    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo." << filename << endl;
        return {result, -1};
    }
    fileSize = file.tellg();
    file.close();

    size_t chunkSize = (fileSize + numThreads - 1) / numThreads;

    for (size_t i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = min(start + chunkSize, fileSize);
        threads.emplace_back(ParallelFileReader(filename, ruc, found, result, resultMutex), start, end);
    }

    for (auto& t : threads) {
        t.join();
    }

    long elapsedTime = DetermineTime::getMillisecondsPassed(startTime);

    return {result, elapsedTime};
}
