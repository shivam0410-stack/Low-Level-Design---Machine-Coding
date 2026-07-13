#include <bits/stdc++.h>
using namespace std;

struct TravelRequest {
    int source;
    int destination;
    int stopsCount;
};

class DirectionRequests {
    int floorsCount;
    int liftCapacity;
    int maxStops;
    bool isUp;

    vector<TravelRequest> requests;
    set<int> stops;

    bool between(int start, int stop, int end) {
        if (isUp)
            return start < stop && stop < end;
        return start > stop && stop > end;
    }

    // Number of existing intermediate stops for the new request
    int stopsForNewRequest(int source, int destination) {
        int cnt = 0;

        for (int stop : stops) {
            if (between(source, stop, destination)) {
                cnt++;
                if (cnt > maxStops)
                    return cnt;
            }
        }

        return cnt;
    }

    // Check whether adding new source/destination violates maxStops
    // for already accepted requests.
    bool areOldRequestsFeasible(int source, int destination) {
        vector<int> newStops;

        if (!stops.count(source))
            newStops.push_back(source);

        if (!stops.count(destination))
            newStops.push_back(destination);

        for (auto &req : requests) {
            int extraStops = 0;

            for (int stop : newStops) {
                if (between(req.source, stop, req.destination))
                    extraStops++;
            }

            if (req.stopsCount + extraStops > maxStops)
                return false;
        }

        return true;
    }

    // Check capacity on every floor segment.
    bool checkCapacity(int source, int destination) {
        vector<int> diff(floorsCount + 1, 0);

        for (auto &req : requests) {
            int start = min(req.source, req.destination);
            int end = max(req.source, req.destination);

            diff[start]++;
            diff[end]--;
        }

        diff[min(source, destination)]++;
        diff[max(source, destination)]--;

        int passengers = 0;

        for (int floor = 0; floor < floorsCount; floor++) {
            passengers += diff[floor];

            if (passengers > liftCapacity)
                return false;
        }

        return true;
    }

public:
    DirectionRequests(int floorsCount,
                      int liftCapacity,
                      int maxStops,
                      bool isUp)
        : floorsCount(floorsCount),
          liftCapacity(liftCapacity),
          maxStops(maxStops),
          isUp(isUp) {}

    bool isFeasible(int source, int destination) {

        if (!checkCapacity(source, destination))
            return false;

        int newRequestStops = stopsForNewRequest(source, destination);

        if (newRequestStops > maxStops)
            return false;

        if (!areOldRequestsFeasible(source, destination))
            return false;

        vector<int> insertedStops;

        if (!stops.count(source))
            insertedStops.push_back(source);

        if (!stops.count(destination))
            insertedStops.push_back(destination);

        // Update stop count of already accepted requests.
        for (int stop : insertedStops) {
            for (auto &req : requests) {
                if (between(req.source, stop, req.destination))
                    req.stopsCount++;
            }
        }

        requests.push_back({source, destination, newRequestStops});

        stops.insert(source);
        stops.insert(destination);

        return true;
    }
};

class LiftSystem {
    int floorsCount;

    DirectionRequests upRequests;
    DirectionRequests downRequests;

    bool isValidRequest(int source, int destination) {
        if (source < 0 || source >= floorsCount)
            return false;

        if (destination < 0 || destination >= floorsCount)
            return false;

        if (source == destination)
            return false;

        return true;
    }

public:
    LiftSystem(int floorsCount,
               int liftCapacity,
               int maxStops)
        : floorsCount(floorsCount),
          upRequests(floorsCount, liftCapacity, maxStops, true),
          downRequests(floorsCount, liftCapacity, maxStops, false) {}

    bool requestPickup(int source, int destination) {
        if (!isValidRequest(source, destination))
            return false;

        if (source < destination)
            return upRequests.isFeasible(source, destination);

        return downRequests.isFeasible(source, destination);
    }
};

int main() {
    cout << boolalpha;

    // ==========================================================
    // Example 1 - maxStops violation
    // ==========================================================
    cout << "Example 1\n";

    LiftSystem ls1(30, 3, 2);

    cout << ls1.requestPickup(0, 20) << endl;    // true
    cout << ls1.requestPickup(10, 15) << endl;   // true
    cout << ls1.requestPickup(12, 18) << endl;   // false

    cout << "\nExample 2\n";

    // ==========================================================
    // Example 2 - capacity violation
    // ==========================================================
    LiftSystem ls2(15, 2, 5);

    cout << ls2.requestPickup(0, 12) << endl;    // true
    cout << ls2.requestPickup(1, 11) << endl;    // true
    cout << ls2.requestPickup(2, 10) << endl;    // false
    cout << ls2.requestPickup(14, 3) << endl;    // true

    return 0;
}