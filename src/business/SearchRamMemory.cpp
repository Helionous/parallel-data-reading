#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>
#include <iostream>
#include "DetermineTime.h"
#include "FileReader.h"
#include "SearchParallelism.h"
#include "Frame.h"

using namespace std;
using namespace tbb;

class SearchRamMemory {

    public:
    SearchRamMemory(const vector<Person>& data, const string& ruc)
        : data(data), ruc(ruc), found(false) {}

    SearchRamMemory(SearchRamMemory& other, split)
        : data(other.data), ruc(other.ruc), found(false) {}

    void join(const SearchRamMemory& rhs) {
        if (rhs.found) {
            result = rhs.result;
            found = true;
        }
    }

    void operator()(const blocked_range<vector<Person>::const_iterator>& range) {
        for (auto it = range.begin(); it != range.end(); ++it) {
            if (it->getRuc() == ruc) {
                if (!found) {
                    result = *it;
                    found = true;
                }
                break; // Salir temprano si se encuentra
            }
        }
    }

    Person getResult() const {
        return result;
    }

    private:
        const vector<Person>& data;
        const string& ruc;
        Person result;
        bool found;
};


pair<Person, long long> performParallelMemorySearch(const string& ruc) {
    vector<Person> data = FileReader::readPersonsFromFile("/home/lionos/Documents/padron_reducido_ruc.txt");
    //vector<Person> data = FileReader::readPersonsFromFile("/home/lionos/Documents/data.txt");
    long long startTime = DetermineTime::getCurrentMillisecondsTime();

    SearchRamMemory search(data, ruc);
    parallel_reduce(blocked_range<vector<Person>::const_iterator>(data.begin(), data.end()), search);

    long long elapsedTime = DetermineTime::getMillisecondsPassed(startTime);
    return {search.getResult(), elapsedTime};
}