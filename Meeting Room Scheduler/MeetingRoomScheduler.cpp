#include <bits/stdc++.h>
using namespace std;


struct Booking {
    string bookingId;
    int startTime;
    int endTime;
};


struct BookingDetails {
    int roomId;
    int employeeId;
    int startTime;
    int endTime;
};


struct Comparator {
    bool operator()(const Booking& booking1, const Booking& booking2) {
        if(booking1.startTime != booking2.startTime) {
            return booking1.startTime < booking2.startTime;
        }

        return booking1.bookingId < booking2.bookingId;
    }
};


class MeetingRoomScheduler {
    int roomsCount, employeesCount;
    unordered_map<string, BookingDetails> bookings;
    vector<set<Booking, Comparator>> roomBookings, employeeBookings;

    bool isRoomAvailable(int roomId, int startTime, int endTime) {
        auto collisionBooking = roomBookings[roomId].lower_bound({"", startTime, endTime});

        if(collisionBooking != roomBookings[roomId].end()) {
            if(collisionBooking -> startTime <= endTime) {
                return false;
            }
        }

        if(collisionBooking != roomBookings[roomId].begin()) {
            collisionBooking--;
            if(collisionBooking -> endTime >= startTime) {
                return false;
            }
        }

        return true;
    }

public:
    MeetingRoomScheduler(int roomsCount, int employeesCount) : roomsCount(roomsCount), employeesCount(employeesCount) {
        roomBookings.resize(roomsCount);
        employeeBookings.resize(employeesCount);
    }

    bool bookRoom(string bookingId, int employeeId, int roomId, int startTime, int endTime) {
        if(startTime > endTime || startTime < 0) {
            return false;
        }

        if(isRoomAvailable(roomId, startTime, endTime)) {
            bookings[bookingId] = {roomId, employeeId, startTime, endTime};
            roomBookings[roomId].insert({bookingId, startTime, endTime});
            employeeBookings[employeeId].insert({bookingId, startTime, endTime});
            return true;
        }

        return false;
    }

    vector<int> getAvailableRooms(int startTime, int endTime) {
        if(startTime > endTime) {
            return {};
        }

        vector<int> availableRooms;
        for(int roomId = 0; roomId < roomsCount; roomId++) {
            if(isRoomAvailable(roomId, startTime, endTime)) {
                availableRooms.push_back(roomId);
            }
        }

        return availableRooms;
    }

    bool cancelBooking(string bookingId) {
        auto bookingIt = bookings.find(bookingId);

        if(bookingIt == bookings.end()) {
            return false;
        }

        BookingDetails bookingDetails = bookingIt->second;
        int roomId = bookingDetails.roomId;
        int employeeId = bookingDetails.employeeId;
        int startTime = bookingDetails.startTime;
        int endTime = bookingDetails.endTime;

        bookings.erase(bookingIt);
        roomBookings[roomId].erase({bookingId, startTime, endTime});
        employeeBookings[employeeId].erase({bookingId, startTime, endTime});

        return true;
    }

    vector<string> listBookingsForRoom(int roomId) {
        vector<string> bookingsList;

        for(auto& booking : roomBookings[roomId]) {
            bookingsList.push_back(booking.bookingId);
        }

        return bookingsList;
    }

    vector<string> listBookingsForEmployee(int employeeId) {
        vector<string> bookingsList;

        for(auto& booking : employeeBookings[employeeId]) {
            bookingsList.push_back(booking.bookingId);
        }

        return bookingsList;
    }
};


int main() {
    return 0;
}