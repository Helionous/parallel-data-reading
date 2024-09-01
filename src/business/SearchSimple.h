#ifndef SEARCHSIMPLE_H
#define SEARCHSIMPLE_H

#include "Person.h"
#include <string>
#include <utility>

using namespace std;

class SearchSimple {
    public:
        SearchSimple(const string& filePath);

        Person dataReading(const string& ruc);
        pair<Person, long> searchSimpleByRuc(const string& ruc);

    private:
        string filename;
};

#endif
