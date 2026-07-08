#include <bits/stdc++.h>
using namespace std;


class ParkingLot {
    int floors;
    vector<vector<vector<int>>> parking;
    vector<vector<vector<string>>> vehicleParked, ticket;
    vector<vector<int>> floorVehicle;
    unordered_map<string, string> ticketToSpot, vehicleNumberToSpot;
    vector<set<string>> freeSpots;

    vector<int> getLocation(const string& spotId) {
        int index1 = -1, index2 = -1;
        for(int i=0;i<spotId.length();i++) {
            if(spotId[i] == '-' && index1 != -1) {
                index2 = i;
                break;
            }
            else if(spotId[i] == '-') {
                index1 = i;
            }
        }

        return {stoi(spotId.substr(0, index1)), stoi(spotId.substr(index1 + 1, index2 - index1)), stoi(spotId.substr(index2 + 1, spotId.length()))};
    }

    void acquireSpot(const string& spotId, const string& vehicleNumber, const string& ticketId, int vehicleType) {
        vector<int> loc = getLocation(spotId);

        vehicleParked[loc[0]][loc[1]][loc[2]] = vehicleNumber;
        ticket[loc[0]][loc[1]][loc[2]] = ticketId;

        floorVehicle[loc[0]][vehicleType]--;

        ticketToSpot[ticketId] = spotId;
        vehicleNumberToSpot[vehicleNumber] = spotId;
    }

    string park0(int vehicleType, const string& vehicleNumber, const string& ticketId) {
        string spotId = "";
        for(int i=0;i<floors;i++) {
            if(freeSpots[i].empty()) {
                spotId = *freeSpots[i].begin();
                freeSpots[i].erase(*freeSpots[i].begin());
                break;
            }
        }

        if(spotId == "") {
            return "";
        }

        acquireSpot(spotId, vehicleNumber, ticketId, vehicleType);

        return spotId;
    }

    string park1(int vehicleType, const string& vehicleNumber, const string& ticketId) {
        int selectedFloor = -1, freeSpaces = 0;
        for(int i=0;i<floors;i++) {
            if(floorVehicle[i][vehicleType] > freeSpaces) {
                selectedFloor = i;
                freeSpaces = floorVehicle[i][vehicleType];
            }
        }

        if(selectedFloor == -1) {
            return "";
        }

        string spotId = *freeSpots[selectedFloor].begin();

        acquireSpot(spotId, vehicleNumber, ticketId, vehicleType);

        return spotId;
    }

public:
    void init(const vector<vector<vector<int>>>& inputParking) {
        parking = inputParking;
        floors = parking.size();
        int rows = parking[0].size();
        int columns = parking[0][0].size();

        vehicleParked.assign(floors, vector<vector<string>> (rows, vector<string> (columns, "")));
        ticket.assign(floors, vector<vector<string>> (rows, vector<string> (columns, "")));
        floorVehicle.assign(floors, vector<int> (5, 0));
        freeSpots.resize(floors);

        for(int i=0;i<floors;i++) {
            for(int j=0;j<rows;j++) {
                for(int k=0;k<columns;k++) {
                    if(parking[i][j][k] == 0) {
                        continue;
                    }
                    floorVehicle[i][parking[i][j][k]]++;
                    freeSpots[i].insert(to_string(i) + '-' + to_string(j) + '-' + to_string(k));
                }
            }
        }
    }

    string park(int vehicleType, const string& vehicleNumber, const string& ticketId, int parkingStrategy) {
        if(parkingStrategy == 0) {
            return park0(vehicleType, vehicleNumber, ticketId);
        }
        if(parkingStrategy == 1) {
            return park1(vehicleType, vehicleNumber, ticketId);
        }
        return "Invalid Parking Strategy\n";
    }

    bool removeVehicle(const string& spotId) {
        vector<int> loc = getLocation(spotId);
        if(vehicleParked[loc[0]][loc[1]][loc[2]] == "") {
            return false;
        }

        vehicleNumberToSpot.erase(vehicleParked[loc[0]][loc[1]][loc[2]]);
        ticketToSpot.erase(ticket[loc[0]][loc[1]][loc[2]]);

        vehicleParked[loc[0]][loc[1]][loc[2]] = "";
        ticket[loc[0]][loc[1]][loc[2]] = "";

        int vehicleType = parking[loc[0]][loc[1]][loc[2]];
        floorVehicle[loc[0]][vehicleType]++;

        return true;
    }

    string searchVehicle(const string& query) {
        auto spotIt = vehicleNumberToSpot.find(query);
        if(spotIt == vehicleNumberToSpot.end()) {
            spotIt = ticketToSpot.find(query);
            if(spotIt == ticketToSpot.end()) {
                return "";
            }
            return spotIt->second;
        }
        return spotIt->second;
    }

    int getFreeSpotsCount(int floor, int vehicleType) {
        return floorVehicle[floor][vehicleType];
    }
};


int main() {
    return 0;
}