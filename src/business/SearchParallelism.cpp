#include "SearchParallelism.h"
#include "FileReader.h"
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

std::vector<Person> performParallelSearch() {
    FileReader reader;
    return reader.readPersonsFromFile("/run/media/lionos/Lion/2024-I/Parallel-Programming/unit-iii/data.txt");
}
