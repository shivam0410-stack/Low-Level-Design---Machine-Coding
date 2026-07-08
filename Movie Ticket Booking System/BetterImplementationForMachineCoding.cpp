#include <bits/stdc++.h>
using namespace std;

struct Show {
    int showId, movieId, cinemaId, rows, cols;
    long long startTime;
    vector<vector<int>> seats; 
    int freeCount;

    Show(int sid, int mid, int cid, int r, int c, long long st) 
        : showId(sid), movieId(mid), cinemaId(cid), rows(r), cols(c), startTime(st) {
        seats.assign(r, vector<int>(c, 0));
        freeCount = r * c;
    }
};

class Solution {
    // Indexing for O(1) or O(log N) lookups
    unordered_map<int, Show*> showMap;                     // showId -> Show object
    unordered_map<int, pair<int, int>> cinemaDims;         // cinemaId -> {rows, cols}
    unordered_map<int, set<int>> cityToCinemas;            // cityId -> sorted cinemaIds
    unordered_map<int, vector<Show*>> cinemaMovieToShows;  // cinemaId_movieId (hash) -> shows
    unordered_map<string, pair<int, vector<string>>> tickets; // ticketId -> {showId, [seats]}

    // Helper to create a unique key for cinema + movie combo
    long long getCinemaMovieKey(int cid, int mid) {
        return (long long)cid * 1000 + mid; // Given mid <= 20 and cid <= 1000
    }

public:
    void init() {
        // Clear all state (Required for testing environments)
        showMap.clear();
        cinemaDims.clear();
        cityToCinemas.clear();
        cinemaMovieToShows.clear();
        tickets.clear();
    }

    void addCinema(int cinemaId, int cityId, int screenCount, int rows, int cols) {
        cinemaDims[cinemaId] = {rows, cols};
        cityToCinemas[cityId].insert(cinemaId);
    }

    void addShow(int showId, int movieId, int cinemaId, int screenIndex, long long start, long long end) {
        auto dims = cinemaDims[cinemaId];
        Show* newShow = new Show(showId, movieId, cinemaId, dims.first, dims.second, start);
        showMap[showId] = newShow;
        cinemaMovieToShows[getCinemaMovieKey(cinemaId, movieId)].push_back(newShow);
    }

    // --- LISTING METHODS ---

    vector<int> listCinemas(int movieId, int cityId) {
        vector<int> result;
        // cityToCinemas[cityId] is a std::set, so it is already sorted by cinemaId
        for (int cid : cityToCinemas[cityId]) {
            if (!cinemaMovieToShows[getCinemaMovieKey(cid, movieId)].empty()) {
                result.push_back(cid);
            }
        }
        return result;
    }

    vector<int> listShows(int movieId, int cinemaId) {
        vector<Show*> &movieShows = cinemaMovieToShows[getCinemaMovieKey(cinemaId, movieId)];
        
        // Sort as per requirement: startTime DESC, then showId ASC
        sort(movieShows.begin(), movieShows.end(), [](Show* a, Show* b) {
            if (a->startTime != b->startTime) return a->startTime > b->startTime;
            return a->showId < b->showId;
        });

        vector<int> result;
        for (auto s : movieShows) result.push_back(s->showId);
        return result;
    }

    // --- BOOKING LOGIC ---

    vector<string> bookTicket(string ticketId, int showId, int count) {
        if (showMap.find(showId) == showMap.end()) return {};
        Show* s = showMap[showId];
        if (s->freeCount < count) return {};

        vector<string> reserved;

        // Step 1: Continuous Seats
        for (int i = 0; i < s->rows; i++) {
            int startCol = -1, consecutive = 0;
            for (int j = 0; j < s->cols; j++) {
                if (s->seats[i][j] == 0) {
                    if (consecutive == 0) startCol = j;
                    consecutive++;
                    if (consecutive == count) {
                        for (int k = startCol; k <= j; k++) {
                            s->seats[i][k] = 1;
                            reserved.push_back(to_string(i) + "-" + to_string(k));
                        }
                        goto finalize; // Jump out of nested loops
                    }
                } else {
                    consecutive = 0;
                }
            }
        }

        // Step 2: Fallback (Lowest Row/Col)
        for (int i = 0; i < s->rows && (int)reserved.size() < count; i++) {
            for (int j = 0; j < s->cols && (int)reserved.size() < count; j++) {
                if (s->seats[i][j] == 0) {
                    s->seats[i][j] = 1;
                    reserved.push_back(to_string(i) + "-" + to_string(j));
                }
            }
        }

        // Check if we actually filled the request (Edge case: seats scattered)
        if ((int)reserved.size() < count) {
            // Note: In real life we'd rollback, but problem implies count check upfront
            // If the code reaches here, it means seats were available but not bookable (impossible here)
        }

    finalize:
        s->freeCount -= count;
        tickets[ticketId] = {showId, reserved};
        return reserved;
    }

    bool cancelTicket(string ticketId) {
        if (tickets.find(ticketId) == tickets.end()) return false;
        auto entry = tickets[ticketId];
        Show* s = showMap[entry.first];
        for (string seat : entry.second) {
            int dash = seat.find('-');
            int r = stoi(seat.substr(0, dash));
            int c = stoi(seat.substr(dash + 1));
            s->seats[r][c] = 0;
        }
        s->freeCount += entry.second.size();
        tickets.erase(ticketId);
        return true;
    }

    int getFreeSeatsCount(int showId) {
        if (showMap.find(showId) == showMap.end()) return 0;
        return showMap[showId]->freeCount;
    }
};

int main() {
    Solution obj;
    obj.init();

    // 1. Add Cinema
    obj.addCinema(0, 1, 4, 5, 10);

    // 2. Add Shows
    obj.addShow(1, 4, 0, 1, 1710516108725LL, 1710523308725LL);
    obj.addShow(2, 11, 0, 3, 1710516108725LL, 1710523308725LL);

    // 3. Test listCinemas
    vector<int> cinemas = obj.listCinemas(4, 1);
    cout << "Cinemas for Movie 4 in City 1: ";
    for(int id : cinemas) cout << id << " ";
    cout << endl;

    // 4. Test bookTicket
    vector<string> seats = obj.bookTicket("tkt-1", 1, 4);
    cout << "Booked seats for tkt-1: ";
    for(string s : seats) cout << s << " ";
    cout << endl;

    // 5. Check Free Seats
    cout << "Free seats remaining in Show 1: " << obj.getFreeSeatsCount(1) << endl;

    // 6. Test cancelTicket
    bool cancelled = obj.cancelTicket("tkt-1");
    cout << "Cancellation status: " << (cancelled ? "Success" : "Failed") << endl;
    cout << "Free seats after cancellation: " << obj.getFreeSeatsCount(1) << endl;

    return 0;
}

