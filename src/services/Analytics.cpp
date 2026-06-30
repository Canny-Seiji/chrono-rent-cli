#include "services/Analytics.hpp"
#include <algorithm>

void Analytics::sortByUsage(std::vector<RentalRecord>& records) {
    std::sort(records.begin(), records.end(), [](const RentalRecord& a, const RentalRecord& b) {
        return a.recordId < b.recordId;
    });
}

RentalRecord* Analytics::findRecordById(std::vector<RentalRecord>& records, const std::string& id) {
    for (auto& record : records) {
        if (record.recordId == id) return &record;
    }
    return nullptr;
}