#include <bits/stdc++.h>
using namespace std;

// Use Node class/struct with key, value instead of vector<string> << pair<string, string> << struct Node
// struct Node {
//     string key;
//     string value;
// };


class CustomHashMap {
    double minLoadFactor;
    double maxLoadFactor;
    int keyCount;
    int bucketCount;
    vector<vector<vector<string>>> buckets;

    int calculateHash(const string& key) {
        int hash = key.length()*key.length();
        for(auto& ch : key) {
            hash += (ch - 'a' + 1);
        }
        return hash;
    }

    void grow() {
        bucketCount *= 2;
        vector<vector<vector<string>>> newBuckets(bucketCount);
        for(auto& bucket : buckets) {
            for(auto& pair : bucket) {
                int bucketIndex = calculateHash(pair[0])%bucketCount;
                newBuckets[bucketIndex].push_back(pair);
            }
        }
        buckets = newBuckets;
    }

    void shrink() {
        bucketCount /= 2;
        vector<vector<vector<string>>> newBuckets(bucketCount);
        for(auto& bucket : buckets) {
            for(auto& pair : bucket) {
                int bucketIndex = calculateHash(pair[0])%bucketCount;
                newBuckets[bucketIndex].push_back(pair);
            }
        }
        buckets = newBuckets;
    }

    void rehashing() {
        int loadFactor = round((double)keyCount / bucketCount * 100) / 100;

        while(loadFactor > maxLoadFactor) {
            grow();
            loadFactor = round((double)keyCount / bucketCount * 100) / 100;
        }

        while(bucketCount > 2 && loadFactor < minLoadFactor) {
            shrink();
            loadFactor = round((double)keyCount / bucketCount * 100) / 100;
        }
    }

public:
    CustomHashMap(double minLoadFactor, double maxLoadFactor) : minLoadFactor(minLoadFactor), maxLoadFactor(maxLoadFactor) {
        keyCount = 0;
        bucketCount = 2;
        buckets.resize(bucketCount);
    }

    void put(const string& key, const string& value) {
        int bucketIndex = calculateHash(key)%bucketCount;
        for(auto& pair : buckets[bucketIndex]) {
            if(pair[0] == key) {
                pair[1] = value;
                return ;
            }
        }

        buckets[bucketIndex].push_back({key, value});
        keyCount++;

        rehashing();
    }

    string get(const string& key) {
        int bucketIndex = calculateHash(key)%bucketCount;
        for(auto& pair : buckets[bucketIndex]) {
            if(pair[0] == key) {
                return pair[1];
            }
        }
        return "";
    }

    string remove(const string& key) {
        string value = "";
        int bucketIndex = calculateHash(key)%bucketCount;
        for(auto& pair : buckets[bucketIndex]) {
            if(pair[0] == key) {
                value = pair[1];
                int bucketSize = buckets[bucketIndex].size();
                swap(pair, buckets[bucketIndex][bucketSize - 1]);

                buckets[bucketIndex].pop_back();
                keyCount--;

                rehashing();
                break;
            }
        }
        
        return value;
    }

    vector<string> getBucketKeys(int bucketIndex) {
        if(bucketIndex < 0 || bucketIndex >= bucketCount) {
            return {};
        }

        vector<string> keys;
        sort(buckets[bucketIndex].begin(), buckets[bucketIndex].end(), [](const vector<string>& a, const vector<string>& b) {
            return a[0] < b[0];
        });

        for(auto& pair : buckets[bucketIndex]) {
            keys.push_back(pair[0]);
        }
        return keys;
    }

    int size() {
        return keyCount;
    }

    int bucketsCount() {
        return bucketCount;
    }
};


int main() {
    return 0;
}