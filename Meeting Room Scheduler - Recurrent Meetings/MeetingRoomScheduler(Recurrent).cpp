#include <bits/stdc++.h>
using namespace std;


struct Booking {
    string bookingId;
    int startTime, endTime;
};

struct BookingDetails {
    int roomId;
    int employeeId;
    int startTime, duration, repeatDuration;
};

struct Comparator {
    bool operator()(const Booking& booking1, const Booking& booking2) {
        return booking1.startTime < booking2.startTime;
    }
};


class MeetingRoomScheduler {
    int roomsCount, employeesCount;
    unordered_map<string, BookingDetails> bookings;
    vector<set<Booking, Comparator>> roomBookings, employeeBookings;

    bool isRoomAvailable(int roomId, int startTime, int endTime) {
        Booking booking = {"", startTime, endTime};
        auto nextBooking = roomBookings[roomId].lower_bound(booking);
        if(nextBooking != roomBookings[roomId].end()) {
            if(nextBooking->startTime <= endTime) {
                return false;
            }
        }

        if(nextBooking != roomBookings[roomId].begin()) {
            auto previousBooking = --nextBooking;
            if(previousBooking->endTime >= startTime) {
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

    bool bookRoom(string bookingId, int employeeId, int roomId, int startTime, int duration, int repeatDuration) {
        if(duration <= 0 || startTime < 0) {
            return false;
        }

        int start = startTime;
        int end = startTime + duration - 1;
        for(int instance = 0; instance < 20; instance++) {
            if(isRoomAvailable(roomId, start, end) == false) {
                return false;
            }
            start += repeatDuration;
            end += repeatDuration;
        }

        start = startTime;
        end = startTime + duration - 1;
        for(int instance = 0; instance < 20; instance++) {
            Booking booking = {bookingId, start, end};

            roomBookings[roomId].insert(booking);
            employeeBookings[employeeId].insert(booking);
            
            start += repeatDuration;
            end += repeatDuration;
        }

        bookings[bookingId] = {roomId, employeeId, startTime, duration, repeatDuration};
        return true;
    }

    vector<int> getAvailableRooms(int startTime, int endTime) {
        if(startTime > endTime) {
            return {};
        }

        vector<int> rooms;
        for(int i=0;i<roomsCount;i++) {
            if(isRoomAvailable(i, startTime, endTime) == true) {
                rooms.push_back(i);
            }
        }
        return rooms;
    }

    bool cancelBooking(string bookingId) {
        auto bookingIterator = bookings.find(bookingId);
        if(bookingIterator == bookings.end()) {
            return false;
        }

        BookingDetails booking = bookingIterator->second;
        int startTime = booking.startTime;
        int endTime = startTime + booking.duration - 1;
        int repeatDuration = booking.repeatDuration;
        int roomId = booking.roomId;
        int employeeId = booking.employeeId;

        for(int instance = 0; instance < 20; instance++) {
            Booking bookingInstance = {bookingId, startTime, endTime};

            roomBookings[roomId].erase(bookingInstance);
            employeeBookings[employeeId].erase(bookingInstance);

            startTime += repeatDuration;
            endTime += repeatDuration;
        }

        bookings.erase(bookingId);
        return true;
    }

    vector<string> listBookingsForRoom(int roomId, int n) {
        vector<string> bookings;
        for(auto& booking : roomBookings[roomId]) {
            bookings.push_back(booking.bookingId + '-' + to_string(booking.startTime) + '-' + to_string(booking.endTime));
            if(bookings.size() == n) break;
        }
        return bookings;
    }

    vector<string> listBookingsForEmployee(int employeeId, int n) {
        vector<string> bookings;
        for(auto& booking : employeeBookings[employeeId]) {
            bookings.push_back(booking.bookingId + '-' + to_string(booking.startTime) + '-' + to_string(booking.endTime));
            if(bookings.size() == n) break;
        }
        return bookings;
    }
};


int main() {
    return 0;
}