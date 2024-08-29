#include "DetermineTime.h"
#include "FileReader.h"
#include "SearchParallelism.h"
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>
#include <QString>

using namespace std;
using namespace tbb;

class SearchParallellism {
    public:
    SearchParallellism(const vector<Person>& data, const string& ruc)
        : data(data), ruc(ruc), found(false) {}

    SearchParallellism(SearchParallellism& other, split)
        : data(other.data), ruc(other.ruc), found(false) {}

    void join(const SearchParallellism& rhs) {
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

pair<Person, long long> performParallelSearch(const string& ruc) {
    vector<Person> data = FileReader::readPersonsFromFile("/run/media/lionos/Lion/2024-I/Parallel-Programming/unit-iii/data.txt");

    long long startTime = DetermineTime::getCurrentMillisecondsTime();

    SearchParallellism search(data, ruc);
    parallel_reduce(
        blocked_range<vector<Person>::const_iterator>(data.begin(), data.end()),search
    );

    long long elapsedTime = DetermineTime::getMillisecondsPassed(startTime);

    return {search.getResult(), elapsedTime};
}
