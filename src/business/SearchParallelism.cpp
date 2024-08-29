#include "DetermineTime.h"
#include "Person.h"
#include "ParallelFileReader.h"
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <fstream>
#include <iostream>

using namespace std;

pair<Person, long long> performParallelSearch(const string& ruc) {
    string filename = "/home/lionos/Documents/padron_reducido_ruc.txt";
    long long startTime = DetermineTime::getCurrentMillisecondsTime();

    Person result;
    atomic<bool> found(false);
    mutex resultMutex;

    size_t numThreads = 8;
    vector<thread> threads;
    size_t fileSize;

    ifstream file(filename, ios::ate | ios::binary);
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

    long long elapsedTime = DetermineTime::getMillisecondsPassed(startTime);

    return {result, elapsedTime};
}
