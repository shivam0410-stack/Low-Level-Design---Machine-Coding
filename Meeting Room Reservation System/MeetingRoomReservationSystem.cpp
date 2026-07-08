// Lessons
// Have multiple functions like hasOverlap, isAvailable, etc. to avoid making mistakes and more readable code.
// Try handle edge cases like duplicate ids n all. Get clarity with interviewer on this.

#include <bits/stdc++.h>
using namespace std;


class RoomBooking {
public:
    vector<string> rooms;
    unordered_map<string, set<pair<int, int>>> bookedMeetings; //{endTime, startTime}
    unordered_map<string, pair<int, int>> meetingDuration;
    unordered_map<string, string> meetingToRoomMap;

    RoomBooking(vector<string> roomIds) {
        rooms = roomIds;
        sort(rooms.begin(), rooms.end());
    }

    string bookMeeting(string meetingId, int startTime, int endTime) {
        for(auto& room : rooms) {
            auto& meetings = bookedMeetings[room];
            auto nextMeetingIt = meetings.upper_bound({startTime - 1, INT_MAX});
            if(nextMeetingIt == meetings.end() || (*nextMeetingIt).second > endTime) {
                meetings.insert({endTime, startTime});
                meetingDuration[meetingId] = {startTime, endTime};
                meetingToRoomMap[meetingId] = room;
                return room;
            }
        }
        return "";
    }

    bool cancelMeeting(string meetingId) {
        // Meeting isn't booked or doesn't exist;
        if(meetingToRoomMap.find(meetingId) == meetingToRoomMap.end()) {
            return false;
        }

        string roomId = meetingToRoomMap[meetingId];
        meetingToRoomMap.erase(meetingId);

        pair<int, int> duration = meetingDuration[meetingId];
        meetingDuration.erase(meetingId);

        bookedMeetings[roomId].erase({duration.second, duration.first});
        return true;
    }
};

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

int main() {
    // Example 1
    RoomBooking rb({"roomA", "roomB"});

    std::cout << "Running Example 1..." << std::endl;
    assert(rb.bookMeeting("m1", 10, 20) == "roomA");
    assert(rb.bookMeeting("m2", 15, 25) == "roomB");
    assert(rb.bookMeeting("m3", 20, 30) == "");      // Conflict
    assert(rb.cancelMeeting("m1") == true);
    assert(rb.bookMeeting("m4", 20, 30) == "roomA"); // Now free
    std::cout << "Example 1 Passed!" << std::endl;

    // Example 2
    RoomBooking rb2({"Z1", "A1", "M3"});

    std::cout << "\nRunning Example 2..." << std::endl;
    assert(rb2.bookMeeting("x", 5, 5) == "A1");      // Smallest alphabetically
    assert(rb2.bookMeeting("y", 5, 6) == "M3");
    assert(rb2.cancelMeeting("nope") == false);      // Not found
    assert(rb2.bookMeeting("z", 6, 10) == "A1");     // 5..5 and 6..10 don't overlap
    std::cout << "Example 2 Passed!" << std::endl;

    // Edge: point meeting, adjacent meetings no conflict
    RoomBooking rb4({"r1"});
    assert(rb4.bookMeeting("a", 5, 5)   == "r1");
    assert(rb4.bookMeeting("b", 6, 6)   == "r1");  // 5..5 and 6..6 no overlap
    assert(rb4.bookMeeting("c", 5, 6)   == "");    // conflicts with both
    cout << "Point meeting edge cases passed\n";

    return 0;
}
