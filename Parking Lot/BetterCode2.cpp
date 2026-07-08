#include <bits/stdc++.h>
using namespace std;

// ─────────────────────────────────────────────
//  Enums
// ─────────────────────────────────────────────
enum VehicleType { TWO_WHEELER = 2, FOUR_WHEELER = 4 };
enum ParkingStrategy { LOWEST_INDEX = 0, MAX_FREE_FLOOR = 1 };

// ─────────────────────────────────────────────
//  ParkingSpot
// ─────────────────────────────────────────────
class ParkingSpot {
public:
    int floor, row, col;
    int type;          // 2, 4, or 0 (inactive)
    bool occupied;
    string vehicleNumber;
    string ticketId;

    ParkingSpot() : floor(-1), row(-1), col(-1), type(0), occupied(false) {}
    ParkingSpot(int f, int r, int c, int t)
        : floor(f), row(r), col(c), type(t), occupied(false) {}

    string spotId() const {
        return to_string(floor) + "-" + to_string(row) + "-" + to_string(col);
    }

    bool isAvailable() const { return type != 0 && !occupied; }

    void park(const string& vNum, const string& tktId) {
        occupied    = true;
        vehicleNumber = vNum;
        ticketId    = tktId;
    }

    void vacate() {
        occupied      = false;
        vehicleNumber = "";
        ticketId      = "";
    }
};

// ─────────────────────────────────────────────
//  Floor
// ─────────────────────────────────────────────
class Floor {
public:
    int floorIndex;
    vector<vector<ParkingSpot>> spots;   // [row][col]

    // Per vehicle-type: ordered set of available spotIds
    // Key: vehicleType (2 or 4)
    map<int, set<pair<int,int>>> freeSpots;   // set of (row, col)
    map<int, int> freeCount;                  // vehicleType -> count

    Floor() {}
    Floor(int idx, const vector<vector<int>>& grid) : floorIndex(idx) {
        int rows = grid.size();
        int cols = grid[0].size();
        spots.resize(rows, vector<ParkingSpot>(cols));
        freeCount[2] = 0;
        freeCount[4] = 0;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                spots[r][c] = ParkingSpot(idx, r, c, grid[r][c]);
                if (grid[r][c] == 2 || grid[r][c] == 4) {
                    freeSpots[grid[r][c]].insert({r, c});
                    freeCount[grid[r][c]]++;
                }
            }
        }
    }

    // Returns spotId or "" if none available
    string assignSpot(int vehicleType, const string& vNum, const string& tktId) {
        auto& fs = freeSpots[vehicleType];
        if (fs.empty()) return "";

        auto it = fs.begin();           // lowest (row, col)
        int r = it->first, c = it->second;
        fs.erase(it);
        freeCount[vehicleType]--;

        spots[r][c].park(vNum, tktId);
        return spots[r][c].spotId();
    }

    bool vacateSpot(int row, int col) {
        ParkingSpot& sp = spots[row][col];
        if (!sp.occupied) return false;
        sp.vacate();
        freeSpots[sp.type].insert({row, col});
        freeCount[sp.type]++;
        return true;
    }

    int getFreeCount(int vehicleType) const {
        auto it = freeCount.find(vehicleType);
        return it != freeCount.end() ? it->second : 0;
    }
};

// ─────────────────────────────────────────────
//  ParkingLot
// ─────────────────────────────────────────────
class ParkingLot {
    vector<Floor> floors;

    // Lookup maps
    unordered_map<string, string> vehicleNumberToSpot;
    unordered_map<string, string> ticketIdToSpot;
    // spotId -> Floor index (for quick removal)
    unordered_map<string, int> spotToFloor;

    // Parse "f-r-c" into {floor, row, col}
    tuple<int,int,int> parseSpotId(const string& spotId) const {
        int d1 = spotId.find('-');
        int d2 = spotId.find('-', d1 + 1);
        int f = stoi(spotId.substr(0, d1));
        int r = stoi(spotId.substr(d1 + 1, d2 - d1 - 1));
        int c = stoi(spotId.substr(d2 + 1));
        return {f, r, c};
    }

    // Strategy 0: lowest index floor that has a free spot of given type
    string park0(int vehicleType, const string& vNum, const string& tktId) {
        for (auto& floor : floors) {
            if (floor.getFreeCount(vehicleType) > 0) {
                string sid = floor.assignSpot(vehicleType, vNum, tktId);
                if (sid != "") {
                    vehicleNumberToSpot[vNum]  = sid;
                    ticketIdToSpot[tktId]       = sid;
                    spotToFloor[sid]            = floor.floorIndex;
                    return sid;
                }
            }
        }
        return "";
    }

    // Strategy 1: floor with maximum free spots of given type (ties: lowest floor index)
    string park1(int vehicleType, const string& vNum, const string& tktId) {
        int bestFloor = -1, bestCount = -1;
        for (auto& floor : floors) {
            int cnt = floor.getFreeCount(vehicleType);
            if (cnt > bestCount) {
                bestCount = cnt;
                bestFloor = floor.floorIndex;
            }
        }
        if (bestFloor == -1 || bestCount == 0) return "";

        string sid = floors[bestFloor].assignSpot(vehicleType, vNum, tktId);
        if (sid != "") {
            vehicleNumberToSpot[vNum]  = sid;
            ticketIdToSpot[tktId]       = sid;
            spotToFloor[sid]            = bestFloor;
        }
        return sid;
    }

public:
    // parking[i][j][k]: type at floor i, row j, col k
    void init(const vector<vector<vector<int>>>& parking) {
        int numFloors = parking.size();
        floors.clear();
        floors.reserve(numFloors);
        for (int i = 0; i < numFloors; i++) {
            floors.emplace_back(i, parking[i]);
        }
    }

    string park(int vehicleType, const string& vehicleNumber,
                const string& ticketId, int parkingStrategy) {
        if (parkingStrategy == LOWEST_INDEX)
            return park0(vehicleType, vehicleNumber, ticketId);
        if (parkingStrategy == MAX_FREE_FLOOR)
            return park1(vehicleType, vehicleNumber, ticketId);
        return "";
    }

    bool removeVehicle(const string& spotId) {
        auto floorIt = spotToFloor.find(spotId);
        if (floorIt == spotToFloor.end()) return false;

        int floorIdx = floorIt->second;
        auto [f, r, c] = parseSpotId(spotId);

        ParkingSpot& sp = floors[floorIdx].spots[r][c];
        if (!sp.occupied) return false;

        // Clean up lookup maps
        vehicleNumberToSpot.erase(sp.vehicleNumber);
        ticketIdToSpot.erase(sp.ticketId);
        spotToFloor.erase(spotId);

        floors[floorIdx].vacateSpot(r, c);
        return true;
    }

    string searchVehicle(const string& query) const {
        auto it = vehicleNumberToSpot.find(query);
        if (it != vehicleNumberToSpot.end()) return it->second;
        it = ticketIdToSpot.find(query);
        if (it != ticketIdToSpot.end()) return it->second;
        return "";
    }

    int getFreeSpotsCount(int floor, int vehicleType) const {
        if (floor < 0 || floor >= (int)floors.size()) return -1;
        return floors[floor].getFreeCount(vehicleType);
    }
};

// ─────────────────────────────────────────────
//  Driver / Tests
// ─────────────────────────────────────────────
int main() {
    ParkingLot lot;

    vector<vector<vector<int>>> parking = {{
        {4, 4, 2, 2},
        {2, 4, 2, 0},
        {0, 2, 2, 2},
        {4, 4, 4, 0}
    }};

    lot.init(parking);

    cout << "Free 2W on floor 0: " << lot.getFreeSpotsCount(0, 2) << "\n"; // 7
    cout << "Free 4W on floor 0: " << lot.getFreeSpotsCount(0, 4) << "\n"; // 6

    string sid = lot.park(2, "bh234", "tkt4534", 0);
    cout << "Parked at: " << sid << "\n";                     // 0-0-2

    cout << "Search bh234:   " << lot.searchVehicle("bh234")   << "\n"; // 0-0-2
    cout << "Search tkt4534: " << lot.searchVehicle("tkt4534") << "\n"; // 0-0-2

    cout << "Free 2W after park: " << lot.getFreeSpotsCount(0, 2) << "\n"; // 6

    bool removed = lot.removeVehicle("0-0-2");
    cout << "Removed: " << (removed ? "true" : "false") << "\n";           // true

    cout << "Free 2W after remove: " << lot.getFreeSpotsCount(0, 2) << "\n"; // 7

    // Strategy 1 test
    string sid2 = lot.park(4, "car001", "tkt001", 1);
    cout << "Strategy 1 parked at: " << sid2 << "\n"; // lowest idx on floor w/ most 4W free

    return 0;
}