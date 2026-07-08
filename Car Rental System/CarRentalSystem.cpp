#include <bits/stdc++.h>
using namespace std;

struct Car {
    int costPerDay, freeKmsPerDay, costPerKm;
};

struct Booking {
    string carPlate, fromDate, tillDate;
};

class CarRentalSystem {
    unordered_map<string, Car> cars;
    unordered_map<string, set<vector<string>>> carOrders;  // carLicensePlate -> {fromDate, tillDate, orderId}
    unordered_map<string, int> startReading;
    unordered_map<string, Booking> orders;

    int calculateDays(const string& startDate, const string& endDate) {
        int start = (startDate[8] - '0')*10 + startDate[9] - '0';
        int end = (endDate[8] - '0')*10 + endDate[9] - '0';
        return end - start + 1;
    }

public:
    void addCar(string licensePlate, int costPerDay, int freeKmsPerDay, int costPerKm) {
        if(cars.find(licensePlate) != cars.end()) {
            return;
        }
        cars[licensePlate] = {costPerDay, freeKmsPerDay, costPerKm};
    }

    bool bookCar(string orderId, string carLicensePlate, string fromDate, string tillDate) {
        if(cars.find(carLicensePlate) == cars.end()) {
            return false;
        }

        auto collisionOrder = carOrders[carLicensePlate].lower_bound({fromDate, "", ""});
        if(collisionOrder != carOrders[carLicensePlate].end() && tillDate >= (*collisionOrder)[0]) {
            return false;
        }

        if(collisionOrder != carOrders[carLicensePlate].begin()) {
            collisionOrder--;
            if(fromDate <= (*collisionOrder)[1]) {
                return false;
            }
        }

        carOrders[carLicensePlate].insert({fromDate, tillDate, orderId});
        orders[orderId] = {carLicensePlate, fromDate, tillDate};

        return true;
    }

    void startTrip(string orderId, int odometerReading) {
        startReading[orderId] = odometerReading;
    }

    int endTrip(string orderId, int finalOdometerReading, string endDate) {
        Booking& booking = orders[orderId];
        string carLicensePlate = booking.carPlate;
        string startDate = booking.fromDate;
        string effectiveEndDate = booking.tillDate;

        int chargeableDays = calculateDays(startDate, effectiveEndDate);

        if(endDate != effectiveEndDate) {
            carOrders[carLicensePlate].erase({startDate, effectiveEndDate, orderId});

            effectiveEndDate = endDate;
            carOrders[carLicensePlate].insert({startDate, effectiveEndDate, orderId});
            booking.tillDate = effectiveEndDate;
        }

        chargeableDays = max(chargeableDays, calculateDays(startDate, effectiveEndDate));
        int distance = finalOdometerReading - startReading[orderId];

        Car& car = cars[carLicensePlate];
        int totalCost = chargeableDays * car.costPerDay;
        int extraDistance = distance - chargeableDays * car.freeKmsPerDay;

        if(extraDistance > 0) {
            totalCost += extraDistance * car.costPerKm;
        }

        return totalCost;
    }
};

int main() {
    return 0;
}