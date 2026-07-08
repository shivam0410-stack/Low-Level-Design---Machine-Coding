// Classes
// City, Cinema, User, Seat, Show, Movie, Ticket
// TicketManager, BookingManager, CinemaManager

// City -> Cinema -> Show -> Movie
// Show -> Seat


// Lessons
// Don't think too much about vector or maps, have both if confused
// Maps for better time complexity
// For Data Structures used, think of it like a DSA problem, use optimized things + general storage(vectors)
// Booking Logic is not correct, revisit later
// Sorting/Comparator is required, revisit


#include <bits/stdc++.h>
#define long long long
using namespace std;

class Show {
    public:
        unordered_set<string> bookedSeats;
    
        int showId, movieId, cinemaId, screenIndex, screenRow, screenColumn;
        int totalSeats;
        long startTime, endTime;
    
        Show(int showId, int movieId, int cinemaId,
            int screenIndex, long startTime, long endTime,
            int screenRow, int screenColumn) :
            showId(showId), movieId(movieId), cinemaId(cinemaId),
            screenIndex(screenIndex), startTime(startTime), endTime(endTime),
            screenRow(screenRow), screenColumn(screenColumn)
            {
                totalSeats = screenRow*screenColumn;
            }
        
        int getFreeSeatsCount() {
            return totalSeats - bookedSeats.size();
        }
    
        vector<string> reserveSeats(int seatsCount) {
            vector<string> seats;
            for(int i=1;i<=screenRow;i++) {
                for(int j=1;j<=screenColumn;j++) {
                    string seat = to_string(i) + '-' + to_string(j);
                    if(bookedSeats.find(seat) == bookedSeats.end()) {
                        bookedSeats.insert(seat);
                        seats.push_back(seat);
                        seatsCount--;
                        if(seatsCount == 0) {
                            return seats;
                        }
                    }
                }
            }
            return {};
        }
    
        void freeUpSeats(vector<string> reservedSeats) {
            for(auto seat : reservedSeats) {
                bookedSeats.erase(seat);
            }
        }
    };

class Cinema {
public:
    vector<Show*> shows;
    unordered_map<int, vector<Show*>> movieToShow;

    int cinemaId;
    int cityId;
    int screenCount;
    int screenRow;
    int screenColumn;

    Cinema(int cinemaId, int cityId, int screenCount,
        int screenRow, int screenColumn) :
        cinemaId(cinemaId), cityId(cityId), screenCount(screenCount),
        screenRow(screenRow), screenColumn(screenColumn)
        {}

    void addShow(int showId, int movieId, int cinemaId,
        int screenIndex, long startTime, long endTime) {
            Show* show = new Show(showId, movieId, cinemaId,
                screenIndex, startTime, endTime, screenRow,
                screenColumn);
            shows.push_back(show);
            movieToShow[movieId].push_back(show);
        }
    
    vector<int> listShows(int movieId) {
        vector<int> showIds;
        for(Show* show : movieToShow[movieId]) {
            showIds.push_back(show->showId);
        }
        return showIds;
    }

    Show* getShowById(int showId) {
        for(auto show : shows) {
            if(show->showId == showId) {
                return show;
            }
        }
        return nullptr;
    }
};

class CinemaManager {
public:
    vector<Cinema*> cinemas;
    unordered_map<int, Cinema*> cinemaMap;

    CinemaManager() {}

    void addCinema(int cinemaId, int cityId, int screenCount,
        int screenRow, int screenColumn) {
            Cinema* cinema = new Cinema(cinemaId, cityId,
                screenCount, screenRow, screenColumn);
            cinemas.push_back(cinema);
            cinemaMap[cinemaId] = cinema;
        }
    
    vector<int> listCinemas(int movieId, int cityId) {
        vector<int> cinemaIds;
        for(auto cinema : cinemas) {
            if(cinema->cityId != cityId) {
                continue;
            }
            vector<int> showIds = cinema->listShows(movieId);
            if(!showIds.empty()) {
                cinemaIds.push_back(cinema->cinemaId);
            }
        }
        cout<<"Cinema IDs: ";
        for(auto id : cinemaIds) {
            cout<<id<<" ";
        }
        cout<<'\n';
        return cinemaIds;
    }

    vector<int> listShows(int movieId, int cinemaId) {
        Cinema* cinema = cinemaMap[cinemaId];
        vector<int> showIds = cinema->listShows(movieId);
        cout<<"Show IDs: ";
        for(auto id : showIds) {
            cout<<id<<" ";
        }
        cout<<'\n';
        return showIds;
    }

    Show* getShowById(int showId) {
        for(auto cinema : cinemas) {
            return cinema->getShowById(showId);
        }
        return nullptr;
    }

    void addShow(int showId, int movieId, int cinemaId,
        int screenIndex, long startTime, long endTime) {
        cinemaMap[cinemaId]->addShow(showId, movieId, cinemaId,
            screenIndex, startTime, endTime);
    }
};




class TicketManager {
public:
    CinemaManager* cinemaManager;
    unordered_map<string, int> ticketToShow;
    unordered_map<string, vector<string>> ticketToSeats;

    TicketManager(CinemaManager* cinemaManager) :
        cinemaManager(cinemaManager) {

    }

    vector<string> bookTicket(string ticketId, int showId, int ticketsCount) {
        Show* show = cinemaManager->getShowById(showId);
        if(show->getFreeSeatsCount() < ticketsCount) {
            cout<<"Required number of seats not available\n";
            return {};
        }

        vector<string> bookedSeats = show->reserveSeats(ticketsCount);
        ticketToShow[ticketId] = showId;
        ticketToSeats[ticketId] = bookedSeats;
        for(auto seat : bookedSeats) {
            cout<<seat<<" ";
        }
        cout<<'\n';
        return bookedSeats;
    }

    bool cancelTicket(string ticketId) {
        if(ticketToShow.find(ticketId) == ticketToShow.end()) {
            cout<<"Booking doesn't exist\n";
            return false;
        }

        int showId = ticketToShow[ticketId];
        ticketToShow.erase(ticketId);

        Show* show = cinemaManager->getShowById(showId);
        vector<string> bookedSeats = ticketToSeats[ticketId];
        show->freeUpSeats(bookedSeats);

        cout<<"Cancelled Ticket\n";
        return true;
    }
};


int main() {
//     Solution obj=new Solution();
// obj.init(helper)
    CinemaManager* cinemaManager = new CinemaManager();
// Use -> for pointers, remove labels, and ensure correct types
cinemaManager->addCinema(0, 1, 4, 5, 10);

cinemaManager->addShow(1, 4, 0, 1, 1710516108725LL, 1710523308725LL);
cinemaManager->addShow(2, 11, 0, 3, 1710516108725LL, 1710523308725LL);

cinemaManager->listCinemas(0, 1);

// Fixed typo: changed 'cinemaManger' to 'cinemaManager'
cinemaManager->listShows(4, 0);
cinemaManager->listShows(11, 0);

TicketManager* ticketManager = new TicketManager(cinemaManager);

// Assuming ticketId is a std::string or const char*
ticketManager->bookTicket("tkt-1", 1, 4);

// Use -> for the ticketManager pointer
ticketManager->cancelTicket("tkt-1");

// returned true
// obj.getFreeSeatsCount(showId = 1) returned 50
    return 0;
}