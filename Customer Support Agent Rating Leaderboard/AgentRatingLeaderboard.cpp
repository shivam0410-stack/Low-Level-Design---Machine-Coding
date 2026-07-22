#include <bits/stdc++.h>
using namespace std;


class MonthlyRating {
    string month;
    int ratingSum, ratingCount;

public:
    MonthlyRating(const string& month, int rating) : month(month) {
        ratingSum = rating;
        ratingCount = 1;
    }

    void updateRating(int rating) {
        ratingSum += rating;
        ratingCount++;
    }

    double getAverageRating() {
        double averageRating = (double)ratingSum / ratingCount;
        return round(averageRating * 10) / 10;
    }
};


class Agent {
    string name;
    int ratingSum, ratingCount;
    unordered_map<string, MonthlyRating*> ratingByMonth;

    void updateMonthlyRating(int rating, const string& date) {
        string month = date.substr(0, 7);
        auto it = ratingByMonth.find(month);

        if(it != ratingByMonth.end()) {
            it->second->updateRating(rating);
            return ;
        }

        ratingByMonth[month] = new MonthlyRating(month, rating);
    }

public:
    Agent(const string& name) : name(name) {
        ratingSum = ratingCount = 0;
    }

    void rateAgent(int rating, const string& date) {
        ratingSum += rating;
        ratingCount++;
        updateMonthlyRating(rating, date);
    }

    double getAverageRating() {
        double averageRating = (double)ratingSum / ratingCount;
        return round(averageRating * 10) / 10;
    }

    double getAverageMonthlyRating(const string& month) {
        auto it = ratingByMonth.find(month);
        if(it != ratingByMonth.end()) {
            return it->second->getAverageRating();
        }

        return 0;
    }
};


class AgentRatingLeaderboard {
    unordered_map<string, Agent*> agents;

    Agent* getAgent(const string& name) {
        auto agentIt = agents.find(name);

        if(agentIt != agents.end()) {
            return agentIt->second;
        }

        return agents[name] = new Agent(name);
    }

    struct Comparator {
        bool operator()(const pair<double, string>& r1, const pair<double, string>& r2) const {
            if(r1.first != r2.first) {
                return r1.first < r2.first;
            }
            return r1.second > r2.second;
        }
    };

    vector<string> ratingsInFormat(priority_queue<pair<double, string>, vector<pair<double, string>>, Comparator>& sortedRatings) {
        vector<string> ratings;
        while(!sortedRatings.empty()) {
            auto [averageRating, name] = sortedRatings.top();
            sortedRatings.pop();
            ratings.push_back(name + ',' + format("{:.1f}", averageRating));
        }
        return ratings;
    }

public:
    void rateAgent(const string& agentName, int rating, const string& date) {
        Agent* agent = getAgent(agentName);
        agent->rateAgent(rating, date);
    }

    vector<string> getAverageRatings() {
        priority_queue<pair<double, string>, vector<pair<double, string>>, Comparator> sortedRatings;
        for(auto& [name, agent] : agents) {
            sortedRatings.push({agent->getAverageRating(), name});
        }

        return ratingsInFormat(sortedRatings);
    }

    vector<string> getBestAgentsByMonth(const string& month) {
        priority_queue<pair<double, string>, vector<pair<double, string>>, Comparator> sortedRatings;
        for(auto& [name, agent] : agents) {
            sortedRatings.push({agent->getAverageMonthlyRating(month), name});
        }

        return ratingsInFormat(sortedRatings);
    }
};


int main() {
    // Instantiate the primary object pointer as requested
    AgentRatingLeaderboard* leaderboard = new AgentRatingLeaderboard();

    // =========================================================================
    // EXAMPLE 1: General Average Ratings
    // =========================================================================
    std::cout << "--- Running Example 1 ---" << std::endl;
    
    leaderboard->rateAgent("Alice", 5, "2025-03-12");
    leaderboard->rateAgent("Bob", 4, "2025-03-13");
    leaderboard->rateAgent("Alice", 3, "2025-03-15");
    leaderboard->rateAgent("Bob", 5, "2025-03-18");

    std::vector<std::string> avgRatings = leaderboard->getAverageRatings();
    
    std::cout << "Output:" << std::endl;
    for (const std::string& entry : avgRatings) {
        std::cout << entry << std::endl;
    }
    std::cout << std::endl;

    // Resetting/re-creating the pointer for Example 2 to keep logs clean
    delete leaderboard;
    leaderboard = new AgentRatingLeaderboard();

    // =========================================================================
    // EXAMPLE 2: Best Agents by Month
    // =========================================================================
    std::cout << "--- Running Example 2 ---" << std::endl;

    leaderboard->rateAgent("Alice", 5, "2025-02-02");
    leaderboard->rateAgent("Bob", 3, "2025-02-05");
    leaderboard->rateAgent("Charlie", 4, "2025-02-10");
    leaderboard->rateAgent("Bob", 5, "2025-03-12");
    leaderboard->rateAgent("Alice", 2, "2025-03-15");

    std::vector<std::string> bestByMonth = leaderboard->getBestAgentsByMonth("2025-02");

    std::cout << "Output:" << std::endl;
    for (const std::string& entry : bestByMonth) {
        std::cout << entry << std::endl;
    }

    // Clean up dynamic memory
    delete leaderboard;
    return 0;
}