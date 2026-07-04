#ifndef CUSTOMER_MANAGER_HPP
#define CUSTOMER_MANAGER_HPP

#include <list>
#include <algorithm>
#include "../models/Customer.hpp"

class CustomerManager {
private:
    std::list<Customer> customers;

public:
    void addCustomer(const Customer& c);
    std::string generateUniqueId(const std::string& f, const std::string& m, const std::string& l);
    
    // Sorting: 1 = Name, 2 = Status, 3 = Rental Expense
    void sortCustomers(int criteria);
    
    void displayReport() const;
    void displayDashboard(int vehicleCount) const;
    void endRentalByPlate(const std::string& plate);
    std::list<Customer>& getCustomers(); 
};

#endif
