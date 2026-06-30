#ifndef CUSTOMER_MANAGER_HPP
#define CUSTOMER_MANAGER_HPP

#include <vector>
#include <algorithm>
#include "models/Customer.hpp"

class CustomerManager {
private:
    std::vector<Customer> customers;

public:
    void addCustomer(const Customer& c);
    std::string generateUniqueId(const std::string& f, const std::string& m, const std::string& l);
    
    // Sorting: 1 = Latest, 2 = Currently Renting, 3 = Rate (High-Low)
    void sortCustomers(int criteria);
    
    void displayReport() const;
    std::vector<Customer>& getCustomers(); 
};

#endif