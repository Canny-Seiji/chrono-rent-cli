# ChronoRent CLI Guide

ChronoRent is a C++ console application for managing a small vehicle rental fleet. It supports renting vehicles, returning vehicles, adding/removing vehicles, viewing the fleet, viewing customers, and auto-saving reports on exit.

## Build and Run

Requirements:
- C++17 compiler, such as `g++`
- `mingw32-make` on Windows

Commands:

```powershell
mingw32-make
.\bin\ChronoRent.exe
```

Optional:

```powershell
mingw32-make run
```

Clean build files:

```powershell
mingw32-make clean
```

## Main Menu Flow

The entry point is `src/main.cpp`.

Program startup:
1. `Inventory::loadFromFile("data/fleet.txt")` loads all vehicles.
2. `RentalManager::loadRentals("data/rentals.txt", myFleet)` loads active rentals.
3. Active rentals are copied into `CustomerManager` so they appear in View Customers.

Menu options:

| Option | Action | Main Classes Used |
| --- | --- | --- |
| `1` | Rent Vehicle | `Parser`, `Inventory`, `CustomerManager`, `RentalManager` |
| `2` | View Fleet | `Inventory` |
| `3` | Return Vehicle | `Parser`, `RentalManager`, `CustomerManager` |
| `4` | View Customers | `CustomerManager`, `Analytics` |
| `5` | Add Vehicle | `Parser`, `Inventory`, `Car` |
| `6` | Remove Vehicle | `Parser`, `Inventory` |
| `0` | Exit and save | `Analytics`, `Inventory`, `RentalManager` |

Program shutdown:
1. Active rental records are sorted with `Analytics::sortByUsage`.
2. Fleet data is saved to `data/fleet.txt`.
3. Active rentals are saved to `data/rentals.txt`.
4. Customer dashboard/report is auto-saved to `data/customer_report.txt`.

## Data Flow

### Rent Vehicle

1. User selects option `1`.
2. `Parser::getExistingPlate` asks for a plate and checks that it exists in the fleet.
3. The app checks `Vehicle::getRentedStatus`.
4. Customer details are collected through parser validation functions.
5. `CustomerManager::generateUniqueId` creates a unique customer ID.
6. A `Customer` is created and its `RentalInfo` is marked active.
7. The selected vehicle is marked rented with `Vehicle::setRentedStatus(true)`.
8. The customer is added to `CustomerManager`.
9. A `RentalRecord` is added to `RentalManager`.

Important result:
- Active customer appears in View Customers.
- Vehicle status becomes rented/unavailable.
- Estimated charge is calculated from rental days and rate.

### Return Vehicle

1. User selects option `3`.
2. `Parser::getExistingPlate` validates the plate.
3. `RentalManager::returnVehicle` finds the active rental record.
4. It calculates total charge with `RentalInfo::calculateCurrentCharge`.
5. Vehicle status is set back to available.
6. Rental record is removed from the active rental list.
7. `CustomerManager::endRentalByPlate` stores the completed charge on the customer.
8. Customer becomes inactive and the vehicle pointer is cleared.

Important result:
- Regular `Revenue` becomes `0.00` if no rentals are active.
- `Net Revenue` still keeps accumulated completed charges.
- Inactive customers keep their final charge in the customer list.

### View Customers

1. User selects option `4`.
2. `CustomerManager::displayDashboard` prints:
   - Number of cars
   - Number of customers
   - Number of active customers
   - Revenue
   - Net Revenue
3. `CustomerManager::displayReport` prints the customer table.

Revenue meanings:
- `Revenue` is the current charge from active rentals only.
- `Net Revenue` is the accumulated total of all customer charges, including returned/inactive customers.

### Exit and Auto-Save

When the user selects `0`, the program saves:
- Fleet list to `data/fleet.txt`
- Active rentals to `data/rentals.txt`
- Customer dashboard and customer table to `data/customer_report.txt`

## Important Classes and Functions

### `Vehicle` and `Car`

Files:
- `include/models/Vehicle.hpp`
- `src/models/Vehicle.cpp`

Purpose:
- Represents a vehicle in the fleet.
- Stores model, plate number, daily rate, and rented status.

Key functions:
- `getModel()` returns the model name.
- `getPlate()` returns the plate number.
- `getRate()` returns the daily rental rate.
- `getRentedStatus()` checks if the vehicle is rented.
- `setRentedStatus(bool)` updates availability.
- `calculateLateFee(int hours)` calculates late fees.

Current late fee rule:
- `Car::calculateLateFee` returns `hours * 15.0`.

### `Customer` and `RentalInfo`

Files:
- `include/models/Customer.hpp`
- `src/models/Customer.cpp`

Purpose:
- `Customer` stores customer identity and contact details.
- `RentalInfo` stores rental state for that customer.

Key functions:
- `Customer::getFullName()` combines first, middle, and last name without extra spaces.
- `RentalInfo::getHoursRemaining()` returns remaining rental time.
- `RentalInfo::getHoursOverdue()` returns overdue time.
- `RentalInfo::getRentalDays()` calculates charged rental days.
- `RentalInfo::calculateCurrentCharge()` calculates active rental charge.
- `RentalInfo::getTotalCharge()` returns completed charge plus current charge.

Important field:
- `completedCharge` stores returned rental totals so inactive customers do not show `0.00`.

### `RentalRecord`

Files:
- `include/models/RentalRecord.hpp`
- `src/models/RentalRecord.cpp`

Purpose:
- Represents one active rental transaction.
- Connects a transaction ID, customer, and vehicle.

Used by:
- `RentalManager` for active rentals.
- `Analytics::sortByUsage`.

### `Inventory`

Files:
- `include/services/Inventory.hpp`
- `src/services/Inventory.cpp`

Purpose:
- Manages the fleet list.

Key functions:
- `addVehicle(Vehicle* v)` adds a vehicle.
- `removeVehicle(const std::string& plate)` removes a vehicle by plate.
- `findVehicle(const std::string& plate)` returns the matching vehicle pointer.
- `displayFleet()` prints the fleet table.
- `getVehicleCount()` returns fleet size.
- `saveToFile(...)` writes fleet data.
- `loadFromFile(...)` reads fleet data.

Fleet file format:

```text
PLATE|MODEL|RATE|IS_RENTED
```

Example:

```text
FEU1234|BYD Sealion 7|11000|0
```

### `RentalManager`

Files:
- `include/services/RentalManager.hpp`
- `src/services/RentalManager.cpp`

Purpose:
- Manages active rental records.

Key functions:
- `processRental(...)` rents a vehicle using the older rental flow.
- `addRentalRecord(const RentalRecord& record)` adds an active rental record.
- `returnVehicle(const std::string& plate)` returns a rented vehicle and removes its active rental record.
- `saveRentals(...)` writes active rentals.
- `loadRentals(...)` reads active rentals and reconnects them to fleet vehicles.
- `getActiveRentals()` exposes the active rentals list.

Rental file format:

```text
TRANSACTION_ID|CUSTOMER_FULL_NAME|PLATE
```

Example:

```text
TX-001|Juan Santos Cruz|FEU1234
```

### `CustomerManager`

Files:
- `include/services/CustomerManager.hpp`
- `src/services/CustomerManager.cpp`

Purpose:
- Stores customers shown in the dashboard and customer report.

Key functions:
- `addCustomer(const Customer& c)` adds a customer.
- `generateUniqueId(...)` creates a unique customer ID.
- `sortCustomers(int criteria)` sorts customers when called.
- `displayDashboard(int vehicleCount)` prints dashboard totals.
- `displayReport()` prints the customer table.
- `endRentalByPlate(const std::string& plate)` marks a customer inactive and saves completed charge.
- `contactExists(const std::string& contact)` prevents duplicate contact numbers.
- `getCustomers()` returns the customer list for analytics/autosave.

Customer table columns:
- `ID`
- `Name`
- `Plate`
- `Status`
- `Remaining(hrs)`
- `Rate`
- `Charge`

### `Analytics`

Files:
- `include/services/Analytics.hpp`
- `src/services/Analytics.cpp`

Purpose:
- Provides calculations and report-saving helpers.

Key functions:
- `sortByUsage(std::list<RentalRecord>& records)` sorts active rentals by transaction ID.
- `findRecordById(...)` searches for a transaction.
- `calculateRevenue(...)` totals active rental charges only.
- `calculateNetRevenue(...)` totals all customer charges, including completed rentals.
- `autoSaveCustomerReport(...)` writes the dashboard and customer list to `data/customer_report.txt`.

### `Parser`

Files:
- `include/utils/Parser.hpp`
- `src/utils/Parser.cpp`

Purpose:
- Handles validation, cleanup, and user input.

Key function groups:
- File parsing: `split`
- String cleanup: `trim`, `toUpper`, `toLower`
- Validation: `isValidPlate`, `isValidLicense`, `isValidContact`
- Input prompts: `getValidInt`, `getValidDouble`, `getValidPlate`, `getValidName`, `getValidLicense`, `getValidContact`, `getExistingPlate`
- ID generation: `generateCustomerId`, `findUniqueId`

Current input rules:
- Plate: 7 characters, 3 letters followed by 4 digits, example `ABC1234`
- License: 11 characters, 1 letter followed by 10 digits, example `A1234567890`
- Contact: 11 digits, example `09123456789`
- Rate: minimum `8000.00`
- Rental duration: `1` to `365` days

### `Colors`

File:
- `include/utils/Colors.hpp`

Purpose:
- Stores ANSI color constants used by console output.

Common usage:
- Yellow for headers and labels.
- Green for successful/positive values.
- Red for errors, inactive status, or negative values.

## Data Files

### `data/fleet.txt`

Stores all vehicles.

Format:

```text
PLATE|MODEL|RATE|IS_RENTED
```

`IS_RENTED` values:
- `0` means available.
- `1` means rented.

### `data/rentals.txt`

Stores active rentals only.

Format:

```text
TRANSACTION_ID|CUSTOMER_FULL_NAME|PLATE
```

When a vehicle is returned, the rental is removed from this file on the next save.

### `data/customer_report.txt`

Auto-generated when the app exits.

Contains:
- Dashboard totals
- Customer list table

This file is a report output, not the main source of active rental data.

## Notes for Groupmates

- `Revenue` and `Net Revenue` are intentionally different:
  - `Revenue` is for active rentals.
  - `Net Revenue` accumulates active plus completed customer charges.
- Returned customers become inactive but keep their final charge.
- `bin/` contains build output and is usually ignored by Git.
- If the app builds on one computer but not another, run `mingw32-make clean` then `mingw32-make`.
- If Git says local files would be overwritten by pull, check for generated files such as `data/customer_report.txt` before pulling.

