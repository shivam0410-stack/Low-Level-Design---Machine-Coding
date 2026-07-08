#include <bits/stdc++.h>
using namespace std;


struct Restaurant {
    string id;
    int totalRating;
    int ratingCount;
};


struct Comparator {
    bool operator()(const Restaurant& a, const Restaurant& b) const {
        int scaledA = (a.totalRating * 10) / a.ratingCount;
        int scaledB = (b.totalRating * 10) / b.ratingCount;
        if(scaledA == scaledB) {
            return a.id < b.id;
        }
        return scaledA > scaledB;
    }
};


class RestaurantFoodOrderingSystem {
    int maxRestaurantsToReturn;
    set<string> restaurants;
    unordered_map<string, set<string>> foodToRestaurants;
    unordered_map<string, string> orderToFood, orderToRestaurant;
    set<Restaurant, Comparator> ratedRestaurants;
    unordered_map<string, set<Restaurant, Comparator>> ratedRestaurantsByFood;
    unordered_map<string, pair<int, int>> restaurantRatings;
    unordered_map<string, pair<int, int>> foodRatings;

public:
    RestaurantFoodOrderingSystem() : maxRestaurantsToReturn(20) {}

    void orderFood(string orderId, string restaurantId, string foodItemId) {
        orderToFood[orderId] = foodItemId;
        orderToRestaurant[orderId] = restaurantId;
        restaurants.insert(restaurantId);
        foodToRestaurants[foodItemId].insert(restaurantId);
    }

    void rateOrder(string orderId, int rating) {
        string restaurantId = orderToRestaurant[orderId];
        auto restaurantIt = restaurantRatings.find(restaurantId);
        if(restaurantIt == restaurantRatings.end()) {
            restaurantRatings[restaurantId] = {rating, 1};
            ratedRestaurants.insert({restaurantId, rating, 1});
        }
        else {
            ratedRestaurants.erase({restaurantId, restaurantIt->second.first, restaurantIt->second.second});
            restaurantIt->second.first += rating;
            restaurantIt->second.second++;
            ratedRestaurants.insert({restaurantId, restaurantIt->second.first, restaurantIt->second.second});
        }

        string foodItemId = orderToFood[orderId];
        string foodId = foodItemId + '#' + restaurantId;
        auto foodIt = foodRatings.find(foodId);
        if(foodIt == foodRatings.end()) {
            foodRatings[foodId] = {rating, 1};
            ratedRestaurantsByFood[foodItemId].insert({restaurantId, rating, 1});
        }
        else {
            ratedRestaurantsByFood[foodItemId].erase({restaurantId, foodIt->second.first, foodIt->second.second});
            foodIt->second.first += rating;
            foodIt->second.second++;
            ratedRestaurantsByFood[foodItemId].insert({restaurantId, foodIt->second.first, foodIt->second.second});
        }
    }

    vector<string> getTopRestaurantsByFood(string foodItemId) {
        vector<string> list;
        for(auto& restaurant : ratedRestaurantsByFood[foodItemId]) {
            list.push_back(restaurant.id);
            if(list.size() >= 20) {
                return list;
            }
        }
        
        string foodId = foodItemId + '#';
        for(auto& restaurant : foodToRestaurants[foodItemId]) {
            if(foodRatings.find(foodId + restaurant) == foodRatings.end()) {
                list.push_back(restaurant);
            }
            if(list.size() >= 20) {
                break;
            }
        }

        return list;
    }

    vector<string> getTopRatedRestaurants() {
        vector<string> list;
        for(auto& restaurant : ratedRestaurants) {
            list.push_back(restaurant.id);
            if(list.size() >= 20) {
                return list;
            }
        }

        for(auto& restaurant : restaurants) {
            if(restaurantRatings.find(restaurant) == restaurantRatings.end()) {
                list.push_back(restaurant);
            }
            if(list.size() >= 20) {
                return list;
            }
        }

        return list;
    }
};


int main() {
    return 0;
}