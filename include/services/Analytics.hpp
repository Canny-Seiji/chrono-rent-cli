#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "models/RentalRecord.hpp"
#include <vector>

class Analytics {
public:
    // Uses a sorting algorithm to rank by performance/usage
    static void sortByUsage(std::vector<RentalRecord>& records);
    // Uses a searching algorithm to find a specific transaction
    static RentalRecord* findRecordById(std::vector<RentalRecord>& records, const std::string& id);
};
#endif