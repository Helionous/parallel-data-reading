#ifndef SEARCHRAMMEMORY_H
#define SEARCHRAMMEMORY_H

#include <vector>
#include "Person.h"

using namespace std;

class SearchRamMemory {
    static vector<Person> persons;

    public:
        SearchRamMemory();
        long loadData();

    private:
        mutex dataMutex;
};
#endif
