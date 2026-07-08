#include <bits/stdc++.h>
using namespace std;

class Helper {
public:
    void print(string s) { cout << s; }
    void println(string s) { cout << s << endl; }
};

struct Spot {
    int floor, row, col;
    int type; // 2 or 4
    bool occupied;

    Spot(int f, int r, int c, int t) {
        floor = f; row = r; col = c;
        type = t;
        occupied = false;
    }

    string getId() const {
        return to_string(floor) + "-" + to_string(row) + "-" + to_string(col);
    }
};

// Comparator for ordering spots by (floor, row, col)
struct SpotCompare {
    bool operator()(const Spot* a, const Spot* b) const {
        if (a->floor != b->floor) return a->floor < b->floor;
        if (a->row != b->row) return a->row < b->row;
        return a->col < b->col;
    }
};

class ParkingLot {
    Helper* helper;

    int floors, rows, cols;

    // All spots
    vector<vector<vector<Spot*>>> grid;

    // Free spots per floor per type
    vector<unordered_map<int, set<Spot*, SpotCompare>>> freeSpots;

    // Free count per floor per type
    vector<unordered_map<int, int>> freeCount;

    // Lookups
    unordered_map<string, Spot*> vehicleMap;
    unordered_map<string, Spot*> ticketMap;

public:
    void init(Helper* h, vector<vector<vector<int>>> parking) {
        helper = h;
        floors = parking.size();
        rows = parking[0].size();
        cols = parking[0][0].size();

        grid.resize(floors, vector<vector<Spot*>>(rows, vector<Spot*>(cols, nullptr)));
        freeSpots.resize(floors);
        freeCount.resize(floors);

        for (int f = 0; f < floors; f++) {
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    int type = parking[f][r][c];
                    if (type == 0) continue;

                    Spot* s = new Spot(f, r, c, type);
                    grid[f][r][c] = s;

                    freeSpots[f][type].insert(s);
                    freeCount[f][type]++;
                }
            }
        }
    }

    string park(int vehicleType, string vehicleNumber, string ticketId, int strategy) {
        if (vehicleMap.count(vehicleNumber) || ticketMap.count(ticketId)) {
            return ""; // already parked
        }

        Spot* chosen = nullptr;

        // Strategy 0: lowest index
        if (strategy == 0) {
            for (int f = 0; f < floors; f++) {
                if (!freeSpots[f][vehicleType].empty()) {
                    chosen = *freeSpots[f][vehicleType].begin();
                    break;
                }
            }
        }

        // Strategy 1: max free spots floor
        else if (strategy == 1) {
            int bestFloor = -1, maxCount = -1;

            for (int f = 0; f < floors; f++) {
                if (freeCount[f][vehicleType] > maxCount) {
                    maxCount = freeCount[f][vehicleType];
                    bestFloor = f;
                }
            }

            if (bestFloor != -1 && !freeSpots[bestFloor][vehicleType].empty()) {
                chosen = *freeSpots[bestFloor][vehicleType].begin();
            }
        }

        if (!chosen) return "";

        // Mark occupied
        chosen->occupied = true;
        freeSpots[chosen->floor][vehicleType].erase(chosen);
        freeCount[chosen->floor][vehicleType]--;

        vehicleMap[vehicleNumber] = chosen;
        ticketMap[ticketId] = chosen;

        return chosen->getId();
    }

    bool removeVehicle(string spotId) {
        vector<int> loc = parse(spotId);
        int f = loc[0], r = loc[1], c = loc[2];

        Spot* s = grid[f][r][c];
        if (!s || !s->occupied) return false;

        // Find and erase from maps
        string vehicleToRemove = "", ticketToRemove = "";

        for (auto &it : vehicleMap) {
            if (it.second == s) {
                vehicleToRemove = it.first;
                break;
            }
        }

        for (auto &it : ticketMap) {
            if (it.second == s) {
                ticketToRemove = it.first;
                break;
            }
        }

        if (vehicleToRemove != "") vehicleMap.erase(vehicleToRemove);
        if (ticketToRemove != "") ticketMap.erase(ticketToRemove);

        // Mark free
        s->occupied = false;
        freeSpots[f][s->type].insert(s);
        freeCount[f][s->type]++;

        return true;
    }

    string searchVehicle(string query) {
        if (vehicleMap.count(query)) {
            return vehicleMap[query]->getId();
        }
        if (ticketMap.count(query)) {
            return ticketMap[query]->getId();
        }
        return "";
    }

    int getFreeSpotsCount(int floor, int vehicleType) {
        return freeCount[floor][vehicleType];
    }

private:
    vector<int> parse(string spotId) {
        vector<int> res;
        stringstream ss(spotId);
        string token;

        while (getline(ss, token, '-')) {
            res.push_back(stoi(token));
        }
        return res;
    }
};