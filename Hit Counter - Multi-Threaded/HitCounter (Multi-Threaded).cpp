// Test this code in online compiler
// Mutex not getting compiled locally

#include <bits/stdc++.h>
// #include <mutex.h>
using namespace std;

class HitCounter {
    int totalPages;
    vector<mutex> locks;
    vector<int> pageVisits;

public:
    void init(int totalPages) {
        totalPages = totalPages;
        locks.resize(totalPages);
        pageVisits.assign(totalPages, 0);
    }

    void incrementVisitCount(int pageIndex) {
        if(locks[pageIndex].lock()) {
            pageVisits[pageIndex]++;
        }
    }

    int getVisitCount(int pageIndex) {
        return pageVisits[pageIndex];
    }
};


int main() {
    return 0;
}