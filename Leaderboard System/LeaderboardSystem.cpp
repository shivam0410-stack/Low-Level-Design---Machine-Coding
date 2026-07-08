#include <bits/stdc++.h>
using namespace std;

// Can create a class/struct for UserScore with userId and score but need to be aware that if
// we create set<UserScore*> then whenever updating the user score
// first -> erase from set
// second -> update the score
// third -> add back to set
// otherwise ordering breaks and a lot of thing will go wrong as set expects immutable objects

struct Comparator {
    bool operator()(const pair<string, int>& user1, const pair<string, int>& user2) const {
        if(user1.second == user2.second) {
            return user1.first < user2.first;
        }
        return user1.second > user2.second;
    }
};


class LeaderBoardSystem {
    unordered_map<string, vector<string>> playerToUsers;
    unordered_map<string, int> playerScores;
    unordered_map<string, int> userScores;
    set<pair<string, int>, Comparator> sortedScores;

    void mapPlayerUser(const string& playerId, const string& userId) {
        playerToUsers[playerId].push_back(userId);
    }

    void createUser(const string& userId, const int& score) {
        userScores[userId] = score;
        sortedScores.insert({userId, score});
    }

    void updatePlayerScore(const string& playerId, const int& score) {
        playerScores[playerId] += score;
    }

    void updateUserScore(const string& userId, const int& score) {
        auto it = sortedScores.find({userId, userScores[userId]});
        sortedScores.erase(it);

        userScores[userId] += score;
        sortedScores.insert({userId, userScores[userId]});
    }

public:
    void addUser(string userId, vector<string> playerIds) {
        int total = 0;
        for(auto& playerId : playerIds) {
            mapPlayerUser(playerId, userId);
            total += playerScores[playerId];
        }
        createUser(userId, total);
    }

    void addScore(string playerId, int score) {
        updatePlayerScore(playerId, score);
        for(auto& userId : playerToUsers[playerId]) {
            updateUserScore(userId, score);
        }
    }

    vector<string> getTopK(int k) {
        vector<string> topKUserIds;
        for(auto it = sortedScores.begin(); it != sortedScores.end() && k--; it++) {
            topKUserIds.push_back(it->first);
        }
        return topKUserIds;
    }
};


int main() {
    return 0;
}