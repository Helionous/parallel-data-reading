#ifndef SEARCH_PARALLELISM_H
#define SEARCH_PARALLELISM_H

#include <string>
#include "Person.h"

using namespace std;

class SearchParallelism {
    public:
        SearchParallelism(const string& filePath);

        pair<Person, long> performParallelSearch(const string& ruc);

    private:
        const string filename;
};
#endif
