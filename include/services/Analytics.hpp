#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "../models/RentalRecord.hpp"
#include "../models/Customer.hpp"
#include <list>
#include <string>

// Analytics class to provide analytical functionalities for rental records
class Analytics {
public:
    // Uses a sorting algorithm to rank by performance/usage
    static void sortByUsage(std::list<RentalRecord>& records);
    // Uses a searching algorithm to find a specific transaction
    static RentalRecord* findRecordById(std::list<RentalRecord>& records, const std::string& id);
    static double calculateRevenue(const std::list<Customer>& customers);
    static double calculateNetRevenue(const std::list<Customer>& customers);
    static void autoSaveCustomerReport(const std::string& filename, const std::list<Customer>& customers, int vehicleCount);
};
#endif
