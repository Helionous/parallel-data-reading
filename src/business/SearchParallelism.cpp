#include "SearchParallelism.h"
#include "FileReader.h"
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>

class RUCSearch {

public:
    RUCSearch(const std::vector<Person>& data, const std::string& ruc)
        : data(data), ruc(ruc), result(), found(false) {}

    RUCSearch(RUCSearch& other, tbb::split)
        : data(other.data), ruc(other.ruc), result(), found(false) {}

    void join(const RUCSearch& rhs) {
        if (rhs.found) {
            result = rhs.result;
            found = true;
        }
    }

    void operator()(const tbb::blocked_range<std::vector<Person>::const_iterator>& range) {
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
    const std::vector<Person>& data;
    const std::string& ruc;
    Person result;
    bool found;
};

Person performParallelSearch(const std::string& ruc) {
    FileReader reader;
    std::vector<Person> data = reader.readPersonsFromFile("/run/media/lionos/Lion/2024-I/Parallel-Programming/unit-iii/data.txt");

    RUCSearch search(data, ruc);
    tbb::parallel_reduce(
        tbb::blocked_range<std::vector<Person>::const_iterator>(data.begin(), data.end()),
        search
    );

    return search.getResult();
}
