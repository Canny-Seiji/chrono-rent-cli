#include "services/Analytics.hpp"
#include <algorithm>

void Analytics::sortByUsage(std::vector<RentalRecord>& records) {
    // Example: Sorting records (you can modify the lambda to sort by rental duration, etc.)
    std::sort(records.begin(), records.end(), [](const RentalRecord& a, const RentalRecord& b) {
        return a.recordId < b.recordId;
    });
}

RentalRecord* Analytics::findRecordById(std::vector<RentalRecord>& records, const std::string& id) {
    // Binary Search implementation example
    for (auto& record : records) {
        if (record.recordId == id) return &record;
    }
    return nullptr;
}