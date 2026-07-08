#include <bits/stdc++.h>
using namespace std;

class RoomBooking {
    vector<string> rooms;  // sorted lexicographically
    // per room: set of booked intervals {start, end}
    unordered_map<string, set<pair<int,int>>> roomIntervals;
    // meetingId -> {roomId, start, end}
    unordered_map<string, tuple<string,int,int>> meetingInfo;

    bool hasOverlap(const set<pair<int,int>>& intervals, int s, int e) {
        if (intervals.empty()) return false;

        // First interval whose start > s
        auto it = intervals.upper_bound({s, INT_MAX});

        // Interval after s: overlaps if start <= e
        if (it != intervals.end() && it->first <= e)
            return true;

        // Interval before or at s: overlaps if end >= s
        if (it != intervals.begin()) {
            --it;
            if (it->second >= s)
                return true;
        }

        return false;
    }

public:
    RoomBooking(vector<string> roomIds) {
        rooms = roomIds;
        sort(rooms.begin(), rooms.end());  // lexicographic order
    }

    // O(R * log N) worst case, but exits early on first free room
    string bookMeeting(string meetingId, int start, int end) {
        if (meetingInfo.count(meetingId)) return "";  // duplicate id

        for (const string& room : rooms) {
            if (!hasOverlap(roomIntervals[room], start, end)) {
                roomIntervals[room].insert({start, end});
                meetingInfo[meetingId] = {room, start, end};
                return room;
            }
        }
        return "";
    }

    // O(log N) — direct lookup and erase
    bool cancelMeeting(string meetingId) {
        if (!meetingInfo.count(meetingId)) return false;

        string room = get<0>(meetingInfo[meetingId]);
        int s = get<1>(meetingInfo[meetingId]);
        int e = get<2>(meetingInfo[meetingId]);

        roomIntervals[room].erase({s, e});
        meetingInfo.erase(meetingId);
        return true;
    }
};


// ── Tests ──────────────────────────────────────────────────────────────────
#include <cassert>
#include <iostream>

int main() {
    // Example 1
    RoomBooking rb({"roomA", "roomB"});
    assert(rb.bookMeeting("m1", 10, 20) == "roomA");
    assert(rb.bookMeeting("m2", 15, 25) == "roomB");
    assert(rb.bookMeeting("m3", 20, 30) == "");    // inclusive conflict at 20
    assert(rb.cancelMeeting("m1")       == true);
    assert(rb.bookMeeting("m4", 20, 30) == "roomA");
    cout << "Example 1 passed\n";

    // Example 2
    RoomBooking rb2({"Z1", "A1", "M3"});
    assert(rb2.bookMeeting("x", 5, 5)   == "A1");  // lex smallest
    assert(rb2.bookMeeting("y", 5, 6)   == "M3");
    assert(rb2.cancelMeeting("nope")    == false);
    assert(rb2.bookMeeting("z", 6, 10)  == "A1");  // 5..5 and 6..10 no overlap
    cout << "Example 2 passed\n";

    // Edge: same meetingId twice
    RoomBooking rb3({"r1"});
    assert(rb3.bookMeeting("dup", 1, 5) == "r1");
    assert(rb3.bookMeeting("dup", 6, 9) == "");    // duplicate active id
    cout << "Edge cases passed\n";

    // Edge: point meeting, adjacent meetings no conflict
    RoomBooking rb4({"r1"});
    assert(rb4.bookMeeting("a", 5, 5)   == "r1");
    assert(rb4.bookMeeting("b", 6, 6)   == "r1");  // 5..5 and 6..6 no overlap
    assert(rb4.bookMeeting("c", 5, 6)   == "");    // conflicts with both
    cout << "Point meeting edge cases passed\n";

    return 0;
}