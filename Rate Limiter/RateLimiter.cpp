#include <bits/stdc++.h>
using namespace std;


class IStrategy {
public:
    virtual bool isAllowed(const int& timestamp) = 0;
};


class FixedWindowCounter : public IStrategy {
    int maxRequests;
    int timeInterval;
    int currentWindow;
    int requestsCount;

public:
    FixedWindowCounter(const int& maxRequests, const int& timeInterval) : maxRequests(maxRequests), timeInterval(timeInterval) {
        currentWindow = 0;
        requestsCount = 0;
    }

    bool isAllowed(const int& timestamp) {
        int window = timestamp/timeInterval;
        if(currentWindow != window) {
            currentWindow = window;
            requestsCount = 0;
        }

        if(requestsCount < maxRequests) {
            requestsCount++;
            return true;
        }
        return false;
    }
};


class SlidingWindowCounter : public IStrategy {
    int maxRequests;
    int timeInterval;
    queue<int> timestamps;

public:
    SlidingWindowCounter(const int& maxRequests, const int& timeInterval) : maxRequests(maxRequests), timeInterval(timeInterval) {}

    bool isAllowed(const int& timestamp) {
        while(!timestamps.empty() && timestamps.front() + timeInterval - 1 < timestamp) {
            timestamps.pop();
        }

        if(timestamps.size() < maxRequests) {
            timestamps.push(timestamp);
            return true;
        }
        return false;
    }
};




class RateLimiter {
    unordered_map<string, IStrategy*> resourceStrategy;

    IStrategy* createStrategy(const string& strategy, const string& limits) {
        int index;
        for(int i=0;i<limits.length();i++) {
            if(limits[i] == ',') {
                index = i;
                break;
            }
        }

        int maxRequests = stoi(limits.substr(0, index));
        int timeInterval = stoi(limits.substr(index+1, limits.size()));

        if(strategy == "fixed-window-counter") {
            return new FixedWindowCounter(maxRequests, timeInterval);
        }
        else {
            return new SlidingWindowCounter(maxRequests, timeInterval);
        }
    }

public:
    void addResource(const string& resourceId, const string& strategy, const string& limits) {
        auto resourceStrategyIt = resourceStrategy.find(resourceId);
        if(resourceStrategyIt != resourceStrategy.end()) {
            delete resourceStrategyIt->second;
        }

        IStrategy* newStrategy = createStrategy(strategy, limits);
        resourceStrategy[resourceId] = newStrategy;
    }

    bool isAllowed(const string& resourceId, const int& timestamp) {
        auto strategyIt = resourceStrategy.find(resourceId);
        if(strategyIt == resourceStrategy.end()) {
            cout<<"No Strategy defined for the resource: "<<resourceId<<'\n';
            throw runtime_error("...");
        }

        IStrategy* strategy = strategyIt->second;
        return strategy->isAllowed(timestamp);
    }
};


int main() {
    return 0;
}