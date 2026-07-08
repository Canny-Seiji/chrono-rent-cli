#include "../../include/services/Analytics.hpp"

// Sorts the rental records by their recordId in ascending order
void Analytics::sortByUsage(std::list<RentalRecord>& records) {
    records.sort([](const RentalRecord& a, const RentalRecord& b) {
        return a.recordId < b.recordId;
    });
}

// Finds a rental record by its ID and returns a pointer to it, or nullptr if not found
RentalRecord* Analytics::findRecordById(std::list<RentalRecord>& records, const std::string& id) {
    for (auto& record : records) {
        if (record.recordId == id) return &record;
    }
    return nullptr;
}
