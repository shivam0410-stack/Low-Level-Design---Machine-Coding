#include <bits/stdc++.h>
using namespace std;

struct TrainRecord {
    string trainId;
    int platformNumber;
    int arrivalTime;
    int departureTime;
};

class TrainPlatformManager {
    int platformCount;
    set<pair<int, int>> availablePlatforms;
    vector<map<int, TrainRecord>> platformHistory;
    unordered_map<string, TrainRecord> trainSnapshot;

public:
    TrainPlatformManager(int platformCount) : platformCount(platformCount) {
        for(int platformNumber=0;platformNumber<platformCount;platformNumber++) {
            availablePlatforms.insert({0, platformNumber});
        }
        platformHistory.resize(platformCount);
    }

    string assignPlatform(string trainId, int arrivalTime, int waitTime) {
        pair<int, int> choosenPlatform = {-1, INT_MAX};
        for(auto& platform : availablePlatforms) {
            if(platform.first <= arrivalTime) {
                if(choosenPlatform.second > platform.second) {
                    choosenPlatform = platform;
                }
            }
            else {
                break;
            }
        }

        if(choosenPlatform.first == -1) {
            choosenPlatform = *availablePlatforms.begin();
        }
        availablePlatforms.erase(choosenPlatform);

        int waitBeforeArrival = max(0, choosenPlatform.first - arrivalTime);
        int newArrivalTime = arrivalTime + waitBeforeArrival;
        int departureTime = newArrivalTime + waitTime - 1;

        int platformNumber = choosenPlatform.second;
        platformHistory[platformNumber][departureTime] = {trainId, platformNumber, newArrivalTime, departureTime};
        trainSnapshot[trainId] = {trainId, platformNumber, newArrivalTime, departureTime};

        availablePlatforms.insert({departureTime + 1, platformNumber});

        return to_string(platformNumber) + "," + to_string(waitBeforeArrival);
    }

    string getTrainAtPlatform(int platformNumber, int timestamp) {
        auto trainIterator = platformHistory[platformNumber].lower_bound(timestamp);
        if(trainIterator == platformHistory[platformNumber].end()) {
            return "";
        }

        TrainRecord trainRecord = trainIterator->second;
        if(trainRecord.arrivalTime > timestamp) {
            return "";
        }

        return trainRecord.trainId;
    }

    int getPlatformOfTrain(string trainId, int timestamp) {
        auto trainIterator = trainSnapshot.find(trainId);
        if(trainIterator == trainSnapshot.end()) {
            return -1;
        }

        TrainRecord trainRecord = trainIterator->second;
        if(trainRecord.arrivalTime > timestamp || trainRecord.departureTime < timestamp) {
            return -1;
        }

        return trainRecord.platformNumber;
    }
};

int main() {
    return 0;
}