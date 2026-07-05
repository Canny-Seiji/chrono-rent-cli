#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "../models/RentalRecord.hpp"
#include <list>

class Analytics {
public:
    // Uses a sorting algorithm to rank by performance/usage
    static void sortByUsage(std::list<RentalRecord>& records);
    // Uses a searching algorithm to find a specific transaction
    static RentalRecord* findRecordById(std::list<RentalRecord>& records, const std::string& id);
};
#endif
