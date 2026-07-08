#include <bits/stdc++.h>
using namespace std;


class ClickCounter {
    vector<int> timestamps;

public:
    void recordClick(int timestamp) {
        timestamps.push_back(timestamp);
    }

    int getRecentClicks(int timestamp) {
        auto start = upper_bound(timestamps.begin(), timestamps.end(), timestamp - 300);
        auto end = upper_bound(timestamps.begin(), timestamps.end(), timestamp);
        return end - start;
    }
};


int main() {
    return 0;
}