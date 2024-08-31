#include "FileReaderRam.h"
#include "SearchRamMemory.h"
#include <fstream>
#include <iostream>
#include <thread>

using namespace std;

vector<Person> SearchRamMemory::persons;

SearchRamMemory::SearchRamMemory() = default;

long SearchRamMemory::loadData() {
    string filename = "/home/lionos/Documents/padron_reducido_ruc.txt";

    ifstream file(filename, ios::ate | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        return 0;
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

    return persons.size();
}
