// Possible commands:

// create_parking_lot <parking_lot_id> <no_of_floors> <no_of_slots_per_floor>
// park_vehicle <vehicle_type> <reg_no> <color>
// unpark_vehicle <ticket_id>
// display <display_type> <vehicle_type>
// Possible values of display_type: free_count, free_slots, occupied_slots
// exit

#include <bits/stdc++.h>

using namespace std;

enum class VehicleType {
    Truck,
    Car,
    Bike
};

VehicleType parseVehicleType(const string& s) {
    if (s == "CAR") return VehicleType::Car;
    if (s == "BIKE") return VehicleType::Bike;
    if (s == "TRUCK") return VehicleType::Truck;
    // throw invalid_argument("Invalid vehicle type");
    cout<<s<<" ";
    return VehicleType::Truck;
}

class Vehicle {
public:
    string _reg_no;
    string _color;
    VehicleType _vehicleType;

    Vehicle(string reg_no, string color, VehicleType vehicleType) {
        _reg_no = reg_no;
        _color = color;
        _vehicleType = vehicleType;
    }
};

class ParkingSlot {
public:
    int _floorNo;
    int _slotNo;
    Vehicle* _vehicle;
    VehicleType _vehicleType;

    ParkingSlot(int floorNo, int slotNo, VehicleType vehicleType, Vehicle* vehicle = nullptr) {
        _floorNo = floorNo;
        _slotNo = slotNo;
        _vehicle = vehicle;
        _vehicleType = vehicleType;
    }
};

class ParkingFloor {
    int _floorNo;
    int _slots;
    unordered_map<Vehicle*, ParkingSlot*> _occupiedSlots;
    unordered_map<VehicleType, vector<ParkingSlot*>> _availableSlots;

public:
    ParkingFloor(int floorNo, int slots) {
        _floorNo = floorNo;
        _slots = slots;

        // Assume slots >= 3
        _availableSlots[VehicleType::Truck].push_back(new ParkingSlot(floorNo, 1, VehicleType::Truck));
        _availableSlots[VehicleType::Car].push_back(new ParkingSlot(floorNo, 2, VehicleType::Car));
        _availableSlots[VehicleType::Car].push_back(new ParkingSlot(floorNo, 3, VehicleType::Car));

        for(int i=4;i<=slots;i++) {
            _availableSlots[VehicleType::Bike].push_back(new ParkingSlot(floorNo, i, VehicleType::Bike));
        }
    }

    ParkingSlot* parkVehicle(Vehicle* vehicle) {
        if(_availableSlots[vehicle->_vehicleType].empty()) {
            return nullptr;
        }
        ParkingSlot* parkingSlot = _availableSlots[vehicle->_vehicleType].back();
        _availableSlots[vehicle->_vehicleType].pop_back();
        _occupiedSlots[vehicle] = parkingSlot;
        return parkingSlot;
    }

    bool unparkVehicle(Vehicle* vehicle) {
        ParkingSlot* parkingSlot = _occupiedSlots[vehicle];
        if(parkingSlot == nullptr) {
            return false;
        }
        _occupiedSlots.erase(vehicle);
        _availableSlots[vehicle->_vehicleType].push_back(parkingSlot);
        return true;
    }
};


class ParkingLot {
    string _id;
    int _floors, _slots_per_floor;
    vector<ParkingFloor*> _parkingFloors;

public:
    ParkingLot(string id, int floors, int slots_per_floor) {
        _id = id;
        _floors = floors;
        _slots_per_floor = slots_per_floor;
        
        for(int i=1;i<=floors;i++) {
            _parkingFloors.push_back(new ParkingFloor(i, slots_per_floor));
        }

        cout<<"Parking Lot created\n";
    }

    ParkingSlot* parkVehicle(Vehicle* vehicle) {
        for(int i=0;i<_floors;i++) {
            ParkingSlot* parkingSlot = _parkingFloors[i]->parkVehicle(vehicle);
            if(parkingSlot == nullptr) {
                continue;
            }
            return parkingSlot;
        }
        return nullptr;
    }

    void unparkVehicle(Vehicle* vehicle) {
        for(int i=0;i<_floors;i++) {
            bool unparked = _parkingFloors[i]->unparkVehicle(vehicle);
            if(unparked == true) {
                return ;
            }
        }
    }
};


// class Ticket {
// public:
//     int _id;
//     Vehicle* _vehicle;

//     Ticket(int id, Vehicle vehicle) {
//         _id = id;
//         _vehicle = vehicle;
//     }
// };


class TicketManager {
    string _parkingLotId;
    unordered_map<string, Vehicle*> _ticketToVehicle;
public:
    TicketManager(string parkingLotId) {
        _parkingLotId = parkingLotId;
    }

    string generateTicket(Vehicle* vehicle, ParkingSlot* parkingSlot) {
        string ticketId = _parkingLotId + '_' + to_string(parkingSlot->_floorNo) + '_' + to_string(parkingSlot->_slotNo);
        // Ticket* ticket = new Ticket(ticketId, vehicle);
        _ticketToVehicle[ticketId] = vehicle;
        return ticketId;
    }

    Vehicle* getVehicle(string ticketId) {
        return _ticketToVehicle[ticketId];
    }
};


int main() {
    string s;
    cin>>s;
    ParkingLot *parkingLot = nullptr;
    TicketManager *ticketManager = nullptr;
    if(s == "create_parking_lot") {
        string parking_lot_id;
        int no_of_floors, no_of_slots_per_floor;
        cin>>parking_lot_id>>no_of_floors>>no_of_slots_per_floor;
        parkingLot = new ParkingLot(parking_lot_id, no_of_floors, no_of_slots_per_floor);
        ticketManager = new TicketManager(parking_lot_id);
    }
    else {
        cout<<"Invalid request\n";
        return 0;
    }
    while (1) {
        cout<<endl;
        cin>>s;
        if(s == "exit") {
            break;
        }
        if(s == "park_vehicle") {
            string type, reg_no;
            string color;
            cin>>type>>reg_no>>color;
            VehicleType vehicle_type = parseVehicleType(type);
            Vehicle *vehicle = new Vehicle(reg_no, color, vehicle_type);

            // Park
            ParkingSlot* parkingSlot = parkingLot->parkVehicle(vehicle);
            if(parkingSlot == nullptr) {
                cout<<"Parking Lot is full\n";
                continue;
            } 
            string ticketId = ticketManager->generateTicket(vehicle, parkingSlot);
            cout<<"Parked vehicle. Ticket ID: "<<ticketId;
        }
        else if(s == "unpark_vehicle") {
            string ticket_id;
            cin>>ticket_id;
            Vehicle* vehicle = ticketManager->getVehicle(ticket_id);
            if(vehicle == nullptr) {
                cout<<"Invalid Ticket\n";
                continue;
            }
            parkingLot->unparkVehicle(vehicle);
            cout<<"Unparked vehicle with Registration Number: "<<vehicle->_reg_no;
        }
        else if(s == "display") {
            // To be code:
        }
        else {
            cout<<"Invalid request";
        }
        cout<<'\n';
    }
    
    return 0;
}