#ifndef SEARCHRAMMEMORY_H
#define SEARCHRAMMEMORY_H

#include <vector>
#include "Person.h"

using namespace std;

class SearchRamMemory {

    public:
        SearchRamMemory(const string& filePath);

        void loadData();
        pair<Person, long> SearchByRuc(const string& ruc);

    private:
        mutex dataMutex;
        string filename;
    public:
        static vector<Person> persons;
};
#endif
