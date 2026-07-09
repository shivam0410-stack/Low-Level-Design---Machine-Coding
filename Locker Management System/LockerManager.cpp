#include <bits/stdc++.h>
using namespace std;


struct Locker {
    string id;
    string size;

    bool operator==(const Locker& locker) const {
        return id == locker.id;
    }

    string getId() { return id; }
};


struct LockerHash {
    int operator()(const Locker& locker) const noexcept {
        return hash<string>{}(locker.id);
    }
};


struct LockerComparator {
    bool operator()(const Locker& locker1, const Locker& locker2) const {
        return locker1.id < locker2.id;
    }
};


class LockerManager {
    // Improvments
    // Keep map of locker id to lockers as single source of truth
    // Use locker ids everywhere else when required.
    // This way we won't have to keep locker copies in Sync everywhere.
    unordered_map<string, set<Locker, LockerComparator>> availableLockers;
    unordered_set<Locker, LockerHash> occupiedLockers;
    unordered_map<string, int> lockerCount;

    int getLockerCount(const string& size) {
        return lockerCount[size];
    }

    string getSize(const string& lockerId) {
        string size;
        for(auto& ch : lockerId) {
            if(ch >= 'A' && ch <= 'Z') size.push_back(ch);
            else break;
        }
        return size;
    }

    bool isSizeValid(const string& size) {
        if(size == "S" || size == "M" || size == "L" || size == "XL" || size == "XXL") {
            return true;
        }

        return false;
    }

public:
    void addSlot(const string& size) {
        if(!isSizeValid(size)) {
            return ;
        }

        int nextId = getLockerCount(size) + 1;
        Locker locker = {size + to_string(nextId), size};
        availableLockers[size].insert(locker);
        lockerCount[size]++;
    }

    Locker findSlot(const string& size) {
        if(!isSizeValid(size) || availableLockers[size].empty()) {
            return {};
        }

        return *(availableLockers[size].begin());
    }

    bool occupySlot(const string& lockerId) {
        string size = getSize(lockerId);
        if(!isSizeValid(size)) {
            return false;
        }

        Locker locker = {lockerId, size};
        auto lockerIt = availableLockers[size].find(locker);
        if(lockerIt == availableLockers[size].end()) {
            return false;
        }

        availableLockers[size].erase(locker);
        occupiedLockers.insert(locker);
        return true;
    }

    bool freeSlot(const string& lockerId) {
        // Size validation not required since this locker was already occupied.
        string size = getSize(lockerId);
        Locker locker = {lockerId, size};

        auto lockerIt = occupiedLockers.find(locker);
        if(lockerIt == occupiedLockers.end()) {
            return false;
        }

        availableLockers[size].insert(locker);
        occupiedLockers.erase(locker);
        return true;
    }
};


int main() {
    // Example 1
    std::cout << "--- Example 1 ---" << std::endl;
    LockerManager* sys1 = new LockerManager();
    sys1->addSlot("M"); // creates "M1"
    sys1->addSlot("L");  // creates "L1"

    Locker m1 = sys1->findSlot("M");     // returns a free medium locker ( id "M1")
    std::cout << "Found locker ID: " << m1.getId() << std::endl;

    bool occupyM1 = sys1->occupySlot(m1.getId());        // true
    std::cout << "Occupy M1: " << (occupyM1 ? "true" : "false") << std::endl;

    bool freeM1 = sys1->freeSlot(m1.getId());          // true
    std::cout << "Free M1: " << (freeM1 ? "true" : "false") << std::endl;
    
    delete sys1;


    // Example 2
    std::cout << "\n--- Example 2 ---" << std::endl;
    LockerManager* sys2 = new LockerManager();
    sys2->addSlot("S"); // S1
    sys2->addSlot("S"); // S2

    Locker sA = sys2->findSlot("S");     // get one small locker S1
    std::cout << "Found sA ID: " << sA.getId() << std::endl;

    bool occupySA_1 = sys2->occupySlot(sA.getId());        // true
    std::cout << "Occupy sA first time: " << (occupySA_1 ? "true" : "false") << std::endl;

    bool occupySA_2 = sys2->occupySlot(sA.getId());        // false (already taken)
    std::cout << "Occupy sA second time: " << (occupySA_2 ? "true" : "false") << std::endl;

    Locker sB = sys2->findSlot("S");     // returns a different free small locker, S2
    std::cout << "Found sB ID: " << sB.getId() << std::endl;

    delete sys2;

    return 0;
}
