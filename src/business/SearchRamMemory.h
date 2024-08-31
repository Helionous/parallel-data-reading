#ifndef SEARCHRAMMEMORY_H
#define SEARCHRAMMEMORY_H

#include <vector>
#include "Person.h"

using namespace std;

class SearchRamMemory {

    public:
        SearchRamMemory();

        void loadData();
        pair<Person, long> SearchByRuc(const string& ruc);

    private:
        mutex dataMutex;
    public:
        static vector<Person> persons;
};
#endif
