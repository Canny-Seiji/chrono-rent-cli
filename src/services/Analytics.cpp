#include "../../include/services/Analytics.hpp"

void Analytics::sortByUsage(std::list<RentalRecord>& records) {
    records.sort([](const RentalRecord& a, const RentalRecord& b) {
        return a.recordId < b.recordId;
    });
}

RentalRecord* Analytics::findRecordById(std::list<RentalRecord>& records, const std::string& id) {
    for (auto& record : records) {
        if (record.recordId == id) return &record;
    }
    return nullptr;
}
